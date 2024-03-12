#pragma once

#include <Common/Visitor.h>
#include <Value/ValueNode.h>
#include <Value/RefNode.h>
#include <Value/PiNode.h>
#include <Value/UnivNode.h>
#include <Value/NeutralNode.h>
#include <memory>
#include <iostream>
#include <type_traits>

namespace value {

template<typename R>
class ValueVisitor : public Visitor<Value, R> {
public:
    virtual R visit(Value& value) {
        auto ptr = &value;
        switch (value.ty()) {
#define DEF_CASE(TYPE, METHOD) \
            case ValueTy::TYPE:  \
                return this->visit_##METHOD(*static_cast<TYPE*>(ptr));
            FOR_VALUE_TYPES(DEF_CASE)
#undef DEF_CASE
            default:
                throw std::runtime_error("Unknown Term Ptr.");
        }
    }

protected:
#define DEF_VISITOR_METHOD(TYPE, METHOD) \
    virtual R visit_##METHOD(TYPE& node) { \
        return Visitor<Value, R>::visit(node); \
    }

    FOR_VALUE_TYPES(DEF_VISITOR_METHOD)

#undef DEF_VISITOR_METHOD
};
}

using value::ValueVisitor;

