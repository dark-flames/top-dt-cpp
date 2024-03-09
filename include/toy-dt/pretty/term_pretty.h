#include <pretty/common.h>
#include <common/visitor.h>
#include <term.h>
#include <string>

using namespace term;

class LambdaPrettyPrinter final: TermVisitor<PrettyPrintResult>,
        PrettyPrint
{
public:
    PrettyPrintResult IdxLambda (std::shared_ptr<Idx> target) {
        return (PrettyPrint(std::to_string(target->v))).perform();
    }

    PrettyPrintResult visitLambda (std::shared_ptr<Lambda> target) {
        auto body = TermVisitor<PrettyPrintResult>::visit(target->body);
        return (lambda() >> (target->name) >> dot() >> body).perform();
    }

    PrettyPrintResult visitApp (std::shared_ptr<App> target) {
        auto fun = TermVisitor<PrettyPrintResult>::visit(target->fun);
        auto param = TermVisitor<PrettyPrintResult>::visit(target->param);
        return ( PrettyPrint(fun) >>= param ).perform();
    }

    PrettyPrintResult visitPi (std::shared_ptr<Pi> target) {
        auto domain = TermVisitor<PrettyPrintResult>::visit(target->domain);
        auto codomain= TermVisitor<PrettyPrintResult>::visit(target->codomain);
        return (pi_ty() >> paren(PrettyPrint(target->name) >>= colon() >>= domain) >> codomain).perform();
    }
};