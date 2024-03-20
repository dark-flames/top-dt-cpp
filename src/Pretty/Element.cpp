//
// Created by darkflames on 3/20/24.
//

#include <Pretty/Element.h>
#include <Pretty/Document.h>
#include <Pretty/Formatter.h>

using namespace std;


void StringElement::try_format(Formatter& formatter) {
    formatter.try_place(this->str.length());
}

void StringElement::format(Formatter& formatter) {
    formatter.push_string(this->str);
}

void SplitElement::try_format(Formatter& formatter) {
    formatter.try_split(this->level);
}

void SplitElement::format(Formatter& formatter) {
    formatter.flexible_split(this->level);
}

SubDocumentElement::SubDocumentElement(DocumentPtr document) : document(std::move(document)) {}

SubDocumentElement::~SubDocumentElement() {
    this->document.reset(nullptr);
}

void SubDocumentElement::try_format(Formatter& formatter) {
    if (this->document->wrapper == BlockWrapper::None) {
        for (auto& element : this->document->elements) {
            element->try_format(formatter);
        }
    } else {
        formatter.try_place(1);
        formatter.start_block(SplitType::WrappedBlock);

        formatter.try_split(SplitType::WrappedBlock);
        for (auto& element : this->document->elements) {
            element->try_format(formatter);
        }

        formatter.end_block(SplitType::WrappedBlock);
        formatter.try_split(SplitType::WrappedBlock);
        formatter.try_place(1);
    }
}

void SubDocumentElement::format(Formatter& formatter) {
    string prefix;
    string suffix;
    switch (this->document->wrapper) {
        case BlockWrapper::None:
            break;
        case BlockWrapper::Parentheses:
            prefix = "(";
            suffix = ")";
            break;
        case BlockWrapper::Bracket:
            prefix = "[";
            suffix = "]";
            break;
        case BlockWrapper::Brace:
            prefix = "{";
            suffix = "}";
            break;
    }

    if (this->document->wrapper == BlockWrapper::None) {
        for (auto& element : this->document->elements) {
            element->format(formatter);
        }
    } else {
        formatter.push_string(prefix);
        formatter.flexible_split(SplitType::WrappedBlock);
        formatter.start_block(SplitType::WrappedBlock);
        for (auto& element : this->document->elements) {
            element->format(formatter);
        }
        formatter.end_block(SplitType::WrappedBlock);
        formatter.flexible_split(SplitType::WrappedBlock);
        formatter.push_string(suffix);
    }
}