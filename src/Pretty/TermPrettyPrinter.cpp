#include <Pretty/TermPrettyPrinter.h>
#include <Pretty/Tokens.h>


using namespace term;
using namespace std;

TermPrettyPrinterState::TermPrettyPrinterState(
    Precedence pred,
    Associativity assoc
) : precedence(pred), associativity(assoc) {}

TermPrettyPrinter& TermPrettyPrinter::push_name(string& n) {
    this->name_stack.push_back(n);
    return *this;
}

TermPrettyPrinter& TermPrettyPrinter::pop_name() {
    this->name_stack.pop_back();
    return *this;
}

string TermPrettyPrinter::get_name(DBIndex i) const {
    return this->name_stack.at(this->name_stack.size() - 1 - i);
}

DocumentPtr TermPrettyPrinter::sub_pretty(Precedence pred, Associativity assoc, Term& term) {
    auto state = this->swap_state(TermPrettyPrinterState(pred, assoc));
    auto result = this->visit(term);

    this->recovery(state);

    return result;
}

DocumentPtr TermPrettyPrinter::pretty_print_level(Term& node) {
    auto state = this->get_state();

    auto level_printer = LevelPrettyPrinter(this);
    level_printer.visit(node);
    auto doc = level_printer.get_result();

    this->recovery(state);
    return doc;
}

DocumentPtr TermPrettyPrinter::with_precedence(
    Precedence pred, function<void(Document&)> callback
) {
    BlockWrapper wrapper = BlockWrapper::None;
    if (
        (this->get_state().precedence > pred) || (
            this->get_state().precedence == pred &&
            this->get_state().associativity != Associativity::Right
        )
        ) {
        wrapper = BlockWrapper::Parentheses;
    }

    auto doc = make_unique<Document>(wrapper);
    callback(*doc);

    return doc;
}

DocumentPtr TermPrettyPrinter::visit_var(Var& target) {
    return this->with_precedence(
        Precedence::Atom, [&](auto& doc) {
            doc << this->get_name(target.i);
        }
    );
}

DocumentPtr TermPrettyPrinter::visit_def_ref(DefRef& node) {
    return this->with_precedence(
        Precedence::Atom, [&](auto& doc) {
            doc << node.entry;
        }
    );
}

DocumentPtr TermPrettyPrinter::visit_lambda(Lambda& node) {
    auto lambda_visitor = LambdaPrettyPrinter(this);
    return lambda_visitor.visit(node);
}

DocumentPtr TermPrettyPrinter::visit_llambda(LLambda& node) {
    auto lambda_visitor = LambdaPrettyPrinter(this);
    return lambda_visitor.visit(node);
}

DocumentPtr TermPrettyPrinter::visit_app(App& node) {
    auto fun = this->sub_pretty(Precedence::App, Associativity::Left, *node.fun);

    auto param = this->sub_pretty(Precedence::App, Associativity::Left, *node.param);
    return this->with_precedence(
        Precedence::App, [&](auto& doc) {
            doc << fun << token::app_split << param;
        }
    );
}

DocumentPtr TermPrettyPrinter::visit_pi(Pi& node) {
    auto domain = this->sub_pretty(
        node.name == "_" ? Precedence::Abs : Precedence::Atom,
        Associativity::None,
        *node.domain
    );

    this->push_name(node.name);
    auto codomain = this->sub_pretty(Precedence::Abs, Associativity::Right, *node.codomain);
    this->pop_name();

    return this->with_precedence(
        Precedence::Abs, [&](auto& doc) {
            DocumentPtr domain_doc;
            if (node.name == "_") {
                domain_doc = std::move(domain);
            } else {
                domain_doc = this->with_precedence(
                    Precedence::Doc, [&](auto& s_doc) {
                        s_doc << node.name << token::space
                              << token::colon << token::space
                              << domain;
                    }
                );
            }

            doc << domain_doc << token::space << token::arrow << token::after_arrow << codomain;
        }
    );
}

