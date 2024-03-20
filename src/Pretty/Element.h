//
// Created by darkflames on 3/20/24.
//
#pragma once

#include <string>
#include <memory>
#include <Pretty/Tokens.h>
#include <Exception/Common.h>

class Document;

class Formatter;

using DocumentPtr = std::unique_ptr<Document>;

class Element {
public:
    virtual ~Element() {}

    virtual void try_format(Formatter& formatter) {
        throw ImpossibleException("");
    }

    virtual void format(Formatter& formatter) {
        throw ImpossibleException("");
    }
};

class StringElement final : public Element {
private:
    std::string str;
public:
    explicit StringElement(std::string& str) : str(str) {}

    ~StringElement() final = default;

    void try_format(Formatter& formatter) final;

    void format(Formatter& formatter) final;
};

class SplitElement final : public Element {
private:
    SplitType level;
public:
    explicit SplitElement(SplitType level) : level(level) {}

    ~SplitElement() final = default;

    void try_format(Formatter& formatter) final;

    void format(Formatter& formatter) final;
};

class SubDocumentElement final : public Element {
private:
    DocumentPtr document;
public:
    explicit SubDocumentElement(DocumentPtr document);

    ~SubDocumentElement() final;

    void try_format(Formatter& formatter) final;

    void format(Formatter& formatter) final;
};

using ElementPtr = std::unique_ptr<Element>;
