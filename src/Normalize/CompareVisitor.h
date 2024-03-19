#pragma once

#include <Value/ValueVisitor.h>
#include <Normalize/EvalVisitor.h>
#include "EvalVisitor.h"

class EvalVisitor;

enum class Equality : int {
    Eq,
    ConditionallyEq,
    UnEq
};

class CompareVisitor : public ValueVisitor<Equality> {
private:
    Value* rhs_value;
    EvalVisitor& evaluator;

    Equality try_rhs_eta_conv_or(Value& node, Equality equality);

    Equality compare_as_lambda(
        value::Value& lhs,
        value::Value& rhs,
        bool is_level_binder = false
    );

protected:
    Equality visit_spine(value::Spine& node) final;

    Equality visit_lambda(value::Lambda& node) final;

    Equality visit_pi(value::Pi& node) final;

    Equality visit_univ(value::Univ& node) final;

    Equality visit_univ_omega(value::UnivOmega& node) final;

    Equality visit_level(value::Level& node) final;

    Equality visit_app(value::App& node) final;

    Equality visit_lpi(value::LPi& node) final;

    Equality visit_var(value::Var& node) final;

    Equality visit_llambda(value::LLambda& node) final;

public:
    CompareVisitor compare_with(Value* rhs) {
        return CompareVisitor(rhs, this->evaluator);
    }

    CompareVisitor& set_rhs(Value* rhs) {
        this->rhs_value = rhs;
        return *this;
    }

    CompareVisitor(Value* rhs, EvalVisitor& evaluator) : rhs_value(rhs), evaluator(evaluator) {}
};

