// Copyright 2020 Infinite Synthesis(T.C.V.). All rights reserved.

#ifndef STARLIGHT_LAYOUT_LAYOUT_ALGORITHM_H_
#define STARLIGHT_LAYOUT_LAYOUT_ALGORITHM_H_

#include "layout/layout_enum.h"

namespace starlight {

class LayoutAlgorithm {
 public:
  LayoutAlgorithm() {}
  virtual ~LayoutAlgorithm() = default;

  virtual void Initialize(float width,
                          float height,
                          LayoutMode width_mode,
                          LayoutMode height_mode) {}

  virtual void Update(float width,
                      float height,
                      LayoutMode width_mode,
                      LayoutMode height_mode) {}

  virtual void Measure() {}

  virtual void Alignment() {}

 protected:
};

}  // namespace starlight

#endif