
#include <gtest/gtest.h>

#include "Interpreter.hpp"

namespace testing {

template<class T, class ActualRange>
inline void AreEqual(std::initializer_list<T> expect, const ActualRange &actual) {
    auto actualIter = std::begin(actual);
    auto expectIter = std::begin(expect);

    size_t expected_actual = std::distance(actualIter, std::end(actual));
    size_t expected_distance = std::distance(expectIter, std::end(expect));
    
    EXPECT_EQ(expected_actual, expected_distance) << "Mismatch in size";

    for( ; expectIter != std::end(expect) 
        && actualIter != std::end(actual);
         ++expectIter, ++actualIter) {
        auto message = "Mismatch in position "
                     + std::to_string(std::distance(std::begin(expect), expectIter));
        EXPECT_EQ(*expectIter, *actualIter) << message;
    }
};

} // namespace testing

using namespace Interpreter;

Token _plus  { Operator::Plus   };
Token _minus { Operator::Minus  };
Token _mul   { Operator::Mul    };
Token _div   { Operator::Div    };
Token _lparen{ Operator::LParen };
Token _rparen{ Operator::RParen };

Token _1{ 1.0f }, _2{ 2.0f }, _3{ 3.0f }, _4{ 4.0f }, _5{ 5.0f };

//* Token tests
TEST(token_test, Should_get_type_for_operator_token) {
    Token opToken{ Operator::Plus };
    EXPECT_EQ(TokenType::Operator, opToken.GetType());
}

TEST(token_test, Should_get_type_for_number_token) {
    Token numToken{ 1.4f };
    EXPECT_EQ(1.4f, numToken.GetNumber());
}

TEST(token_test, Should_get_operator_code_from_operator_token) {
    Token token{ Operator::Plus };
    EXPECT_EQ(Operator::Plus, token);
}

TEST(token_test, Should_get_number_value_from_number_token) {
    Token token{ 1.4f };
    EXPECT_EQ(1.4f, token);
}

// Operator tests
TEST(operator_test, Should_get_same_precedence_for_operator_pairs) {
    EXPECT_EQ(PrecedenceOf(Operator::Plus), PrecedenceOf(Operator::Minus));
    EXPECT_EQ(PrecedenceOf(Operator::Mul), PrecedenceOf(Operator::Div));
}

TEST(operator_test, Should_get_greater_precedence_for_multiplicative_operators) {
    EXPECT_LT(PrecedenceOf(Operator::Plus), PrecedenceOf(Operator::Mul));
    EXPECT_LT(PrecedenceOf(Operator::Minus), PrecedenceOf(Operator::Div));
}

//* Lexer tests
TEST(lexer_test, Should_tokenize_empty_expression) {
    Tokens tokens = Lexer::Tokenize("");
    EXPECT_TRUE(tokens.empty());
}

TEST(lexer_test, Should_tokenize_single_plus_operator) {
    Tokens tokens = Lexer::Tokenize("+");
    testing::AreEqual({ _plus }, tokens);
}

TEST(lexer_test, Should_tokenize_floating_point_number) {
    Tokens tokens = Lexer::Tokenize("12.34");
    testing::AreEqual({ 12.34 }, tokens);
}

TEST(lexer_test, Should_tokenize_plus_and_number) {
    Tokens tokens = Lexer::Tokenize("+12.34");
    testing::AreEqual({ _plus, Token(12.34f) }, tokens);
}

TEST(lexer_test, Should_skip_spaces) {
    Tokens tokens = Lexer::Tokenize(" 1 +  12.34   ");
    testing::AreEqual({ _1, _plus, Token(12.34f) }, tokens);
}

TEST(lexer_test, Should_tokenize_complex_expression) {
    Tokens tokens = Lexer::Tokenize(" 12.34 + 2*3/ (4-5)");
    testing::AreEqual({Token{12.34f}, _plus, _2, _mul, _3, _div, _lparen, _4, _minus, _5, _rparen}, tokens);
}

//* Parser tests
TEST(parser_test, Should_parse_empty_list) {
    Tokens tokens = Parser::Parse({});
    EXPECT_TRUE(tokens.empty());
}

TEST(parser_test, Should_parse_single_number) {
    Tokens tokens = Parser::Parse({_1});
    testing::AreEqual({ _1 }, tokens);
}

TEST(parser_test, Should_parse_num_plus_num) {
    Tokens tokens = Parser::Parse({_1, _plus, _2});
    testing::AreEqual({ _1, _2, _plus}, tokens);
}

