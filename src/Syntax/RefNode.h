#pragma once

#include <Common/Visitor.h>
#include <Common/Types.h>
#include <Syntax/Nodes.h>
#include <Syntax/SyntaxNode.h>

#include <memory>

namespace syntax {

class Ref : public Syntax {
public:
    Id name;

    Ref(Id& name) : name(name) {}

    SyntaxTy ty() final {
        return SyntaxTy::Ref;
    }

    SyntaxPtr copy() final {
        return make_syn_ptr<Ref>(this->name);
    }
};
}
