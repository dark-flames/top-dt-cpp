#pragma once

#include <Term/Nodes.h>
#include <memory>
#include <variant>
#include <type_traits>
#include <stdexcept>

namespace term {
template<typename N>
using NodePtr = std::shared_ptr<N>;

class Term {
public:
    virtual TermTy ty() { throw std::runtime_error("Cannot call ty() on the base class `Term`"); }
};

using TermPtr = NodePtr<Term>;

template<typename N>
NodePtr<N> specialize_term_ptr(TermPtr& ptr) {
    return std::static_pointer_cast<N>(ptr);
}

template<typename N>
TermPtr generalize_term_ptr(NodePtr<N>& ptr) {
    return std::static_pointer_cast<Term>(ptr);
}

}
