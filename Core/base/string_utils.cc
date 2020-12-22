// Copyright 2020 Infinite Synthesis(T.C.V.). All rights reserved.

#include <ctype.h>
#include <errno.h>
#include <limits>
#include <cstdint>
#include <stdint.h>
#include <limits.h>

#include "base/string_utils.h"

#ifdef __cplusplus
namespace base {
#endif

bool SplitString(const std::string& target,
                 char separator,
                 std::vector<std::string>& result) {
  typedef std::string::size_type size_t;
  size_t i = 0;
  size_t value_start = INT_MAX;
  while (i < target.size()) {
    bool last_c = i == target.size() - 1;
    if (target[i] != separator && value_start == INT_MAX) {
      value_start = i;
    } else if (target[i] == separator || i == target.size() - 1) {
      size_t sub_num = last_c ? target.size() - value_start : i - value_start;
      if (value_start != INT_MAX) {
        result.push_back(target.substr(value_start, sub_num));
        value_start = INT_MAX;
      }
    }
    i++;
  }
  return !result.empty();
}

bool StringToFloat(const std::string& input, float& output) {
  int old_error = errno;
  errno = 0;
  char* endptr = NULL;
  float d = strtof(input.c_str(), &endptr);
  bool valid = (errno == 0 && !input.empty() &&
                input.c_str() + input.length() == endptr && !isspace(input[0]));
  if (errno == 0)
    errno = old_error;
  if (valid)
    output = d;
  return valid;
}

bool StringToDouble(const std::string& input, double& output) {
  int old_error = errno;
  errno = 0;
  char* endptr = NULL;
  double d = strtod(input.c_str(), &endptr);
  bool valid = (errno == 0 && !input.empty() &&
                input.c_str() + input.length() == endptr && !isspace(input[0]));
  if (errno == 0)
    errno = old_error;
  if (valid)
    output = d;
  return valid;
}

bool StringToInt(const std::string& input, int64_t& output, uint8_t base) {
  int old_error = errno;
  errno = 0;
  char* endptr = NULL;
  int64_t i = strtoll(input.c_str(), &endptr, base);
  bool valid = (errno == 0 && !input.empty() &&
                input.c_str() + input.length() == endptr && !isspace(input[0]));
  if (errno == 0)
    errno = old_error;
  if (valid)
    output = i;
  return valid;
}

#ifdef __cplusplus
}
#endif