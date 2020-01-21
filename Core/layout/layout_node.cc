// Copyright 2020 Infinite Synthesis(T.C.V.). All rights reserved.

#include "layout/layout_node.h"
#include "layout/flex_layout.h"
#include "layout/layout_algorithm.h"
#include "layout/style.h"

#include <iostream>

namespace starlight {

LayoutNode::LayoutNode()
    : parent_(nullptr),
      prev_(nullptr),
      next_(nullptr),
      first_child_(nullptr),
      last_child_(nullptr),
      child_count_(0),
      dirty_(false),
      css_style_(std::make_unique<CSSStyle>()),
      layout_algorithm_(nullptr),
      layout_info_(LayoutInfo()),
      offset_top_(.0f),
      offset_left_(.0f),
      offset_width_(.0f),
      offset_height_(.0f) {}

LayoutNode::~LayoutNode() {}

LayoutNode* LayoutNode::FindNode(int index) {
  if (index == 0) {
    return first_child_;
  }
  if (index < 0) {
    return nullptr;
  }

  LayoutNode* node = first_child_;
  while (index-- && node != nullptr) {
    node = node->next_;
  }

  return node;
}

int LayoutNode::FindNode(LayoutNode* node) {
  int index = 0;
  LayoutNode* find = first_child_;

  while (find != nullptr && find != node) {
    find = find->next_;
    ++index;
  }

  return index;
}

void LayoutNode::InsertChild(LayoutNode* child, LayoutNode* reference) {
  if (child_count_ == 0) {
    first_child_ = child;
    last_child_ = child;
  } else if (reference == nullptr) {
    last_child_->next_ = child;
    child->prev_ = last_child_;
    last_child_ = child;
  } else {
    LayoutNode* pre = reference->prev_;
    if (pre != NULL) {
      pre->next_ = child;
      child->prev_ = pre;
      child->next_ = reference;
      reference->prev_ = child;
    } else {
      child->next_ = first_child_;
      first_child_->prev_ = child;
      first_child_ = child;
    }
  }
  child->parent_ = this;
  ++child_count_;

  MarkDirty();
}

void LayoutNode::InsertChild(LayoutNode* child, int index) {
  if (child->parent_) {
    child->parent_->RemoveChild(child);
  }
  InsertChild(child, FindNode(index));
}

void LayoutNode::InsertChild(LayoutNode* child) {
  InsertChild(child, -1);
}

void LayoutNode::RemoveChild(LayoutNode* child) {
  if (child == nullptr || child_count_ == 0)
    return;
  LayoutNode* pre = child->prev_;
  LayoutNode* next = child->next_;

  child->parent_ = nullptr;
  if (pre == nullptr && next == nullptr) {
    first_child_ = nullptr;
    last_child_ = nullptr;
  } else if (pre == nullptr) {
    next->prev_ = pre;
    first_child_ = next;
  } else if (next == nullptr) {
    pre->next_ = next;
    last_child_ = pre;
  } else {
    next->prev_ = pre;
    pre->next_ = next;
  }
  child_count_--;

  MarkDirty();
}

LayoutNode* LayoutNode::RemoveChild(int index) {
  LayoutNode* node = FindNode(index);
  RemoveChild(node);
  return node;
}

void LayoutNode::SetStyle(const std::string& name,
                          const std::string& value,
                          bool reset) {
  css_style_->SetStyle(name, value, reset);
  MarkDirty();
}

void LayoutNode::MarkDirty(const bool recursion) {
  if (!dirty()) {
    dirty_ = true;
    if (parent_ && recursion) {
      parent_->MarkDirty();
    }
  }
}

void LayoutNode::UpdateLayoutInfo(float parent_width, float parent_height) {
  layout_info_.min_width_ =
      css_style_->min_width().GetComputedValue(parent_width);
  layout_info_.min_height_ =
      css_style_->min_height().GetComputedValue(parent_height);
  layout_info_.max_width_ =
      css_style_->max_width().IsAuto()
          ? 10E6
          : css_style_->max_width().GetComputedValue(parent_width);
  layout_info_.max_height_ =
      css_style_->max_height().IsAuto()
          ? 10E6
          : css_style_->max_height().GetComputedValue(parent_height);

  layout_info_.padding_[kCSSDirectionTop] =
      css_style_->padding_top().GetComputedValue(parent_width);
  layout_info_.padding_[kCSSDirectionLeft] =
      css_style_->padding_left().GetComputedValue(parent_width);
  layout_info_.padding_[kCSSDirectionBottom] =
      css_style_->padding_bottom().GetComputedValue(parent_width);
  layout_info_.padding_[kCSSDirectionRight] =
      css_style_->padding_right().GetComputedValue(parent_width);

  layout_info_.margin_[kCSSDirectionTop] =
      css_style_->margin_top().GetComputedValue(parent_width);
  layout_info_.margin_[kCSSDirectionLeft] =
      css_style_->margin_left().GetComputedValue(parent_width);
  layout_info_.margin_[kCSSDirectionBottom] =
      css_style_->margin_bottom().GetComputedValue(parent_width);
  layout_info_.margin_[kCSSDirectionRight] =
      css_style_->margin_right().GetComputedValue(parent_width);
}

float LayoutNode::ApplyWidthConstraints(float width) const {
  // clamp min-width
  width = std::max(width, layout_info_.min_width_);
  // clamp max-width
  width = std::min(width, layout_info_.max_width_);
  // clamp content box size > 0
  float min_border_box_width = layout_info_.padding_[kCSSDirectionLeft] +
                               layout_info_.padding_[kCSSDirectionRight] +
                               css_style_->border_left() +
                               css_style_->border_right();
  width = std::max(width, min_border_box_width);
  return width;
}

float LayoutNode::ApplyHeightConstraints(float height) const {
  // clamp min-height
  height = std::max(height, layout_info_.min_height_);
  // clamp max-height
  height = std::min(height, layout_info_.max_height_);
  // clamp content box size > 0
  float min_border_box_height = layout_info_.padding_[kCSSDirectionTop] +
                                layout_info_.padding_[kCSSDirectionBottom] +
                                css_style_->border_top() +
                                css_style_->border_bottom();
  height = std::max(height, min_border_box_height);
  return height;
}

void LayoutNode::ReLayout(int left, int top, int right, int bottom) {
  UpdateMeasure(right - left, bottom - top, kLayoutModeExact, kLayoutModeExact);
  UpdateAlignment();
}

FloatSize LayoutNode::UpdateMeasure(float width,
                                    float height,
                                    LayoutMode width_mode,
                                    LayoutMode height_mode) {
  DisplayType display = css_style_->display();
  switch (display) {
    case kDisplayFlex: {
      if (layout_algorithm_) {
        layout_algorithm_->Update(width, height, width_mode, height_mode);
      } else {
        layout_algorithm_ = new FlexLayoutAlgorithm(this, css_style_.get());
        layout_algorithm_->Initialize(width, height, width_mode, height_mode);
      }
      layout_algorithm_->Measure();
      break;
    }
    case kDisplayGrid: {
      // TODO:
      break;
    }
    case kDisplayNone: {
      UpdateMeasureWithDisplayNone();
      break;
    }
    default:
      break;
  }
  return FloatSize(offset_width_, offset_height_);
}

void LayoutNode::UpdateAlignment() {
  // no algorithm with `display: none`
  if (layout_algorithm_) {
    layout_algorithm_->Alignment();

    LayoutNode* child = first_child_;
    while (child != nullptr) {
      child->UpdateAlignment();
      child = child->next_;
    }
  }
}

/**
 * node and all its descendants are measured and positioned 0
 */
void LayoutNode::UpdateMeasureWithDisplayNone() {
  if (layout_algorithm_) {
    layout_algorithm_ = nullptr;
  }

  offset_top_ = .0f;
  offset_left_ = .0f;
  offset_width_ = .0f;
  offset_height_ = .0f;

  LayoutNode* child = first_child_;
  while (child != nullptr) {
    child->UpdateMeasureWithDisplayNone();
    child = child->next_;
  }
}

const CSSStyle* LayoutNode::css_style() const {
  return css_style_.get();
}

void LayoutNode::SetContext(void* const context) {
  context_ = context;
}

void LayoutNode::SetOffsetTop(float offset_top) {
  offset_top_ = offset_top;
}

void LayoutNode::SetOffsetLeft(float offset_left) {
  offset_left_ = offset_left;
}

void LayoutNode::SetOffsetWidth(float offset_width) {
  offset_width_ = offset_width;
}

void LayoutNode::SetOffsetHeight(float offset_height) {
  offset_height_ = offset_height;
}

}  // namespace starlight
