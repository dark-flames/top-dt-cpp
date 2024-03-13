//
// Created by darkflames on 3/10/24.
//
#include <Pretty/Block.h>
#include <numeric>
#include <format>

using namespace std;

string repeat(const char* str, unsigned int n) {
    std::string r;
    for (int i = 0; i < n; ++i) {
        r += str;
    }
    return r;
}

Block::Block() {
    Block(BlockWrapper::None, Indentation::Inline, 0);
}

Block::Block(BlockWrapper w, Indentation s, unsigned int t) {
    lines.emplace_back();
    wrapper = w;
    style = s;
    tabs = t;
}

Block& Block::push(const string& token) {
    this->lines.back().push(token);

    return *this;
}

Block& Block::new_line(const string& token) {
    this->lines.push_back(Line(token));

    return *this;
}

string Block::format_with(
    const char* tab,
    const char* line,
    unsigned int carried_tabs
) {
    string current;
    for (auto i = this->lines.begin(); i != this->lines.end(); ++i) {
        if (i != this->lines.begin() && i != this->lines.end() - 1) {
            current += line;
        }
        current += i->format_with(tab, line, carried_tabs + this->tabs);
    }

    const char* s[] = {"", "", ""};

    switch (this->style) {
        case Indentation::Inline:
            break;
        case Indentation::KR:
            s[1] = line;
            s[2] = line;
        case Indentation::Allman:
            s[0] = line;
    }

    switch (this->wrapper) {
        case BlockWrapper::Parentheses:
            return format("{}({}{}{})", s[0], s[1], current, s[2]);
        case BlockWrapper::Brace:
            return format("{}[{}{}{}]", s[0], s[1], current, s[2]);
        case BlockWrapper::Bracket:
            return format("{}{}{}{}{}{}", s[0], "{", s[1], current, s[2], "}");
        default:
            return current;
    }
}

string Block::format_inline() {
    return this->format_with(" ", "", 0);
}

Block& Block::push_block(BlockPtr& block) {
    this->lines.back().append_block(block);
    this->lines.emplace_back();

    return *this;
}

Block& Block::sub_block(const function<BlockPtr(BlockPtr&)>& inner) {
    auto block = make_unique<Block>(
        BlockWrapper::None,
        this->style,
        0
    );
    auto res = inner(block);
    this->push_block(res);
    return *this;
}

Line& Line::push(const string& token) {
    this->tokens.push_back(token);
    return *this;
}

void Line::append_block(BlockPtr& block) {
    this->extended_blocks = std::move(block);
}

string Line::format_with(
    const char* tab,
    const char* line,
    unsigned int tab_count
) {
    string current = repeat(tab, tab_count);
    for (const auto& token : this->tokens) {
        current += token;
    }
    if (this->extended_blocks.has_value()) {
        current += this->extended_blocks.value()->format_with(
            tab,
            line,
            tab_count
        );
    }

    return current;
}