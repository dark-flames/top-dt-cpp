#pragma once

#include <Term/Nodes.h>
#include <memory>
#include <variant>
#include <type_traits>
#include <stdexcept>

namespace term {

class Term {
public:
    virtual TermTy ty() { throw std::runtime_error("Cannot call ty() on the base class `Term`"); }
};

using TermPtr = NodePtr<Term>;

template<typename N>
inline NodePtr<N> specialize_term_ptr(TermPtr& ptr) {
    return std::static_pointer_cast<N>(ptr);
}

template<typename N>
inline TermPtr generalize_term_ptr(NodePtr<N>& ptr) {
    return std::static_pointer_cast<Term>(ptr);
}

template<typename N, typename... Args>
inline TermPtr make_term_ptr(Args&&... args) {
    auto ptr = std::make_shared<N>(args...);

    return generalize_term_ptr(ptr);
}

}

using term::Term;
using term::TermPtr;
