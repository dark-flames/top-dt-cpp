#pragma once

#include <Common/Types.h>
#include <Declaration/Common.h>
#include <Syntax/SyntaxNode.h>

#include <vector>

class UncheckedDeclaration : public Declaration {
public:
    SyntaxPtr type;
    SyntaxPtr body;

    UncheckedDeclaration(
        SyntaxPtr& type,
        SyntaxPtr& body
    ) : type(std::move(type)), body(std::move(body)) {};

    DeclarationState state() final {
        return DeclarationState::Unchecked;
    }
};

class UncheckedSignature : public Declaration {
public:
    Id name;
    SyntaxPtr type;

    UncheckedSignature(
        Id name,
        SyntaxPtr& type
    ) : name(name), type(std::move(type)) {};

    DeclarationState state() final {
        return DeclarationState::UncheckedSignature;
    }
};

class UncheckedBody : public Declaration {
public:
    Id name;
    SyntaxPtr body;

    UncheckedBody(
        Id name,
        SyntaxPtr& body
    ) : name(name), body(std::move(body)) {};

    DeclarationState state() final {
        return DeclarationState::UncheckedBody;
    }
};

using UncheckedPtr = std::unique_ptr<Declaration>;
using UncheckedDecls = std::vector<UncheckedPtr>;

inline UncheckedPtr raw_signature(Id name, SyntaxPtr type) {
    UncheckedPtr ptr = std::make_unique<UncheckedSignature>(name, type);

    return ptr;
}

inline UncheckedPtr raw_body(Id name, SyntaxPtr body) {
    UncheckedPtr ptr = std::make_unique<UncheckedBody>(name, body);

    return ptr;
}
