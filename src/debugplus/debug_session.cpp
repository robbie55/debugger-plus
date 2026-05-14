#include "debug_session.h"

#include <iostream>
#include <stdexcept>
#include <type_traits>
#include <variant>

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

void DebugSession::Launch() {
  _target.BreakpointCreateByName("main");

  _process = _target.LaunchSimple(nullptr, nullptr, nullptr);

  if (!_process.IsValid()) {
    throw std::runtime_error("DebugSession(std::string_view exe_path) -- Invalid process: ");
  }

  if (_process.GetState() == lldb::StateType::eStateStopped) {
    std::cout << "Process launched and state stopped\n";
  }
}

void DebugSession::Act(actions::Action user_action) {
  std::visit(
      [this](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, actions::Quit>) {
          this->Quit();
        } else if constexpr (std::is_same_v<T, actions::Run>) {
        }
      },
      user_action);
}
