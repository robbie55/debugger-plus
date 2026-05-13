#pragma once

namespace parser {
  enum DebugCmd : unsigned char { kRun, kContinue, kStep, kNext, kBreak, kPrint, kQuit };

  DebugCmd GetInput();
}  // namespace parser
