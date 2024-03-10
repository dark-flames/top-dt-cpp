//
// Created by darkflames on 3/8/24.
//

#include <Term/PiNode.h>
#include <Term/RefNode.h>
#include <Pretty/Term.h>
#include <iostream>

using namespace std;
using namespace term;

int main(int argc, char *argv[]) {
    auto test = Pi::make_term_ptr(
            std::string("p"),
            Lambda::make_term_ptr(std::string("a"), Var::make_term_ptr(0)),
            Var::make_term_ptr(0)
        );
    auto block = TermPrettyPrinter::pretty(test);
    std::cout << block->format_inline() << endl;
    return 0;
}