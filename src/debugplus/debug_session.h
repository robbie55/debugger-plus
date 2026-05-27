#pragma once

#include <cassert>
#include <string_view>

#include "actions/actions.h"
#include "lldb/API/SBDebugger.h"

class DebugSession {
 private:
  lldb::SBDebugger _debugger;
  lldb::SBTarget _target;
  lldb::SBProcess _process;
  // lldb::SBListener _listener;

  bool _quit_requested{false};

 public:
  // move-no copy, wrapper serves as the reigns for a single session
  DebugSession() = delete;
  DebugSession(const DebugSession& other) = delete;
  DebugSession(DebugSession&& other) noexcept = default;
  DebugSession& operator=(const DebugSession& other) = delete;
  DebugSession& operator=(DebugSession&& other) noexcept = default;

  ~DebugSession() = default;

  explicit DebugSession(std::string_view exe_path);

  void PrintState();

  [[nodiscard]] bool ShouldQuit() const { return _quit_requested; };
  bool HasProcessExited();
  void Quit() { _quit_requested = true; };

  void Act(const actions::Action& user_action);
  void Run();
  void Break(const actions::Break& break_action);
  void Next();
  void Continue();
  void Step();
};
