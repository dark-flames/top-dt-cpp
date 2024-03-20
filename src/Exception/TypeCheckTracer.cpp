//
// Created by darkflames on 3/20/24.
//

#include <Exception/TypeCheckTracer.h>
#include <TypeCheck/TypeChecker.h>
#include <memory>
#include <iostream>

using namespace std;

DocumentPtr TypeCheckTracer::pretty_context_node(ContextNode& node) {
    if (node.size() == 0) {
        return make_unique<Document>();
    } else {
        auto prev = this->pretty_context_node(*node.get_prev());
        if (node.get_prev()->size() != 0) {
            *prev << token::space << token::comma << token::space;
        }
        auto name = node.get_name();

        *prev << name << token::space << token::colon << token::space;
        auto vty = node.get_ty();
        if (vty.has_value()) {
            auto tty = type_checker->read_back(*vty.value());
            *prev << this->term_printer.visit(*tty);
        } else {
            *prev << token::level;
        }
        this->term_printer.push_name(name);
        return prev;
    }
}

DocumentPtr TypeCheckTracer::pretty_context() {
    this->term_printer.clean_names();
    return this->pretty_context_node(*this->context.get_tail());
}

DocumentPtr TypeCheckTracer::pretty_term(Term& term) {
    return this->term_printer.visit(term);
}

DocumentPtr TypeCheckTracer::pretty_syntax(Syntax& syn) {
    return this->syntax_printer.visit(syn);
}

DocumentPtr TypeCheckTracer::pretty_value(Value& value) {
    auto term = this->type_checker->read_back(value);
    return this->pretty_term(*term);
}

void TypeCheckTracer::throw_error(TypeCheckException& exception) {
    auto doc = make_unique<Document>();
    auto context_doc = this->pretty_context();
    auto err = exception.format(*this);
    *doc << "UnderContext:" << token::endl
         << context_doc << token::endl
         << err;
    auto formatter = Formatter(Style::SoftSplit, 120, 2);
    auto element = Document::as_element(doc);
    formatter.format(*element);
    cerr << formatter.to_string() << endl;
    throw new FormalizedTypeCheckErr(std::move(doc));
}

TypeCheckTracer::TypeCheckTracer(
    TypeChecker* type_checker,
    Context& context
) : type_checker(type_checker), context(context) {
    term_printer = TermPrettyPrinter();
    syntax_printer = SyntaxPrettyPrinter();
}
