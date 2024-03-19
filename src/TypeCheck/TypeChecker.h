#pragma once

#include <Value/Context.h>
#include <Value/Closure.h>
#include <Normalize/EvalVisitor.h>
#include <Normalize/ReadBackVisitor.h>
#include <Declaration/DeclarationResolver.h>
#include <TypeCheck/ExprCheckVisitor.h>
#include <TypeCheck/ExprInferVisitor.h>
#include <TypeCheck/LevelCheckVisitor.h>
#include <Pretty/Term.h>
#include <memory>

class TypeChecker {
private:

    Context context;
    EvalVisitor* eval_visitor;
    CompareVisitor* compare_visitor;
    ExprCheckVisitor* check_visitor;
    ExprTypeInferVisitor* infer_visitor;
    LevelCheckVisitor* level_visitor;
    ReadBackVisitor* read_back_visitor;
public:
    DeclarationResolver* resolver;

    TypeChecker& add_decl(DeclarationPtr& decl);

    ValuePtr bind(Id& name, VTyPtr ty);

    ValuePtr bind_level(Id& name);

    void pop();

    TermAndType find_ref(Id& name);

    TermPtr find_level_ref(Id& name);

    ValuePtr eval_closure(Closure& closure, ValuePtr v);

    ValuePtr eval(Term& t);

    TermPtr read_back(Value& value);

    TermPtr check_level(Syntax& syn);

    TermPtr check_expr(Syntax& syn, VTy* as);

    TyAndLevel check_ty(Syntax& syn);

    TermAndType infer_expr(Syntax& syn);

    Equality conv(Value& l, Value* r);

    TermPtr normalize_entry(Entry entry);

    TypeChecker() {
        resolver = new DeclarationResolver(this);
        context = Context();
        eval_visitor = new EvalVisitor(*this->resolver);
        compare_visitor = new CompareVisitor(nullptr, *this->eval_visitor);
        check_visitor = new ExprCheckVisitor(this);
        infer_visitor = new ExprTypeInferVisitor(this);
        level_visitor = new LevelCheckVisitor(this);
        read_back_visitor = new ReadBackVisitor(*this->eval_visitor);
    }

    ~TypeChecker() {
        delete this->resolver;
        delete this->eval_visitor;
        delete this->compare_visitor;
        delete this->check_visitor;
        delete this->infer_visitor;
        delete this->level_visitor;
    }
};
