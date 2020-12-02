#pragma once

#include "BinaryExpression.h"
#include "Number.h"
#include "Sum.h"

class Mul final : public BinaryExpression
{
public:
    Mul(std::shared_ptr<Expression> lhs, std::shared_ptr<Expression> rhs)
        : BinaryExpression(std::move(lhs), std::move(rhs))
    {
    }

    std::shared_ptr<Expression> diff(const std::string_view var) const override
    {
        if (lhs_->contains_var(var) && rhs_->contains_var(var)) {
            return std::make_shared<Sum>(
                std::make_shared<Mul>(lhs_->diff(var), rhs_),
                std::make_shared<Mul>(lhs_, rhs_->diff(var))
            );
        }
        if (!lhs_->contains_var(var) && !rhs_->contains_var(var)) {
            return std::make_shared<Number>(0);
        }
        auto lhs = lhs_->contains_var(var)
            ? lhs_->diff(var)
            : lhs_;

        auto rhs = rhs_->contains_var(var)
            ? rhs_->diff(var)
            : rhs_;

        return std::make_shared<Mul>(lhs, rhs);
    }

    std::string tostring() const override
    {
        return "(" + lhs_->tostring() + " * " + rhs_->tostring() + ")";
    }

    double evaluate(double x) const override
    {
        return lhs_->evaluate(x) * rhs_->evaluate(x);
    }

};
