#include <exception>
#include <iostream>
#include <span>

#include "debugplus/actions/actions.h"
#include "debugplus/debug_session.h"
#include "debugplus/lldb_context.h"
#include "debugplus/parser/parser.h"

namespace {
  void PrintState(DebugSession& session) {
    SessionSnapshot snapshot{session.GetSnapshot()};

    std::cout << snapshot.state << '\n';
    std::cout << snapshot.stop_reason << '\n';

    if (snapshot.frame) {
      FrameInfo frame_info{*snapshot.frame};

      std::cout << frame_info.file_path << '\n';
      std::cout << frame_info.function_name << '\n';
      std::cout << frame_info.line << '\n';
      std::cout << frame_info.pc << '\n';
    }
  }

  int Run(int argc, std::span<char*> argv) {
    if (argc != 2) {
      std::cout << "Invalid Usage, program expects: './debugger {path_to_exe}'\n";
      return 1;
    }

    try {
      LLDBContext lldb_runtime{};

      DebugSession session{argv[1]};

      while (!session.ShouldQuit()) {
        std::cout << "(debugplus) ";

        actions::Action user_action{parser::Parse()};
        session.Act(user_action);

        PrintState(session);
      }

      return 0;
    } catch (std::exception& e) {
      std::cerr << "Fatal: " << e.what() << '\n';
      return 1;
    }
  }
}  // namespace

int main(int argc, char* argv[]) { return Run(argc, std::span(argv, static_cast<size_t>(argc))); }
