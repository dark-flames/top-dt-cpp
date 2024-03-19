#pragma once

#include <Parser/ToyLexer.h>
#include <Parser/Parser.tab.h>
#include <sstream>

class ToyParser {
private:
    ToyLexer* lexer = nullptr;
    yy::Parser* parser = nullptr;

    UncheckedDecls result;
public:
    explicit ToyParser(std::istream& in) {
        lexer = new ToyLexer(&in);
        //lexer->set_debug(1);
        parser = new yy::Parser(*lexer, *this);
        //parser->set_debug_stream(std::cerr);
        //parser->set_debug_level(1);
    }

    void push_decl(UncheckedPtr decl);

    UncheckedDecls get_result() {
        return std::move(this->result);
    }

    void parse();

    ~ToyParser() {
        delete this->lexer;
        delete this->parser;
    }
};