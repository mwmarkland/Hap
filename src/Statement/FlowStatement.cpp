#include "FlowStatement.h"
#include "BooleanExpression.h"
#include "UndefinedExpression.h"
#include "Value.h"
#include "flow.h"
#include "unique_cast.h"

#include <ostream>

using namespace std;

namespace hap {

FlowStatement::FlowStatement
  (const string& keyword,
   unique_ptr<Expression> expression,
   unique_ptr<Statement> statement)
  : keyword(keyword),
    expression(move(expression)),
    statement(move(statement)) {}

void FlowStatement::write(ostream& stream) const {
  stream << keyword << ' ';
  expression->write(stream);
  stream << '\n';
  statement->write(stream);
}

#define FLOW_STATEMENT(NAME, KEYWORD) \
  NAME##Statement::NAME##Statement \
    (unique_ptr<Expression> expression, \
     unique_ptr<Statement> statement) \
    : FlowStatement(KEYWORD, move(expression), move(statement)) {}

FLOW_STATEMENT(If, "if")
FLOW_STATEMENT(When, "when")
FLOW_STATEMENT(Whenever, "whenever")
FLOW_STATEMENT(While, "while")
FLOW_STATEMENT(RepeatWhen, "repeat_when")
FLOW_STATEMENT(RepeatWhenever, "repeat_whenever")

#undef FLOW_STATEMENT

unique_ptr<Value> IfStatement::exec(Environment& environment) const {
  auto value(expression->eval(environment));
  value->assert_type(Value::BOOLEAN);
  auto condition(static_unique_cast<BooleanExpression>(move(value)));
  if (condition->value)
    statement->exec(environment);
  return unique_ptr<Value>(new UndefinedExpression());
}

unique_ptr<Value> WhenStatement::exec(Environment&) const {
  throw runtime_error("unimplemented when");
}

unique_ptr<Value> WheneverStatement::exec(Environment&) const {
  throw runtime_error("unimplemented whenever");
}

unique_ptr<Value> WhileStatement::exec(Environment& environment) const {
  while (true) {
    auto value(expression->eval(environment));
    value->assert_type(Value::BOOLEAN);
    auto condition(static_unique_cast<BooleanExpression>(move(value)));
    if (!condition->value)
      break;
    try {
      statement->exec(environment);
    } catch (const flow::Last&) {
      break;
    } catch (const flow::Next&) {
      continue;
    }
  }
  return unique_ptr<Value>(new UndefinedExpression());
}

unique_ptr<Value> RepeatWhenStatement::exec(Environment&) const {
  throw runtime_error("unimplemented repeat_when");
}

unique_ptr<Value> RepeatWheneverStatement::exec(Environment&) const {
  throw runtime_error("unimplemented repeat_whenever");
}

}