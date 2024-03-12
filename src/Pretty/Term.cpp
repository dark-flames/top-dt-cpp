#include <Pretty/Term.h>
#include <Pretty/Tokens.h>


using namespace term;
using namespace std;

TermPrettyPrinter& TermPrettyPrinter::push_name(string& n) {
    this->state->name_stack.push_back(n);
    return *this;
}

TermPrettyPrinter& TermPrettyPrinter::pop_name() {
    this->state->name_stack.pop_back();
    return *this;
}

string TermPrettyPrinter::get_name(Idx i) const {
    return this->state->name_stack.at(this->state->name_stack.size() - 1 - i);
}

BlockPtr TermPrettyPrinter::sub_pretty(Term& term) {
    auto printer = new TermPrettyPrinter(this->state);

    printer->visit(term);

    if(this->state->block_tabs) {
        printer->result->tab();
    }

    return std::move(printer->result);
}

LevelPrettyPrinter TermPrettyPrinter::create_level_printer() {
    return LevelPrettyPrinter(this);
}

void TermPrettyPrinter::visit_var(Var& target) {
    *this->result << this->get_name(target.i);
}

void TermPrettyPrinter::visit_lambda(Lambda& node) {
    this->push_name(node.name);
    auto body = this->sub_pretty(*node.body);
    this->pop_name();

    *this->result << token::lambda <=> node.name <=> token::dot <=> body;
}

void TermPrettyPrinter::visit_llambda(LLambda& node) {
    this->push_name(node.name);
    auto body = this->sub_pretty(*node.body);
    this->pop_name();

    *this->result << token::_omega <=> node.name <=> token::dot <=> body;
}

void TermPrettyPrinter::visit_app(App& node) {
    auto fun = this->sub_pretty(*node.fun);
    TermPtr param;

    if (is_level_term(node.param->ty())) {
        auto level_printer = this->create_level_printer();

        level_printer.visit(*node.param);

        auto block = level_printer.get_result();
        *(this->result) << fun <=> block;
    } else {
        auto param = this->sub_pretty(*node.param);

        *(this->result) << fun <=> param;
    }
}

void TermPrettyPrinter::visit_pi(Pi& node) {
    auto domain = this->sub_pretty(*node.domain);

    this->push_name(node.name);
    auto codomain = this->sub_pretty(*node.codomain);
    this->pop_name();

    *this->result << token::pi;
    this->result->sub_block(
        [&](BlockPtr& s) {
            *s << node.name <=> token::colon <=> domain;
            s->set_wrapper(BlockWrapper::Parentheses);
            return std::move(s);
        }
    );

    *this->result <=> token::dot <=> codomain;
}

void TermPrettyPrinter::visit_univ(Univ& node) {
    auto level_printer = this->create_level_printer();

    level_printer.visit(*node.level);

    auto block = level_printer.get_result();

    block->set_wrapper(BlockWrapper::Bracket);

    *this->result << token::univ << token::under_line << block;
}

void TermPrettyPrinter::visit_univ_omega(UnivOmega& node) {
    *this->result << token::univ << token::under_line << token::omega;
}

void TermPrettyPrinter::visit_lpi(LPi& node) {
    this->push_name(node.name);
    auto codomain = this->sub_pretty(*node.codomain);
    this->pop_name();

    *this->result << token::omega <=> node.name <=> token::dot <=> codomain;
}

BlockPtr LevelPrettyPrinter::sub_pretty(Term& term) {
    auto printer = this->term_printer->create_level_printer();

    printer.visit(term);

    return printer.get_result();
}

BlockPtr LevelPrettyPrinter::get_result() {
    if (!this->base.has_value()) {
        auto block = this->term_printer
            ->create_block();

        block->push(to_string(this->offset));

        this->base = std::move(block);
    } else if (this->offset != 0) {
        *this->base.value() <=> token::add <=> to_string(this->offset);
    }

    return std::move(this->base.value());
}

void LevelPrettyPrinter::visit_lmax(LMax& node) {
    this->clear_result();
    auto l = this->sub_pretty(*node.l);
    auto r = this->sub_pretty(*node.r);

    *l <=> token::lmax <=> r;

    this->base = std::move(l);
}

void LevelPrettyPrinter::visit_lzero(LZero& node) {
    this->clear_result();
}

void LevelPrettyPrinter::visit_lsuc(LSuc& node) {
    this->visit(*node.level);
    ++ this->offset;
}

void LevelPrettyPrinter::visit_lvar(LVar& node) {
    this->clear_result();
    auto block = this->term_printer->create_block();

    *block << this->term_printer->get_name(node.i);

    this->base = std::move(block);
}
