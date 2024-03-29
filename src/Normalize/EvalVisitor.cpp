#include <Normalize/EvalVisitor.h>
#include <Declaration/DeclarationResolver.h>

using namespace std;


Environment EvalVisitor::bind(bool is_level) {
    ValuePtr v;
    if (is_level) {
        v = value::l_var(this->env.size());
    } else {
        v = value::var(this->env.size());
    }
    auto result = this->env.push(v);
    return result;
}


ValuePtr EvalVisitor::eval_closure(value::Closure& closure, ValuePtr value) {
    auto new_env = closure.env.push(value);
    auto visitor = EvalVisitor(new_env, this->declaration_resolver);
    return visitor.visit(*closure.term);
}

ValuePtr EvalVisitor::value_application(ValuePtr f, ValuePtr p) {
    if (f->ty() == value::ValueTy::Lambda) {
        auto fun = dynamic_cast<value::Lambda*>(f.get());
        return this->eval_closure(fun->body, std::move(p));
    } else if (f->ty() == value::ValueTy::LLambda) {
        auto fun = dynamic_cast<value::LLambda*>(f.get());
        return this->eval_closure(fun->body, std::move(p));
    } else {
        //f must be a neutral term
        return value::neutral_app(std::move(f), std::move(p));
    }
}

ValuePtr EvalVisitor::value_application(Value& f, Value& p) {
    return this->value_application(f.copy(), p.copy());
}

ValuePtr EvalVisitor::visit_var(term::Var& node) {
    return this->env.find(node.i);
}

ValuePtr EvalVisitor::visit_lvar(term::LVar& node) {
    return this->env.find(node.i);
}


ValuePtr EvalVisitor::visit_lambda(term::Lambda& node) {
    return value::lambda(node.name, value::Closure(this->env.copy(), node.body->copy()));
}

ValuePtr EvalVisitor::visit_llambda(term::LLambda& node) {
    return value::l_lambda(node.name, value::Closure(this->env.copy(), node.body->copy()));
}

ValuePtr EvalVisitor::visit_app(term::App& node) {
    auto f = this->visit(*node.fun);
    auto p = this->visit(*node.param);

    return this->value_application(std::move(f), std::move(p));
}

ValuePtr EvalVisitor::visit_pi(term::Pi& node) {
    auto domain = this->visit(*node.domain);

    auto codomain = Closure(this->env.copy(), node.codomain->copy());

    return value::pi(node.name, std::move(domain), std::move(codomain));
}

ValuePtr EvalVisitor::visit_lpi(term::LPi& node) {
    return value::l_pi(node.name, Closure(this->env.copy(), node.codomain->copy()));
}

ValuePtr EvalVisitor::visit_lzero(term::LZero& node) {
    return value::l_nat(0);
}

ValuePtr EvalVisitor::visit_lsuc(term::LSuc& node) {
    return value::l_incr(this->visit(*node.level), 1);
}

ValuePtr EvalVisitor::visit_lmax(term::LMax& node) {
    return value::l_max(
        this->visit(*node.l),
        this->visit(*node.r)
    );
}

ValuePtr EvalVisitor::visit_univ(term::Univ& node) {
    return value::univ(this->visit(*node.level));
}

ValuePtr EvalVisitor::visit_univ_omega(term::UnivOmega& node) {
    return value::univ_omega();
}

ValuePtr EvalVisitor::visit_def_ref(term::DefRef& node) {
    auto decl = this->declaration_resolver.get_body_or_waiting(node.entry);
    auto copied = decl->body->copy();
    return this->visit(*copied);
}









