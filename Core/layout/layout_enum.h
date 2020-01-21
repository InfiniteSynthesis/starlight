// Copyright 2020 Infinite Synthesis(T.C.V.). All rights reserved.

#ifndef STARLIGHT_LAYOUT_LAYOUT_ENUM_H_
#define STARLIGHT_LAYOUT_LAYOUT_ENUM_H_

#ifdef __cplusplus
namespace starlight {
#endif

const size_t kCSSDirectionTop = 0;
const size_t kCSSDirectionLeft = 1;
const size_t kCSSDirectionBottom = 2;
const size_t kCSSDirectionRight = 3;

struct FloatSize {
  FloatSize(float width, float height) : width_(width), height_(height) {}
  float width_;
  float height_;
};

/**
 * Exact: with definite size;
 * Undefined: size should be constructed by its children;
 * AtMost: size will be exact afterwards - can be determined by parents;
 */
enum LayoutMode { kLayoutModeExact, kLayoutModeUndefined, kLayoutModeAtMost };

enum PositionType { kPositionRelative, kPositionAbsolute, kPositionFixed };

enum DisplayType { kDisplayFlex, kDisplayGrid, kDisplayNone };

/**
 * The flex-direction property specifies how flex items are placed in the flex
 * container, by setting the direction of the flex container’s main axis.
 */
enum FlexDirectionType {
  kFlexDirectionColumn,
  kFlexDirectionColumnReverse,
  kFlexDirectionRow,
  kFlexDirectionRowReverse
};

/**
 * The flex-wrap property controls whether the flex container is single-line or
 * multi-line, and the direction of the cross-axis, which determines the
 * direction new lines are stacked in.
 */
enum FlexWrapType { kFlexWrapNoWrap, kFlexWrapWrap, kFlexWrapWrapReverse };

/**
 * The justify-content property aligns flex items along the main axis of the
 * current line of the flex container.*/
enum JustifyContentType {
  kJustifyContentFlexStart,
  kJustifyContentFlexEnd,
  kJustifyContentCenter,
  kJustifyContentSpaceBetween,
  kJustifyContentSpaceAround
};

/**
 * Flex items can be aligned in the cross axis of the current line of the flex
 * container, similar to justify-content but in the perpendicular direction.
 * align-items sets the default alignment for all of the flex container’s items.
 */
enum AlignItemsType {
  kAlignItemsFlexStart,
  kAlignItemsCenter,
  kAlignItemsFlexEnd,
  kAlignItemsStretch,
};

/**
 * align-self allows this default alignment to be overridden for individual flex
 * items.*/
enum AlignSelfType {
  kAlignSelfFlexStart = kAlignItemsFlexStart,
  kAlignSelfCenter = kAlignItemsCenter,
  kAlignSelfFlexEnd = kAlignItemsFlexEnd,
  kAlignSelfStretch = kAlignItemsStretch,
  kAlignSelfAuto
};

/**
 * align-content property aligns a flex container’s lines within the flex
 * container when there is extra space in the cross-axis
 */
enum AlignContentType {
  kAlignContentFlexStart,
  kAlignContentFlexEnd,
  kAlignContentCenter,
  kAlignContentSpaceBetween,
  kAlignContentSpaceAround,
  kAlignContentStretch
};

#ifdef __cplusplus
}
#endif

#endif