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
}  // namespace
