//
// Created by darkflames on 3/10/24.
//
#include <Pretty/Document.h>

using namespace std;

Document& Document::push(ElementPtr&& element) {
    this->elements.push_back(std::move(element));
    return *this;
}

Document& Document::set_wrapper(BlockWrapper w) {
    this->wrapper = w;
    return *this;
}

Document& Document::operator<<(std::string& str) {
    auto e = make_unique<StringElement>(str);
    return this->push(std::move(e));
}

Document& Document::operator<<(std::string&& str) {
    return *this << str;
}

Document& Document::operator<<(const char* str) {
    string s = str;
    return *this << s;
}

Document& Document::operator<<(SplitType& token) {
    return this->push(make_unique<SplitElement>(token));
}

Document& Document::operator<<(const SplitType& token) {
    return this->push(make_unique<SplitElement>(token));
}

Document& Document::operator<<(DocumentPtr& document) {
    return this->push(make_unique<SubDocumentElement>(std::move(document)));
}

Document& Document::operator<<(DocumentPtr&& document) {
    return this->push(make_unique<SubDocumentElement>(std::move(document)));
}


