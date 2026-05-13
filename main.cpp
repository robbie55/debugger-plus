#include <exception>
#include <iostream>

#include "DebugSession.h"
#include "LLDBContext.h"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cout << "Invalid Usage, program expects: './debugger {path_to_exe}'\n";
    return 1;
  }
  try {
    LLDBContext lldb_runtime{};

    DebugSession session{argv[1]};

    return 0;
  } catch (std::exception& e) {
    std::cerr << "Fatal: " << e.what() << '\n';
    return 1;
  }
}
