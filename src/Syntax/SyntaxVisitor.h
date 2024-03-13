#pragma once

#include <Common/Visitor.h>
#include <Syntax/SyntaxNode.h>
#include <Syntax/PiNode.h>
#include <Syntax/RefNode.h>
#include <Syntax/UnivNode.h>
#include <Exception/TypeCheck.h>
#include <memory>
#include <iostream>
#include <type_traits>

namespace syntax {
template<typename R>
class SyntaxVisitor : public Visitor<Syntax, R> {
public:
    virtual R visit(Syntax& term) override {
        auto ptr = &term;
        switch (term.ty()) {
#define DEF_CASE(TYPE, METHOD) \
            case SyntaxTy::TYPE:  \
                return this->visit_##METHOD(*static_cast<TYPE*>(ptr));
            FOR_SYN_TYPES(DEF_CASE)
#undef DEF_CASE
            default:
                throw std::runtime_error("Unknown Syn Ptr.");
        }
    }

protected:
#define STRINGIZE(s) #s
#define CONCAT(s1, s2) STRINGIZE(s1 ## s2)

#define DEF_VISITOR_METHOD(TYPE, METHOD) \
    virtual R visit_##METHOD(TYPE& node) { \
        throw EmptySyntaxVisitorMethod(CONCAT(visit_, METHOD)); \
    }

    FOR_SYN_TYPES(DEF_VISITOR_METHOD)

#undef DEF_VISITOR_METHOD
#undef STRINGIZE
#undef CONCAT
};
}

using syntax::SyntaxVisitor;