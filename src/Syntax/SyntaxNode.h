#pragma once

#include <Syntax/Nodes.h>
#include <memory>
#include <variant>
#include <type_traits>
#include <stdexcept>

namespace syntax {

class Syntax;
using SyntaxPtr = NodePtr<Syntax>;

class Syntax {
public:
    virtual SyntaxTy ty() { throw std::runtime_error("Cannot call ty() on the base class `Syntax`"); }

    virtual SyntaxPtr copy() { throw std::runtime_error("Cannot call copy() on the base class `Syntax`"); }
};

template<typename N, typename... Args>
inline SyntaxPtr make_syn_ptr(Args&&... args) {
    SyntaxPtr ptr = std::make_unique<N>(args...);

    return ptr;
}
}

using syntax::Syntax;
using syntax::SyntaxPtr;
