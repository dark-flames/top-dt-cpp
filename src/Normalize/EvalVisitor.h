#pragma once
#include <Term/TermNode.h>
#include <Term/TermVisitor.h>
#include <Value/Helper.h>
#include <Normalize/CompareVisitor.h>

#include <memory>

class EvalState {
public:
    EvalState() {}
};

using EvalStatePtr = std::shared_ptr<EvalState>;

class EvalVisitor : public TermVisitor<ValuePtr> {
private:
    Environment env;
    EvalStatePtr state;

    Environment bind(bool is_level = false);
protected:
    ValuePtr visit_lmax(term::LMax& node) final;

    ValuePtr visit_lambda(term::Lambda& node) final;

    ValuePtr visit_llambda(term::LLambda& node) final;

    ValuePtr visit_pi(term::Pi& node) final;

    ValuePtr visit_univ(term::Univ& node) final;

    ValuePtr visit_lzero(term::LZero& node) final;

    ValuePtr visit_univ_omega(term::UnivOmega& node) final;

    ValuePtr visit_lvar(term::LVar& node) final;

    ValuePtr visit_app(term::App& node) final;

    ValuePtr visit_lpi(term::LPi& node) final;

    ValuePtr visit_var(term::Var& node) final;

    ValuePtr visit_lsuc(term::LSuc& node) final;
public:
    ValuePtr bind_in_place(bool is_level = false) {
        this->env = this->bind(is_level);

        return this->env.last();
    }

    EvalVisitor& pop_variable() {
        auto new_env = this->env.pop();

        this->env = new_env;

        return *this;
    }

    ValuePtr eval_closure(value::Closure& closure, ValuePtr value);

    ValuePtr value_application(ValuePtr f, ValuePtr p);

    ValuePtr value_application(Value& f, Value& p);

    unsigned int env_size() const {
        return this->env.size();
    }

    explicit EvalVisitor(EvalStatePtr& state) : env(), state(state) {}

    EvalVisitor(Environment& env, EvalStatePtr& state) : env(env), state(state) {}
};