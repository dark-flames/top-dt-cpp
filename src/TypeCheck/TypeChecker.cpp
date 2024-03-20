#include <TypeCheck/TypeChecker.h>

using namespace std;

TypeChecker& TypeChecker::add_decl(DeclarationPtr& decl) {
    this->resolver->push_unchecked(decl);
    return *this;
}

ValuePtr TypeChecker::bind(Id& name, VTyPtr ty) {
    auto new_ctx = this->context.push(name, ty);
    this->context = new_ctx;
    auto v = this->eval_visitor->bind_in_place(false);

    return v;
}

ValuePtr TypeChecker::bind_level(Id& name) {
    auto new_ctx = this->context.push_level(name);
    this->context = new_ctx;
    auto v = this->eval_visitor->bind_in_place(true);

    return v;
}

void TypeChecker::pop() {
    auto new_ctx = this->context.pop();
    this->context = new_ctx;
    auto v = this->eval_visitor->pop_variable();
}

TermAndType TypeChecker::find_ref(Id& name) {
    auto context_ref = this->context.find(name);

    if (context_ref.has_value()) {
        auto index = context_ref.value().second;
        return make_pair(
            term::var(index),
            std::move(context_ref.value().first)
        );
    } else {
        auto ref = this->resolver->get_signature_or_waiting(name);
        return make_pair(
            term::def_ref(name),
            ref->get_ty().copy()
        );
    }
}

TermPtr TypeChecker::find_level_ref(Id& name) {
    auto context_ref = this->context.find_level(name);

    if (context_ref.has_value()) {
        auto index = context_ref.value();
        return term::var(index);
    } else {
        throw UnknownReference(name);
    }
}

ValuePtr TypeChecker::eval_closure(Closure& closure, ValuePtr v) {
    return this->eval_visitor->eval_closure(closure, std::move(v));
}

ValuePtr TypeChecker::eval(Term& t) {
    return this->eval_visitor->visit(t);
}

TermPtr TypeChecker::read_back(Value& value) {
    return this->read_back_visitor->visit(value);
}

TermPtr TypeChecker::check_level(Syntax& syn) {
    return this->level_visitor->visit(syn);
}

TermPtr TypeChecker::check_expr(Syntax& syn, VTy* as) {
    auto old = this->check_visitor->set_ty(as);
    auto result = this->check_visitor->visit(syn);
    this->check_visitor->set_ty(old);
    return result;
}

TyAndLevel TypeChecker::check_ty(Syntax& syn) {
    auto res = this->infer_visitor->visit(syn);
    auto term = std::move(res.first);
    auto ty = std::move(res.second);

    if (ty->ty() == ValueTy::Univ) {
        auto univ = dynamic_cast<value::Univ*>(ty.get());
        return make_pair(std::move(term), std::move(univ->level));
    } else if (ty->ty() == ValueTy::UnivOmega) {
        optional<ValuePtr> nothing = {};
        return make_pair(std::move(term), std::move(nothing));
    } else {
        auto e = NotType(syn.copy(), ty->copy());
        this->throw_err(e);
        throw ImpossibleException("");
    }
}

TermAndType TypeChecker::infer_expr(Syntax& syn) {
    return this->infer_visitor->visit(syn);
}

Equality TypeChecker::conv(Value& l, Value* r) {
    this->compare_visitor->set_rhs(r);

    return this->compare_visitor->visit(l);
}

TermPtr TypeChecker::normalize_entry(Entry entry) {
    auto decl = this->resolver->get_body_or_waiting(entry);
    auto copied = decl->body->copy();
    auto decl_value = this->eval(*copied);

    return this->read_back_visitor->visit(*decl_value);
}

void TypeChecker::throw_err(TypeCheckException& exception) {
    this->tracer->throw_error(exception);
}
