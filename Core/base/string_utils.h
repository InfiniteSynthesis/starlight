// Copyright 2020 Infinite Synthesis(T.C.V.). All rights reserved.

#ifndef STARLIGHT_BASE_STRING_UTILS_H_
#define STARLIGHT_BASE_STRING_UTILS_H_

#include <string>
#include <vector>

#ifdef __cplusplus
namespace base {
#endif

bool SplitString(const std::string& target,
                 char separator,
                 std::vector<std::string>& result);
bool StringToFloat(const std::string& input, float& output);
bool StringToDouble(const std::string& input, double& output);
bool StringToInt(const std::string& input, int64_t& output, uint8_t base = 10);

#ifdef __cplusplus
}
#endif

#endif