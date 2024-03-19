#include <TypeCheck/ExprCheckVisitor.h>
#include <Exception/TypeCheck.h>
#include <TypeCheck/TypeChecker.h>
#include <Term/Helper.h>

using namespace std;

TermPtr ExprCheckVisitor::visit_lambda(syntax::Lambda& node) {
    if (this->as->ty() == ValueTy::Pi) {
        auto as_pi = dynamic_cast<value::Pi*>(this->as);

        auto v = this->type_checker->bind(node.name, as_pi->domain->copy());
        auto codomain = this->type_checker
            ->eval_closure(as_pi->codomain, std::move(v));
        auto body = this->type_checker->check_expr(
            *node.body, codomain.get()
        );
        this->type_checker->pop();
        return term::lambda(node.name, std::move(body));
    } else if (this->as->ty() == ValueTy::LPi) {
        auto as_pi = dynamic_cast<value::LPi*>(this->as);

        auto v = this->type_checker->bind_level(node.name);
        auto codomain = this->type_checker
            ->eval_closure(as_pi->codomain, std::move(v));

        auto body = this->type_checker->check_expr(
            *node.body, codomain.get()
        );
        this->type_checker->pop();
        return term::l_lambda(node.name, std::move(body));
    } else {
        throw NonPi();
    }
}

TermPtr ExprCheckVisitor::visit(Syntax& node) {
    switch (node.ty()) {
        case SyntaxTy::Lambda:
            return SyntaxVisitor<TermPtr>::visit(node);
        default:
            auto result = this->type_checker->infer_expr(node);
            auto cov_res = this->type_checker->conv(*result.second, this->as);

            if (cov_res == Equality::Eq) {
                return std::move(result.first);
            } else {
                auto ty = this->type_checker->read_back(*result.second);
                auto expect = this->type_checker->read_back(*this->as);

                auto tm_block = TermPrettyPrinter::pretty_inline(result.first);
                auto ty_block = TermPrettyPrinter::pretty_inline(ty);
                auto expect_block = TermPrettyPrinter::pretty_inline(expect);

                cout << "Term:" << endl << tm_block->format_inline() << endl;
                cout << "Actual:" << endl << ty_block->format_inline() << endl;
                cout << "Expect" << endl << expect_block->format_inline() << endl;
                throw UnificationException();
            }
    }
}
