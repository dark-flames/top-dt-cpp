#pragma once

#include <Common/Visitor.h>
#include <Syntax/Nodes.h>
#include <Syntax/SyntaxNode.h>

#include <memory>
#include <string>

namespace syntax {
class LNat : public Syntax {
public:
    MetaNat l;

    explicit LNat(MetaNat l) : l(l) {}

    SyntaxTy ty() final {
        return SyntaxTy::LNat;
    }

    SyntaxPtr copy() final {
        return make_syn_ptr<LNat>(this->l);
    }
};

class LSuc : public Syntax {
public:
    SyntaxPtr level;

    explicit LSuc(SyntaxPtr& level) : level(std::move(level)) {}

    SyntaxTy ty() final {
        return SyntaxTy::LSuc;
    }

    SyntaxPtr copy() final {
        return make_syn_ptr<LSuc>(this->level->copy());
    }
};

class LMax : public Syntax {
public:
    SyntaxPtr l;
    SyntaxPtr r;

    LMax(SyntaxPtr& l, SyntaxPtr& r) : l(std::move(l)), r(std::move(r)) {}

    SyntaxTy ty() final {
        return SyntaxTy::LMax;
    }

    SyntaxPtr copy() final {
        return make_syn_ptr<LMax>(this->l->copy(), this->r->copy());
    }
};

class Univ : public Syntax {
public:
    SyntaxPtr level;

    Univ(SyntaxPtr& l) : level(std::move(l)){}

    SyntaxTy ty() final {
        return SyntaxTy::Univ;
    }

    SyntaxPtr copy() final {
        return make_syn_ptr<Univ>(this->level->copy());
    }
};

class Level : public Syntax {
public:
    Level() = default;

    SyntaxTy ty() final {
        return SyntaxTy::Level;
    }

    SyntaxPtr copy() final {
        return make_syn_ptr<Level>();
    }
};
}
