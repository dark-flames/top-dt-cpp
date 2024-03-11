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

using namespace std;

namespace value {

template<typename R>
class ValueVisitor : public Visitor<Value, R> {
public:
    virtual R visit(ValuePtr & term) {
#define DEF_CASE(TYPE, METHOD) \
            if (term->ty() == ValueTy::TYPE) { \
                auto ptr = specialize_value_ptr<TYPE>(term); \
                return this->visit_##METHOD(ptr); \
            }
        FOR_VALUE_TYPES(DEF_CASE)
#undef DEF_CASE
        throw std::runtime_error("Unknown value ptr.");
    }

protected:
#define DEF_VISITOR_METHOD(TYPE, METHOD) \
    virtual R visit_##METHOD(NodePtr<TYPE>& node) { \
        ValuePtr ptr = generalize_value_ptr(node); \
        return Visitor<Value, R>::visit(ptr); \
    }

    FOR_VALUE_TYPES(DEF_VISITOR_METHOD)

#undef DEF_VISITOR_METHOD
};

}

