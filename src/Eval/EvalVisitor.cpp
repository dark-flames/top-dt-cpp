#include <Eval/EvalVisitor.h>
#include <Value/Helper.h>

using namespace std;


Environment EvalVisitor::bind(bool is_level) {
    ValuePtr v;
    if (is_level) {
        v = value::l_var(this->env.size());
    } else {
        v = value::var(this->env.size());
    }

    return this->env.push(v);
}


ValuePtr EvalVisitor::eval_closure(value::Closure& closure, ValuePtr& value){
    auto new_env = closure.env.push(value);
    auto visitor = EvalVisitor(new_env, this->state);
    return visitor.visit(closure.term);
}

ValuePtr EvalVisitor::visit_var(NodePtr<term::Var>& node) {
    return this->env.find(this->env.size() - 1 - node->i);
}

ValuePtr EvalVisitor::visit_lvar(NodePtr<term::LVar>& node) {
    return this->env.find(this->env.size() - 1 - node->i);
}


ValuePtr EvalVisitor::visit_lambda(NodePtr<term::Lambda>& node) {
    auto new_env = this->bind();
    auto closure = value::Closure(new_env, node->body);

    return value::lambda(closure);
}

ValuePtr EvalVisitor::visit_llambda(NodePtr<term::LLambda>& node) {
    auto new_env = this->bind(true);
    auto closure = value::Closure(new_env, node->body);

    return value::l_lambda(closure);
}

ValuePtr EvalVisitor::visit_app(NodePtr<term::App>& node) {
    auto f = this->visit(node->fun);
    auto p = this->visit(node->param);

    if(f->ty() == value::ValueTy::Lambda) {
        auto fun = value::specialize_value_ptr<value::Lambda>(f);
        return this->eval_closure(fun->body, p);
    } else if(f->ty() == value::ValueTy::LLambda) {
        auto fun = value::specialize_value_ptr<value::LLambda>(f);
        return this->eval_closure(fun->body, p);
    }else {
        //f must be a neutral term
        return value::neutral_app(f, p);
    }
}

ValuePtr EvalVisitor::visit_pi(NodePtr<term::Pi>& node) {
    auto domain = this->visit(node->domain);

    auto new_env = this->bind();
    auto codomain = value::Closure(new_env, node->codomain);

    return value::pi(domain, codomain);
}

ValuePtr EvalVisitor::visit_lpi(NodePtr<term::LPi>& node) {
    auto new_env = this->bind(true);
    auto codomain = value::Closure(new_env, node->codomain);

    return value::l_pi(codomain);
}

ValuePtr EvalVisitor::visit_lzero(NodePtr<term::LZero>& node) {
    return value::l_nat(0);
}

ValuePtr EvalVisitor::visit_lsuc(NodePtr<term::LSuc>& node) {
    auto inner = this->visit(node->level);

    return value::l_incr(inner, 1);
}

ValuePtr EvalVisitor::visit_lmax(NodePtr<term::LMax>& node) {
    auto l = this->visit(node->l);
    auto r = this->visit(node->r);

    return value::l_max(l, r);
}

ValuePtr EvalVisitor::visit_univ(NodePtr<term::Univ>& node) {
    auto l = this->visit(node->level);

    return value::univ(l);
}

ValuePtr EvalVisitor::visit_univ_omega(NodePtr<term::UnivOmega>& node) {
    return value::univ_omega();
}









