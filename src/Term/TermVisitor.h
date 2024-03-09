#pragma once

#include <Common/Visitor.h>
#include <Term/TermNode.h>
#include <Term/PiNode.h>
#include <Term/RefNode.h>
#include <Term/PiNode.h>

namespace term {
template<typename R>
class TermVisitor: Visitor<Term, R>, IdxVisitor<R>, LambdaVisitor<R>, AppVisitor<R>, PiVisitor<R>
{
public:
    R visit(std::shared_ptr<Term> target) override;
};
}

