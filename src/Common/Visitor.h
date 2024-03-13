#pragma once

#include <memory>
#include <stdexcept>

template<typename N>
using NodePtr = std::unique_ptr<N>;

template<typename T, typename R>
class Visitor {
public:
    virtual R visit(T& target) {
        throw std::runtime_error("Implement visit method.");
    }
};

using MetaNat = unsigned int;