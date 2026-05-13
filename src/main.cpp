#include <exception>
#include <iostream>

#include "debugplus/debug_session.h"
#include "debugplus/lldb_context.h"
#include "debugplus/parser/parser.h"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cout << "Invalid Usage, program expects: './debugger {path_to_exe}'\n";
    return 1;
  }

  try {
    LLDBContext lldb_runtime{};

    DebugSession session{argv[1]};
    session.Launch();

    parser::DebugCmd user_in{};
    while (user_in != parser::kQuit) {
      user_in = parser::GetInput();

      // feed in user input to LLDB
    }

    return 0;
  } catch (std::exception& e) {
    std::cerr << "Fatal: " << e.what() << '\n';
    return 1;
  }
}
