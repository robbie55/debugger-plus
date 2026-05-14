#pragma once

#include <cstddef>
#include <variant>

namespace actions {
  struct Run {};
  struct Continue {};
  struct Step {};
  struct Next {};
  struct Break {
    std::size_t line{};
  };
  struct Print {
    std::size_t variable{};
  };
  struct Quit {};
  struct Unknown {};

  using Action =
      std::variant<std::monostate, Run, Continue, Step, Next, Break, Print, Quit, Unknown>;
};  // namespace actions
