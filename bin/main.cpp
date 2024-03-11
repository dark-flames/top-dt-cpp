#include <term.h>
#include <value.h>
#include <normalize.h>
#include <pretty.h>
#include <iostream>

using namespace std;

term::TermPtr id () {
    return term::lambda("a", term::var(0));
}

int main(int argc, char* argv[]) {
    auto term = term::app(id(), id());

    auto block = TermPrettyPrinter::pretty_inline(term);
    std::cout << "Normalize:" << block->format_inline() << endl;

    auto state = make_shared<EvalState>();

    auto visitor = EvalVisitor(state);

    auto result = visitor.visit(term);

    return 0;
}