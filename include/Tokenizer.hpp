#pragma once
#include <vector>
#include <cstring>

#include "Token.hpp"

namespace Interpreter {

using Tokens = std::vector<Token>;

namespace Lexer {

Tokens Tokenize(const char* expression);

class Tokenizer {
public:
    Tokenizer(const char* expr)
    : m_Current(expr)
    , m_End(expr + strlen(expr))
    , m_CurrentSymbol(*m_Current)
    { }
    
    void Tokenize();
    const Tokens& Result() const { return m_Result; };

public:
    void ScanNumber();
    void ScanOperator();
    void MoveNext();
    bool EndOfExpression();

private:
    Tokens m_Result;
    const char* m_Current;
    const char* m_End;
    char m_CurrentSymbol;

private:
    bool  is_operator(char ch);
    bool  is_digit(char ch);
    float to_digit(char ch);
    bool  is_dot(char ch);
    bool  is_number(char ch);

};

} // namespace Lexer

} // namespace Interpreter