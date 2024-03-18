#pragma once

#include <Common/Visitor.h>
#include <Common/Types.h>
#include <Term/Nodes.h>
#include <Term/TermNode.h>

#include <memory>
#include <string>

namespace term {

class Var : public Term {
public:
    DBIndex i;

    explicit Var(DBIndex i) : i(i) {}

    TermTy ty() final {
        return TermTy::Var;
    }

    TermPtr copy() final {
        return make_term_ptr<Var>(this->i);
    }
};

class DefRef : public Term {
public:
    Id entry;

    explicit DefRef(Id entry) : entry(entry) {}

    TermTy ty() final {
        return TermTy::DefRef;
    }

    TermPtr copy() final {
        return make_term_ptr<DefRef>(this->entry);
    }
};
}
