//
// Created by darkflames on 3/8/24.
//

#include <Term/PiNode.h>
#include <Term/RefNode.h>
#include <Eval/EvalVisitor.h>
#include <Pretty/Term.h>
#include <iostream>

using namespace std;
using namespace term;

int main(int argc, char* argv[]) {
    auto test = LPi::make_term_ptr("l", Pi::make_term_ptr(
        std::string("p"),
        UnivOmega::make_term_ptr(),
        Univ::make_term_ptr(LMax::make_term_ptr(
            LSuc::make_term_ptr(LSuc::make_term_ptr(LVar::make_term_ptr(1))),
            LSuc::make_term_ptr(LZero::make_term_ptr())
        ))
    ));

    auto block = TermPrettyPrinter::pretty_inline(test);
    std::cout << block->format_inline() << endl;
    return 0;
}