DocumentPtr TermPrettyPrinter::visit_lpi(LPi& node) {
    this->push_name(node.name);
    auto codomain = this->sub_pretty(Precedence::Abs, Associativity::Right, *node.codomain);
    this->pop_name();

    return this->with_precedence(
        Precedence::Abs, [&](auto& doc) {
            DocumentPtr domain = this->with_precedence(
                Precedence::Atom, [&](auto& s_doc) {
                    s_doc << token::level;
                }
            );
            if (node.name != "_") {
                domain = this->with_precedence(
                    Precedence::Doc, [&](auto& s_doc) {
                        s_doc << node.name << token::space
                              << token::colon << token::space
                              << std::move(domain);
                    }
                );
            }

            doc << domain << token::space << token::arrow << token::after_arrow << codomain;
        }
    );
}

DocumentPtr TermPrettyPrinter::visit_univ(Univ& node) {
    auto level = this->sub_pretty(Precedence::Abs, Associativity::None, *node.level);

    return this->with_precedence(
        Precedence::Abs, [&](auto& block) {
            block << token::univ << token::app_split << level;
        }
    );
}

DocumentPtr TermPrettyPrinter::visit_univ_omega(UnivOmega& node) {
    return this->with_precedence(
        Precedence::Atom, [&](auto& block) {
            block << token::omega;
        }
    );
}

DocumentPtr TermPrettyPrinter::visit_lmax(LMax& node) {
    return this->pretty_print_level(node);
}

DocumentPtr TermPrettyPrinter::visit_lzero(LZero& node) {
    return this->pretty_print_level(node);
}

DocumentPtr TermPrettyPrinter::visit_lvar(LVar& node) {
    return this->pretty_print_level(node);
}

DocumentPtr TermPrettyPrinter::visit_lsuc(LSuc& node) {
    return this->pretty_print_level(node);
}

TermPrettyPrinter::TermPrettyPrinter(Precedence pred, Associativity assoc) :
    WithState(TermPrettyPrinterState(pred, assoc)) {}

DocumentPtr LevelPrettyPrinter::sub_pretty(Term& term) {
    auto state = this->term_printer->swap_state(
        TermPrettyPrinterState(
            Precedence::App,
            Associativity::Left
        ));
    auto sub_printer = LevelPrettyPrinter(this->term_printer);
    sub_printer.visit(term);
    this->term_printer->recovery(state);
    return sub_printer.get_result();
}

DocumentPtr LevelPrettyPrinter::get_result() {
    if (!this->base.has_value()) {
        auto doc = make_unique<Document>();

        *doc << to_string(this->offset);

        return doc;
    } else if (this->offset != 0) {
        *this->base.value() << token::space
                            << token::add << token::after_op
                            << to_string(this->offset);
    }

    return std::move(this->base.value());
}

void LevelPrettyPrinter::visit_lmax(LMax& node) {
    auto l = this->sub_pretty(*node.l);
    auto r = this->sub_pretty(*node.r);

    this->base = this->term_printer->with_precedence(
        Precedence::App,
        [&](auto& doc) {
            doc << l << token::space << token::lmax << token::after_op << r;
        }
    );
    this->offset = 0;
}

void LevelPrettyPrinter::visit_lzero(LZero& node) {
    this->offset = 0;
    this->base = {};
}

void LevelPrettyPrinter::visit_lsuc(LSuc& node) {
    this->visit(*node.level);
    ++this->offset;
}

void LevelPrettyPrinter::visit_lvar(LVar& node) {
    this->offset = 0;
    this->base = this->term_printer->with_precedence(
        Precedence::Atom,
        [&](auto& doc) {
            doc << this->term_printer->get_name(node.i);
        }
    );
}

DocumentPtr LambdaPrettyPrinter::finish(Term& body) {
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

    return this->term_printer->with_precedence(
        Precedence::Abs, [&](auto& block) {
            block << token::lambda << token::space;
            for (auto& name : this->bind_list) {
                block << name << token::space;
            }
            block << token::arrow << token::after_arrow
                  << body_block;
        }
    );
}

DocumentPtr LambdaPrettyPrinter::visit_lambda(Lambda& node) {
    this->bind_list.push_back(node.name);
    if (node.body->ty() == TermTy::Lambda || node.body->ty() == TermTy::LLambda) {
        return this->visit(*node.body);
    } else {
        return this->finish(*node.body);
    }
}

DocumentPtr LambdaPrettyPrinter::visit_llambda(LLambda& node) {
    this->bind_list.push_back(node.name);
    if (node.body->ty() == TermTy::Lambda || node.body->ty() == TermTy::LLambda) {
        return this->visit(*node.body);
    } else {
        return this->finish(*node.body);
    }
}
