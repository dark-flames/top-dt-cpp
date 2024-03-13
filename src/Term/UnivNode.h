#pragma once

#include <Common/Visitor.h>
#include <Term/Nodes.h>
#include <Term/TermNode.h>

#include <memory>
#include <string>

namespace term {
class LVar : public Term {
public:
    Idx i;

    explicit LVar(Idx i) : i(i) {}

    TermTy ty() final {
        return TermTy::LVar;
    }

    TermPtr copy() final {
        return make_term_ptr<LVar>(this->i);
    }
};

class LZero : public Term {
public:
    LZero() = default;

    TermTy ty() final {
        return TermTy::LZero;
    }

    TermPtr copy() final {
        return make_term_ptr<LZero>();
    }
};

class LSuc : public Term {
public:
    TermPtr level;

    explicit LSuc(TermPtr& level) : level(std::move(level)) {}

    TermTy ty() final {
        return TermTy::LSuc;
    }

    TermPtr copy() final {
        return make_term_ptr<LSuc>(this->level->copy());
    }
};

class LMax : public Term {
public:
    TermPtr l;
    TermPtr r;

    LMax(TermPtr& l, TermPtr& r) : l(std::move(l)), r(std::move(r)) {}

    TermTy ty() final {
        return TermTy::LMax;
    }

    TermPtr copy() final {
        return make_term_ptr<LMax>(this->l->copy(), this->r->copy());
    }
};

class Univ : public Term {
public:
    TermPtr level;

    explicit Univ(TermPtr& l) : level(std::move(l)) {}

    TermTy ty() final {
        return TermTy::Univ;
    }

    TermPtr copy() final {
        return make_term_ptr<Univ>(this->level->copy());
    }
};

class UnivOmega : public Term {
public:
    TermTy ty() final {
        return TermTy::UnivOmega;
    }

    TermPtr copy() final {
        return make_term_ptr<UnivOmega>();
    }
};
}
