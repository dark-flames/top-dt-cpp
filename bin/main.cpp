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

        cout << "finish" << endl;
    } else {
        cerr << "Pleas input filename" << endl;
    }

    return 0;
}