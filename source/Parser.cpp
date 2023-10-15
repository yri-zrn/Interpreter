#include "Parser.hpp"
#include <iostream>

namespace Interpreter {

int PrecedenceOf(Operator op) {
    return (op == Operator::Mul || op == Operator::Div) ? 1 : 0;
}

namespace Parser {

Tokens Parse(const Tokens& expression) {
    ShuntingYardParser parser{ expression };
    parser.Parse();
    return parser.Result();
}

void ShuntingYardParser::Parse() {
    // parse tokens
    
    for (; m_Current != m_End; ++m_Current) {
        switch(m_Current->GetType()) {
        case TokenType::Number:
            ParseNumber();
            break;
        case TokenType::Operator:
            ParseOperator();
            break;
        }
    }
    PopToOutputUntil([this](){
        return StackHasNoOperators(); 
    });
}

const Tokens& ShuntingYardParser::Result() const {
    return m_Output;
}


void ShuntingYardParser::ParseNumber() {
    m_Output.push_back(*m_Current);
}

void ShuntingYardParser::ParseOperator() {
    switch (m_Current->GetOperator()) {
    case Operator::LParen:
        PushToStack();
        break;
    
    case Operator::RParen:
        PopToOutputUntil([this](){ return LeftParenthesisOnTop(); });
        PopLeftParenthesis();
        break;
    
    case Operator::Plus:
    case Operator::Minus:
    case Operator::Mul:
    case Operator::Div:
        PopToOutputUntil([this](){ 
            return LeftParenthesisOnTop() 
                || OperatorWithLessPrecedenceOnTop(); 
        });
        PushToStack();
        break;
    }
}


void ShuntingYardParser::PushToOutput() {
    m_Output.push_back(*m_Current);
}

template<typename Delegate>
void ShuntingYardParser::PopToOutputUntil(Delegate whenToEnd) {
    while (!m_OperatorsStack.empty() && !whenToEnd()) {
        m_Output.push_back(m_OperatorsStack.top());
        m_OperatorsStack.pop();
    }
}

void ShuntingYardParser::PushToStack() {
    m_OperatorsStack.push(*m_Current);
}


void ShuntingYardParser::PopLeftParenthesis() {
    if (m_OperatorsStack.empty() || m_OperatorsStack.top() != Operator::LParen) {
            throw std::logic_error("Opening parenthesis not found");
    }
    m_OperatorsStack.pop();
}

bool ShuntingYardParser::OperatorWithLessPrecedenceOnTop() const {
    return PrecedenceOf(m_OperatorsStack.top().GetOperator())
         < PrecedenceOf(m_Current->GetOperator());
}

bool ShuntingYardParser::LeftParenthesisOnTop() const {
    return m_OperatorsStack.top().GetOperator() == Operator::LParen;
}

bool ShuntingYardParser::StackHasNoOperators() const {
    if (m_OperatorsStack.top() == Token(Operator::LParen)) {
        throw std::logic_error("Closing parenthesis not found");
    }
    return m_OperatorsStack.empty();
}

} // namespace Parser

} // namespace Interpreter
