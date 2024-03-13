#pragma once

#include <Syntax/SyntaxVisitor.h>
#include <Term/TermNode.h>
#include <Value/Nodes.h>
#include <Value/Context.h>
#include <Normalize/EvalVisitor.h>
#include <Exception/TypeCheck.h>

#include <utility>

class TypeChecker;

using TermAndType = std::pair<TermPtr, VTyPtr>;
using TyAndLevel = std::pair<TermPtr, std::optional<VTyPtr>>;

class ExprTypeInferVisitor : public SyntaxVisitor<TermAndType> {
private:
    TypeChecker* type_checker;
protected:
    TermAndType visit_ref(syntax::Ref& node) final;

    TermAndType visit_app(syntax::App& node) final;

    TermAndType visit_pi(syntax::Pi& node) final;

    TermAndType visit_univ(syntax::Univ& node) final;

public:
    ExprTypeInferVisitor() {}

    ExprTypeInferVisitor(TypeChecker* type_checker) : type_checker(type_checker) {}
};