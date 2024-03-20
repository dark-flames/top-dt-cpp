#include <Pretty/TermPrettyPrinter.h>
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

string TermPrettyPrinter::get_name(DBIndex i) const {
    return this->state->name_stack.at(this->state->name_stack.size() - 1 - i);
}

BlockPtr TermPrettyPrinter::sub_pretty(Precedence pred, Associativity assoc, Term& term) {
    auto printer = TermPrettyPrinter(pred, assoc, this->state);

    printer.visit(term);

    if (this->state->block_tabs) {
        printer.result->tab();
    }

    return std::move(printer.result);
}

LevelPrettyPrinter TermPrettyPrinter::create_level_printer() {
    return LevelPrettyPrinter(this, this->precedence, this->associativity);
}

void TermPrettyPrinter::pretty_print_level(Term& node) {
    auto level_printer = LevelPrettyPrinter(this, this->precedence, this->associativity);

    level_printer.visit(node);
}

void TermPrettyPrinter::with_precedence(Precedence pred, function<void(Block&)> callback) {
    if (
        (this->precedence > pred) ||
        (this->precedence == pred && this->associativity != Associativity::Right)
        ) {
        this->result->sub_block(
            [&](Block& b) {
                callback(b);
                b.set_wrapper(BlockWrapper::Parentheses);
            }
        );
    } else {
        this->result->sub_block(callback);
    }
}

void TermPrettyPrinter::visit_var(Var& target) {
    this->with_precedence(
        Precedence::Atom, [&](auto& block) {
            block << this->get_name(target.i);
        }
    );
}

void TermPrettyPrinter::visit_def_ref(DefRef& node) {
    this->with_precedence(
        Precedence::Atom, [&](auto& block) {
            block << node.entry;
        }
    );
}

void TermPrettyPrinter::visit_lambda(Lambda& node) {
    auto lambda_visitor = LambdaPrettyPrinter(this);
    lambda_visitor.visit(node);
}

void TermPrettyPrinter::visit_llambda(LLambda& node) {
    auto lambda_visitor = LambdaPrettyPrinter(this);
    lambda_visitor.visit(node);
}

void TermPrettyPrinter::visit_app(App& node) {
    auto fun = this->sub_pretty(Precedence::App, Associativity::Left, *node.fun);

    auto param = this->sub_pretty(Precedence::App, Associativity::Left, *node.param);
    this->with_precedence(
        Precedence::App, [&](auto& block) {
            block << fun << token::space << param;
        }
    );
}

void TermPrettyPrinter::visit_pi(Pi& node) {
    auto domain = this->sub_pretty(
        node.name == "_" ? Precedence::Abs : Precedence::Atom,
        Associativity::None,
        *node.domain
    );

    this->push_name(node.name);
    auto codomain = this->sub_pretty(Precedence::Abs, Associativity::Right, *node.codomain);
    this->pop_name();

    this->with_precedence(
        Precedence::Abs, [&](auto& block) {
            if (node.name != "_") {
                block << token::left_paren << token::space
                      << node.name << token::space
                      << token::colon << token::space;
        }

            block << domain << token::space;

            if (node.name != "_") {
                block << token::right_paren;
            }

            block << token::arrow << codomain;
        }
    );
}

void TermPrettyPrinter::visit_lpi(LPi& node) {
    this->push_name(node.name);
    auto codomain = this->sub_pretty(Precedence::Abs, Associativity::Right, *node.codomain);
    this->pop_name();

    this->with_precedence(
        Precedence::Abs, [&](auto& block) {
            if (node.name != "_") {
                block << token::left_paren << token::space
                      << node.name << token::space
                      << token::colon << token::space
                      << token::level << token::space
                      << token::right_paren;
            } else {
                block << token::level;
            }

            block << token::space << token::arrow << codomain;
        }
    );
}

void TermPrettyPrinter::visit_univ(Univ& node) {
    auto level = this->sub_pretty(Precedence::Abs, Associativity::None, *node.level);

    this->with_precedence(
        Precedence::Abs, [&](auto& block) {
            block << token::univ << token::space << level;
        }
    );
}

void TermPrettyPrinter::visit_univ_omega(UnivOmega& node) {
    this->with_precedence(
        Precedence::Atom, [&](auto& block) {
            block << token::omega;
        }
    );
}

void TermPrettyPrinter::visit_lmax(LMax& node) {
    this->pretty_print_level(node);
}

void TermPrettyPrinter::visit_lzero(LZero& node) {
    this->pretty_print_level(node);
}

void TermPrettyPrinter::visit_lvar(LVar& node) {
    this->pretty_print_level(node);
}

void TermPrettyPrinter::visit_lsuc(LSuc& node) {
    this->pretty_print_level(node);
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
        *this->base.value() << token::space << token::add << token::space << to_string(this->offset);
    }

    return std::move(this->base.value());
}

void LevelPrettyPrinter::visit_lmax(LMax& node) {
    this->clear_result();
    auto l = this->sub_pretty(*node.l);
    auto r = this->sub_pretty(*node.r);

    *l << token::space << token::lmax << token::space << r;

    this->base = std::move(l);
}

void LevelPrettyPrinter::visit_lzero(LZero& node) {
    this->clear_result();
}

void LevelPrettyPrinter::visit_lsuc(LSuc& node) {
    this->visit(*node.level);
    ++this->offset;
}

void LevelPrettyPrinter::visit_lvar(LVar& node) {
    this->clear_result();
    auto block = this->term_printer->create_block();

    *block << this->term_printer->get_name(node.i);

    this->base = std::move(block);
}

void LambdaPrettyPrinter::finish(Term& body) {
    for (auto& name : this->bind_list) {
        this->term_printer->push_name(name);
    }

    auto body_block = this->term_printer
        ->sub_pretty(
            Precedence::Abs,
            Associativity::Right,
            body
        );

    for (auto& name : this->bind_list) {
        this->term_printer->pop_name();
    }

    this->term_printer->with_precedence(
        Precedence::Abs, [&](auto& block) {
            block << token::lambda << token::space;
            for (auto& name : this->bind_list) {
                block << name << token::space;
            }
            block << token::arrow << token::space
                  << body_block;
        }
    );
}

void LambdaPrettyPrinter::visit_lambda(Lambda& node) {
    this->bind_list.push_back(node.name);
    if (node.body->ty() == TermTy::Lambda || node.body->ty() == TermTy::LLambda) {
        this->visit(*node.body);
    } else {
        this->finish(*node.body);
    }
}

void LambdaPrettyPrinter::visit_llambda(LLambda& node) {
    this->bind_list.push_back(node.name);
    if (node.body->ty() == TermTy::Lambda || node.body->ty() == TermTy::LLambda) {
        this->visit(*node.body);
    } else {
        this->finish(*node.body);
    }
}
