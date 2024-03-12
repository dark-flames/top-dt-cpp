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

class CompareVisitor: public ValueVisitor<Equality> {
private:
    Value* rhs_value;
    EvalVisitor* evaluator;

    Equality try_rhs_eta_conv_or(value::Value& node, Equality equality);

    Equality compare_as_lambda(
        value::Value& lhs,
        value::Value& rhs,
        bool is_level_binder = false
    );
protected:
    Equality visit_spine(value::Spine& node) override;

    Equality visit_lambda(value::Lambda& node) override;

    Equality visit_pi(value::Pi& node) override;

    Equality visit_univ(value::Univ& node) override;

    Equality visit_univ_omega(value::UnivOmega& node) override;

    Equality visit_level(value::Level& node) override;

    Equality visit_app(value::App& node) override;

    Equality visit_lpi(value::LPi& node) override;

    Equality visit_var(value::Var& node) override;

    Equality visit_llambda(value::LLambda& node) override;
public:
    CompareVisitor compare_with(Value* lhs) {
        return CompareVisitor(lhs, this->evaluator);
    }
    CompareVisitor(Value* rhs, EvalVisitor* evaluator) : rhs_value(rhs), evaluator(evaluator) {}
};

