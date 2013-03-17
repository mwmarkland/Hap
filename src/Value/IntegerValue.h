#ifndef HAP_INTEGERVALUE_H
#define HAP_INTEGERVALUE_H

#include "Value.h"

namespace hap {

class IntegerValue : public Value {
public:
  IntegerValue(int value)
    : value(value) {}
  virtual Value::Type type() const final override {
    return Type::INTEGER;
  }
  virtual IntegerValue* copy() const final override {
    return new IntegerValue(*this);
  }
  virtual std::unique_ptr<Value> eval(Environment&) const final override;
  virtual void write(std::ostream&) const final override;
  int value;
};

}

#endif