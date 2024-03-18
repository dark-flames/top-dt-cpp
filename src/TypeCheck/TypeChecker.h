#pragma once

#include <Value/Context.h>
#include <Value/Closure.h>
#include <Normalize/EvalVisitor.h>
#include <Declaration/DeclarationResolver.h>
#include <TypeCheck/ExprCheckVisitor.h>
#include <TypeCheck/ExprInferVisitor.h>
#include <TypeCheck/LevelCheckVisitor.h>
#include <Pretty/Term.h>
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
    ValuePtr bind(Id& name, VTyPtr ty);

    ValuePtr bind_level(Id& name);

    void pop();

    TermAndType find_ref(Id& name);

    TermPtr find_level_ref(Id& name);

    ValuePtr eval_closure(Closure& closure, ValuePtr v);

    ValuePtr eval(Term& t);

    TermPtr check_level(Syntax& syn);

    TermPtr check_expr(Syntax& syn, VTy* as);

    TyAndLevel check_ty(Syntax& syn);

    TermAndType infer_expr(Syntax& syn);

    Equality conv(Value& l, Value* r);

    TypeChecker() {
        resolver = DeclarationResolver(this);
        context = Context();
        auto env = Environment();
        auto eval_state = std::make_shared<EvalState>();
        eval_visitor = EvalVisitor(env, eval_state);
        compare_visitor = CompareVisitor(nullptr, &this->eval_visitor);
        check_visitor = ExprCheckVisitor(this);
        infer_visitor = ExprTypeInferVisitor(this);
        level_visitor = LevelCheckVisitor(this);
    }
};

using TypeCheckerPtr = std::shared_ptr<TypeChecker>;
