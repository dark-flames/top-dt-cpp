#include <TypeCheck/LevelCheckVisitor.h>
#include <TypeCheck/TypeChecker.h>

using namespace std;

TermPtr LevelCheckVisitor::visit_ref(syntax::Ref& node) {
    return this->type_checker->find_level_ref(node.name);
}

TermPtr LevelCheckVisitor::visit_lmax(syntax::LMax& node) {
    return term::l_max(this->visit(*node.l), this->visit(*node.r));
}

TermPtr LevelCheckVisitor::visit_lnat(syntax::LNat& node) {
    return term::l_nat(node.l);
}

TermPtr LevelCheckVisitor::visit_lsuc(syntax::LSuc& node) {
    return term::l_suc(this->visit(*node.level));
}

TermPtr LevelCheckVisitor::visit(Syntax& node) {
    switch (node.ty()) {
        case SyntaxTy::Ref:
        case SyntaxTy::LMax:
        case SyntaxTy::LNat:
        case SyntaxTy::LSuc:
            return SyntaxVisitor<TermPtr>::visit(node);
        default:
            auto e = NotLevelTerm(node.copy());
            this->type_checker->throw_err(e);
            throw ImpossibleException("");
    }
}

