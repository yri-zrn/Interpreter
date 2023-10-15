#include "Tokenizer.hpp"

namespace Interpreter {

namespace Lexer {

Tokens Tokenize(const char* expression) {
    Tokenizer tokenizer{ expression };
    tokenizer.Tokenize();
    return tokenizer.Result();
}

void Tokenizer::Tokenize() {
    while (!EndOfExpression()) {
        if (is_number(m_CurrentSymbol)) {
            ScanNumber();
        }
        else if (is_operator(m_CurrentSymbol)) {
            ScanOperator();
        }
        else {
            
            MoveNext();
        }
    }
}

void Tokenizer::ScanNumber() {
    float number = 0.0f;
    bool number_is_integer = true;
    float mantissa_rank = 1.0f;

    while (is_number(m_CurrentSymbol) && !EndOfExpression()) {
        if (is_digit(m_CurrentSymbol)) {
            if (number_is_integer) {
                number *= 10.0f;
            }
            else {
                mantissa_rank *= 10.0f;
            }
            number += to_digit(m_CurrentSymbol) / mantissa_rank;
        }
        else if (is_dot(m_CurrentSymbol)) {
            number_is_integer = false;
        }

        MoveNext();
    }

    m_Result.emplace_back(number);
}

void Tokenizer::ScanOperator() {
    m_Result.emplace_back(static_cast<Operator>(m_CurrentSymbol));
    MoveNext();
}

void Tokenizer::MoveNext() {
    ++m_Current;
    m_CurrentSymbol = *m_Current;
}

bool Tokenizer::EndOfExpression() {
    return m_Current == m_End;
}

/* UTILS */

bool Tokenizer::is_operator(char ch) {
    return ch == '+' || ch == '-'
        || ch == '*' || ch == '/'
        || ch == '(' || ch == ')';
}

bool Tokenizer::is_digit(char ch) {
    return ch == '0' || ch == '1' || ch == '2' || ch == '3' || ch == '4'
        || ch == '5' || ch == '6' || ch == '7' || ch == '8' || ch == '9';
}

float Tokenizer::to_digit(char ch) {
    if (ch == '0') return 0.0f;
    if (ch == '1') return 1.0f;
    if (ch == '2') return 2.0f;
    if (ch == '3') return 3.0f;
    if (ch == '4') return 4.0f;
    if (ch == '5') return 5.0f;
    if (ch == '6') return 6.0f;
    if (ch == '7') return 7.0f;
    if (ch == '8') return 8.0f;
    if (ch == '9') return 9.0f;
    return -1.0f;
}

bool Tokenizer::is_dot(char ch) {
    return ch == '.' || ch == ',';
}

bool Tokenizer::is_number(char ch) {
    return is_digit(ch) || is_dot(ch);
}

} // namespace Lexer

} // namespace Interpreter