#include "ListValue.h"

#include "UndefinedValue.h"
#include "indirect_equal.h"

#include <ostream>

using namespace std;

namespace hap {

bool ListValue::equal(const Expression& expression) const {
  if (const auto that
      = dynamic_cast<const ListValue*>(&expression)) {
    return values.size() == that->values.size()
      && std::equal
        (begin(values), end(values),
         begin(that->values), indirect_equal<Value>());
  }
  return false;
}

ListValue::ListValue(const ListValue& that) {
  for (const auto& value : that.values)
    values.push_back(shared_ptr<Value>(value->copy()));
}

std::shared_ptr<Value> ListValue::at(const int index) const {
  if (index < 0 || index >= values.size())
    return shared_ptr<Value>(new UndefinedValue());
  return values[index];
}

bool ListValue::less(const Value& that) const {
  return Value::less(that)
    || (that.type() == Value::LIST
      && values < static_cast<const ListValue&>(that).values);
}

void ListValue::write(ostream& stream) const {
  stream << "[ ";
  for (const auto& value : values)
    stream << *value << ", ";
  stream << ']';
}

}
