#pragma once

#include <Value/Context.h>
#include <Pretty/TermPrettyPrinter.h>
#include <Pretty/SyntaxPrettyPrinter.h>
#include <Exception/TypeCheck.h>

class TypeChecker;

class TypeCheckTracer {
private:
    Context& context;
    TermPrettyPrinter term_printer;
    SyntaxPrettyPrinter syntax_printer;
    TypeChecker* type_checker;

    DocumentPtr pretty_context_node(ContextNode& node);

    DocumentPtr pretty_context();

public:
    TypeCheckTracer(TypeChecker* type_checker, Context& context);

    DocumentPtr pretty_value(Value& value);

    DocumentPtr pretty_syntax(Syntax& syn);

    DocumentPtr pretty_term(Term& term);

    void throw_error(TypeCheckException& exception);
};
