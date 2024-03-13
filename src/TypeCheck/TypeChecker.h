#pragma once

#include <Value/Context.h>
#include <Value/Closure.h>
#include <Normalize/EvalVisitor.h>
#include <Declaration/DeclarationResolver.h>
#include <TypeCheck/ExprCheckVisitor.h>
#include <TypeCheck/ExprInferVisitor.h>
#include <TypeCheck/LevelCheckVisitor.h>
#include <memory>

class TypeChecker {
private:
    DeclarationResolver resolver;
    Context context;
    EvalVisitor eval_visitor;
    CompareVisitor compare_visitor;
    ExprCheckVisitor check_visitor;
    ExprTypeInferVisitor infer_visitor;
    LevelCheckVisitor level_visitor;
public:
    ValuePtr bind(Identifier& name, VTyPtr ty);

    ValuePtr bind_level(Identifier& name);

    void pop();

    TermAndType find_ref(Identifier& name);

    TermPtr find_level_ref(Identifier& name);

    ValuePtr eval_closure(Closure& closure, ValuePtr v);

    ValuePtr eval(Term& t);

    TermPtr check_level(Syntax& syn);

    TermPtr check_expr(Syntax& syn, VTy* as);

    TyAndLevel check_ty(Syntax& syn);

    TermAndType infer_expr(Syntax& syn);

    Equality conv(Value& l, Value* r);
};

using TypeCheckerPtr = std::shared_ptr<TypeChecker>;
