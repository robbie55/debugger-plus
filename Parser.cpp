#include "Parser.h"

#include <iostream>

namespace {
  std::istream& operator>>(std::istream& in, parser::DebugCmd& cmd) {
    char user_input{};

    in >> user_input;
    switch (user_input) {
      case 'r':
        cmd = parser::kRun;
      case 'c':
        cmd = parser::kContinue;
      case 's':
        cmd = parser::kStep;
      case 'n':
        cmd = parser::kNext;
      case 'b':
        cmd = parser::kBreak;
      case 'p':
        cmd = parser::kPrint;
      case 'q':
        cmd = parser::kQuit;
      default: {
      }
    }

    return in;
  }

}  // namespace

namespace parser {
  DebugCmd GetInput() {
    DebugCmd cmd{};
    std::cin >> cmd;

    return cmd;
  }
}  // namespace parser
