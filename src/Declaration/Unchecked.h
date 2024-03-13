#pragma once
#include <Declaration/Common.h>
#include <Syntax/SyntaxNode.h>

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

using UncheckedPtr = std::unique_ptr<UncheckedDeclaration>;
