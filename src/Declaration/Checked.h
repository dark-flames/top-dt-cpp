#pragma once
#include <Declaration/Common.h>
#include <Term/TermNode.h>

#include <string>
class CheckedSignature : public WithSignature {
public:
    VTyPtr type;
    SyntaxPtr body;

    CheckedSignature(VTyPtr& type, SyntaxPtr& body) : type(std::move(type)), body(std::move(body)) {}

    DeclarationState state() override {
        return DeclarationState::SignatureOnly;
    }

    VTy& get_ty() final {
        return *this->type;
    }
};

class CheckedDeclaration : public WithSignature {
public:
    VTyPtr type;
    TermPtr body;

    CheckedDeclaration(VTyPtr& type, TermPtr& body) :
        type(std::move(type)), body(std::move(body)) {};

    DeclarationState state() override {
        return DeclarationState::Finished;
    }

    VTy& get_ty() final {
        return *this->type;
    }
};

using SignaturePtr = std::unique_ptr<CheckedSignature>;
using CheckedPtr = std::unique_ptr<CheckedDeclaration>;
