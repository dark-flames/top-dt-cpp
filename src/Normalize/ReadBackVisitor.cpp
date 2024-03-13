#include <Normalize/ReadBackVisitor.h>

using namespace std;

term::Idx level_to_index(unsigned int c, value::Lvl l) {
    return c - l - 1;
}

TermPtr ReadBackVisitor::visit_var(value::Var& node) {
    auto root = term::var(level_to_index(this->evaluator.env_size(), node.l));

    if (node.spine->ty() == value::ValueTy::Spine) {
        return root;
    } else {
        auto spine_visitor = SpineVisitor(this, root);

        return spine_visitor.visit(*node.spine);
    }
}

TermPtr ReadBackVisitor::visit_lambda(value::Lambda& node) {
    auto var = this->evaluator.bind_in_place();

    auto body = this->evaluator.eval_closure(node.body, std::move(var));
    auto body_term = this->visit(*body);

    this->evaluator.pop_variable();

    return term::lambda(node.name, std::move(body_term));
}

TermPtr ReadBackVisitor::visit_llambda(value::LLambda& node) {
    auto var = this->evaluator.bind_in_place(true);

    auto body = this->evaluator.eval_closure(node.body, std::move(var));
    auto body_term = this->visit(*body);

    this->evaluator.pop_variable();

    return term::l_lambda(node.name, std::move(body_term));
}

TermPtr ReadBackVisitor::visit_pi(value::Pi& node) {
    auto domain_term = this->visit(*node.domain);

    auto var = this->evaluator.bind_in_place();
    auto codomain = this->evaluator.eval_closure(node.codomain, std::move(var));
    auto codomain_term = this->visit(*codomain);
    this->evaluator.pop_variable();

    return term::pi(node.name, std::move(domain_term), std::move(codomain_term));
}

TermPtr ReadBackVisitor::visit_lpi(value::LPi& node) {
    auto var = this->evaluator.bind_in_place(true);
    auto codomain = this->evaluator.eval_closure(node.codomain, std::move(var));
    auto codomain_term = this->visit(*codomain);
    this->evaluator.pop_variable();

    return term::l_pi(node.name, std::move(codomain_term));
}

TermPtr ReadBackVisitor::visit_level(value::Level& node) {
    auto first = true;
    TermPtr result;
    for (auto pair : node.m) {
        auto item = term::l_plus_nat(
            term::l_var(level_to_index(this->evaluator.env_size(), pair.first)),
            pair.second
        );

        if (first) {
            result = std::move(item);
            first = false;
        } else {
            result = term::l_max(std::move(result), std::move(item));
        }
    }

    if (first) {
        return term::l_nat(node.pure);
    } else {
        return term::l_plus_nat(std::move(result), node.pure);
    }
}

TermPtr ReadBackVisitor::visit_univ(value::Univ& node) {
    return term::univ(
        this->visit(*node.level)
    );
}

TermPtr ReadBackVisitor::visit_univ_omega(value::UnivOmega& node) {
    return term::univ_omega();
}


TermPtr SpineVisitor::visit_spine(value::Spine& node) {
    return std::move(this->root);
}

TermPtr SpineVisitor::visit_app(value::App& node) {
    return term::app(
        this->visit(*node.next),
        this->read_back_visitor->visit(*node.parameter)
    );
}