TEST(parser_test, Should_parse_add_add) {
    Tokens tokens = Parser::Parse({ _1, _plus, _2, _plus, _3 });
    testing::AreEqual({ _1, _2, _plus, _3, _plus }, tokens);
}

TEST(parser_test, Should_parse_add_mul) {
    //    1 + 2 * 3 
    // -> 1 2 3 * +
    Tokens tokens1 = Parser::Parse({ _1, _plus, _2, _mul, _3 });
    testing::AreEqual({ _1, _2, _3, _mul, _plus }, tokens1);

    //    1 * 2 + 3 
    // -> 1 2 * 3 +
    Tokens tokens2 = Parser::Parse({ _1, _mul, _2, _plus, _3 });
    testing::AreEqual({ _1, _2, _mul, _3, _plus }, tokens2);
}

TEST(parser_test, Should_parse_complex_expression) {
    //    1 + 2 * 3 - 4 / 5 
    // -> 1 2 3 * + 4 5 / -
    Tokens tokens = Parser::Parse({ _1, _plus, _2, _mul, _3, _minus, _4, _div, _5});
    testing::AreEqual({ _1, _2, _3, _mul, _plus, _4, _5, _div, _minus }, tokens);
}

TEST(parser_test, Should_skip_parentheses_around_number) {
    // ( 1 ) -> 1
    Tokens tokens = Parser::Parse({ _lparen, _1, _rparen });
    testing::AreEqual({ _1 }, tokens);
}

TEST(parser_test, Should_parse_expression_with_parenthesis_in_beginning) {
    //    ( 1 + 2 * 3 )
    // -> 1 2 3 * +
    Tokens tokens = Parser::Parse({ _lparen, _1, _plus, _2, _mul, _3, _rparen });
    testing::AreEqual({ _1, _2, _3, _mul, _plus }, tokens);
}

TEST(parser_test, Should_throw_expection_when_opening_parenthesis_not_found) {
    Tokens tokens = { _1, _rparen };
    EXPECT_THROW(Parser::Parse(tokens), std::logic_error);
}

TEST(parser_test, Should_throw_expection_when_closing_parenthesis_not_found) {
    Tokens tokens = { _lparen, _1 };
    EXPECT_THROW(Parser::Parse(tokens), std::logic_error);
}

TEST(parser_test, Should_parse_complex_expression_with_parentheses) {
    //    ( 1 + 2 ) * ( 3 / ( 4 - 5 ) )
    // -> 1 2 + 3 4 5 - / *
    Tokens tokens = Parser::Parse({ _lparen, _1, _plus, _2, _rparen, _mul, _lparen, _3, _div, _lparen, _4, _minus, _5, _rparen, _rparen });
    testing::AreEqual({ _1, _2, _plus, _3, _4, _5, _minus, _div, _mul }, tokens);
}

//* Evaluator tests
TEST(evaluator_test, Should_evaluate_empty_list) {
    float result = Evaluator::Evaluate({});
    EXPECT_FLOAT_EQ(result, 0.0f);
}

TEST(evaluator_test, Should_evaluate_single_number) {
    float result = Evaluator::Evaluate({ _1 });
    EXPECT_FLOAT_EQ(result, 1.0f);
}

TEST(evaluator_test, Should_evaluate_number_plus_number) {
    // 1 2 + -> 3
    float result = Evaluator::Evaluate({ _1, _2, _plus });
    EXPECT_FLOAT_EQ(result, 3.0f);
}

TEST(evaluator_test, Should_evaluate_add_and_mul) {
    // 1 2 3 * + -> 7
    float result1 = Evaluator::Evaluate({ _1, _2, 3, _mul, _plus });
    EXPECT_FLOAT_EQ(result1, 7.0f);

    // 1 2 3 + * -> 5
    float result2 = Evaluator::Evaluate({ _1, _2, _mul, 3, _plus });
    EXPECT_FLOAT_EQ(result2, 5.0f);
}

TEST(evaluator_test, Should_evaluate_complex_expression) {
    // 1 2 + 3 4 5 - / * -> 9
    float result = Evaluator::Evaluate({ _1, _2, _plus, _3, _4, _5, _minus, _div, _mul });
    EXPECT_FLOAT_EQ(result, -9.0f);
}

// Interpreter tests
TEST(interpreter_test, Should_interpretate_empty_expression) {
    float result = Interpretate("");
    EXPECT_FLOAT_EQ(result, 0.0f);
}

TEST(interpreter_test, Should_interpretate_complex_expression) {
    float result = Interpretate( "( 1.05 + 2,15 ) * ( 3,4 / ( 4.3 - 5,3 ) ) " );
    EXPECT_FLOAT_EQ(result, -10.88f);
}