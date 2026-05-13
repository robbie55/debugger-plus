#pragma once

#include "lldb/API/SBDebugger.h"

class LLDBContext {
 public:
  LLDBContext() { lldb::SBDebugger::Initialize(); };
  ~LLDBContext() { lldb::SBDebugger::Terminate(); }

  LLDBContext(const LLDBContext& other) = delete;
  LLDBContext(LLDBContext&& other) noexcept = delete;
  LLDBContext& operator=(const LLDBContext& other) = delete;
  LLDBContext& operator=(LLDBContext&& other) noexcept = delete;
};
