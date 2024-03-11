#pragma once

#include <memory>
#include <stdexcept>

template<typename N>
using NodePtr = std::shared_ptr<N>;

template<typename T, typename R>
class Visitor {
public:
    virtual R visit(std::shared_ptr<T>& target) {
        throw std::runtime_error("Implement visit method.");
    }
};