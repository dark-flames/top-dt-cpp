#pragma once

#include <vector>
#include <string>
#include <optional>
#include <functional>
#include <memory>

using namespace std;

class Block;

class Line;

using BlockPtr = shared_ptr<Block>;


enum class BlockWrapper : int {
    None,
    Parentheses,
    Bracket,
    Brace,
};

enum class Indentation : int {
    Inline,
    KR, // newline after `{`
    Allman // `{` in an independent line
};

class Block {
private:
    vector<Line> lines;
    BlockWrapper wrapper;
    Indentation style;
    unsigned int tabs;
public:
    Block();

    explicit Block(BlockWrapper w, Indentation s, unsigned int t);

    Block& push(const string& token);

    Block& new_line(const string& token);

    string format_with(
        const char* tab,
        const char* line,
        unsigned int carried_tabs
    );

    string format_inline();

    Block& push_block(BlockPtr& block);

    Block& sub_block(const function<BlockPtr(BlockPtr&)>& inner);

    Block& set_wrapper(BlockWrapper w) {
        this->wrapper = w;
        return *this;
    }

    Block& tab(unsigned int c = 1) {
        this->tabs += c;
        return *this;
    }

    Block& set_style(Indentation s) {
        this->style = s;
        return *this;
    }

    Block& operator<<(const string& token) {
        return this->push(token);
    }

    Block& operator<<(const char* token) {
        return this->push(token);
    }

    Block& operator<<(BlockPtr& block) {
        return this->push_block(block);
    }

    Block& operator<=> (const string& token) {
        return *this << " " << token;
    }

    Block& operator<=>(const char* token) {
        return *this << " " << token;
    }

    Block& operator<=>(BlockPtr& block) {
        return *this << " " << block;
    }
};

class Line {
private:
    vector<string> tokens;
    optional<BlockPtr> extended_blocks;
public:
    Line() : tokens(), extended_blocks() {}

    explicit Line(const string& token) : tokens({token}), extended_blocks() {}

    Line& push(const string& token);

    void append_block(BlockPtr& block);

    string format_with(
        const char* tab,
        const char* line,
        unsigned int tab_count
    );
};

