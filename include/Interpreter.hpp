#include "Evaluator.hpp"

namespace Interpreter {

inline float Interpretate(const char* expression) {
    return Evaluator::Evaluate(Parser::Parse(Lexer::Tokenize(expression)));
}

} // namespace Interpreter