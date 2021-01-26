#pragma once

#include "Expression.h"

class Number final : public Expression
{
public:
    Number() : value_() {};
    explicit Number(const int& value) : value_(value) {};

    std::shared_ptr<Expression> diff(const std::string_view) const override { return std::make_shared<Number>(0); }

    std::string tostring() const override { return std::to_string(value_); }

    double evaluate(double) const override { return value_; }

    bool contains_var(const std::string_view) const override { return false; }

private:
    int value_;
};
