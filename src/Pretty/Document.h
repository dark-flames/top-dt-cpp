#pragma once

#include <vector>
#include <string>
#include <optional>
#include <functional>
#include <memory>
#include <Pretty/Tokens.h>
#include <Pretty/Formatter.h>
#include <Exception/Common.h>
#include <Pretty/Element.h>

enum class BlockWrapper : int {
    None,
    Parentheses,
    Bracket,
    Brace,
};

class Document;

using DocumentPtr = std::unique_ptr<Document>;

class Document {
private:
    std::vector<ElementPtr> elements;
    BlockWrapper wrapper;
public:
    explicit Document(BlockWrapper w = BlockWrapper::None)
        : wrapper(w), elements() {}

    Document& push(ElementPtr&& element);

    Document& set_wrapper(BlockWrapper w);

    Document& operator<<(std::string& str);

    Document& operator<<(std::string&& str);

    Document& operator<<(const char* str);

    Document& operator<<(SplitType& token);

    Document& operator<<(const SplitType& token);

    Document& operator<<(DocumentPtr& document);

    Document& operator<<(DocumentPtr&& document);

    static ElementPtr as_element(DocumentPtr& doc) {
        return make_unique<SubDocumentElement>(std::move(doc));
    }

    friend class SubDocumentElement;
};


