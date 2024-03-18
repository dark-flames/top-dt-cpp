#pragma once

#include <Term/TermNode.h>
#include <Value/ValueNode.h>
#include <Syntax/SyntaxNode.h>
#include <string>
#include <memory>
#include <stdexcept>

enum class DeclarationState : int {
    UncheckedSignature,
    UncheckedBody,
    Unchecked,
    SignatureOnly,
    Finished,
};

class Declaration {
public:
    Declaration() {}

    virtual DeclarationState state() {
        throw std::runtime_error("Cannot call status() on the base class `Syntax`");
    }
};

using DeclarationPtr = std::unique_ptr<Declaration>;

class WithSignature : public Declaration {
public:
    virtual VTy& get_ty() {
        throw std::runtime_error("Cannot call get_ty() on the base class `WithSignature`");
    }
};

using WithSignaturePtr = std::unique_ptr<WithSignature>;
