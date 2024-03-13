#pragma once

#include <Value/ValueVisitor.h>
#include <Normalize/EvalVisitor.h>

#include <memory>

class ReadBackState;

using ReadBackStatePtr = std::shared_ptr<ReadBackState>;

class ReadBackState {
private:
public:
    ReadBackState() {}
};

class ReadBackVisitor : public ValueVisitor<TermPtr> {
private:
    ReadBackStatePtr state;
    EvalVisitor evaluator;
protected:
    TermPtr visit_var(value::Var& node) final;

    TermPtr visit_lambda(value::Lambda& node) final;

    TermPtr visit_llambda(value::LLambda& node) final;

    TermPtr visit_pi(value::Pi& node) final;

    TermPtr visit_lpi(value::LPi& node) final;

    TermPtr visit_level(value::Level& node) final;

    TermPtr visit_univ(value::Univ& node) final;

    TermPtr visit_univ_omega(value::UnivOmega& node) final;

public:
    ReadBackVisitor(
        ReadBackStatePtr& state,
        EvalVisitor& evaluator
    ) : state(state), evaluator(evaluator) {}
};

using ReadBackVisitorPtr = std::shared_ptr<ReadBackVisitor>;

class SpineVisitor : public ValueVisitor<TermPtr> {
private:
    ReadBackVisitor* read_back_visitor;
    TermPtr root;
public:
    TermPtr visit_spine(value::Spine& node) final;

    TermPtr visit_app(value::App& node) final;

    SpineVisitor(
        ReadBackVisitor* read_back_visitor,
        TermPtr& root
    ) : read_back_visitor(read_back_visitor), root(std::move(root)) {}
};