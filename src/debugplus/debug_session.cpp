#include "debug_session.h"

#include <cassert>
#include <stdexcept>
#include <type_traits>
#include <variant>

#include "lldb/API/SBDefines.h"

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

  assert(bp && bp.GetNumLocations() == 1 && "Invalid Breakpoint: More than one location");

  // TODO: Figure out what to do from here
  // lldb::SBBreakpointLocation loc{};
}

void DebugSession::Act(const actions::Action& user_action) {
  std::visit(
      [this](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, actions::Quit>) {
          this->Quit();
        } else if constexpr (std::is_same_v<T, actions::Run>) {
          this->Run();
        } else if constexpr (std::is_same_v<T, actions::Break>) {
          this->Break(arg);
        }
      },
      user_action);
}
