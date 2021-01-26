#include "Parser.h"

#include "ParserInternal.h"
#include "Stack.h"

#include <unordered_map>

namespace parser 
{
namespace internal 
{
inline namespace 
{

std::shared_ptr<Expression> token_to_expression(const ParsedToken& token)
{
    switch (token.type) {
    case TokenType::number: return std::make_shared<Number>(std::stoi(token.value));
    case TokenType::variable: return std::make_shared<Variable>(token.value);
    default: return nullptr;
    }
}

std::shared_ptr<Expression> function_token_to_expression(TokenType token_type, std::shared_ptr<Expression> param) {
    switch (token_type) {
    case TokenType::negate: return std::make_shared<Negate>(std::move(param));
    case TokenType::sin: return std::make_shared<Sin>(std::move(param));
    case TokenType::cos: return std::make_shared<Cos>(std::move(param));
    default: return nullptr;
    }
}

std::shared_ptr<Expression> token_to_expression(
    TokenType token_type,
    const std::shared_ptr<Expression>& lhs,
    const std::shared_ptr<Expression>& rhs) {
    switch (token_type) {
    case TokenType::sum: return std::make_shared<Sum>(lhs, rhs);
    case TokenType::sub: return std::make_shared<Sub>(lhs, rhs);
    case TokenType::div: return std::make_shared<Div>(lhs, rhs);
    case TokenType::mul: return std::make_shared<Mul>(lhs, rhs);
    default: return nullptr;
    }
}

bool is_operator(TokenType type) {
    switch (type) {
    case TokenType::sum:
    case TokenType::sub:
    case TokenType::div:
    case TokenType::mul:
    case TokenType::negate: return true;
    default: return false;
    }
}

bool is_unary_or_function(TokenType type) {
    switch (type) {
    case TokenType::negate:
    case TokenType::sin:
    case TokenType::cos: return true;
    default: return false;
    }
}
} // namespace
} // namespace internal


std::shared_ptr<Expression> parse(std::string input) {
    input.erase(std::remove_if(input.begin(), input.end(), ::isspace), input.end());

    //! todo check for balanced brackets
    auto tokens = internal::convert_to_reverse_notation(internal::splice_to_tokens(input));

    internal::Stack<std::shared_ptr<Expression>> temp;
    temp.reserve(tokens.size());

    for (auto& token : tokens) {
        if (is_unary_or_function(token.type)) {
            temp.push(function_token_to_expression(token.type, temp.pop()));
        } else if (is_operator(token.type) && temp.size() >= 2) {
            auto rhs = temp.pop();
            auto lhs = temp.pop();
            temp.push(token_to_expression(token.type, lhs, rhs));
        } else {
            temp.push(token_to_expression(token));
        }
    }

    return temp.pop();
}

} // namespace parser