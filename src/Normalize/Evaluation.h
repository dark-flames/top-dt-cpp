#include <Normalize/EvalVisitor.h>
#include <Normalize/ReadBackVisitor.h>
#include <Normalize/CompareVisitor.h>

#include <iostream>

ValuePtr eval_open_term(Environment& env, TermPtr& t) {
    auto eval_state = std::make_shared<EvalState>();
    auto rb_state = std::make_shared<ReadBackState>();
    auto evaluator = EvalVisitor(env, eval_state);
    return evaluator.visit(*t);
}

TermPtr normalize_open_term(Environment& env, TermPtr& t) {
    auto eval_state = std::make_shared<EvalState>();
    auto rb_state = std::make_shared<ReadBackState>();
    auto evaluator = EvalVisitor(env, eval_state);
    auto v = evaluator.visit(*t);

    auto quoter = ReadBackVisitor(rb_state, evaluator);

    return quoter.visit(*v);
}

ValuePtr eval_term(TermPtr& t) {
    auto env = Environment();
    return eval_open_term(env, t);
}

TermPtr normalize(TermPtr& t) {
    auto env = Environment();
    return normalize_open_term(env, t);
}

Equality compare_value(ValuePtr& t, ValuePtr&  r) {
    auto eval_state = std::make_shared<EvalState>();
    auto env = Environment();
    auto evaluator = EvalVisitor(env, eval_state);


    auto comparator = CompareVisitor(r.get(), &evaluator);

    return comparator.visit(*t);
}