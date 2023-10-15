#include "Token.hpp"

namespace Interpreter {

std::ostream& operator<<(std::ostream& os, const Token token) {
    if (token.m_Type == TokenType::Number)
        os << token.m_Number;
    if (token.m_Type == TokenType::Operator)
        os << token.m_Operator;
    return os;
}

bool operator==(const Token& lhs, const Token& rhs) {
    if (lhs.GetType() == TokenType::Number 
     && rhs.GetType() == TokenType::Number) {
        return lhs.GetNumber() == rhs.GetNumber();
    }
    if (lhs.GetType() == TokenType::Operator 
     && rhs.GetType() == TokenType::Operator) {
        return lhs.GetOperator() == rhs.GetOperator();
    }
    return false;
}

bool operator!=(const Token& lhs, const Token& rhs) {
    return !(lhs == rhs);
}
} // namespace Interpreter