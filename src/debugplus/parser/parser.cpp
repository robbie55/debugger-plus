#include "parser.h"

#include <cassert>
#include <charconv>
#include <cstddef>
#include <iostream>
#include <limits>
#include <optional>
#include <span>

#include "debugplus/actions/actions.h"

namespace {
  bool InputClosed() { return std::cin.eof(); }

  void ResetInputState() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }

  std::optional<size_t> GetNumericArg(std::string_view arg) {
    size_t numeric_arg{};
    auto [ptr, ec] = std::from_chars(arg.data(), arg.data() + arg.size(), numeric_arg);

    if (ec == std::errc::invalid_argument || ec == std::errc::result_out_of_range) {
      return std::nullopt;
    }

    return numeric_arg;
  }

  actions::Action ParseBreak(std::span<const std::string_view> args) {
    if (args.size() < 2) {
      return actions::Action{actions::Unknown{}};
    }

    std::optional<size_t> numeric_arg{GetNumericArg(args[1])};
    if (numeric_arg == std::nullopt) {
      return actions::Action{actions::Unknown{}};
    }

    return actions::Action{actions::Break{*numeric_arg}};
  }

  actions::Action ParsePrint(std::span<const std::string_view> args) {
    if (args.size() < 2) {
      return actions::Action{actions::Unknown{}};
    }

    std::optional<size_t> numeric_arg{GetNumericArg(args[1])};
    if (numeric_arg == std::nullopt) {
      return actions::Action{actions::Unknown{}};
    }

    return actions::Action{actions::Print{*numeric_arg}};
  }

}  // namespace

namespace parser {
  /**
   *  -- IMPORTANT --
   *
   * std::string_views defined in parser do not live outside this file
   *
   */

  std::vector<std::string_view> Tokenize(std::string_view user_input) {
    std::vector<std::string_view> tokens{};

    size_t start{};
    size_t end{};

    while ((start = user_input.find_first_not_of(" \t", end)) != std::string_view::npos) {
      end = user_input.find_first_of(" \t", start);
      tokens.emplace_back(user_input.substr(start, end - start));
    }

    return tokens;
  }

  actions::Action Parse() {
    std::string line{};

    if (!std::getline(std::cin, line)) {
      // if eof, quit
      if (InputClosed()) {
        return actions::Action{actions::Quit{}};
      }

      // else, treat like an empty enter
      ResetInputState();
      return actions::Action{std::monostate{}};
    }

    std::vector<std::string_view> tokens{Tokenize(line)};

    return Dispatch(tokens);
  }

  actions::Action Dispatch(const std::vector<std::string_view>& tokens) {
    assert(tokens.size() != 0 && "Assert throw in actions::Dispatch: tokens size is 0");

    const std::string_view verb{tokens[0]};

    if (verb == "run" || verb == "r") {
      return actions::Run{};
    }
    if (verb == "continue" || verb == "c") {
      return actions::Continue{};
    }
    if (verb == "step" || verb == "s") {
      return actions::Step{};
    }
    if (verb == "next" || verb == "n") {
      return actions::Next{};
    }
    if (verb == "break" || verb == "b") {
      std::span<const std::string_view> args(tokens);
      return ParseBreak(args);
    }
    if (verb == "print" || verb == "p") {
      std::span<const std::string_view> args(tokens);
      return ParsePrint(args);
    }
    if (verb == "quit" || verb == "q") {
      return actions::Quit{};
    }

    return actions::Unknown{};
  }
}  // namespace parser
