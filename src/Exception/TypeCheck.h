#pragma once
#include <Exception/Common.h>
#include <Exception/Declaration.h>
#include <Value/Context.h>
#include <Syntax/SyntaxNode.h>
#include <Value/ValueNode.h>
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

    ContextFoundNonLevel(VTy& ty) : ty(ty) {}
};

class TypeCheckException : public Exception {};


class UnknownReference : public TypeCheckException {
public:
    std::string ref;

    explicit UnknownReference(
        std::string& ref
    ) : ref(ref) {}

    explicit UnknownReference(
        UnknownEntry& inner
    ) : ref(inner.entry) {}
};

class CyclingDependencyReference : public TypeCheckException {
public:
    std::vector<std::string> cycle;

    CyclingDependencyReference(
        CyclingDependency& inner
    ) : cycle(inner.cycle) {}
};

class NonPi : public TypeCheckException {};

class ApplyNonPi : public TypeCheckException {};

class NotType : public TypeCheckException {};

class UnificationException : public TypeCheckException {};
