#pragma once

#include <Common/Visitor.h>
#include <Syntax/SyntaxNode.h>
#include <memory>
#include <iostream>
#include <type_traits>

namespace syntax {
template<typename R>
class SyntaxVisitor : public Visitor<Syntax, R> {
public:
    virtual R visit(Syntax& term) final {
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
#define DEF_VISITOR_METHOD(TYPE, METHOD) \
    virtual R visit_##METHOD(TYPE& node) { \
        return Visitor<Syntax, R>::visit(node); \
    }

    FOR_SYN_TYPES(DEF_VISITOR_METHOD)

#undef DEF_VISITOR_METHOD
};
}

using syntax::SyntaxVisitor ;