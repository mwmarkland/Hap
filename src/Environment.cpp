#include "Environment.h"
#include "UndefinedValue.h"

#include <sstream>
#include <stdexcept>

using namespace std;

namespace hap {

Environment::Environment(Environment* const parent)
  : parent(parent) {}

void Environment::define(const string& name, unique_ptr<Value> value) {
  variables.insert(make_pair(name, move(value)));
}

Value& Environment::operator[](const string& name) {
  const auto existing(variables.find(name));
  if (existing == variables.end()) {
    if (!parent)
      return *variables.insert
        (make_pair(name, shared_ptr<Value>(new UndefinedValue())))
        .first->second;
    return (*parent)[name];
  }
  return *existing->second;
}

}
