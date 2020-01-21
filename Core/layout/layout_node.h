// Copyright 2020 Infinite Synthesis(T.C.V.). All rights reserved.

#ifndef STARLIGHT_LAYOUT_LAYOUT_NODE_H_
#define STARLIGHT_LAYOUT_LAYOUT_NODE_H_

#include <memory>
#include <vector>

#include "layout/layout_enum.h"

namespace starlight {

class LayoutAlgorithm;
class CSSStyle;

// handle percentage value
// value: top[0] -> left[1] -> bottom[2] -> right[3]
struct LayoutInfo {
  LayoutInfo()
      : min_width_(.0f), min_height_(.0f), max_width_(10E6), max_height_(10E6) {
    padding_.resize(4, .0f);
    margin_.resize(4, .0f);
  }
  float min_width_;
  float min_height_;
  float max_width_;
  float max_height_;
  std::vector<float> padding_;
  std::vector<float> margin_;
};

class LayoutNode {
 public:
  LayoutNode();
  ~LayoutNode();

  // layout tree construct
  LayoutNode* FindNode(int index);
  int FindNode(LayoutNode* node);
  void InsertChild(LayoutNode* child, LayoutNode* reference);
  void InsertChild(LayoutNode* child, int index);
  void InsertChild(LayoutNode* child);
  void RemoveChild(LayoutNode* child);
  LayoutNode* RemoveChild(int index);

  // CSS style
  void SetStyle(const std::string& name,
                const std::string& value,
                bool reset = false);

  // dirty
  inline void MarkDirty(const bool recursion = true);

  // clamp
  void UpdateLayoutInfo(float parent_width, float parent_height);
  float ApplyWidthConstraints(float width) const;
  float ApplyHeightConstraints(float height) const;

  // layout
  void ReLayout(int left, int top, int right, int bottom);
  FloatSize UpdateMeasure(float width,
                          float height,
                          LayoutMode width_mode,
                          LayoutMode height_mode);
  void UpdateAlignment();
  void UpdateMeasureWithDisplayNone();

 private:
  LayoutNode* parent_;
  LayoutNode* prev_;
  LayoutNode* next_;
  LayoutNode* first_child_;
  LayoutNode* last_child_;
  unsigned child_count_;

  bool dirty_;

  std::unique_ptr<CSSStyle> css_style_;
  LayoutAlgorithm* layout_algorithm_;

  LayoutInfo layout_info_;

  // layout results
  float offset_top_;
  float offset_left_;
  float offset_width_;
  float offset_height_;

  void* context_ = nullptr;

 public:
  // getters
  inline LayoutNode* parent() const { return parent_; }
  inline LayoutNode* prev() const { return prev_; }
  inline LayoutNode* next() const { return next_; }
  inline LayoutNode* first_child() const { return first_child_; }
  inline LayoutNode* last_child() const { return last_child_; }
  inline unsigned child_count() const { return child_count_; }
  inline bool dirty() const { return dirty_; }
  const CSSStyle* css_style() const;
  inline LayoutAlgorithm* layout_algorithm() const { return layout_algorithm_; }
  const LayoutInfo& layout_info() const { return layout_info_; }
  LayoutInfo& GetModifiableLayoutInfo() { return layout_info_; }
  inline float offset_top() const { return offset_top_; }
  inline float offset_left() const { return offset_left_; }
  inline float offset_width() const { return offset_width_; }
  inline float offset_height() const { return offset_height_; }

  inline void* context() const { return context_; }

  // setter
  void SetContext(void* const context);
  void SetOffsetTop(float offset_top);
  void SetOffsetLeft(float offset_left);
  void SetOffsetWidth(float offset_width);
  void SetOffsetHeight(float offset_height);
};

}  // namespace starlight

#endif