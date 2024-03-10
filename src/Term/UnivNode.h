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

    static TermPtr make_term_ptr(Idx i) {
        return std::make_shared<LVar>(i);
    }
};

class LZero : public Term {
public:
    LZero() {}

    TermTy ty() final {
        return TermTy::LZero;
    }

    static TermPtr make_term_ptr() {
        return std::make_shared<LZero>();
    }
};

class LSuc : public Term {
public:
    TermPtr level;
    explicit LSuc(TermPtr& level) : level(level) {}

    TermTy ty() final {
        return TermTy::LSuc;
    }

    static TermPtr make_term_ptr(TermPtr& level) {
        return std::make_shared<LSuc>(level);
    }
};

class LMax : public Term {
public:
    TermPtr l;
    TermPtr r;

    LMax(TermPtr& l, TermPtr& r) : l(l), r(r) {}

    TermTy ty() final {
        return TermTy::LMax;
    }

    static TermPtr make_term_ptr(TermPtr& l, TermPtr& r) {
        return std::make_shared<LMax>(l, r);
    }
};

class Univ : public Term {
public:
    TermPtr level;

    explicit Univ(TermPtr& l) : level(l) {}

    TermTy ty() final {
        return TermTy::Univ;
    }

    static TermPtr make_term_ptr(TermPtr& l) {
        return std::make_shared<Univ>(l);
    }
};

class UnivOmega : public Term {
public:
    TermTy ty() final {
        return TermTy::UnivOmega;
    }

    static TermPtr make_term_ptr() {
        return std::make_shared<UnivOmega>();
    }
};
}
