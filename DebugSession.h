#pragma once

#include <cassert>
#include <iostream>
#include <stdexcept>

#include "lldb/API/SBDebugger.h"
#include "lldb/API/SBError.h"
#include "lldb/lldb-enumerations.h"
// #include "lldb/API/SBListener.h"

class DebugSession {
 private:
  lldb::SBDebugger _debugger;
  lldb::SBTarget _target;
  lldb::SBProcess _process;
  // lldb::SBListener _listener;

 public:
  // move-no copy, wrapper serves as the reigns for a single session
  DebugSession() = delete;
  DebugSession(const DebugSession& other) = delete;
  DebugSession(DebugSession&& other) noexcept = default;
  DebugSession& operator=(const DebugSession& other) = delete;
  DebugSession& operator=(DebugSession&& other) noexcept = default;

  ~DebugSession() = default;

  explicit DebugSession(std::string_view exe_path)
      : _debugger{lldb::SBDebugger::Create()},
        _target{_debugger.CreateTarget(std::string(exe_path).c_str())} {
    // synchronous callbacks for now, as we introduce an event loop, we can revisit the SetAsync
    _debugger.SetAsync(false);

    if (!_target.IsValid()) {
      throw std::runtime_error(
          "DebugSession(std::string_view exe_path) -- Invalid path to executable: " +
          std::string(exe_path));
    }

    // set breakpoint initial on main
    _target.BreakpointCreateByName("main");

    lldb::SBError error;
    _process = _target.LaunchSimple(nullptr, nullptr, nullptr);

    if (!_process.IsValid()) {
      throw std::runtime_error("DebugSession(std::string_view exe_path) -- Invalid process: ");
    }

    if (_process.GetState() == lldb::StateType::eStateStopped) {
      std::cout << "Process launched and state stopped\n";
    }
  };
};
