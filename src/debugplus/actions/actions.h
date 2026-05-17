#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <variant>

namespace actions {
  struct Run {};
  struct Continue {};
  struct Step {};
  struct Next {};
  struct Break {
    uint32_t line{};
    std::string file;
  };
  struct Print {
    std::uint32_t variable{};
  };
  struct Quit {};
  struct Unknown {};

  using Action =
      std::variant<std::monostate, Run, Continue, Step, Next, Break, Print, Quit, Unknown>;
};  // namespace actions
