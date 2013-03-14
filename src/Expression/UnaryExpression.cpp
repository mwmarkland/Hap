#include "UnaryExpression.h"
#include "Value.h"

#include <sstream>

using namespace std;

namespace hap {

unique_ptr<Value> UnaryExpression::eval(Environment& environment) const {
  if (operator_.unary)
    return operator_.unary(environment, expression);
  ostringstream message;
  message << "unimplemented unary operator " << operator_;
  throw runtime_error(message.str());
}

void UnaryExpression::write(ostream& stream) const {
  stream << '(' << operator_ << ' ';
  expression->write(stream);
  stream << ')';
}

}