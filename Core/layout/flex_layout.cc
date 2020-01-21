// Copyright 2020 Infinite Synthesis(T.C.V.). All rights reserved.

#include <algorithm>
#include <iostream>

#include "layout/flex_layout.h"
#include "layout/layout_node.h"
#include "layout/style.h"

namespace starlight {

FlexLayoutAlgorithm::FlexLayoutAlgorithm(LayoutNode* container,
                                         const CSSStyle* container_style)
    : LayoutAlgorithm(),
      container_(container),
      container_style_(container_style),
      main_available_size_(.0f),
      cross_available_size_(.0f),
      main_axis_mode_(kLayoutModeExact),
      cross_axis_mode_(kLayoutModeExact),
      main_axis_horizontal_(false),
      main_axis_front_(1),
      main_axis_after_(3),
      cross_axis_front_(0),
      cross_axis_after_(2) {}

FlexLayoutAlgorithm::~FlexLayoutAlgorithm() {}

void FlexLayoutAlgorithm::SolveDirction() {
  if (container_) {
    main_axis_horizontal_ = container_style_->IsMainAxisHorizontal();
    main_axis_front_ = main_axis_horizontal_ ? 1 : 0;
    main_axis_after_ = main_axis_horizontal_ ? 3 : 2;
    cross_axis_front_ = main_axis_horizontal_ ? 0 : 1;
    cross_axis_after_ = main_axis_horizontal_ ? 2 : 3;
  }
}

void FlexLayoutAlgorithm::ResolveSizeAndMode(float width,
                                             float height,
                                             LayoutMode width_mode,
                                             LayoutMode height_mode) {
  const auto& container_padding = container_->layout_info().padding_;

  float content_width = width;
  float content_height = height;
  if (width_mode != kLayoutModeUndefined) {
    width = container_->ApplyWidthConstraints(width);
    content_width = width - container_padding[kCSSDirectionLeft] -
                    container_padding[kCSSDirectionRight] -
                    container_style_->border_left() -
                    container_style_->border_right();
  }
  if (height_mode != kLayoutModeAtMost) {
    height = container_->ApplyHeightConstraints(height);
    content_height = height - container_padding[kCSSDirectionTop] -
                     container_padding[kCSSDirectionBottom] -
                     container_style_->border_top() -
                     container_style_->border_bottom();
  }

  main_available_size_ = main_axis_horizontal_ ? content_width : content_height;
  cross_available_size_ =
      main_axis_horizontal_ ? content_height : content_width;
  main_axis_mode_ = main_axis_horizontal_ ? width_mode : height_mode;
  cross_axis_mode_ = main_axis_horizontal_ ? height_mode : width_mode;

  for (auto& item_info : item_info_) {
    item_info.item_->UpdateLayoutInfo(content_width, content_height);
  }
}

/**
 * initialize flex layout algorithm
 * part 1. determine layout mode and available space
 * part 2. explore items
 */
void FlexLayoutAlgorithm::Initialize(float width,
                                     float height,
                                     LayoutMode width_mode,
                                     LayoutMode height_mode) {
  // determine layout mode and available space
  SolveDirction();

  bool need_order = false;
  // traverse child to classify them
  LayoutNode* child = container_->first_child();
  while (child) {
    const CSSStyle* child_style = child->css_style();
    DisplayType child_display = child_style->display();

    if (child_display == kDisplayNone) {
      child->UpdateMeasureWithDisplayNone();
    } else {
      PositionType child_position = child_style->position();
      if (child_position == kPositionRelative) {
        if (child_style->order() != 0) {
          need_order = true;
        }
        item_info_.push_back(ItemInfo(child));
      } else {
        absolute_items.push_back(child);
      }
    }
    child = child->next();
  }

  // if order is set, need sort. use stable_sort to prevent messing items
  if (need_order) {
    std::stable_sort(
        item_info_.begin(), item_info_.end(),
        [](const ItemInfo& item_a, const ItemInfo& item_b) -> bool {
          return item_a.item_->css_style()->order() <
                 item_b.item_->css_style()->order();
        });
  }

  ResolveSizeAndMode(width, height, width_mode, height_mode);
}

/**
 * update container's layout mode and available space
 */
void FlexLayoutAlgorithm::Update(float width,
                                 float height,
                                 LayoutMode width_mode,
                                 LayoutMode height_mode) {
  // determine layout mode and available space
  ResolveSizeAndMode(width, height, width_mode, height_mode);
}

void FlexLayoutAlgorithm::Measure() {
  CalculateFlexBasis();
  DetermineContainerMainSize();
  CollectIntoFlexlines();
  ResolveFlexlines();

  DetermineHypotheticalCrossSize();
  CalculateFlexlineCrossSize();
  ExpandFlexlineCrossSizeDueToAlignContentStretch();
  DetermineFlexItemUsedCrossSize();
  DetermineContainerUsedCrossSize();

  float offset_border_box_width =
      (main_axis_horizontal_ ? main_available_size_ : cross_available_size_) +
      container_->layout_info().padding_[kCSSDirectionLeft] +
      container_->layout_info().padding_[kCSSDirectionRight] +
      container_style_->border_left() + container_style_->border_right();
  float offset_border_boxheight =
      (main_axis_horizontal_ ? cross_available_size_ : main_available_size_) +
      container_->layout_info().padding_[kCSSDirectionTop] +
      container_->layout_info().padding_[kCSSDirectionBottom] +
      container_style_->border_top() + container_style_->border_bottom();

  container_->SetOffsetWidth(offset_border_box_width);
  container_->SetOffsetHeight(offset_border_boxheight);
}

void FlexLayoutAlgorithm::Alignment() {
  MainAxisAlignment();
  CrossAxisAlignment();
}

void FlexLayoutAlgorithm::CalculateFlexBasis() {
  for (auto& item_info : item_info_) {
    LayoutNode* item = item_info.item_;
    const CSSStyle* item_style = item->css_style();

    // determine the flex base size
    const Length& flex_basis = item_style->flex_basis();
    switch (flex_basis.type()) {
      // If the item has a definite used flex basis, that’s the flex base size.
      case base::kLengthFixed: {
        item_info.flex_base_size_ = flex_basis.value();
        break;
      }
      case base::kLengthPercentage: {
        item_info.flex_base_size_ =
            main_axis_mode_ == kLayoutModeUndefined
                ? .0f
                : flex_basis.GetComputedValue(main_available_size_);
        break;
      }
      case base::kLengthAuto: {
        const Length& item_main_axis_size =
            main_axis_horizontal_ ? item_style->width() : item_style->height();
        // std::cout << "mainaxissize:" << item << " "
        //           << item_main_axis_size.type() << " "
        //           << item_main_axis_size.value() << std::endl;
        switch (item_main_axis_size.type()) {
          case base::kLengthFixed: {
            item_info.flex_base_size_ = item_main_axis_size.value();
            break;
          }
          case base::kLengthPercentage: {
            item_info.flex_base_size_ =
                main_axis_mode_ == kLayoutModeUndefined
                    ? .0f
                    : item_main_axis_size.GetComputedValue(
                          main_available_size_);
            break;
          }
          case base::kLengthAuto: {
            float item_layout_width = .0f;
            float item_layout_height = .0f;
            LayoutMode item_layout_width_mode = kLayoutModeUndefined;
            LayoutMode item_layout_height_mode = kLayoutModeUndefined;

            AlignItemsType align_type =
                item_style->align_self() == kAlignSelfAuto
                    ? (container_style_->align_items())
                    : AlignItemsType(item_style->align_self());
            if (align_type == kAlignItemsStretch) {
              float& cross_size = main_axis_horizontal_ ? item_layout_height
                                                        : item_layout_width;
              LayoutMode& cross_mode = main_axis_horizontal_
                                           ? item_layout_height_mode
                                           : item_layout_width_mode;
              cross_size = cross_available_size_;
              cross_mode = kLayoutModeExact;
            }

            FloatSize layout_result = item->UpdateMeasure(
                item_layout_width, item_layout_height, item_layout_width_mode,
                item_layout_height_mode);

            // std::cout << "CalculateFlexBasis:width:" << item << " "
            //           << item_layout_width << " height:" <<
            //           item_layout_height
            //           << " widthmode:" << item_layout_width_mode
            //           << " heightmode:" << item_layout_height_mode
            //           << " resultwidth:" << layout_result.width_
            //           << " resultheight:" << layout_result.height_ <<
            //           std::endl;
            item_info.flex_base_size_ = main_axis_horizontal_
                                            ? layout_result.width_
                                            : layout_result.height_;
            break;
          }
        }
        break;
      }
    }

    // determine hypothetical main size:item’s flex base size clamped according
    // to its used min and max main sizes (and flooring the content box size at
    // zero).
    item_info.hypothetical_main_size_ =
        main_axis_horizontal_
            ? item->ApplyWidthConstraints(item_info.flex_base_size_)
            : item->ApplyHeightConstraints(item_info.flex_base_size_);
    // std::cout << "flex_base_size_:" << item_info.flex_base_size_ << " "
    //           << "hypothetical_main_size_:" <<
    //           item_info.hypothetical_main_size_
    //           << std::endl;
  }
}

void FlexLayoutAlgorithm::DetermineContainerMainSize() {
  if (main_axis_mode_ == kLayoutModeExact) {
    return;  // do nothing
  }
  // For this computation, auto margins on flex items are treated as 0.
  float main_size_sum = .0f;
  for (auto& item_info : item_info_) {
    LayoutNode* item = item_info.item_;
    main_size_sum += item_info.hypothetical_main_size_ +
                     item->layout_info().margin_[main_axis_front_] +
                     item->layout_info().margin_[main_axis_after_];
  }
  if (main_axis_mode_ == kLayoutModeUndefined) {
    main_available_size_ = main_size_sum;
  } else if (main_axis_mode_ == kLayoutModeAtMost) {
    main_available_size_ = std::max(main_size_sum, main_available_size_);
  }

  for (auto& item_info : item_info_) {
    item_info.item_->UpdateLayoutInfo(
        main_axis_horizontal_ ? main_available_size_ : cross_available_size_,
        main_axis_horizontal_ ? cross_available_size_ : main_available_size_);
  }
}

void FlexLayoutAlgorithm::CollectIntoFlexlines() {
  size_t next_index = 0;
  flex_lines_.clear();
  while (CollectIntoSignleFlexline(next_index)) {
    continue;
  }
}

bool FlexLayoutAlgorithm::CollectIntoSignleFlexline(size_t& next_index) {
  bool is_single_line = container_style_->flex_wrap() == kFlexWrapNoWrap;
  float asum_flex_basis_size = .0f;
  float total_flex_grow = .0f;
  float total_flex_shrink = .0f;
  float total_weighted_flex_shrink = .0f;
  float sum_hypothetical_main_size = .0f;
  bool line_has_item = false;

  size_t start_index = next_index;

  for (; next_index < item_info_.size(); ++next_index) {
    ItemInfo& item_info = item_info_[next_index];
    LayoutNode* item = item_info.item_;
    const CSSStyle* item_style = item->css_style();
    // If the flex container is single-line, collect all the flex items into a
    // single flex line.
    // If it is multi-line, collect consecutive items one by one until the first
    // time that the next collected item would not fit into the flex container’s
    // inner main size. If the very first uncollected item wouldn’t
    // fit, collect just it into the line.
    float item_outer_hypothetical_main_size =
        item_info.hypothetical_main_size_ +
        item->layout_info().margin_[main_axis_front_] +
        item->layout_info().margin_[main_axis_after_];
    if ((!is_single_line &&
         sum_hypothetical_main_size + item_outer_hypothetical_main_size >
             main_available_size_ &&
         line_has_item)) {
      break;
    }
    line_has_item = true;
    asum_flex_basis_size += item_info.flex_base_size_ +
                            item->layout_info().margin_[main_axis_front_] +
                            item->layout_info().margin_[main_axis_after_];
    total_flex_grow += item_style->flex_grow();
    total_flex_shrink += item_style->flex_shrink();
    total_weighted_flex_shrink +=
        item_style->flex_shrink() * item_info.flex_base_size_;
    sum_hypothetical_main_size += item_outer_hypothetical_main_size;
  }

  if (next_index > start_index) {
    // determine the used flex factor:
    // [outer hypothetical main sizes sum] v.s. [container’s inner main size]
    bool should_apply_grow = sum_hypothetical_main_size <= main_available_size_;
    flex_lines_.push_back(FlexLine(
        start_index, next_index, asum_flex_basis_size, total_flex_grow,
        total_flex_shrink, total_weighted_flex_shrink,
        sum_hypothetical_main_size, main_available_size_ - asum_flex_basis_size,
        should_apply_grow));
    return true;
  }
  return false;
}

void FlexLayoutAlgorithm::ResolveFlexlines() {
  for (size_t line_index = 0; line_index < flex_lines_.size(); ++line_index) {
    ResolveSingleFlexline(flex_lines_[line_index]);
  }
}

void FlexLayoutAlgorithm::ResolveSingleFlexline(FlexLine& current_line) {
  FreezeInflexibleItems(current_line);
  while (ResolveFlexibleLengths(current_line)) {
    continue;
  }
}

/**
 * resolving Flexible Lengths - step 2
 * reference: https://www.w3.org/TR/css-flexbox-1/#resolve-flexible-lengths
 * size inflexible items. Freeze, setting its target main size to its
 * hypothetical main size
 */
void FlexLayoutAlgorithm::FreezeInflexibleItems(FlexLine& current_line) {
  std::vector<ItemInfo*> inflexible_item_infos;
  for (size_t line_item_index = current_line.start_;
       line_item_index < current_line.end_; ++line_item_index) {
    ItemInfo& item_info = item_info_[line_item_index];
    const CSSStyle* item_style = item_info.item_->css_style();
    float flex_factor = current_line.should_apply_grow_
                            ? item_style->flex_grow()
                            : item_style->flex_shrink();
    // any item that has a flex factor of zero
    // if using the flex grow factor: any item that has a flex base size greater
    // than its hypothetical main size
    // if using the flex shrink factor: any item that has a flex base size
    // smaller than its hypothetical main size
    if (flex_factor == .0f ||
        (current_line.should_apply_grow_ &&
         item_info.flex_base_size_ > item_info.hypothetical_main_size_) ||
        (!current_line.should_apply_grow_ &&
         item_info.flex_base_size_ < item_info.hypothetical_main_size_)) {
      item_info.used_main_size_ = item_info.hypothetical_main_size_;
      inflexible_item_infos.push_back(&item_info);
    }
  }

  FreezeViolations(current_line, inflexible_item_infos);
  current_line.initial_free_space_ = current_line.remaining_free_space_;
}

void FlexLayoutAlgorithm::FreezeViolations(
    FlexLine& current_line,
    std::vector<ItemInfo*>& inflexible_item_infos) {
  // Calculate initial free space. Sum the outer sizes of all items on the line,
  // and subtract this from the flex container’s inner main size. For frozen
  // items, use their outer target main size; for other items, use their outer
  // flex base size.
  for (auto item_info : inflexible_item_infos) {
    LayoutNode* item = item_info->item_;
    const CSSStyle* item_style = item->css_style();
    float child_size = item_info->used_main_size_;
    current_line.remaining_free_space_ -=
        child_size - item_info->flex_base_size_;
    current_line.total_flex_grow_ -= item_style->flex_grow();
    current_line.total_flex_shrink_ -= item_style->flex_shrink();
    current_line.total_weighted_flex_shrink_ -=
        item_style->flex_shrink() * item_info->flex_base_size_;
    current_line.total_weighted_flex_shrink_ =
        std::max(current_line.total_weighted_flex_shrink_, .0f);
    item_info->frozen_ = true;
  }
}

bool FlexLayoutAlgorithm::ResolveFlexibleLengths(FlexLine& current_line) {
  float total_violation = .0f;
  float used_free_space = .0f;
  std::vector<ItemInfo*> min_violations;
  std::vector<ItemInfo*> max_violations;

  bool should_apply_grow = current_line.should_apply_grow_;
  float sum_flex_factors = should_apply_grow ? current_line.total_flex_grow_
                                             : current_line.total_flex_shrink_;
  // Calculate the remaining free space as for initial free space, above. If the
  // sum of the unfrozen flex items’ flex factors is less than one, multiply the
  // initial free space by this sum. If the magnitude of this value is less than
  // the magnitude of the remaining free space, use this as the remaining free
  // space.
  if (sum_flex_factors > 0 && sum_flex_factors < 1) {
    float fractional = current_line.initial_free_space_ * sum_flex_factors;
    if (std::abs(fractional) < std::abs(current_line.remaining_free_space_))
      current_line.remaining_free_space_ = fractional;
  }

  for (size_t line_item_index = current_line.start_;
       line_item_index < current_line.end_; ++line_item_index) {
    ItemInfo& item_info = item_info_[line_item_index];
    LayoutNode* item = item_info.item_;
    const CSSStyle* item_style = item->css_style();

    if (item_info.frozen_) {
      continue;
    }

    float child_size = item_info.flex_base_size_;
    float extra_space = .0f;
    if (current_line.remaining_free_space_ > 0 &&
        current_line.total_flex_grow_ > 0 && should_apply_grow) {
      extra_space = current_line.remaining_free_space_ *
                    item_style->flex_grow() / current_line.total_flex_grow_;
    } else if (current_line.remaining_free_space_ < 0 &&
               current_line.total_weighted_flex_shrink_ > 0 &&
               !should_apply_grow && item_style->flex_shrink()) {
      extra_space = current_line.remaining_free_space_ *
                    item_style->flex_shrink() * child_size /
                    current_line.total_weighted_flex_shrink_;
    }
    child_size += extra_space;

    float adjusted_child_size = main_axis_horizontal_
                                    ? item->ApplyWidthConstraints(child_size)
                                    : item->ApplyHeightConstraints(child_size);
    item_info.used_main_size_ = adjusted_child_size;
    used_free_space += adjusted_child_size - item_info.flex_base_size_;

    float violation = adjusted_child_size - child_size;
    if (violation > 0)
      min_violations.push_back(&item_info);
    else if (violation < 0)
      max_violations.push_back(&item_info);
    total_violation += violation;
  }

  if (total_violation) {
    FreezeViolations(current_line,
                     total_violation < 0 ? max_violations : min_violations);
  } else {
    current_line.remaining_free_space_ -= used_free_space;
  }

  return total_violation;
}

void FlexLayoutAlgorithm::DetermineHypotheticalCrossSize() {
  for (auto& item_info : item_info_) {
    LayoutNode* item = item_info.item_;
    const CSSStyle* item_style = item->css_style();

    float item_layout_width = .0f;
    float item_layout_height = .0f;
    LayoutMode item_layout_width_mode = kLayoutModeExact;
    LayoutMode item_layout_height_mode = kLayoutModeExact;
    float& item_layout_cross_size =
        main_axis_horizontal_ ? item_layout_height : item_layout_width;
    LayoutMode& item_layout_cross_mode = main_axis_horizontal_
                                             ? item_layout_height_mode
                                             : item_layout_width_mode;

    const Length& item_cross_size =
        main_axis_horizontal_ ? item_style->height() : item_style->width();
    switch (item_cross_size.type()) {
      case base::kLengthFixed: {
        item_layout_cross_size = item_cross_size.value();
        item_layout_cross_mode = kLayoutModeExact;
        break;
      }
      case base::kLengthPercentage: {
        if (cross_axis_mode_ == kLayoutModeUndefined) {
          item_layout_cross_size = cross_available_size_;  // TODO: -margin
          item_layout_cross_mode = kLayoutModeUndefined;
        } else {
          item_layout_cross_size =
              item_cross_size.GetComputedValue(cross_available_size_);
          item_layout_cross_mode = kLayoutModeExact;
        }
        break;
      }
      case base::kLengthAuto: {
        item_layout_cross_size = cross_available_size_;  // TODO: -margin
        item_layout_cross_mode = kLayoutModeUndefined;
      }
    }

    // set main axis layout size, mode is exact
    if (main_axis_horizontal_) {
      item_layout_width = item_info.used_main_size_;
    } else {
      item_layout_height = item_info.used_main_size_;
    }
    // std::cout << "DetermineHypotheticalCrossSizeLayout:" << item_layout_width
    //           << " " << item_layout_height << " " << item_layout_width_mode
    //           << " " << item_layout_height_mode << std::endl;
    FloatSize layout_result =
        item->UpdateMeasure(item_layout_width, item_layout_height,
                            item_layout_width_mode, item_layout_height_mode);
    item_info.hypothetical_cross_size_ =
        main_axis_horizontal_ ? layout_result.height_ : layout_result.width_;
  }
}

void FlexLayoutAlgorithm::CalculateFlexlineCrossSize() {
  // If the flex container is single-line and has a definite cross size, the
  // cross size of the flex line is the flex container’s inner cross size.
  if (container_style_->flex_wrap() == kFlexWrapNoWrap &&
      flex_lines_.size() > 0 && cross_axis_mode_ == kLayoutModeExact) {
    flex_lines_[0].line_cross_size_ = cross_available_size_;
    return;  // TODO:If the flex container is single-line, then clamp the line’s
             // cross-size to be within the container’s computed min and max
             // cross sizes
  }
  // Among all the items not collected by the previous step, find the largest
  // outer hypothetical cross size.
  for (auto& flexline : flex_lines_) {
    float max_outer_hypothetical_cross_size = .0f;
    for (size_t line_item_index = flexline.start_;
         line_item_index < flexline.end_; ++line_item_index) {
      const CSSStyle* item_style =
          item_info_[line_item_index].item_->css_style();
      float item_outer_hypothetical_cross_size =
          item_info_[line_item_index].hypothetical_cross_size_;
      const Length& cross_margin_front = main_axis_horizontal_
                                             ? item_style->margin_top()
                                             : item_style->margin_left();
      const Length& cross_margin_after = main_axis_horizontal_
                                             ? item_style->margin_bottom()
                                             : item_style->margin_right();
      float margin_refer_cross_size = cross_axis_mode_ == kLayoutModeUndefined
                                          ? .0f
                                          : cross_available_size_;
      float cross_margin_front_value =
          cross_margin_front.GetComputedValue(margin_refer_cross_size);
      float cross_margin_after_value =
          cross_margin_after.GetComputedValue(margin_refer_cross_size);
      item_outer_hypothetical_cross_size +=
          cross_margin_front_value + cross_margin_after_value;

      max_outer_hypothetical_cross_size =
          std::max(max_outer_hypothetical_cross_size,
                   item_outer_hypothetical_cross_size);
    }
    flexline.line_cross_size_ = max_outer_hypothetical_cross_size;
  }
}

void FlexLayoutAlgorithm::ExpandFlexlineCrossSizeDueToAlignContentStretch() {
  if (cross_axis_mode_ == kLayoutModeUndefined ||
      container_style_->align_content() != kAlignContentStretch) {
    return;
  }
  float sum_line_cross_size = .0f;
  for (auto& flexline : flex_lines_) {
    sum_line_cross_size += flexline.line_cross_size_;
  }
  if (sum_line_cross_size > cross_available_size_ &&
      cross_axis_mode_ == kLayoutModeAtMost) {
    return;
  }
  float size_need_expand =
      (cross_available_size_ - sum_line_cross_size) / flex_lines_.size();
  for (auto& flexline : flex_lines_) {
    flexline.line_cross_size_ += size_need_expand;
  }
}

/**
 * Determine the used cross size of each flex item.
 */
void FlexLayoutAlgorithm::DetermineFlexItemUsedCrossSize() {
  for (auto& flexline : flex_lines_) {
    for (size_t line_item_index = flexline.start_;
         line_item_index < flexline.end_; ++line_item_index) {
      ItemInfo& item_info = item_info_[line_item_index];
      const CSSStyle* item_style = item_info.item_->css_style();
      // If a flex item has align-self: stretch, its computed cross size
      // property is auto, and neither of its cross-axis margins are auto, the
      // used outer cross size is the used cross size of its flex line, clamped
      // according to the item’s used min and max cross sizes.
      const Length& item_cross_size =
          main_axis_horizontal_ ? item_style->height() : item_style->width();
      const Length& cross_margin_front = main_axis_horizontal_
                                             ? item_style->margin_top()
                                             : item_style->margin_left();
      const Length& cross_margin_after = main_axis_horizontal_
                                             ? item_style->margin_bottom()
                                             : item_style->margin_right();
      AlignItemsType align_type =
          item_style->align_self() == kAlignSelfAuto
              ? (container_style_->align_items())
              : AlignItemsType(item_style->align_self());
      if (align_type == kAlignItemsStretch && item_cross_size.IsAuto() &&
          !cross_margin_front.IsAuto() && !cross_margin_after.IsAuto()) {
        item_info.used_cross_size_ =
            flexline.line_cross_size_ -
            item_info.item_->layout_info().margin_[cross_axis_front_] -
            item_info.item_->layout_info().margin_[cross_axis_after_];
        // TODO:clamp
        float item_layout_width = main_axis_horizontal_
                                      ? item_info.used_main_size_
                                      : item_info.used_cross_size_;
        float item_layout_height = main_axis_horizontal_
                                       ? item_info.used_cross_size_
                                       : item_info.used_main_size_;
        item_info.item_->ApplyWidthConstraints(item_layout_width);
        item_info.item_->ApplyHeightConstraints(item_layout_height);
        LayoutMode item_layout_width_mode = kLayoutModeExact;
        LayoutMode item_layout_height_mode = kLayoutModeExact;
        item_info.item_->UpdateMeasure(item_layout_width, item_layout_height,
                                       item_layout_width_mode,
                                       item_layout_height_mode);
      } else {
        // Otherwise, the used cross size is the item’s hypothetical cross size.
        item_info.used_cross_size_ = item_info.hypothetical_cross_size_;
      }
    }
  }
}

/**
 * Determine the flex container’s used cross size
 */
void FlexLayoutAlgorithm::DetermineContainerUsedCrossSize() {
  // If the cross size property is a definite size, use that,
  if (cross_axis_mode_ == kLayoutModeExact) {
    return;  // do nothing
  }
  // Otherwise, use the sum of the flex lines' cross sizes
  float sum_flexline_cross_size = .0f;
  for (auto& flexline : flex_lines_) {
    sum_flexline_cross_size += flexline.line_cross_size_;
  }

  if (cross_axis_mode_ == kLayoutModeUndefined) {
    cross_available_size_ = sum_flexline_cross_size;
  } else if (cross_axis_mode_ == kLayoutModeAtMost) {
    cross_available_size_ =
        std::max(sum_flexline_cross_size, cross_available_size_);
  }
  for (auto& item_info : item_info_) {
    item_info.item_->UpdateLayoutInfo(
        main_axis_horizontal_ ? main_available_size_ : cross_available_size_,
        main_axis_horizontal_ ? cross_available_size_ : main_available_size_);
  }
}

void FlexLayoutAlgorithm::MainAxisAlignment() {
  float main_axis_padding_front =
      container_->layout_info().padding_[main_axis_front_];

  for (auto& flexline : flex_lines_) {
    std::vector<LayoutNode*> items(flexline.end_ - flexline.start_, nullptr);
    size_t item_index = 0;
    // If the remaining free space is positive and at least one main-axis margin
    // on this line is auto, distribute the free space equally among these
    // margins. Otherwise, set all auto margins to zero.
    float total_used_main_axis_size = .0f;
    std::vector<std::pair<size_t, bool>> auto_margins;
    for (size_t line_item_index = flexline.start_;
         line_item_index < flexline.end_; ++line_item_index) {
      LayoutNode* item = item_info_[line_item_index].item_;
      items[item_index++] = item;
      const CSSStyle* item_style = item->css_style();
      total_used_main_axis_size +=
          (main_axis_horizontal_ ? item->offset_width()
                                 : item->offset_height()) +
          item->layout_info().margin_[main_axis_front_] +
          item->layout_info().margin_[main_axis_after_];

      const Length& margin_front = main_axis_horizontal_
                                       ? item_style->margin_left()
                                       : item_style->margin_top();
      const Length& margin_after = main_axis_horizontal_
                                       ? item_style->margin_right()
                                       : item_style->margin_bottom();
      if (margin_front.IsAuto()) {
        auto_margins.push_back(std::make_pair(line_item_index, true));
      }
      if (margin_after.IsAuto()) {
        auto_margins.push_back(std::make_pair(line_item_index, false));
      }
    }
    if (total_used_main_axis_size < main_available_size_ &&
        auto_margins.size() > 0) {
      float auto_margin_value =
          (main_available_size_ - total_used_main_axis_size) /
          auto_margins.size();
      for (const auto& auto_margin : auto_margins) {
        item_info_[auto_margin.first]
            .item_->GetModifiableLayoutInfo()
            .margin_[auto_margin.first ? main_axis_front_ : main_axis_after_] =
            auto_margin_value;
      }
      total_used_main_axis_size = main_available_size_;
    }

    // apply justify content
    bool is_reverse = container_style_->IsMainAxisReverse();
    float adjust_main_start = main_axis_padding_front;
    float adjust_main_interval = .0f;
    float remaining_space = main_available_size_ - total_used_main_axis_size;
    int line_flex_item_count = flexline.end_ - flexline.start_;
    switch (container_style_->justify_content()) {
      case kJustifyContentFlexStart: {
        if (is_reverse) {
          adjust_main_start += remaining_space;
        }
        break;
      }
      case kJustifyContentFlexEnd: {
        if (!is_reverse) {
          adjust_main_start += remaining_space;
        }
        break;
      }
      case kJustifyContentCenter: {
        adjust_main_start += remaining_space / 2.0f;
        break;
      }
      case kJustifyContentSpaceBetween: {
        if (line_flex_item_count > 1) {
          adjust_main_interval = remaining_space / (line_flex_item_count - 1);
        }
        break;
      }
      case kJustifyContentSpaceAround: {
        float interval = remaining_space / (line_flex_item_count * 2);
        adjust_main_start += interval;
        adjust_main_interval = 2 * interval;
        break;
      }
    }
    if (is_reverse) {
      std::reverse(items.begin(), items.end());
    }

    for (size_t i = 0; i < items.size(); ++i) {
      LayoutNode* item = items[i];
      adjust_main_start += item->layout_info().margin_[main_axis_front_];
      if (main_axis_horizontal_) {
        item->SetOffsetLeft(adjust_main_start);
      } else {
        item->SetOffsetTop(adjust_main_start);
      }
      adjust_main_start += (main_axis_horizontal_ ? item->offset_width()
                                                  : item->offset_height()) +
                           item->layout_info().margin_[main_axis_after_] +
                           adjust_main_interval;
    }
  }
}

void FlexLayoutAlgorithm::CrossAxisAlignment() {
  // step: [1] apply `align-content` -> [2] apply cross axis `auto` margin ->
  // [3] apply `align-items` | `align-self` -> [4] apply `wrap-reverse`
  bool is_wrap_reverse = container_style_->flex_wrap() == kFlexWrapWrapReverse;
  float cross_axis_padding_front =
      container_->layout_info().padding_[cross_axis_front_];
  float cross_axis_padding_after =
      container_->layout_info().padding_[cross_axis_after_];
  float cross_axis_padding_start =
      is_wrap_reverse ? cross_axis_padding_after : cross_axis_padding_front;
  float total_used_cross_axis_size = .0f;
  for (auto& flexline : flex_lines_) {
    total_used_cross_axis_size += flexline.line_cross_size_;
  }
  float adjust_cross_start = cross_axis_padding_start;
  float adjust_cross_interval = .0f;

  float remaining_cross_axis_space =
      cross_available_size_ - total_used_cross_axis_size;
  switch (container_style_->align_content()) {
    case kAlignContentFlexStart: {
      break;  // do nothing
    }
    case kAlignContentFlexEnd: {
      adjust_cross_start += remaining_cross_axis_space;
      break;
    }
    case kAlignContentCenter: {
      adjust_cross_start += remaining_cross_axis_space / 2.0f;
      break;
    }
    case kAlignContentStretch: {
      break;  // do nothing
    }
    case kAlignContentSpaceBetween: {
      if (flex_lines_.size() > 1) {
        adjust_cross_interval =
            remaining_cross_axis_space / (flex_lines_.size() - 1);
      }
      break;
    }
    case kAlignContentSpaceAround: {
      float interval = remaining_cross_axis_space / (flex_lines_.size() * 2);
      adjust_cross_start += interval;
      adjust_cross_interval = 2 * interval;
      break;
    }
  }

  for (auto& flexline : flex_lines_) {
    for (size_t line_item_index = flexline.start_;
         line_item_index < flexline.end_; ++line_item_index) {
      LayoutNode* item = item_info_[line_item_index].item_;
      const CSSStyle* item_style = item->css_style();
      // apply `auto` margin
      // If its outer cross size (treating those auto margins as zero) is less
      // than the cross size of its flex line, distribute the difference in
      // those sizes equally to the auto margins.
      float item_cross_border_size =
          main_axis_horizontal_ ? item->offset_height() : item->offset_width();
      float item_outer_cross_size =
          item_cross_border_size +
          item->layout_info().margin_[cross_axis_front_] +
          item->layout_info().margin_[cross_axis_after_];
      const Length& margin_front = main_axis_horizontal_
                                       ? item_style->margin_top()
                                       : item_style->margin_left();
      const Length& margin_after = main_axis_horizontal_
                                       ? item_style->margin_bottom()
                                       : item_style->margin_right();
      if (item_outer_cross_size < flexline.line_cross_size_) {
        bool margin_front_auto = margin_front.IsAuto();
        bool margin_after_auto = margin_after.IsAuto();
        float item_cross_remaining_space =
            flexline.line_cross_size_ - item_outer_cross_size;
        if (margin_front_auto && margin_after_auto) {
          item->GetModifiableLayoutInfo().margin_[cross_axis_front_] =
              item_cross_remaining_space / 2.0f;
          item->GetModifiableLayoutInfo().margin_[cross_axis_after_] =
              item_cross_remaining_space / 2.0f;
        } else if (margin_front_auto) {
          item->GetModifiableLayoutInfo().margin_[cross_axis_front_] =
              item_cross_remaining_space;
        } else if (margin_after_auto) {
          item->GetModifiableLayoutInfo().margin_[cross_axis_after_] =
              item_cross_remaining_space;
        }
        if (margin_front_auto || margin_after_auto) {
          item_outer_cross_size = flexline.line_cross_size_;
        }
      }

      // Align all flex items along the cross-axis per align-self, if neither of
      // the item’s cross-axis margins are auto.
      AlignItemsType align_type =
          item_style->align_self() == kAlignSelfAuto
              ? (container_style_->align_items())
              : AlignItemsType(item_style->align_self());
      float item_cross_offset = .0f;
      switch (align_type) {
        case kAlignItemsFlexStart: {
          if (is_wrap_reverse) {
            item_cross_offset =
                cross_axis_padding_front + cross_available_size_ -
                (adjust_cross_start +
                 item->layout_info().margin_[cross_axis_after_] +
                 item_cross_border_size);
          } else {
            item_cross_offset = adjust_cross_start +
                                item->layout_info().margin_[cross_axis_front_];
          }

          break;
        }
        case kAlignItemsFlexEnd: {
          if (is_wrap_reverse) {
            cross_axis_padding_front + cross_available_size_ -
                (adjust_cross_start + flexline.line_cross_size_ -
                 item->layout_info().margin_[cross_axis_front_]);
          } else {
            item_cross_offset =
                (adjust_cross_start + flexline.line_cross_size_ -
                 item->layout_info().margin_[cross_axis_after_]) -
                item_cross_border_size;
          }

          break;
        }
        case kAlignItemsCenter: {
          if (is_wrap_reverse) {
            cross_axis_padding_front + cross_available_size_ -
                (adjust_cross_start + flexline.line_cross_size_ -
                 (flexline.line_cross_size_ - item_outer_cross_size) / 2.0f -
                 item->layout_info().margin_[cross_axis_front_]);
          } else {
            item_cross_offset =
                adjust_cross_start +
                (flexline.line_cross_size_ - item_outer_cross_size) / 2.0f +
                item->layout_info().margin_[cross_axis_front_];
          }

          break;
        }
        case kAlignItemsStretch: {
          if (is_wrap_reverse) {
            item_cross_offset =
                cross_axis_padding_front + cross_available_size_ -
                (adjust_cross_start +
                 item->layout_info().margin_[cross_axis_after_] +
                 item_cross_border_size);
          } else {
            item_cross_offset = adjust_cross_start +
                                item->layout_info().margin_[cross_axis_front_];
          }
          break;
        }
      }

      if (main_axis_horizontal_) {
        item->SetOffsetTop(item_cross_offset);
      } else {
        item->SetOffsetLeft(item_cross_offset);
      }
    }
    // adjust to next flexline
    adjust_cross_start += flexline.line_cross_size_ + adjust_cross_interval;
  }
}

}  // namespace starlight