#include <Pretty/Formatter.h>
#include <Pretty/Element.h>
#include <Exception/Common.h>

using namespace std;


FormatterState::FormatterState(SplitType level_bound, int depth, int current_length)
    : level_bound(level_bound), depth(depth), current_length(current_length) {}

Formatter::Formatter(Style style, int max_width, int tab) :
    style(style), max_width(max_width),
    tab_size(tab), WithState(FormatterState(SplitType::ForceSingleLine, 0, 0)) {
    lines = {""};
}

void Formatter::try_place(int length) {
    if (this->get_state().current_length + length > max_width) {
        throw FormatException();
    } else {
        this->get_state().current_length += length;
    }
}

void Formatter::try_split(SplitType level) {
    if (level <= this->get_state().level_bound) { // newline
        this->get_state().current_length = 0;
        this->try_place(this->get_state().depth * tab_size);
    } else { // space
        this->try_place(1);
    }
}

void Formatter::push_string(string& str) {
    this->get_state().current_length += int(str.length());
    this->lines.back() += str;
}

void Formatter::flexible_split(SplitType level) {
    if (level <= this->get_state().level_bound) { // newline
        string l;
        for (int i = 0; i < this->get_state().depth * this->tab_size; ++i) {
            l += " ";
        }
        this->get_state().current_length += this->get_state().depth * this->tab_size;
        this->lines.push_back(l);
    } else { // space
        this->lines.back() += " ";
        this->get_state().current_length += 1;
    }
}

void Formatter::start_block(SplitType level) {
    if (level <= this->get_state().level_bound) {
        ++this->get_state().depth;
    }
}

void Formatter::end_block(SplitType level) {
    if (level <= this->get_state().level_bound) {
        --this->get_state().depth;
    }
}

void Formatter::format(Element& element) {
    this->lines = {""};
    switch (this->style) {
        case Style::Inline:
            this->recovery(FormatterState(SplitType::ForceSingleLine, 0, 0));
            element.format(*this);
            return;
        case Style::HardSplit:
            this->recovery(FormatterState(SplitType::NewLine, 0, 0));
            element.format(*this);
            return;
        case Style::SoftSplit:
            for (
                int cl = static_cast<int>(SplitType::NewLine);
                cl < static_cast<int>(SplitType::Space);
                cl++
                ) {
                this->recovery(FormatterState(static_cast<SplitType>(cl), 0, 0));
                try {
                    element.try_format(*this);
                } catch (FormatException& e) {
                    continue;
                }
                this->recovery(FormatterState(static_cast<SplitType>(cl), 0, 0));
                element.format(*this);
                return;
            }
            throw CannotFormat(this->tab_size);
    }
}

std::string Formatter::to_string() {
    string result;
    bool first = true;
    for (auto& str : this->lines) {
        if (!first) {
            result += "\n";
        } else {
            first = false;
        }
        result += str;
    }

    return result;
}

