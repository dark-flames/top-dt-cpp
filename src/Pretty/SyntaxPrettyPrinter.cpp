//
// Created by darkflames on 3/20/24.
//

#include <Pretty/SyntaxPrettyPrinter.h>

using namespace std;

SyntaxPrettyPrinterState::SyntaxPrettyPrinterState(Precedence pred, Associativity assoc) :
    precedence(pred), associativity(assoc) {}


DocumentPtr SyntaxPrettyPrinter::sub_pretty(Precedence pred, Associativity assoc, Syntax& node) {
    auto state = this->swap_state(SyntaxPrettyPrinterState(pred, assoc));
    auto result = this->visit(node);

    this->recovery(state);

    return result;
}

DocumentPtr SyntaxPrettyPrinter::with_precedence(
    Precedence pred, Associativity pos,
    std::function<void(Document&)> callback
) {
    BlockWrapper wrapper = BlockWrapper::None;
    if ((this->get_state().precedence > pred) || (
        this->get_state().precedence == pred &&
        this->get_state().associativity != pos
    )) {
        wrapper = BlockWrapper::Parentheses;
    }

    auto doc = make_unique<Document>(wrapper);
    callback(*doc);

    return doc;
}


DocumentPtr SyntaxPrettyPrinter::visit_lmax(syntax::LMax& node) {
    auto l = this->sub_pretty(Precedence::App, Associativity::Left, *node.l);
    auto r = this->sub_pretty(Precedence::App, Associativity::Right, *node.r);

    return this->with_precedence(
        Precedence::Op, Associativity::Left, [&](auto& doc) {
            doc << l << token::space << token::lmax << token::after_op << r;
        }
    );
}

DocumentPtr SyntaxPrettyPrinter::visit_lambda(syntax::Lambda& node) {
    auto visitor = SyntaxLambdaPrettyPrinter(this);

    return visitor.visit(node);
}

DocumentPtr SyntaxPrettyPrinter::visit_pi(syntax::Pi& node) {
    DocumentPtr domain;
    if (node.name == "_") {
        domain = this->sub_pretty(Precedence::Abs, Associativity::Left, *node.codomain);
    } else {
        auto domain_ty = this->sub_pretty(Precedence::Doc, Associativity::Left, *node.codomain);
        domain = this->with_precedence(
            Precedence::Doc, Associativity::None, [&](auto& s_doc) {
                s_doc << node.name << token::space
                      << token::colon << token::space
                      << domain_ty;
            }
        );
    }
    auto codomain = this->sub_pretty(Precedence::Abs, Associativity::Right, *node.codomain);
    return this->with_precedence(
        Precedence::Abs, Associativity::Right, [&](auto& doc) {
            doc << domain << token::space
                << token::arrow << token::after_arrow
                << codomain;
        }
    );
}

DocumentPtr SyntaxPrettyPrinter::visit_univ(syntax::Univ& node) {
    auto level = this->sub_pretty(Precedence::App, Associativity::Right, *node.level);

    return this->with_precedence(
        Precedence::App, Associativity::Left, [&](auto& block) {
            block << token::univ << token::app_split << level;
        }
    );
}

DocumentPtr SyntaxPrettyPrinter::visit_lnat(syntax::LNat& node) {
    return this->with_precedence(
        Precedence::Atom, Associativity::None, [&](auto& block) {
            block << to_string(node.l);
        }
    );
}

DocumentPtr SyntaxPrettyPrinter::visit_level(syntax::Level& node) {
    return this->with_precedence(
        Precedence::Atom, Associativity::None, [&](auto& block) {
            block << token::level;
        }
    );
}

DocumentPtr SyntaxPrettyPrinter::visit_ref(syntax::Ref& node) {
    return this->with_precedence(
        Precedence::Atom, Associativity::None, [&](auto& block) {
            block << node.name;
        }
    );
}

DocumentPtr SyntaxPrettyPrinter::visit_app(syntax::App& node) {
    auto fun = this->sub_pretty(Precedence::App, Associativity::Left, *node.fun);

    auto param = this->sub_pretty(Precedence::App, Associativity::Right, *node.param);
    return this->with_precedence(
        Precedence::App, Associativity::Left, [&](auto& doc) {
            doc << fun << token::app_split << param;
        }
    );
}

DocumentPtr SyntaxPrettyPrinter::visit_lsuc(syntax::LSuc& node) {
    Syntax& current = *node.level;
    int offset = 1;
    while (current.ty() == SyntaxTy::LSuc) {
        auto& current_suc = static_cast<syntax::LSuc&>(current);
        ++offset;
        current = *current_suc.level;
    }
    auto base = this->sub_pretty(Precedence::Op, Associativity::Left, current);

    return this->with_precedence(
        Precedence::Op, Associativity::Left, [&](auto& doc) {
            doc << base << token::space << token::add << to_string(offset);
        }
    );
}

DocumentPtr SyntaxLambdaPrettyPrinter::finish(Syntax& body) {
    auto body_block = this->printer
        ->sub_pretty(
            Precedence::Abs,
            Associativity::Right,
            body
        );

    return this->printer->with_precedence(
        Precedence::Abs, Associativity::Right, [&](auto& doc) {
            doc << token::lambda << token::space;
            for (auto& name : this->bind_list) {
                doc << name << token::space;
            }
            doc << token::arrow << token::after_arrow
                << body_block;
        }
    );
}

DocumentPtr SyntaxLambdaPrettyPrinter::visit_lambda(syntax::Lambda& node) {
    this->bind_list.push_back(node.name);
    if (node.body->ty() == SyntaxTy::Lambda) {
        return this->visit(*node.body);
    } else {
        return this->finish(*node.body);
    }
}
