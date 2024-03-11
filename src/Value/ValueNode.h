#pragma once

#include <Common/Visitor.h>
#include <Value/Nodes.h>
#include <memory>
#include <variant>
#include <type_traits>
#include <stdexcept>

namespace value {
class Value {
public:
    virtual ValueTy ty() { throw std::runtime_error("Cannot call ty() on the base class `Value`"); }
};

using ValuePtr = NodePtr<Value>;

template<typename N>
inline NodePtr<N> specialize_value_ptr(ValuePtr & ptr) {
    return std::static_pointer_cast<N>(ptr);
}

template<typename N>
inline ValuePtr generalize_value_ptr(NodePtr<N>& ptr) {
    return std::static_pointer_cast<Value>(ptr);
}

template<typename N, typename... Args>
inline ValuePtr make_value_ptr(Args&&... args) {
    auto ptr = std::make_shared<N>(args...);

    return generalize_value_ptr(ptr);
}
}

using value::Value;
using value::ValuePtr;