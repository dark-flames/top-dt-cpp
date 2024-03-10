#include <Pretty/Term.h>
#include <Pretty/Tokens.h>


using namespace term;

void TermPrettyPrinter::visit_var(NodePtr<Var> &target) {
    *this->result << this->get_name(target->i);
}

void TermPrettyPrinter::visit_lambda(NodePtr<Lambda> &target) {
    this->push_name(target->name);
    auto body = this->sub_pretty(target->body);
    this->pop_name();

    *this->result << lambda << target->name << dot << body;
}

void TermPrettyPrinter::visit_app(NodePtr<App> &target) {
    auto fun = this->sub_pretty(target->fun);
    auto param = this->sub_pretty(target->param);
    *(this->result) << fun << space << param;
}

void TermPrettyPrinter::visit_pi(NodePtr<Pi> &target) {
    *this->result << pi;
    auto domain = this->sub_pretty(target->domain);

    this->push_name(target->name);
    auto codomain = this->sub_pretty(target->codomain);
    this->pop_name();

    this->result->sub_block([&](BlockPtr &s) {
        *s << target->name << colon << domain;
        s->parenthesized();
        return s;
    });

    *this->result << codomain;
}
