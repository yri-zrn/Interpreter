#pragma once

#include "Parser.hpp"

namespace Interpreter {

namespace Evaluator {

float Evaluate(const Tokens& tokens);

class StackEvaluator {
public:
    StackEvaluator(const Tokens& tokens)
    : m_Current(tokens.cbegin())
    , m_End(tokens.cend())
    { }

    void Evaluate();
    float Result();

private:
    Tokens::const_iterator m_Current;
    Tokens::const_iterator m_End;
    
    TokensStack m_NumbersStack;

private:
    void EvaluateToken();
    void EvaluateNumber();
    void EvaluateOperator();
};

} // namespace Evaluator

} // namespace Interpreter
