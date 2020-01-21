// Copyright 2020 Infinite Synthesis(T.C.V.). All rights reserved.

#ifndef STARLIGHT_LAYOUT_FLEX_LAYOUT_H_
#define STARLIGHT_LAYOUT_FLEX_LAYOUT_H_

#include <vector>

#include "layout/layout_algorithm.h"

namespace starlight {

class CSSStyle;
class LayoutNode;

struct ItemInfo {
  ItemInfo(LayoutNode* item)
      : item_(item),
        flex_base_size_(.0f),
        hypothetical_main_size_(.0f),
        used_main_size_(.0f),
        frozen_(false),
        hypothetical_cross_size_(.0f),
        used_cross_size_(.0f) {}
  LayoutNode* item_;
  float flex_base_size_;
  float hypothetical_main_size_;
  float used_main_size_;
  bool frozen_;
  float hypothetical_cross_size_;
  float used_cross_size_;
};

/**
 * records data for single flex line
 * for resolving flexible lengths on main axis
 */
struct FlexLine {
  FlexLine(size_t start,
           size_t end,
           float sum_flex_basie_size,
           float total_flex_grow,
           float total_flex_shrink,
           float total_weighted_flex_shrink,
           float sum_hypothetical_main_size,
           float remaining_free_space,
           bool should_apply_grow)
      : start_(start),
        end_(end),
        sum_flex_basie_size_(sum_flex_basie_size),
        total_flex_grow_(total_flex_grow),
        total_flex_shrink_(total_flex_shrink),
        total_weighted_flex_shrink_(total_weighted_flex_shrink),
        sum_hypothetical_main_size_(sum_hypothetical_main_size),
        remaining_free_space_(remaining_free_space),
        initial_free_space_(.0f),
        should_apply_grow_(should_apply_grow),
        line_cross_size_(.0f) {}

  size_t start_;
  size_t end_;
  float sum_flex_basie_size_;
  float total_flex_grow_;
  float total_flex_shrink_;
  float total_weighted_flex_shrink_;
  float sum_hypothetical_main_size_;
  float remaining_free_space_;
  float initial_free_space_;
  bool should_apply_grow_;
  float line_cross_size_;
};

class FlexLayoutAlgorithm : public LayoutAlgorithm {
 public:
  FlexLayoutAlgorithm(LayoutNode* container, const CSSStyle* container_style);
  ~FlexLayoutAlgorithm();

  virtual void Initialize(float width,
                          float height,
                          LayoutMode width_mode,
                          LayoutMode height_mode);

  virtual void Update(float width,
                      float height,
                      LayoutMode width_mode,
                      LayoutMode height_mode);

  virtual void Measure();

  virtual void Alignment();

 private:
  // measure funcs
  void CalculateFlexBasis();
  void DetermineContainerMainSize();
  void CollectIntoFlexlines();
  bool CollectIntoSignleFlexline(size_t& next_index);
  void ResolveFlexlines();
  void ResolveSingleFlexline(FlexLine& current_line);
  void FreezeInflexibleItems(FlexLine& current_line);
  void FreezeViolations(FlexLine& current_line,
                        std::vector<ItemInfo*>& inflexible_item_infos);
  bool ResolveFlexibleLengths(FlexLine& current_line);
  void DetermineHypotheticalCrossSize();
  void CalculateFlexlineCrossSize();
  void ExpandFlexlineCrossSizeDueToAlignContentStretch();
  void DetermineFlexItemUsedCrossSize();
  void DetermineContainerUsedCrossSize();

  // align funcs
  void MainAxisAlignment();
  void CrossAxisAlignment();

  // settings
  void SolveDirction();
  void ResolveSizeAndMode(float width,
                          float height,
                          LayoutMode width_mode,
                          LayoutMode height_mode);

  LayoutNode* container_;
  const CSSStyle* container_style_;

  float main_available_size_;
  float cross_available_size_;
  LayoutMode main_axis_mode_;
  LayoutMode cross_axis_mode_;

  bool main_axis_horizontal_;
  size_t main_axis_front_;
  size_t main_axis_after_;
  size_t cross_axis_front_;
  size_t cross_axis_after_;

  std::vector<ItemInfo> item_info_;
  std::vector<LayoutNode*> absolute_items;
  std::vector<FlexLine> flex_lines_;
};

}  // namespace starlight

#endif