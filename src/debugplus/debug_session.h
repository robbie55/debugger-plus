#pragma once

#include <cassert>
#include <string_view>

#include "lldb/API/SBDebugger.h"

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

  explicit DebugSession(std::string_view exe_path);

  void Launch();
};
