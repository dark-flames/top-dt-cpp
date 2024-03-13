#pragma once

#include <Term/Nodes.h>
#include <memory>
#include <variant>
#include <type_traits>
#include <stdexcept>

namespace term {

class Term;
using TermPtr = NodePtr<Term>;
using Ty = Term;
using TyPtr = TermPtr;

class Term {
public:
    virtual TermTy ty() { throw std::runtime_error("Cannot call ty() on the base class `Term`"); }

    virtual TermPtr copy() { throw std::runtime_error("Cannot call copy() on the base class `Term`"); }
};

template<typename N, typename... Args>
inline TermPtr make_term_ptr(Args&&... args) {
    TermPtr ptr = std::make_unique<N>(args...);

    return ptr;
}

}

using term::Term;
using term::Ty;
using term::TermPtr;
using term::TyPtr;
