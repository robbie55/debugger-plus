#include <doctest/doctest.h>

#include <iostream>
#include <sstream>
#include <streambuf>
#include <string>
#include <variant>

#include "debugplus/actions/actions.h"
#include "debugplus/parser/parser.h"

namespace {
  struct SwapStdinBuf {
    std::streambuf* old_buf{};
    std::istringstream fake_buf;
    std::ios::iostate old_state{};
    explicit SwapStdinBuf(const std::string& buf)
        : old_buf{std::cin.rdbuf()}, fake_buf{buf}, old_state{std::cin.rdstate()} {
      std::cin.rdbuf(fake_buf.rdbuf());
    }

    SwapStdinBuf(const SwapStdinBuf& other) = delete;
    SwapStdinBuf(SwapStdinBuf&& other) = delete;
    void operator=(const SwapStdinBuf& other) = delete;
    void operator=(SwapStdinBuf&& other) = delete;

    ~SwapStdinBuf() {
      std::cin.rdbuf(old_buf);
      std::cin.clear(old_state);
    }
  };

  template <typename Expected>
  void CheckDispatch(const std::vector<std::string_view>& tokens) {
    CAPTURE(tokens);

    actions::Action action{parser::Dispatch(tokens)};
    CHECK(std::holds_alternative<Expected>(action));
  }

  TEST_CASE("Parser") {
    SUBCASE("Parser returns actions::Quit on eof") {
      SwapStdinBuf guard{""};
      actions::Action action{parser::Parse()};
      CHECK(std::holds_alternative<actions::Quit>(action));
    }
    SUBCASE("returns monostate on transient stream failure") {
      SwapStdinBuf guard{""};
      std::cin.setstate(std::ios::failbit);
      actions::Action action{parser::Parse()};
      CHECK(std::holds_alternative<std::monostate>(action));
    }
  }

  TEST_CASE("Dispatch") {
    SUBCASE("Dispatch returns actions::Unknown on a malformed Break input") {
      CheckDispatch<actions::Unknown>({"b", ":"});
      CheckDispatch<actions::Unknown>({"b", "10"});
      CheckDispatch<actions::Unknown>({"break", ":"});
      CheckDispatch<actions::Unknown>({"break", "main"});
    }

    SUBCASE("Dispatch correctly returns actions::Break on valid input") {
      CheckDispatch<actions::Break>({"b", "main.cpp:10"});
      CheckDispatch<actions::Break>({"break", "main.cpp:10"});
    }

    SUBCASE("Dispatch correctly returns actions::Unknown on malformed print input") {
      CheckDispatch<actions::Unknown>({"p"});
      CheckDispatch<actions::Unknown>({"print"});
    }

    SUBCASE("Dispatch correctly returns actions::Print on valid print input") {
      CheckDispatch<actions::Print>({"p", "10"});
      CheckDispatch<actions::Print>({"p", "10"});
    }

    SUBCASE("Dispatch correctly returns actions::Next on valid input") {
      CheckDispatch<actions::Next>({"n"});
      CheckDispatch<actions::Next>({"next"});
    }

    SUBCASE("Dispatch correctly returns actions::Continue on valid input") {
      CheckDispatch<actions::Continue>({"c"});
      CheckDispatch<actions::Continue>({"continue"});
    }

    SUBCASE("Dispatch correctly returns actions::Step on valid input") {
      CheckDispatch<actions::Step>({"s"});
      CheckDispatch<actions::Step>({"step"});
    }

    SUBCASE("Dispatch correctly returns actions::Quit on valid input") {
      CheckDispatch<actions::Quit>({"q"});
      CheckDispatch<actions::Quit>({"quit"});
    }

    SUBCASE("Dispatch correctly returns actions::Run on valid input") {
      CheckDispatch<actions::Run>({"r"});
      CheckDispatch<actions::Run>({"run"});
    }
  }

  TEST_CASE("Tokenize") {
    SUBCASE("Tokenize correctly splits input based of space") {
      std::vector<std::string_view> tokens{parser::Tokenize("hello world")};

      CHECK(tokens.size() == 2);
      CHECK(tokens[0] == "hello");
      CHECK(tokens[1] == "world");
    }

    SUBCASE("Tokenize handles extra spaces and tabs") {
      std::vector<std::string_view> tokens{parser::Tokenize("\t   hello \tworld    \t")};

      CHECK(tokens.size() == 2);
      CHECK(tokens[0] == "hello");
      CHECK(tokens[1] == "world");
    }
  }
}  // namespace
