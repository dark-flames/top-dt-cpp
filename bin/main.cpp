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

        auto pretty_printer = TermPrettyPrinter();
        auto doc = pretty_printer.visit(*result);
        auto element = Document::as_element(doc);
        auto formatter = Formatter(Style::SoftSplit, 20, 2);
        formatter.format(*element);
        cout << "Normalize into:" << formatter.to_string() << endl;

    } else {
        cerr << "Pleas input filename" << endl;
    }

    return 0;
}