#pragma once

#include <vector>
#include <string>
#include <optional>
#include <functional>
#include <memory>

class Block;

class Line;

using BlockPtr = std::unique_ptr<Block>;


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
    std::vector<Line> lines;
    BlockWrapper wrapper;
    Indentation style;
    int tabs;
public:
    Block();

    explicit Block(BlockWrapper w, Indentation s, int t);

    Block& push(const std::string& token);

    Block& new_line(const std::string& token);

    std::string format_with(
        const char* tab,
        const char* line,
        int carried_tabs
    );

    std::string format_inline();

    Block& push_block(BlockPtr& block);

    Block& sub_block(std::function<void(Block&)> inner);

    Block& set_wrapper(BlockWrapper w) {
        this->wrapper = w;
        return *this;
    }

    Block& tab(int c = 1) {
        this->tabs += c;
        return *this;
    }

    Block& set_style(Indentation s) {
        this->style = s;
        return *this;
    }

    Block& operator<<(const std::string& token) {
        return this->push(token);
    }

    Block& operator<<(const char* token) {
        return this->push(token);
    }

    Block& operator<<(BlockPtr& block) {
        return this->push_block(block);
    }
};

class Line {
private:
    std::vector<std::string> tokens;
    std::optional<BlockPtr> extended_blocks;
public:
    Line() : tokens(), extended_blocks() {}

    explicit Line(const std::string& token) : tokens({token}), extended_blocks() {}

    Line& push(const std::string& token);

    void append_block(BlockPtr& block);

    std::string format_with(
        const char* tab,
        const char* line,
        int tab_count
    );
};

