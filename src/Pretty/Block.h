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

class Block {
private:
    vector<Line> lines;
    BlockWrapper wrapper;
public:
    Block();

    explicit Block(BlockWrapper w);

    Block &push(const string &token);

    Block &new_line(const string &token);

    string format_with(
            string &space,
            string &line,
            unsigned int tabs, bool incrTabs
        );

    string format_inline();

    Block &push_block(BlockPtr &block);

    Block &sub_block(const function<BlockPtr(BlockPtr &)> &inner);

    Block &parenthesized() {
        this->wrapper = BlockWrapper::Parentheses;
        return *this;
    }

    Block &bracketed() {
        this->wrapper = BlockWrapper::Bracket;
        return *this;
    }

    Block &braced() {
        this->wrapper = BlockWrapper::Brace;
        return *this;
    }

    Block &operator<<(const string &token) {
        return this->push(token);
    }

    Block &operator<<(const char *token) {
        return this->push(token);
    }

    Block &operator<<(BlockPtr &block) {
        return this->push_block(block);
    }
};

class Line {
private:
    vector<string> tokens;
    optional<BlockPtr> extended_blocks;
public:
    Line() : tokens(), extended_blocks() {}

    explicit Line(const string &token) : tokens({token}), extended_blocks() {}

    Line &push(const string &token);

    void append_block(BlockPtr &block);

    string format_with(
            string &space,
            string &line,
            unsigned int tabs,
            bool incrTabs
        );
};

