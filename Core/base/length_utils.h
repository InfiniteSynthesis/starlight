// Copyright 2020 Infinite Synthesis(T.C.V.). All rights reserved.

#ifndef STARLIGHT_BASE_LENGTH_UTILS_H_
#define STARLIGHT_BASE_LENGTH_UTILS_H_

#include <string>
#include "base/length.h"
#include "base/string_utils.h"

namespace base {

bool ToLength(const std::string& value, Length& len) {
  if (value.compare("auto") == 0) {
    len.SetTypeAndValue(kLengthAuto, .0f);
  }
  if (value.empty())
    return false;

  bool is_px = false;
  std::string px_str = value;
  size_t start = value.length() - 2;
  if (start > 0 && value.find("px", start - 1) != std::string::npos) {
    is_px = true;
    px_str = value.substr(0, start);
  }

  float f;
  if (!base::StringToFloat(px_str, f)) {
    return false;
  }
  if (is_px) {
    len.SetTypeAndValue(kLengthFixed, f);
  }
  return true;
}

}  // namespace base

#endif