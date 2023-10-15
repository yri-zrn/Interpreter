#pragma once
#include <stack>

#include "Tokenizer.hpp"

namespace Interpreter {

using TokensStack = std::stack<Token>;

int PrecedenceOf(Operator op);

namespace Parser {

Tokens Parse(const Tokens& expression);

class ShuntingYardParser {
public:

    ShuntingYardParser(const Tokens& tokens) 
    : m_Current(tokens.cbegin())
    , m_End(tokens.cend())
    { }

    void Parse();
    const Tokens& Result() const;

private:
    Tokens::const_iterator m_Current;
    Tokens::const_iterator m_End;

    TokensStack m_OperatorsStack;
    Tokens m_Output;

private:
    void ParseNumber();
    void ParseOperator();
    
    void PushToOutput();
    template<typename Delegate>
    void PopToOutputUntil(Delegate whenToEnd);
    void PushToStack();
    void PopLeftParenthesis();
    
    bool OperatorWithLessPrecedenceOnTop() const;
    bool LeftParenthesisOnTop() const;
    bool StackHasNoOperators() const;
};

} // namespaace Parser

} // namespace Interpreter