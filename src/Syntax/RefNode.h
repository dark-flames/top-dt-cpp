#pragma once

#include <Common/Visitor.h>
#include <Syntax/Nodes.h>
#include <Syntax/SyntaxNode.h>

#include <memory>

using Identifier = std::string;

namespace syntax {

class Ref : public Syntax {
public:
    Identifier name;

    Ref(Identifier& name) : name(name) {}

    SyntaxTy ty() final {
        return SyntaxTy::Ref;
    }

    SyntaxPtr copy() final {
        return make_syn_ptr<Ref>(this->name);
    }
};
}
