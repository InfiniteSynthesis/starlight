// Copyright 2020 Infinite Synthesis(T.C.V.). All rights reserved.

#ifndef STARLIGHT_BASE_LENGTH_H_
#define STARLIGHT_BASE_LENGTH_H_

#ifdef __cplusplus
namespace base {
#endif

enum LengthType { kLengthFixed, kLengthPercentage, kLengthAuto };

struct Length {
 public:
  Length() : type_(kLengthFixed), value_(.0f) {}
  Length(LengthType type, float value = .0f) : type_(type), value_(value) {}

  // getter
  LengthType type() const { return type_; }
  float value() const { return value_; }

  // setter
  void SetType(LengthType type) { type_ = type; }
  void SetValue(float value) { value_ = value; }
  void SetTypeAndValue(LengthType type, float value) {
    type_ = type;
    value_ = value;
  }

  // judge type
  inline bool IsFixed() const { return type_ == kLengthFixed; }
  inline bool IsPercentage() const { return type_ == kLengthPercentage; }
  inline bool IsAuto() const { return type_ == kLengthAuto; }

  float GetComputedValue(float parentValue) const {
    switch (type_) {
      case kLengthFixed:
        return value_;
      case kLengthPercentage:
        return (value_ * parentValue / 100.0f);
      case kLengthAuto:
        return .0f;
    }
    return .0f;
  }

 private:
  LengthType type_;
  float value_;
};

#ifdef __cplusplus
}
#endif

#endif