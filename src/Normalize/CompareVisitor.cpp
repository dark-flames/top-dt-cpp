#include <Normalize/CompareVisitor.h>

using namespace std;
using namespace value;

Equality merge_equality(Equality l, Equality r) {
    if (l == Equality::Eq) {
        return r;
    } else if (l == Equality::ConditionallyEq) {
        if (r == Equality::UnEq) {
            return Equality::UnEq;
        } else {
            return Equality::ConditionallyEq;
        }
    } else {
        return Equality::UnEq;
    }
}

Equality CompareVisitor::try_rhs_eta_conv_or(Value& node, Equality equality) {
    auto is_lambda = this->rhs_value->ty() == ValueTy::Lambda;
    auto is_l_lambda = this->rhs_value->ty() == ValueTy::LLambda;
    if (is_l_lambda || is_lambda) {
        auto new_visitor = this->compare_with(&node);
        auto res = new_visitor.visit(*this->rhs_value);
        return res;
    } else {
        return equality;
    }
}

Equality CompareVisitor::compare_as_lambda(
    value::Value& lhs,
    value::Value& rhs,
    bool is_level_binder
) {
    auto v = this->evaluator.bind_in_place(is_level_binder);
    auto l_body = this->evaluator.value_application(lhs, *v);
    auto r_body = this->evaluator.value_application(rhs, *v);

    auto body_visitor = this->compare_with(r_body.get());
    auto result = body_visitor.visit(*l_body);

    this->evaluator.pop_variable();

    return result;
}

Equality CompareVisitor::visit_lambda(Lambda& node) {
    return this->compare_as_lambda(node, *this->rhs_value, false);
}


Equality CompareVisitor::visit_llambda(LLambda& node) {
    return this->compare_as_lambda(node, *this->rhs_value, true);
}

Equality CompareVisitor::visit_pi(Pi& node) {
    if (this->rhs_value->ty() == ValueTy::Pi) {
        auto rhs = dynamic_cast<value::Pi*>(this->rhs_value);
        auto domain_visitor = this->compare_with(
            rhs->domain.get()
        );
        auto domain_result = domain_visitor.visit(*node.domain);


        auto v = this->evaluator.bind_in_place(false);

        auto l_codomain = this->evaluator.eval_closure(node.codomain, v->copy());
        auto r_codomain = this->evaluator.eval_closure(rhs->codomain, std::move(v));

        auto body_visitor = this->compare_with(r_codomain.get());
        auto codomain_result = body_visitor.visit(*l_codomain);
        this->evaluator.pop_variable();

        return merge_equality(domain_result, codomain_result);
    } else {
        return Equality::UnEq;
    }
}

Equality CompareVisitor::visit_lpi(LPi& node) {
    if (this->rhs_value->ty() == ValueTy::LPi) {
        auto rhs = dynamic_cast<value::LPi*>(this->rhs_value);
        auto v = this->evaluator.bind_in_place(false);

        auto l_codomain = this->evaluator.eval_closure(node.codomain, v->copy());
        auto r_codomain = this->evaluator.eval_closure(rhs->codomain, std::move(v));

        auto body_visitor = this->compare_with(r_codomain.get());
        auto result = body_visitor.visit(*l_codomain);
        this->evaluator.pop_variable();

        return result;
    } else {
        return Equality::UnEq;
    }
}

bool compare_level_map(map < DBLevel, MetaNat > &l, map < DBLevel, MetaNat > &r) {
    for (auto& pair : l) {
        auto r_pair = r.find(pair.first);
        if (r_pair == r.end() || pair.second != r_pair->second) {
            return false;
        }
    }

    return true;
}

Equality CompareVisitor::visit_level(Level& node) {
    if (this->rhs_value->ty() == ValueTy::Level) {
        auto rhs = dynamic_cast<value::Level*>(this->rhs_value);
        if (node.m.empty() && rhs->m.empty()) {
            return node.pure == rhs->pure ? Equality::Eq : Equality::UnEq;
        } else {
            return node.pure == rhs->pure && compare_level_map(node.m, rhs->m) ?
                   Equality::Eq : Equality::ConditionallyEq;
        }
    } else {
        return Equality::UnEq;
    }
}

Equality CompareVisitor::visit_univ(Univ& node) {
    if (this->rhs_value->ty() == ValueTy::Univ) {
        auto rhs = dynamic_cast<value::Univ*>(this->rhs_value);
        auto param_visitor = this->compare_with(
            rhs->level.get()
        );
        return param_visitor.visit(*node.level) == Equality::Eq ?
               Equality::Eq : Equality::UnEq;
    } else {
        return Equality::UnEq;
    }
}

Equality CompareVisitor::visit_univ_omega(UnivOmega& node) {
    return this->rhs_value->ty() == ValueTy::UnivOmega ?
           Equality::Eq : Equality::UnEq;
}

Equality CompareVisitor::visit_spine(Spine& node) {
    return this->rhs_value->ty() == ValueTy::Spine ?
           Equality::Eq : Equality::UnEq;
}

Equality CompareVisitor::visit_app(App& node) {
    if (this->rhs_value->ty() == ValueTy::App) {
        auto rhs = dynamic_cast<value::App*>(this->rhs_value);
        auto param_visitor = this->compare_with(
            rhs->parameter.get()
        );
        if (param_visitor.visit(*node.parameter) == Equality::Eq) {
            auto next_visitor = this->compare_with(
                rhs->next.get()
            );

            return next_visitor.visit(*node.next) == Equality::Eq ?
                   Equality::Eq : Equality::UnEq;
        } else {
            return Equality::UnEq;
        }
    } else {
        return Equality::UnEq;
    }
}

Equality CompareVisitor::visit_var(Var& node) {
    if (this->rhs_value->ty() == ValueTy::Var) {
        auto rhs = dynamic_cast<value::Var*>(this->rhs_value);
        if (rhs->l == node.l) {
            auto new_visitor = this->compare_with(
                rhs->spine.get()
            );
            if (new_visitor.visit(*node.spine) == Equality::Eq) {
                return Equality::Eq;
            } else {
                return Equality::ConditionallyEq;
            }
        } else {
            return Equality::ConditionallyEq;
        }
    } else {
        return this->try_rhs_eta_conv_or(node, Equality::ConditionallyEq);
    }
}

