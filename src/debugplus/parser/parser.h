#pragma once

#include <string_view>
#include <vector>

#include "debugplus/actions/actions.h"

namespace parser {
  actions::Action Parse();
  std::vector<std::string_view> Tokenize(std::string_view);
  actions::Action Dispatch(const std::vector<std::string_view>& tokens);
}  // namespace parser
