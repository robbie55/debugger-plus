#include "parser.h"

#include <iostream>

namespace {
  std::istream& operator>>(std::istream& in, parser::DebugCmd& cmd) {
    char user_input{};

    in >> user_input;
    switch (user_input) {
      case 'r':
        cmd = parser::kRun;
        break;
      case 'c':
        cmd = parser::kContinue;
        break;
      case 's':
        cmd = parser::kStep;
        break;
      case 'n':
        cmd = parser::kNext;
        break;
      case 'b':
        cmd = parser::kBreak;
        break;
      case 'p':
        cmd = parser::kPrint;
        break;
      case 'q':
        cmd = parser::kQuit;
        break;
      default:
        break;
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
