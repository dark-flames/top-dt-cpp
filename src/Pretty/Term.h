#pragma once

#include <Pretty/Common.h>
#include <Common/Visitor.h>
#include <Term/TermNode.h>
#include <Term/TermVisitor.h>
#include <string>

using namespace term;

class LambdaPrettyPrinter final : TermVisitor<PrettyPrintResult>,
                                 PrettyPrint
{
public:
    PrettyPrintResult IdxLambda (std::shared_ptr<Idx> target);

    PrettyPrintResult visitLambda (std::shared_ptr<Lambda> target);

    PrettyPrintResult visitApp (std::shared_ptr<App> target);

    PrettyPrintResult visitPi (std::shared_ptr<Pi> target);
};
