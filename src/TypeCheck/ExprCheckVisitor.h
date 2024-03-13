#pragma once

#include <Syntax/SyntaxVisitor.h>
#include <Term/TermNode.h>
#include <Value/Nodes.h>
#include <Value/Context.h>
#include <Normalize/EvalVisitor.h>
#include <Exception/TypeCheck.h>

class TypeChecker;


class ExprCheckVisitor : public SyntaxVisitor<TermPtr> {
private:
    TypeChecker* type_checker;
    VTy* as;
protected:
    TermPtr visit_lambda(syntax::Lambda& node) override;

public:
    TermPtr visit(Syntax& node) override;

    void set_ty(VTy* ty) {
        this->as = ty;
    }

    ExprCheckVisitor() {}

    ExprCheckVisitor(TypeChecker* type_checker)
        : type_checker(type_checker), as(nullptr) {}
};



