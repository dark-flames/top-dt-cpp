#pragma once
#include <Term/TermNode.h>
#include <Term/TermVisitor.h>

#include <Value/ValueNode.h>
#include <Value/Env.h>
#include <Value/Closure.h>
#include <Value/Helper.h>

#include <memory>


class EvalState {
public:
    EvalState() {}
};

using EvalStatePtr = std::shared_ptr<EvalState>;

class EvalVisitor : public term::TermVisitor<ValuePtr> {
private:
    Environment env;
    EvalStatePtr state;

    Environment bind(bool is_level = false);

    ValuePtr eval_closure(value::Closure& closure, ValuePtr& value);
protected:

    ValuePtr visit_lmax(NodePtr<term::LMax>& node) final;

    ValuePtr visit_lambda(NodePtr<term::Lambda>& node) final;

    ValuePtr visit_llambda(NodePtr<term::LLambda>& node) final;

    ValuePtr visit_pi(NodePtr<term::Pi>& node) final;

    ValuePtr visit_univ(NodePtr<term::Univ>& node) final;

    ValuePtr visit_lzero(NodePtr<term::LZero>& node) final;

    ValuePtr visit_univ_omega(NodePtr<term::UnivOmega>& node) final;

    ValuePtr visit_lvar(NodePtr<term::LVar>& node) final;

    ValuePtr visit_app(NodePtr<term::App>& node) final;

    ValuePtr visit_lpi(NodePtr<term::LPi>& node) final;

    ValuePtr visit_var(NodePtr<term::Var>& node) final;

    ValuePtr visit_lsuc(NodePtr<term::LSuc>& node) final;
public:
    explicit EvalVisitor(EvalStatePtr& state) : env(), state(state) {}

    EvalVisitor(Environment& env, EvalStatePtr& state) : env(env), state(state) {}
};