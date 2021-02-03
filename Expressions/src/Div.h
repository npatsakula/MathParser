#pragma once

#include "BinaryExpression.h"
#include "Mul.h"
#include "Negate.h"
#include "Number.h"
#include "Sub.h"

class Div final : public BinaryExpression
{
public:
    Div(std::shared_ptr<Expression> lhs, std::shared_ptr<Expression> rhs)
        : BinaryExpression(std::move(lhs), std::move(rhs)) {}

    std::shared_ptr<Expression> diff(const std::string_view var) const override {
        // clang-format off
        if (lhs_->contains_var(var) && rhs_->contains_var(var)) {
            return std::make_shared<Div>(
                std::make_shared<Sub>(
                    std::make_shared<Mul>(lhs_->diff(var), rhs_),
                    std::make_shared<Mul>(lhs_, rhs_->diff(var))
                ),
                std::make_shared<Mul>(rhs_, rhs_));
        }
        // rhs is just a coefficient
        if (lhs_->contains_var(var) && !rhs_->contains_var(var)) {
            return std::make_shared<Mul>(
                std::make_shared<Div>(std::make_shared<Number>(1), rhs_),
                lhs_->diff(var));
        }
        // reciprocal rule
        if (!lhs_->contains_var(var) && rhs_->contains_var(var)) {
            return std::make_shared<Negate>(
                std::make_shared<Mul>(
                    lhs_,
                    std::make_shared<Div>(
                        rhs_->diff(var),
                        std::make_shared<Mul>(rhs_, rhs_)
                )));
        }
        // clang-format on
        return std::make_shared<Number>(0);
    }

    std::string to_string() const override { return fmt::format("({} / {})", lhs_->to_string(), rhs_->to_string()); }

    std::string to_expr_string() const override { return fmt::format("div({}, {})", lhs_->to_expr_string(), rhs_->to_expr_string()); }

    double evaluate(double x) const override { return lhs_->evaluate(x) / rhs_->evaluate(x); }
};
