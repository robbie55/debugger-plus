#pragma once
#include <string_view>

#include "lldb/lldb-enumerations.h"

namespace debugplus {
  constexpr std::string_view ToString(lldb::StopReason reason) {
    switch (reason) {
      case lldb::eStopReasonInvalid:
        return "invalid";
      case lldb::eStopReasonNone:
        return "none";
      case lldb::eStopReasonTrace:
        return "trace";
      case lldb::eStopReasonBreakpoint:
        return "breakpoint";
      case lldb::eStopReasonWatchpoint:
        return "watchpoint";
      case lldb::eStopReasonSignal:
        return "signal";
      case lldb::eStopReasonException:
        return "exception";
      case lldb::eStopReasonExec:
        return "exec";
      case lldb::eStopReasonPlanComplete:
        return "plan-complete";
      case lldb::eStopReasonThreadExiting:
        return "thread-exiting";
      case lldb::eStopReasonInstrumentation:
        return "instrumentation";
      case lldb::eStopReasonProcessorTrace:
        return "processor-trace";
      case lldb::eStopReasonFork:
        return "fork";
      case lldb::eStopReasonVFork:
        return "vfork";
      case lldb::eStopReasonVForkDone:
        return "vfork-done";
      case lldb::eStopReasonInterrupt:
        return "interrupt";
      case lldb::eStopReasonHistoryBoundary:
        return "history-boundary";
    }
    // TODO: what should happen if a future LLDB adds a value?
  }
}  // namespace debugplus
