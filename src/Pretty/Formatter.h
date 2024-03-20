#pragma once

#include <Common/Types.h>
#include <Pretty/Tokens.h>
#include <string>
#include <vector>
#include <exception>

class FormatException : std::exception {
};

class Element;

enum class Style : int {
    Inline,
    SoftSplit,
    HardSplit
};

class FormatterState {
public:
    SplitType level_bound;
    int depth;
    int current_length;

    FormatterState(SplitType level_bound, int depth, int current_length);
};

class Formatter : public WithState<FormatterState> {
private:
    std::vector<std::string> lines;
    Style style;
    int max_width;
    int tab_size;
public:
    explicit Formatter(Style style = Style::Inline, int max_width = 0, int tab = 0);

    void try_place(int length);

    void try_split(SplitType level);

    void push_string(std::string& str);

    void flexible_split(SplitType level);

    void start_block(SplitType level);

    void end_block(SplitType level);

    void format(Element& element);

    std::string to_string();
};
