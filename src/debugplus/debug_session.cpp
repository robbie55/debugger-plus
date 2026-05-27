#include "debug_session.h"

#include <cassert>
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <variant>

#include "debugplus/lldb_strings.h"
#include "lldb/API/SBBreakpoint.h"
#include "lldb/API/SBDefines.h"
#include "lldb/API/SBFileSpec.h"
#include "lldb/API/SBLineEntry.h"
#include "lldb/API/SBProcess.h"
#include "lldb/API/SBSourceManager.h"
#include "lldb/API/SBStream.h"
#include "lldb/API/SBTarget.h"
#include "lldb/API/SBThread.h"
#include "lldb/lldb-enumerations.h"

DebugSession::DebugSession(std::string_view exe_path)
    : _debugger{lldb::SBDebugger::Create()},
      _target{_debugger.CreateTarget(std::string(exe_path).c_str())} {
  // synchronous callbacks for now, as we introduce an event loop, we can revisit the SetAsync
  _debugger.SetAsync(false);

  if (!_target.IsValid()) {
    throw std::runtime_error(
        "DebugSession(std::string_view exe_path) -- Invalid path to executable: " +
        std::string(exe_path));
  }
};

bool DebugSession::HasProcessExited() {
  if (!_process.IsValid()) {
    return true;
  }

  lldb::StateType state{_process.GetState()};
  return state == lldb::eStateExited;
}

void DebugSession::Run() {
  _process = _target.LaunchSimple(nullptr, nullptr, nullptr);

  if (!_process.IsValid()) {
    throw std::runtime_error("Process failed to launch");
  }
}

void DebugSession::Break(const actions::Break& break_action) {
  lldb::SBBreakpoint bp{
      _target.BreakpointCreateByLocation(break_action.file.c_str(), break_action.line)};

  if (!bp.IsValid()) {
    throw std::runtime_error("Invalid Breakpoint");
  }
  // TODO: Figure out what to do from here
  // lldb::SBBreakpointLocation loc{};
}

void DebugSession::Next() {
  if (HasProcessExited()) {
    std::cout << "Process has exited\n";
    return;
  }

  lldb::SBThread cur_thread{_process.GetSelectedThread()};
  cur_thread.StepOver();
}

void DebugSession::Step() {
  if (HasProcessExited()) {
    std::cout << "Process has exited\n";
    return;
  }

  lldb::SBThread cur_thread{_process.GetSelectedThread()};
  cur_thread.StepInto();
}

void DebugSession::Continue() {
  if (HasProcessExited()) {
    std::cout << "Process has exited\n";
    return;
  }

  lldb::SBError error{_process.Continue()};
  if (error.Fail()) {
    throw std::runtime_error{"Error continuing execution for program."};
  }
}

void DebugSession::Act(const actions::Action& user_action) {
  std::visit(
      [this](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, actions::Quit>) {
          Quit();
        } else if constexpr (std::is_same_v<T, actions::Run>) {
          Run();
        } else if constexpr (std::is_same_v<T, actions::Break>) {
          Break(arg);
        } else if constexpr (std::is_same_v<T, actions::Next>) {
          Next();
        } else if constexpr (std::is_same_v<T, actions::Continue>) {
          Continue();
        } else if constexpr (std::is_same_v<T, actions::Step>) {
          Step();
        }
      },
      user_action);
}

void DebugSession::PrintState() {
  lldb::StateType state{_process.GetState()};
  std::string state_str{lldb::SBDebugger::StateAsCString(state)};

  lldb::SBThread cur_thread{_process.GetSelectedThread()};
  lldb::StopReason stop_reason{cur_thread.GetStopReason()};

  std::cout << "State: " << state_str << '\n';
  std::cout << "State Reason: " << debugplus::ToString(stop_reason) << '\n';

  // if state is stopped, we can output the source code
  if (state == lldb::eStateStopped) {
    lldb::SBFrame frame{cur_thread.GetSelectedFrame()};
    std::string function_name{frame.GetFunctionName()};

    lldb::SBLineEntry line_entry{frame.GetLineEntry()};
    lldb::SBFileSpec file_spec{line_entry.GetFileSpec()};

    // guard against unknown program, only ever a result of program execution finished I think
    // TODO: Cleaner exit state
    std::string file_path{"Unknown"};
    if (file_spec.GetDirectory() != nullptr && file_spec.GetFilename() != nullptr) {
      file_path = (std::string(file_spec.GetDirectory()) + "/" + file_spec.GetFilename());
    }

    lldb::SBSourceManager mgr{_debugger};
    lldb::SBStream out;
    mgr.DisplaySourceLinesWithLineNumbers(line_entry.GetFileSpec(),  // from frame.GetLineEntry()
                                          line_entry.GetLine(),
                                          2,     // context_before
                                          2,     // context_after
                                          "->",  // marker prepended to the current line
                                          out);
    std::cout << file_path << '\n';
    std::cout << function_name << '\n';
    std::cout << out.GetData() << '\n';
  }
}
