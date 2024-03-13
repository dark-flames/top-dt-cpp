#pragma once

#include <Common/Visitor.h>
#include <Syntax/Nodes.h>
#include <Syntax/SyntaxNode.h>

#include <memory>

namespace syntax {

class Ref : public Syntax {
public:
    std::string name;

    Ref(std::string& name) : name(name) {}

    SyntaxTy ty() final {
        return SyntaxTy::Ref;
    }

    SyntaxPtr copy() final {
        return make_syn_ptr<Ref>(this->name);
    }
};
}
