// Copyright 2020 Infinite Synthesis(T.C.V.). All rights reserved.

#include <iostream>

#include "layout/style.h"
#include "base/length_utils.h"
#include "base/string_utils.h"

namespace starlight {

float CSSStyle::density_ = 1;
float CSSStyle::screen_width_ = 1;
float CSSStyle::zoom_ratio_ = 1;

/**
 * support both '-' and camel case
 */
StyleFuncMap CSSStyle::style_setters_ = {
    {"width", &CSSStyle::SetWidth},
    {"height", &CSSStyle::SetHeight},
    {"min-width", &CSSStyle::SetMinWidth},
    {"minWidth", &CSSStyle::SetMinWidth},
    {"min-height", &CSSStyle::SetMinHeight},
    {"minHeight", &CSSStyle::SetMinHeight},
    {"max-width", &CSSStyle::SetMaxWidth},
    {"maxWidth", &CSSStyle::SetMaxWidth},
    {"max-height", &CSSStyle::SetMaxHeight},
    {"maxHeight", &CSSStyle::SetMaxHeight},
    {"padding", &CSSStyle::SetPadding},
    {"padding-top", &CSSStyle::SetPaddingTop},
    {"paddingTop", &CSSStyle::SetPaddingTop},
    {"padding-left", &CSSStyle::SetPaddingLeft},
    {"paddingLeft", &CSSStyle::SetPaddingLeft},
    {"padding-bottom", &CSSStyle::SetPaddingBottom},
    {"paddingBottom", &CSSStyle::SetPaddingBottom},
    {"padding-right", &CSSStyle::SetPaddingRight},
    {"paddingRight", &CSSStyle::SetPaddingRight},
    {"margin", &CSSStyle::SetMargin},
    {"margin-top", &CSSStyle::SetMarginTop},
    {"marginTop", &CSSStyle::SetMarginTop},
    {"margin-left", &CSSStyle::SetMarginLeft},
    {"marginLeft", &CSSStyle::SetMarginLeft},
    {"margin-bottom", &CSSStyle::SetMarginBottom},
    {"marginBottom", &CSSStyle::SetMarginBottom},
    {"margin-right", &CSSStyle::SetMarginRight},
    {"marginRight", &CSSStyle::SetMarginRight},
    {"border-width", &CSSStyle::SetBorder},
    {"borderWidth", &CSSStyle::SetBorder},
    {"border-top", &CSSStyle::SetBorderTop},
    {"borderTop", &CSSStyle::SetBorderTop},
    {"border-left", &CSSStyle::SetBorderLeft},
    {"borderLeft", &CSSStyle::SetBorderLeft},
    {"border-bottom", &CSSStyle::SetBorderBottom},
    {"borderBottom", &CSSStyle::SetBorderBottom},
    {"border-right", &CSSStyle::SetBorderRight},
    {"borderRight", &CSSStyle::SetBorderRight},
    {"position", &CSSStyle::SetPosition},
    {"display", &CSSStyle::SetDisplay},
    {"flex", &CSSStyle::SetFlex},
    {"flex-grow", &CSSStyle::SetFlexGrow},
    {"flexGrow", &CSSStyle::SetFlexGrow},
    {"flex-shrink", &CSSStyle::SetFlexShrink},
    {"flexShrink", &CSSStyle::SetFlexShrink},
    {"flex-direction", &CSSStyle::SetFlexDirection},
    {"flexDirection", &CSSStyle::SetFlexDirection},
    {"flex-wrap", &CSSStyle::SetFlexWrap},
    {"flexWrap", &CSSStyle::SetFlexWrap},
    {"flex-flow", &CSSStyle::SetFlexFlow},
    {"flexFlow", &CSSStyle::SetFlexFlow},
    {"justify-content", &CSSStyle::SetJustifyContent},
    {"justifyContent", &CSSStyle::SetJustifyContent},
    {"align-items", &CSSStyle::SetAlignItems},
    {"alignItems", &CSSStyle::SetAlignItems},
    {"align-self", &CSSStyle::SetAlignSelf},
    {"alignSelf", &CSSStyle::SetAlignSelf},
    {"align-content", &CSSStyle::SetAlignContent},
    {"alignContent", &CSSStyle::SetAlignContent},
    {"order", &CSSStyle::SetOrder}};

/**
 * default style values
 */
const Length CSS_STYLE_DEFAULT_WIDTH_ = Length(base::kLengthAuto);
const Length CSS_STYLE_DEFAULT_HEIGHT_ = Length(base::kLengthAuto);
const Length CSS_STYLE_DEFAULT_MIN_WIDTH_ = Length();
const Length CSS_STYLE_DEFAULT_MIN_HEIGHT_ = Length();
const Length CSS_STYLE_DEFAULT_MAX_WIDTH_ = Length(base::kLengthAuto);
const Length CSS_STYLE_DEFAULT_MAX_HEIGHT_ = Length(base::kLengthAuto);
const Length CSS_STYLE_DEFAULT_PADDING_TOP_ = Length();
const Length CSS_STYLE_DEFAULT_PADDING_LEFT_ = Length();
const Length CSS_STYLE_DEFAULT_PADDING_BOTTOM_ = Length();
const Length CSS_STYLE_DEFAULT_PADDING_RIGHT_ = Length();
const Length CSS_STYLE_DEFAULT_MARGIN_TOP_ = Length();
const Length CSS_STYLE_DEFAULT_MARGIN_LEFT_ = Length();
const Length CSS_STYLE_DEFAULT_MARGIN_BOTTOM_ = Length();
const Length CSS_STYLE_DEFAULT_MARGIN_RIGHT_ = Length();
const float CSS_STYLE_DEFAULT_BORDER_TOP_ = .0f;
const float CSS_STYLE_DEFAULT_BORDER_LEFT_ = .0f;
const float CSS_STYLE_DEFAULT_BORDER_BOTTOM_ = .0f;
const float CSS_STYLE_DEFAULT_BORDER_RIGHT_ = .0f;

const PositionType CSS_STYLE_DEFAULT_POSITION_ = kPositionRelative;
const DisplayType CSS_STYLE_DEFAULT_DISPLAY_ = kDisplayFlex;

const Length CSS_STYLE_DEFAULT_FLEX_BASIS_ = Length(base::kLengthAuto);
const float CSS_STYLE_DEFAULT_FLEX_GROW_ = .0f;
const float CSS_STYLE_DEFAULT_FLEX_SHRINK_ = .0f;
const FlexDirectionType CSS_STYLE_DEFAULT_FLEX_DIRECTION_ = kFlexDirectionRow;
const FlexWrapType CSS_STYLE_DEFAULT_FLEX_WRAP_ = kFlexWrapNoWrap;
const JustifyContentType CSS_STYLE_DEFAULT_JUSTIFY_CONTENT_ =
    kJustifyContentFlexStart;
const AlignItemsType CSS_STYLE_DEFAULT_ALIGN_ITEMS_ = kAlignItemsStretch;
const AlignSelfType CSS_STYLE_DEFAULT_ALIGN_SELF_ = kAlignSelfAuto;
const AlignContentType CSS_STYLE_DEFAULT_ALIGN_CONTENT_ =
    kAlignContentFlexStart;
const int CSS_STYLE_DEFAULT_ORDER_ = 0;

CSSStyle::CSSStyle() {
  ResetAllStyles();
}

CSSStyle::~CSSStyle() {}

void CSSStyle::ResetAllStyles() {
  width_ = CSS_STYLE_DEFAULT_WIDTH_;
  height_ = CSS_STYLE_DEFAULT_HEIGHT_;
  min_width_ = CSS_STYLE_DEFAULT_MIN_WIDTH_;
  min_height_ = CSS_STYLE_DEFAULT_MIN_HEIGHT_;
  max_width_ = CSS_STYLE_DEFAULT_MAX_WIDTH_;
  max_height_ = CSS_STYLE_DEFAULT_MAX_HEIGHT_;
  padding_top_ = CSS_STYLE_DEFAULT_PADDING_TOP_;
  padding_left_ = CSS_STYLE_DEFAULT_PADDING_LEFT_;
  padding_bottom_ = CSS_STYLE_DEFAULT_PADDING_BOTTOM_;
  padding_right_ = CSS_STYLE_DEFAULT_PADDING_RIGHT_;
  margin_top_ = CSS_STYLE_DEFAULT_MARGIN_TOP_;
  margin_left_ = CSS_STYLE_DEFAULT_MARGIN_LEFT_;
  margin_bottom_ = CSS_STYLE_DEFAULT_MARGIN_BOTTOM_;
  margin_right_ = CSS_STYLE_DEFAULT_MARGIN_RIGHT_;
  border_top_ = CSS_STYLE_DEFAULT_BORDER_TOP_;
  border_left_ = CSS_STYLE_DEFAULT_BORDER_LEFT_;
  border_bottom_ = CSS_STYLE_DEFAULT_BORDER_BOTTOM_;
  border_right_ = CSS_STYLE_DEFAULT_BORDER_RIGHT_;

  // display style
  position_ = CSS_STYLE_DEFAULT_POSITION_;
  display_ = CSS_STYLE_DEFAULT_DISPLAY_;

  // flex style
  flex_basis_ = CSS_STYLE_DEFAULT_FLEX_BASIS_;
  flex_grow_ = CSS_STYLE_DEFAULT_FLEX_GROW_;
  flex_shrink_ = CSS_STYLE_DEFAULT_FLEX_SHRINK_;
  flex_direction_ = CSS_STYLE_DEFAULT_FLEX_DIRECTION_;
  flex_wrap_ = CSS_STYLE_DEFAULT_FLEX_WRAP_;
  justify_content_ = CSS_STYLE_DEFAULT_JUSTIFY_CONTENT_;
  align_items_ = CSS_STYLE_DEFAULT_ALIGN_ITEMS_;
  align_self_ = CSS_STYLE_DEFAULT_ALIGN_SELF_;
  align_content_ = CSS_STYLE_DEFAULT_ALIGN_CONTENT_;
  order_ = CSS_STYLE_DEFAULT_ORDER_;
}

void CSSStyle::SetStyle(const std::string& name,
                        const std::string& value,
                        bool reset) {
  StyleFuncMap::iterator iter = style_setters_.find(name);
  if (iter == style_setters_.end()) {
    return;
  }
  StyleFunc func = iter->second;
  (this->*func)(value, reset);
}

bool CSSStyle::IsMainAxisHorizontal() const {
  return flex_direction_ == kFlexDirectionRow ||
         flex_direction_ == kFlexDirectionRowReverse;
}

bool CSSStyle::IsMainAxisReverse() const {
  return flex_direction_ == kFlexDirectionRowReverse ||
         flex_direction_ == kFlexDirectionColumnReverse;
}

void CSSStyle::Print() const {
  std::cout << "CSSStyle:" << std::endl;
  std::cout << "width: " << width_.value() << std::endl;
  std::cout << "height: " << height_.value() << std::endl;
}

void CSSStyle::SetWidth(const std::string& value, bool reset) {
  if (reset) {
    width_ = CSS_STYLE_DEFAULT_WIDTH_;
  }
  base::ToLength(value, width_);
}

void CSSStyle::SetHeight(const std::string& value, bool reset) {
  if (reset) {
    height_ = CSS_STYLE_DEFAULT_HEIGHT_;
  }
  base::ToLength(value, height_);
}

void CSSStyle::SetMinWidth(const std::string& value, bool reset) {
  if (reset) {
    min_width_ = CSS_STYLE_DEFAULT_MIN_WIDTH_;
  }
  base::ToLength(value, min_width_);
}

void CSSStyle::SetMinHeight(const std::string& value, bool reset) {
  if (reset) {
    min_height_ = CSS_STYLE_DEFAULT_MIN_HEIGHT_;
  }
  base::ToLength(value, min_height_);
}

void CSSStyle::SetMaxWidth(const std::string& value, bool reset) {
  if (reset) {
    max_width_ = CSS_STYLE_DEFAULT_MAX_WIDTH_;
  }
  base::ToLength(value, max_width_);
}

void CSSStyle::SetMaxHeight(const std::string& value, bool reset) {
  if (reset) {
    max_height_ = CSS_STYLE_DEFAULT_MAX_HEIGHT_;
  }
  base::ToLength(value, max_height_);
}

void CSSStyle::SetPadding(const std::string& value, bool reset) {
  if (reset) {
    padding_top_ = CSS_STYLE_DEFAULT_PADDING_TOP_;
    padding_left_ = CSS_STYLE_DEFAULT_PADDING_LEFT_;
    padding_bottom_ = CSS_STYLE_DEFAULT_PADDING_BOTTOM_;
    padding_right_ = CSS_STYLE_DEFAULT_PADDING_RIGHT_;
  }
  std::vector<std::string> values;
  base::SplitString(value, ' ', values);
  if (values.size() == 1) {
    SetPaddingTop(values[0]);
    SetPaddingLeft(values[0]);
    SetPaddingBottom(values[0]);
    SetPaddingRight(values[0]);
  } else if (values.size() == 2) {
    SetPaddingTop(values[0]);
    SetPaddingLeft(values[1]);
    SetPaddingBottom(values[0]);
    SetPaddingRight(values[1]);
  } else if (values.size() == 3) {
    SetPaddingTop(values[0]);
    SetPaddingLeft(values[1]);
    SetPaddingBottom(values[2]);
    SetPaddingRight(values[1]);
  } else if (values.size() > 3) {
    SetPaddingTop(values[0]);
    SetPaddingLeft(values[3]);
    SetPaddingBottom(values[2]);
    SetPaddingRight(values[1]);
  }
}

void CSSStyle::SetPaddingTop(const std::string& value, bool reset) {
  if (reset) {
    padding_top_ = CSS_STYLE_DEFAULT_PADDING_TOP_;
  }
  base::ToLength(value, padding_top_);
}

void CSSStyle::SetPaddingLeft(const std::string& value, bool reset) {
  if (reset) {
    padding_left_ = CSS_STYLE_DEFAULT_PADDING_LEFT_;
  }
  base::ToLength(value, padding_left_);
}

void CSSStyle::SetPaddingBottom(const std::string& value, bool reset) {
  if (reset) {
    padding_bottom_ = CSS_STYLE_DEFAULT_PADDING_BOTTOM_;
  }
  base::ToLength(value, padding_bottom_);
}

void CSSStyle::SetPaddingRight(const std::string& value, bool reset) {
  if (reset) {
    padding_right_ = CSS_STYLE_DEFAULT_PADDING_RIGHT_;
  }
  base::ToLength(value, padding_right_);
}

void CSSStyle::SetMargin(const std::string& value, bool reset) {
  if (reset) {
    margin_top_ = CSS_STYLE_DEFAULT_MARGIN_TOP_;
    margin_left_ = CSS_STYLE_DEFAULT_MARGIN_LEFT_;
    margin_bottom_ = CSS_STYLE_DEFAULT_MARGIN_BOTTOM_;
    margin_right_ = CSS_STYLE_DEFAULT_MARGIN_RIGHT_;
  }
  std::vector<std::string> values;
  base::SplitString(value, ' ', values);
  if (values.size() == 1) {
    SetMarginTop(values[0]);
    SetMarginLeft(values[0]);
    SetMarginBottom(values[0]);
    SetMarginRight(values[0]);
  } else if (values.size() == 2) {
    SetMarginTop(values[0]);
    SetMarginLeft(values[1]);
    SetMarginBottom(values[0]);
    SetMarginRight(values[1]);
  } else if (values.size() == 3) {
    SetMarginTop(values[0]);
    SetMarginLeft(values[1]);
    SetMarginBottom(values[2]);
    SetMarginRight(values[1]);
  } else if (values.size() > 3) {
    SetMarginTop(values[0]);
    SetMarginLeft(values[3]);
    SetMarginBottom(values[2]);
    SetMarginRight(values[1]);
  }
}

void CSSStyle::SetMarginTop(const std::string& value, bool reset) {
  if (reset) {
    margin_top_ = CSS_STYLE_DEFAULT_MARGIN_TOP_;
  }
  base::ToLength(value, margin_top_);
}

void CSSStyle::SetMarginLeft(const std::string& value, bool reset) {
  if (reset) {
    margin_left_ = CSS_STYLE_DEFAULT_MARGIN_LEFT_;
  }
  base::ToLength(value, margin_left_);
}

void CSSStyle::SetMarginBottom(const std::string& value, bool reset) {
  if (reset) {
    margin_bottom_ = CSS_STYLE_DEFAULT_MARGIN_BOTTOM_;
  }
  base::ToLength(value, margin_bottom_);
}

void CSSStyle::SetMarginRight(const std::string& value, bool reset) {
  if (reset) {
    margin_right_ = CSS_STYLE_DEFAULT_MARGIN_RIGHT_;
  }
  base::ToLength(value, margin_right_);
}

void CSSStyle::SetBorder(const std::string& value, bool reset) {
  if (reset) {
    border_top_ = CSS_STYLE_DEFAULT_BORDER_TOP_;
    border_left_ = CSS_STYLE_DEFAULT_BORDER_LEFT_;
    border_bottom_ = CSS_STYLE_DEFAULT_BORDER_BOTTOM_;
    border_right_ = CSS_STYLE_DEFAULT_BORDER_RIGHT_;
  }
  std::vector<std::string> values;
  base::SplitString(value, ' ', values);
  if (values.size() == 1) {
    SetBorderTop(values[0]);
    SetBorderLeft(values[0]);
    SetBorderBottom(values[0]);
    SetBorderRight(values[0]);
  } else if (values.size() == 2) {
    SetBorderTop(values[0]);
    SetBorderLeft(values[1]);
    SetBorderBottom(values[0]);
    SetBorderRight(values[1]);
  } else if (values.size() == 3) {
    SetBorderTop(values[0]);
    SetBorderLeft(values[1]);
    SetBorderBottom(values[2]);
    SetBorderRight(values[1]);
  } else if (values.size() > 3) {
    SetBorderTop(values[0]);
    SetBorderLeft(values[3]);
    SetBorderBottom(values[2]);
    SetBorderRight(values[1]);
  }
}

void CSSStyle::SetBorderTop(const std::string& value, bool reset) {
  if (reset) {
    border_top_ = CSS_STYLE_DEFAULT_BORDER_TOP_;
  }
  Length length_value = Length();
  base::ToLength(value, length_value);
  border_top_ = length_value.value();
}

void CSSStyle::SetBorderLeft(const std::string& value, bool reset) {
  if (reset) {
    border_left_ = CSS_STYLE_DEFAULT_BORDER_LEFT_;
  }
  Length length_value = Length();
  base::ToLength(value, length_value);
  border_left_ = length_value.value();
}

void CSSStyle::SetBorderBottom(const std::string& value, bool reset) {
  if (reset) {
    border_bottom_ = CSS_STYLE_DEFAULT_BORDER_BOTTOM_;
  }
  Length length_value = Length();
  base::ToLength(value, length_value);
  border_bottom_ = length_value.value();
}

void CSSStyle::SetBorderRight(const std::string& value, bool reset) {
  if (reset) {
    border_right_ = CSS_STYLE_DEFAULT_BORDER_RIGHT_;
  }
  Length length_value = Length();
  base::ToLength(value, length_value);
  border_right_ = length_value.value();
}

void CSSStyle::SetPosition(const std::string& value, bool reset) {
  if (reset) {
    position_ = CSS_STYLE_DEFAULT_POSITION_;
  } else if (value.compare("relative") == 0) {
    position_ = kPositionRelative;
  } else if (value.compare("absolute") == 0) {
    position_ = kPositionAbsolute;
  } else if (value.compare("fixed") == 0) {
    position_ = kPositionFixed;
  }
}

void CSSStyle::SetDisplay(const std::string& value, bool reset) {
  if (reset) {
    display_ = CSS_STYLE_DEFAULT_DISPLAY_;
  } else if (value.compare("flex") == 0) {
    display_ = kDisplayFlex;
  } else if (value.compare("grid") == 0) {
    display_ = kDisplayGrid;
  } else if (value.compare("none") == 0) {
    display_ = kDisplayNone;
  }
}

/**
 * reference: https://www.w3.org/TR/css-flexbox-1/#flex-common
 * `flex: initial`: Equivalent to `flex: 0 1 auto`.
 * `flex: auto`: Equivalent to `flex: 1 1 auto`.
 * `flex: none`: Equivalent to `flex: 0 0 auto`.
 * `flex: <positive-number>`: Equivalent to `flex: <positive-number> 1 0`.
 */
void CSSStyle::SetFlex(const std::string& value, bool reset) {
  if (reset) {
    flex_basis_.SetTypeAndValue(base::kLengthAuto, .0f);
    flex_grow_ = .0f;
    flex_shrink_ = 1.0f;
  } else if (value.compare("auto") == 0) {
    flex_basis_.SetTypeAndValue(base::kLengthAuto, .0f);
    flex_grow_ = 1.0f;
    flex_shrink_ = 1.0f;
  } else if (value.compare("none") == 0) {
    flex_basis_.SetTypeAndValue(base::kLengthAuto, .0f);
    flex_grow_ = .0f;
    flex_shrink_ = .0f;
  } else {
    float positive_value = .0f;
    if (base::StringToFloat(value, positive_value)) {
      flex_basis_.SetTypeAndValue(base::kLengthFixed, .0f);
      flex_grow_ = positive_value;
      flex_shrink_ = 1.0f;
    }
  }
}

void CSSStyle::SetFlexBasis(const std::string& value, bool reset) {
  if (reset) {
    flex_basis_ = CSS_STYLE_DEFAULT_FLEX_BASIS_;
  }
  base::ToLength(value, flex_basis_);
}
void CSSStyle::SetFlexGrow(const std::string& value, bool reset) {
  if (reset) {
    flex_grow_ = CSS_STYLE_DEFAULT_FLEX_GROW_;
  }
  base::StringToFloat(value, flex_grow_);
}

void CSSStyle::SetFlexShrink(const std::string& value, bool reset) {
  if (reset) {
    flex_shrink_ = CSS_STYLE_DEFAULT_FLEX_SHRINK_;
  }
  base::StringToFloat(value, flex_shrink_);
}

void CSSStyle::SetFlexDirection(const std::string& value, bool reset) {
  if (reset) {
    flex_direction_ = CSS_STYLE_DEFAULT_FLEX_DIRECTION_;
  } else if (value.compare("column") == 0) {
    flex_direction_ = kFlexDirectionColumn;
  } else if (value.compare("row") == 0) {
    flex_direction_ = kFlexDirectionRow;
  } else if (value.compare("column-reverse") == 0) {
    flex_direction_ = kFlexDirectionColumnReverse;
  } else if (value.compare("row-reverse") == 0) {
    flex_direction_ = kFlexDirectionRowReverse;
  }
}

void CSSStyle::SetFlexWrap(const std::string& value, bool reset) {
  if (reset) {
    flex_wrap_ = CSS_STYLE_DEFAULT_FLEX_WRAP_;
  } else if (value.compare("nowrap") == 0) {
    flex_wrap_ = kFlexWrapNoWrap;
  } else if (value.compare("wrap") == 0) {
    flex_wrap_ = kFlexWrapWrap;
  } else if (value.compare("wrap-reverse") == 0) {
    flex_wrap_ = kFlexWrapWrapReverse;
  }
}

/**
 * The flex-flow property is a shorthand for setting the flex-direction and
 * flex-wrap properties*/
void CSSStyle::SetFlexFlow(const std::string& value, bool reset) {
  if (reset) {
    SetFlexDirection("", true);
    SetFlexWrap("", true);
    return;
  }
  std::vector<std::string> values;
  base::SplitString(value, ' ', values);
  if (values.size() > 0) {
    SetFlexDirection(values[0]);
  }
  if (values.size() > 1) {
    SetFlexWrap(values[1]);
  }
}

void CSSStyle::SetJustifyContent(const std::string& value, bool reset) {
  if (reset) {
    justify_content_ = CSS_STYLE_DEFAULT_JUSTIFY_CONTENT_;
  } else if (value.compare("flex-start") == 0) {
    justify_content_ = kJustifyContentFlexStart;
  } else if (value.compare("flex-end") == 0) {
    justify_content_ = kJustifyContentFlexEnd;
  } else if (value.compare("center") == 0) {
    justify_content_ = kJustifyContentCenter;
  } else if (value.compare("space-between") == 0) {
    justify_content_ = kJustifyContentSpaceBetween;
  } else if (value.compare("space-around") == 0) {
    justify_content_ = kJustifyContentSpaceAround;
  }
}

void CSSStyle::SetAlignItems(const std::string& value, bool reset) {
  if (reset) {
    align_items_ = CSS_STYLE_DEFAULT_ALIGN_ITEMS_;
  } else if (value.compare("flex-start") == 0) {
    align_items_ = kAlignItemsFlexStart;
  } else if (value.compare("center") == 0) {
    align_items_ = kAlignItemsCenter;
  } else if (value.compare("flex-end") == 0) {
    align_items_ = kAlignItemsFlexEnd;
  } else if (value.compare("stretch") == 0) {
    align_items_ = kAlignItemsStretch;
  }
}

void CSSStyle::SetAlignSelf(const std::string& value, bool reset) {
  if (reset) {
    align_self_ = CSS_STYLE_DEFAULT_ALIGN_SELF_;
  } else if (value.compare("auto") == 0) {
    align_self_ = kAlignSelfAuto;
  } else if (value.compare("flex-start") == 0) {
    align_self_ = kAlignSelfFlexStart;
  } else if (value.compare("center") == 0) {
    align_self_ = kAlignSelfCenter;
  } else if (value.compare("flex-end") == 0) {
    align_self_ = kAlignSelfFlexEnd;
  } else if (value.compare("stretch") == 0) {
    align_self_ = kAlignSelfStretch;
  }
}

void CSSStyle::SetAlignContent(const std::string& value, bool reset) {
  if (reset) {
    align_content_ = CSS_STYLE_DEFAULT_ALIGN_CONTENT_;
    if (value.compare("flex-start") == 0) {
      align_content_ = kAlignContentFlexStart;
    } else if (value.compare("center") == 0) {
      align_content_ = kAlignContentCenter;
    } else if (value.compare("flex-end") == 0) {
      align_content_ = kAlignContentFlexEnd;
    } else if (value.compare("stretch") == 0) {
      align_content_ = kAlignContentStretch;
    } else if (value.compare("space-between") == 0) {
      align_content_ = kAlignContentSpaceBetween;
    } else if (value.compare("space-around") == 0) {
      align_content_ = kAlignContentSpaceAround;
    }
  }
}

void CSSStyle::SetOrder(const std::string& value, bool reset) {
  if (reset) {
    order_ = CSS_STYLE_DEFAULT_ORDER_;
  }
  int64_t order = 0;
  if (base::StringToInt(value, order)) {
    order_ = order;
  }
}

}  // namespace starlight