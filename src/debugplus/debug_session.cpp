#include "debug_session.h"

#include <cassert>
#include <cstddef>
#include <optional>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <variant>

#include "lldb/API/SBBreakpoint.h"
// #include "lldb/API/SBBreakpointLocation.h"
#include "lldb/API/SBDefines.h"
// #include "lldb/API/SBError.h "
#include "lldb/API/SBFileSpec.h"
#include "lldb/API/SBLineEntry.h"
#include "lldb/API/SBProcess.h"
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
  lldb::SBThread cur_thread{_process.GetSelectedThread()};
  cur_thread.StepOver();
}

void DebugSession::Step() {
  lldb::SBThread cur_thread{_process.GetSelectedThread()};
  cur_thread.StepInto();
}

void DebugSession::Continue() {
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
        }
      },
      user_action);
}

SessionSnapshot DebugSession::GetSnapshot() {
  lldb::StateType state{_process.GetState()};
  lldb::SBThread cur_thread{_process.GetSelectedThread()};
  lldb::StopReason stop_reason{cur_thread.GetStopReason()};

  std::optional<FrameInfo> frame_info{std::nullopt};

  // if frame is stopped, we can store frame_info, else it's nullptr
  if (state == lldb::eStateStopped) {
    lldb::SBFrame frame{cur_thread.GetSelectedFrame()};
    std::string function_name{frame.GetFunctionName()};

    lldb::SBLineEntry line_entry{frame.GetLineEntry()};
    lldb::SBFileSpec file_spec{line_entry.GetFileSpec()};

    std::string file_path{(std::string(file_spec.GetDirectory()) + file_spec.GetFilename())};

    uint32_t line{line_entry.GetLine()};

    uint64_t pc{frame.GetPC()};

    frame_info =
        FrameInfo{.function_name = function_name, .file_path = file_path, .line = line, .pc = pc};
  }

  return SessionSnapshot{.state = state, .stop_reason = stop_reason, .frame = frame_info};
}
