#pragma once

#include <ostream>

namespace Interpreter {

enum  Operator: char {
    Plus     = '+',
    Minus    = '-',
    Mul      = '*',
    Div      = '/',
    RParen = ')',
    LParen = '('
};

inline std::ostream& operator<<(std::ostream& os, const Operator op) {
    os << static_cast<char>(op);
    return os;
}

enum class TokenType {
    Operator,
    Number
};

class Token {
public:
    Token(Operator op) 
    : m_Type{TokenType::Operator}
    , m_Operator{op} {}

    Token(float num)
    : m_Type{TokenType::Number}
    , m_Number{num} {}

    TokenType GetType() const { return m_Type; };
    Operator GetOperator() const { return m_Operator; };
    float GetNumber() const { return m_Number; };

    friend std::ostream& operator<<(std::ostream& os, const Token token);
    friend bool operator==(const Token& lhs, const Token& rhs);
    friend bool operator!=(const Token& lhs, const Token& rhs);
private:
    TokenType m_Type;
    union {
        Operator m_Operator;
        float m_Number;
    };
};

} // namespace Interpreter