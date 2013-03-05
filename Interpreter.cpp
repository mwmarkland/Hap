#include "Interpreter.h"
#include "Statement.h"

#include <iostream>

using namespace std;

namespace hap {

void Interpreter::run(unique_ptr<Statement> statement) {
  cout << *statement << '\n';
  statement->exec(global);
}

}
