#include <syntax.h>
#include <term.h>
#include <normalize.h>
#include <exception.h>
#include <declaration.h>
#include <type_check.h>
#include <pretty.h>
#include <parser.h>
#include <iostream>
#include <utility>
#include <fstream>

using namespace std;
using namespace term;

int main(int argc, char* argv[]) {
    if (argc == 2) {
        std::ifstream ifs(argv[1]);
        cout << "Opening: " << argv[1] << endl;

        ToyParser parser(ifs);

        parser.parse();

        auto decls = parser.get_result();

        auto type_checker = new TypeChecker();

        for (auto&& decl : decls) {
            type_checker->add_decl(decl);
        }
        auto result = type_checker->normalize_entry("main");
        delete type_checker;
        auto printer_state = make_shared<TermPrettyPrinterState>();
        auto pretty_printer = TermPrettyPrinter(printer_state);

        auto block = pretty_printer.pretty_inline(result);
        cout << "Normalize into:" << block->format_inline() << endl;

    } else {
        cerr << "Pleas input filename" << endl;
    }

    return 0;
}