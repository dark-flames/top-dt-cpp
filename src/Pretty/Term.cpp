#include <Pretty/Term.h>
#include <Pretty/Tokens.h>


using namespace term;

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

BlockPtr TermPrettyPrinter::sub_pretty(TermPtr& term) {
    auto printer = new TermPrettyPrinter(this->state);

    printer->visit(term);

    if(this->state->block_tabs) {
        printer->result->tab();
    }

    return printer->result;
}

LevelPrettyPrinter TermPrettyPrinter::create_level_printer() {
    return LevelPrettyPrinter(this);
}

void TermPrettyPrinter::visit_var(NodePtr<Var>& target) {
    *this->result << this->get_name(target->i);
}

void TermPrettyPrinter::visit_lambda(NodePtr<Lambda>& node) {
    this->push_name(node->name);
    auto body = this->sub_pretty(node->body);
    this->pop_name();

    *this->result << lambda <=> node->name <=> dot <=> body;
}

void TermPrettyPrinter::visit_llambda(NodePtr<LLambda>& node) {
    this->push_name(node->name);
    auto body = this->sub_pretty(node->body);
    this->pop_name();

    *this->result << _omega <=> node->name <=> dot <=> body;
}

void TermPrettyPrinter::visit_app(NodePtr<App>& node) {
    auto fun = this->sub_pretty(node->fun);
    auto param = this->sub_pretty(node->param);
    *(this->result) << fun <=> param;
}

void TermPrettyPrinter::visit_pi(NodePtr<Pi>& node) {
    auto domain = this->sub_pretty(node->domain);

    this->push_name(node->name);
    auto codomain = this->sub_pretty(node->codomain);
    this->pop_name();

    *this->result << pi;
    this->result->sub_block(
        [&](BlockPtr& s) {
            *s << node->name <=> colon <=> domain;
            s->set_wrapper(BlockWrapper::Parentheses);
            return s;
        }
    );

    *this->result <=> dot <=> codomain;
}

void TermPrettyPrinter::visit_univ(NodePtr<Univ>& node) {
    auto level_printer = this->create_level_printer();

    level_printer.visit(node -> level);

    auto block = level_printer.get_result();

    block->set_wrapper(BlockWrapper::Bracket);

    *this->result << univ << under_line << block;
}

void TermPrettyPrinter::visit_univ_omega(NodePtr<UnivOmega>& node) {
    *this->result << univ << under_line << omega;
}

void TermPrettyPrinter::visit_lpi(NodePtr<LPi>& node) {
    this->push_name(node->name);
    auto codomain = this->sub_pretty(node->codomain);
    this->pop_name();

    *this->result << omega <=> node->name <=> dot <=> codomain;
}

BlockPtr LevelPrettyPrinter::sub_pretty(TermPtr& term) {
    auto printer = this->term_printer->create_level_printer();

    printer.visit(term);

    return printer.get_result();
}

BlockPtr LevelPrettyPrinter::get_result() {
    if (!this->base.has_value()) {
        auto block = this->term_printer
            ->create_block();

        block->push(to_string(this->offset));

        this->base = block;
    } else if (this->offset != 0) {
        *this->base.value() <=> add <=> to_string(this->offset);
    }

    return this->base.value();
}

void LevelPrettyPrinter::visit_lmax(NodePtr<LMax>& node) {
    this->clear_result();
    auto l = this->sub_pretty(node->l);
    auto r = this->sub_pretty(node->r);

    *l <=> lmax <=> r;

    this->base = l;
}

void LevelPrettyPrinter::visit_lzero(NodePtr<LZero>& node) {
    this->clear_result();
}

void LevelPrettyPrinter::visit_lsuc(NodePtr<LSuc>& node) {
    this->visit(node->level);
    this->offset ++;
}

void LevelPrettyPrinter::visit_lvar(NodePtr<LVar>& node) {
    this->clear_result();
    auto block = this->term_printer->create_block();

    *block << this->term_printer->get_name(node->i);

    this->base = block;
}
