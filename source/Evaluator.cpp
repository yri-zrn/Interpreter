#include "Evaluator.hpp"

namespace Interpreter {

namespace Evaluator {

float Evaluate(const Tokens& tokens) {
    StackEvaluator evaluator{ tokens };
    evaluator.Evaluate();
    return evaluator.Result();
}

void StackEvaluator::Evaluate() {
    for(; m_Current != m_End; ++m_Current) {
        EvaluateToken();
    }
}

float StackEvaluator::Result() {
    if (m_NumbersStack.empty()) return 0.0f;
    return m_NumbersStack.top().GetNumber();
}

void StackEvaluator::EvaluateToken() {
    switch (m_Current->GetType()) {
        case TokenType::Number:
            EvaluateNumber();
            break;
        case TokenType::Operator:
            EvaluateOperator();
            break;
    }
}

void StackEvaluator::EvaluateNumber() {
    m_NumbersStack.push(*m_Current);
}

void StackEvaluator::EvaluateOperator() {
    // evaluate binary operator
    float rhs = m_NumbersStack.top().GetNumber(); m_NumbersStack.pop();
    float lhs = m_NumbersStack.top().GetNumber(); m_NumbersStack.pop();
    
    switch(m_Current->GetOperator()) {
    case Operator::Plus:
        m_NumbersStack.emplace(lhs + rhs);
        break;
    case Operator::Minus:
        m_NumbersStack.emplace(lhs - rhs);
        break;
    case Operator::Mul:
        m_NumbersStack.emplace(lhs * rhs);
        break;
    case Operator::Div:
        m_NumbersStack.emplace(lhs / rhs);
        break;
    default:
        break;
    }
}

} // namespace Evaluator

} // namespace Interpreter