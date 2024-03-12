#pragma once

#include <Common/Visitor.h>
#include <Value/Nodes.h>
#include <memory>
#include <variant>
#include <type_traits>
#include <stdexcept>

namespace value {
class Value;

using ValuePtr = NodePtr<Value>;
class Value {
public:
    virtual ValueTy ty() { throw std::runtime_error("Cannot call ty() on the base class `Value`"); }

    virtual ValuePtr copy() {throw std::runtime_error("Cannot call copy() on the base class `Value`");}
};
template<typename N, typename... Args>
inline ValuePtr make_value_ptr(Args&&... args) {
    ValuePtr ptr = std::make_unique<N>(args...);

    return ptr;
}

template<typename N>
inline Value& generalize_value(N& v) {
    return static_cast<Value&>(v);
}

template<typename N>
inline N& specialize_value(Value& v) {
    return static_cast<N&>(v);
}
}

using value::Value;
using value::ValuePtr;