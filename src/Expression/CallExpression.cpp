#include "CallExpression.h"
#include "FunExpression.h"
#include "Value.h"

#include "unique_cast.h"

#include <ostream>

using namespace std;

namespace hap {

CallExpression::CallExpression
  (unique_ptr<Expression> function,
   vector<unique_ptr<Expression>>&& expressions)
  : function(move(function)),
    expressions(move(expressions)) {}

unique_ptr<Value>CallExpression::eval(Environment& environment) const {
  auto value(function->eval(environment));
  value->assert_type(Value::FUNCTION);
  const auto function(static_unique_cast<FunExpression>(move(value)));
  return function->call(expressions);
}

void CallExpression::write(ostream& stream) const {
  function->write(stream);
  stream << "(";
  for (const auto& expression : expressions) {
    expression->write(stream);
    stream << ", ";
  }
  stream << ")";
}

}