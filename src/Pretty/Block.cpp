//
// Created by darkflames on 3/10/24.
//
#include <Pretty/Block.h>
#include <numeric>

using namespace std;

string repeat(string &str, unsigned int n) {
    std::string r;
    for (int i = 0; i < n; ++i) {
        r += str;
    }
    return r;
}

Block::Block() {
    lines.emplace_back();
    wrapper = BlockWrapper::None;
}

Block::Block(BlockWrapper w) {
    lines.emplace_back();
    wrapper = w;
}

Block &Block::push(const string &token) {
    this->lines.back().push(token);

    return *this;
}

Block &Block::new_line(const string &token) {
    this->lines.push_back(Line(token));

    return *this;
}

string Block::format_with(
        string &space,
        string &line,
        unsigned int tabs,
        bool incrTabs
    ) {
    string current;
    for (auto &l: this->lines) {
        current += l.format_with(space, line, tabs, incrTabs);
    }

    switch (this->wrapper) {
        case BlockWrapper::None:
            return current;
        case BlockWrapper::Parentheses:
            return "(" + current + ")";
        case BlockWrapper::Brace:
            return "[" + current + "]";
        case BlockWrapper::Bracket:
            return "{" + current + "}";
    }

    return current;
}

string Block::format_inline() {
    string e;
    return this->format_with(e, e, 0, false);
}

Block &Block::push_block(BlockPtr &block) {
    this->lines.back().append_block(block);
    this->lines.emplace_back();

    return *this;
}

Block &Block::sub_block(const function<BlockPtr(BlockPtr &)> &inner) {
    auto block = make_shared<Block>();
    auto res = inner(block);
    this->push_block(res);
    return *this;
}

Line &Line::push(const string &token) {
    this->tokens.push_back(token);
    return *this;
}

void Line::append_block(BlockPtr &block) {
    this->extended_blocks = block;
}

string Line::format_with(
        string &space,
        string &line,
        unsigned int tabs,
        bool incrTabs
    ) {
    string current;
    auto s = repeat(space, tabs);
    for (const auto &token: this->tokens) {
        if (!current.empty()) {
            current += s;
        }
        current += token;
    }

    if (this->extended_blocks.has_value()) {
        current += this->extended_blocks.value()->format_with(
                space,
                line,
                incrTabs ? tabs + 1 : tabs,
                incrTabs
        );
    }
    current += line;

    return current;
}