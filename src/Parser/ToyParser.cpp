#include <Parser/ToyParser.h>
#include <memory>

using namespace std;

void ToyParser::parse() {
    this->parser->parse();
}

void ToyParser::push_decl(DeclarationPtr decl) {
    this->result.push_back(std::move(decl));
}