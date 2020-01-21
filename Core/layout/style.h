// Copyright 2020 Infinite Synthesis(T.C.V.). All rights reserved.

#ifndef STARLIGHT_LAYOUT_STYLE_H_
#define STARLIGHT_LAYOUT_STYLE_H_

#include <string>
#include <unordered_map>

#include "base/length.h"
#include "layout/layout_enum.h"

namespace starlight {

/**
 * support 2 ways of setting styles: by string | by acutal value
 */
class CSSStyle;
typedef void (CSSStyle::*StyleFunc)(const std::string&, bool);
typedef std::unordered_map<std::string, StyleFunc> StyleFuncMap;
using base::Length;
using base::LengthType;

class CSSStyle {
 public:
  CSSStyle();
  ~CSSStyle();

  void ResetAllStyles();

  void SetStyle(const std::string& name,
                const std::string& value,
                bool reset = false);

  bool IsMainAxisHorizontal() const;
  bool IsMainAxisReverse() const;

  void Print() const;

 private:
  // box style
  Length width_;
  Length height_;
  Length min_width_;
  Length min_height_;
  Length max_width_;
  Length max_height_;
  Length padding_top_;
  Length padding_left_;
  Length padding_bottom_;
  Length padding_right_;
  Length margin_top_;
  Length margin_left_;
  Length margin_bottom_;
  Length margin_right_;
  float border_top_;
  float border_left_;
  float border_bottom_;
  float border_right_;

  // display style
  PositionType position_ : 2;
  DisplayType display_ : 2;

  // flex style
  Length flex_basis_;
  float flex_grow_;
  float flex_shrink_;
  FlexDirectionType flex_direction_ : 2;
  FlexWrapType flex_wrap_ : 2;
  JustifyContentType justify_content_ : 3;
  AlignItemsType align_items_ : 3;
  AlignSelfType align_self_ : 3;
  AlignContentType align_content_ : 3;
  int order_;

  static float density_;
  static float screen_width_;
  static float zoom_ratio_;
  static StyleFuncMap style_setters_;

 public:
  // getters
  const Length& width() const { return width_; }
  const Length& height() const { return height_; }
  const Length& min_width() const { return min_width_; }
  const Length& min_height() const { return min_height_; }
  const Length& max_width() const { return max_width_; }
  const Length& max_height() const { return max_height_; }
  const Length& padding_top() const { return padding_top_; }
  const Length& padding_left() const { return padding_left_; }
  const Length& padding_bottom() const { return padding_bottom_; }
  const Length& padding_right() const { return padding_right_; }
  const Length& margin_top() const { return margin_top_; }
  const Length& margin_left() const { return margin_left_; }
  const Length& margin_bottom() const { return margin_bottom_; }
  const Length& margin_right() const { return margin_right_; }
  float border_top() const { return border_top_; }
  float border_left() const { return border_left_; }
  float border_bottom() const { return border_bottom_; }
  float border_right() const { return border_right_; }

  PositionType position() const { return position_; }
  DisplayType display() const { return display_; }

  const Length& flex_basis() const { return flex_basis_; }
  float flex_grow() const { return flex_grow_; }
  float flex_shrink() const { return flex_shrink_; }
  FlexDirectionType flex_direction() const { return flex_direction_; }
  FlexWrapType flex_wrap() const { return flex_wrap_; }
  JustifyContentType justify_content() const { return justify_content_; }
  AlignItemsType align_items() const { return align_items_; }
  AlignSelfType align_self() const { return align_self_; }
  AlignContentType align_content() const { return align_content_; }
  float order() const { return order_; }

  // setter
  void SetWidth(const std::string& value, bool reset = false);
  void SetHeight(const std::string& value, bool reset = false);
  void SetMinWidth(const std::string& value, bool reset = false);
  void SetMinHeight(const std::string& value, bool reset = false);
  void SetMaxWidth(const std::string& value, bool reset = false);
  void SetMaxHeight(const std::string& value, bool reset = false);
  void SetPadding(const std::string& value, bool reset = false);
  void SetPaddingTop(const std::string& value, bool reset = false);
  void SetPaddingLeft(const std::string& value, bool reset = false);
  void SetPaddingBottom(const std::string& value, bool reset = false);
  void SetPaddingRight(const std::string& value, bool reset = false);
  void SetMargin(const std::string& value, bool reset = false);
  void SetMarginTop(const std::string& value, bool reset = false);
  void SetMarginLeft(const std::string& value, bool reset = false);
  void SetMarginBottom(const std::string& value, bool reset = false);
  void SetMarginRight(const std::string& value, bool reset = false);
  void SetBorder(const std::string& value, bool reset = false);
  void SetBorderTop(const std::string& value, bool reset = false);
  void SetBorderLeft(const std::string& value, bool reset = false);
  void SetBorderBottom(const std::string& value, bool reset = false);
  void SetBorderRight(const std::string& value, bool reset = false);

  void SetPosition(const std::string& value, bool reset = false);
  void SetDisplay(const std::string& value, bool reset = false);

  void SetFlex(const std::string& value, bool reset = false);
  void SetFlexBasis(const std::string& value, bool reset = false);
  void SetFlexGrow(const std::string& value, bool reset = false);
  void SetFlexShrink(const std::string& value, bool reset = false);
  void SetFlexDirection(const std::string& value, bool reset = false);
  void SetFlexWrap(const std::string& value, bool reset = false);
  void SetFlexFlow(const std::string& value, bool reset = false);
  void SetJustifyContent(const std::string& value, bool reset = false);
  void SetAlignItems(const std::string& value, bool reset = false);
  void SetAlignSelf(const std::string& value, bool reset = false);
  void SetAlignContent(const std::string& value, bool reset = false);
  void SetOrder(const std::string& value, bool reset = false);
};

}  // namespace starlight

#endif