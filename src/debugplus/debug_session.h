#pragma once

#include <cassert>
#include <cstdint>
#include <optional>
#include <string_view>

#include "actions/actions.h"
#include "lldb/API/SBDebugger.h"
#include "lldb/lldb-enumerations.h"

struct FrameInfo {
  std::string function_name;
  std::string file_path;
  uint32_t line{};
  uint64_t pc{};
};

struct SessionSnapshot {
  lldb::StateType state{};
  lldb::StopReason stop_reason{};
  std::optional<FrameInfo> frame;
};

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

  SessionSnapshot GetSnapshot();

  [[nodiscard]] bool ShouldQuit() const { return _quit_requested; };
  void Quit() { _quit_requested = true; };

  void Act(const actions::Action& user_action);
  void Run();
  void Break(const actions::Break& break_action);
  void Next();
  void Continue();
  void Step();
};
