#ifndef HAP_STATEMENT_H
#define HAP_STATEMENT_H

#include "Environment.h"

#include <memory>
#include <string>
#include <vector>

namespace hap {

class Expression;

class Statement {
public:
  virtual ~Statement();
  virtual std::unique_ptr<Value> exec(Environment&) const = 0;
  virtual void write(std::ostream&) const = 0;
protected:
  Statement() {}
private:
  Statement(const Statement&);
  Statement& operator=(const Statement&);
};

std::ostream& operator<<(std::ostream&, const Statement&);

class BlockStatement : public Statement {
public:
  BlockStatement() {}
  void push(std::unique_ptr<Statement> statement) {
    statements.push_back(std::move(statement));
  }
  virtual std::unique_ptr<Value> exec(Environment&) const override;
  virtual void write(std::ostream&) const override;
private:
  std::vector<std::unique_ptr<Statement>> statements;
};

class VarStatement : public Statement {
public:
  VarStatement(const std::string&, std::unique_ptr<Expression>);
  virtual std::unique_ptr<Value> exec(Environment&) const override;
  virtual void write(std::ostream&) const override;
private:
  std::string identifier;
  std::unique_ptr<Expression> initializer;
};

class FunStatement : public Statement {
public:
  FunStatement
    (const std::string&,
     const std::vector<std::string>&,
     std::shared_ptr<Statement>,
     const Environment&);
  virtual std::unique_ptr<Value> exec(Environment&) const override;
  virtual void write(std::ostream&) const override;
private:
  std::string identifier;
  std::vector<std::string> parameters;
  std::shared_ptr<Statement> body;
  mutable Environment local;
};

class LastStatement : public Statement {
public:
  virtual std::unique_ptr<Value> exec(Environment&) const override;
  virtual void write(std::ostream&) const override;
};

class NextStatement : public Statement {
public:
  virtual std::unique_ptr<Value> exec(Environment&) const override;
  virtual void write(std::ostream&) const override;
};

class RetStatement : public Statement {
public:
  RetStatement(std::unique_ptr<Expression>);
  virtual std::unique_ptr<Value> exec(Environment&) const override;
  virtual void write(std::ostream&) const override;
private:
  std::unique_ptr<Expression> expression;
};

class ExpressionStatement : public Statement {
public:
  ExpressionStatement(std::unique_ptr<Expression>);
  virtual std::unique_ptr<Value> exec(Environment&) const override;
  virtual void write(std::ostream&) const override;
private:
  std::unique_ptr<Expression> expression;
};

class FlowStatement : public Statement {
public:
  FlowStatement
    (const std::string&,
     std::unique_ptr<Expression>,
     std::unique_ptr<Statement>);
  virtual std::unique_ptr<Value> exec(Environment&) const = 0;
  virtual void write(std::ostream&) const override;
protected:
  std::string keyword;
  std::unique_ptr<Expression> expression;
  std::unique_ptr<Statement> statement;
};

#define FLOW_STATEMENT(NAME, KEYWORD) \
class NAME##Statement : public FlowStatement { \
public: \
  NAME##Statement \
    (std::unique_ptr<Expression>, \
     std::unique_ptr<Statement>); \
  virtual std::unique_ptr<Value> exec(Environment&) const override; \
};

FLOW_STATEMENT(If, "if")
FLOW_STATEMENT(When, "when")
FLOW_STATEMENT(Whenever, "whenever")
FLOW_STATEMENT(While, "while")
FLOW_STATEMENT(RepeatWhen, "repeat_when")
FLOW_STATEMENT(RepeatWhenever, "repeat_whenever")

#undef FLOW_STATEMENT

}

#endif
