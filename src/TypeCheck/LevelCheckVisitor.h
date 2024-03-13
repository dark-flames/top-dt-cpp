#pragma once

#include <Syntax/SyntaxVisitor.h>
#include <Term/TermNode.h>
#include <Value/Nodes.h>
#include <Value/Context.h>
#include <Normalize/EvalVisitor.h>
#include <Exception/TypeCheck.h>

class TypeChecker;

class LevelCheckVisitor : public SyntaxVisitor<TermPtr> {
private:
    TypeChecker* type_checker;
protected:
    TermPtr visit_lmax(syntax::LMax& node) override;

    TermPtr visit_lnat(syntax::LNat& node) override;

    TermPtr visit_ref(syntax::Ref& node) override;

    TermPtr visit_lsuc(syntax::LSuc& node) override;

public:
    LevelCheckVisitor() {}

    LevelCheckVisitor(TypeChecker* type_checker) : type_checker(type_checker) {}
};