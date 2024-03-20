#pragma once

#include <Exception/Common.h>
#include <Exception/Declaration.h>
#include <Value/Context.h>
#include <Syntax/SyntaxNode.h>
#include <Value/ValueNode.h>
#include <Pretty/Document.h>
#include <format>
#include <optional>
#include <vector>

class EmptySyntaxVisitorMethod : public Exception {
public:
    const char* method;

    EmptySyntaxVisitorMethod(const char* method) : method(method) {}
};

class ContextFoundLevel : public Exception {
public:
    ContextFoundLevel() = default;
};

class ContextFoundNonLevel : public Exception {
public:
    VTy& ty;

    explicit ContextFoundNonLevel(VTy& ty) : ty(ty) {}
};

class FormalizedTypeCheckErr : public Exception {
public:
    DocumentPtr message;

    explicit FormalizedTypeCheckErr(DocumentPtr message) : message(std::move(message)) {};
};

class TypeCheckTracer;

class TypeCheckException : public Exception {
public:
    virtual DocumentPtr format(TypeCheckTracer& tracer) {
        return std::make_unique<Document>();
    }
};


class UnknownReference : public TypeCheckException {
public:
    std::string ref;

    explicit UnknownReference(
        std::string& ref
    ) : ref(ref) {}

    explicit UnknownReference(
        UnknownEntry& inner
    ) : ref(inner.entry) {}

    DocumentPtr format(TypeCheckTracer& tracer) final;
};

class CyclingDependencyReference : public TypeCheckException {
public:
    std::vector<std::string> cycle;

    explicit CyclingDependencyReference(
        std::vector<std::string> cycle
    ) : cycle(cycle) {}

    DocumentPtr format(TypeCheckTracer& tracer) final;
};

class NonPi : public TypeCheckException {
private:
    SyntaxPtr term;
    VTyPtr ty;
public:
    NonPi(SyntaxPtr term, VTyPtr ty) :
        term(std::move(term)), ty(std::move(ty)) {}

    DocumentPtr format(TypeCheckTracer& tracer) final;
};

class ApplyNonPi : public TypeCheckException {
private:
    SyntaxPtr fun;
    VTyPtr ty;
public:
    ApplyNonPi(SyntaxPtr term, VTyPtr ty) :
        fun(std::move(term)), ty(std::move(ty)) {}

    virtual DocumentPtr format(TypeCheckTracer& tracer);
};

class NotType : public TypeCheckException {
private:
    SyntaxPtr ty;
    VTyPtr tty;
public:
    NotType(SyntaxPtr ty, VTyPtr tty) :
        ty(std::move(ty)), tty(std::move(tty)) {}

    virtual DocumentPtr format(TypeCheckTracer& tracer);
};

class UnificationException : public TypeCheckException {
private:
    SyntaxPtr term;
    VTyPtr ty;
    VTyPtr expect;
public:
    UnificationException(SyntaxPtr term, VTyPtr ty, VTyPtr expect)
        : term(std::move(term)), ty(std::move(ty)), expect(std::move(expect)) {}

    virtual DocumentPtr format(TypeCheckTracer& tracer);
};
