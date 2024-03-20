#include <TypeCheck/ExprInferVisitor.h>
#include <TypeCheck/TypeChecker.h>

using namespace std;

ValuePtr pi_univ(optional<ValuePtr> level_l, optional<ValuePtr> level_r) {
    if (level_l.has_value() && level_r.has_value()) {
        return value::univ(
            value::l_max(
                std::move(level_l.value()),
                std::move(level_r.value())
            )
        );
    } else {
        return value::univ_omega();
    }
}

[[maybe_unused]] TermAndType ExprTypeInferVisitor::visit_ref(syntax::Ref& node) {
    return this->type_checker->find_ref(node.name);
}

TermAndType ExprTypeInferVisitor::visit_app(syntax::App& node) {
    auto f_res = this->type_checker->infer_expr(*node.fun);
    auto f_term = std::move(f_res.first);
    auto f_ty = std::move(f_res.second);

    if (f_ty->ty() == ValueTy::Pi) {
        auto ty = dynamic_cast<value::Pi*>(f_ty.get());

        auto p_term = this->type_checker->check_expr(*node.param, ty->domain.get());
        auto p_value = this->type_checker->eval(*p_term);
        auto res_ty = this->type_checker->eval_closure(ty->codomain, std::move(p_value));

        return make_pair(
            term::app(std::move(f_term), std::move(p_term)),
            std::move(res_ty)
        );
    } else if (f_ty->ty() == ValueTy::LPi) {
        auto ty = dynamic_cast<value::LPi*>(f_ty.get());

        auto p_term = this->type_checker->check_level(*node.param);
        auto p_value = this->type_checker->eval(*p_term);
        auto res_ty = this->type_checker->eval_closure(ty->codomain, std::move(p_value));

        return make_pair(
            term::app(std::move(f_term), std::move(p_term)),
            std::move(res_ty)
        );
    } else {
        auto e = ApplyNonPi(node.fun->copy(), f_ty->copy());
        this->type_checker->throw_err(e);
        throw ImpossibleException("");
    }
}

TermAndType ExprTypeInferVisitor::visit_pi(syntax::Pi& node) {
    if (node.domain->ty() == SyntaxTy::Level) {
        auto v = this->type_checker->bind_level(node.name);
        auto res = this->type_checker->check_ty(*node.codomain);
        this->type_checker->pop();

        return make_pair(
            term::l_pi(node.name, std::move(res.first)),
            value::univ_omega()
        );
    } else {
        auto domain_res = this->type_checker->check_ty(*node.domain);
        auto domain = std::move(domain_res.first);
        auto domain_level = std::move(domain_res.second);

        auto domain_v = this->type_checker->eval(*domain);

        auto v = this->type_checker->bind(node.name, std::move(domain_v));
        auto res = this->type_checker->check_ty(*node.codomain);
        this->type_checker->pop();

        return make_pair(
            term::pi(node.name, std::move(domain), std::move(res.first)),
            pi_univ(std::move(domain_level), std::move(res.second))
        );
    }
}

TermAndType ExprTypeInferVisitor::visit_univ(syntax::Univ& node) {
    auto level = this->type_checker->check_level(*node.level);
    auto v_level = this->type_checker->eval(*level);


    return make_pair(
        term::univ(std::move(level)),
        value::univ(value::l_incr(std::move(v_level), 1))
    );
}


