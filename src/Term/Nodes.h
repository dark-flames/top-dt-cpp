#pragma once

#include <Common/Visitor.h>

namespace term {
using Idx = unsigned int;

#define FOR_TERM_TYPES(E) \
    E(Var, var)           \
    E(DefRef, def_ref)    \
    E(Lambda, lambda)   \
    E(LLambda, llambda) \
    E(App, app) \
    E(Pi, pi)             \
    E(LPi, lpi) \
    E(LVar, lvar) \
    E(LZero, lzero) \
    E(LSuc, lsuc) \
    E(LMax, lmax) \
    E(Univ, univ) \
    E(UnivOmega, univ_omega)


#define MAKE_SIG(_type, _method) class _type;

FOR_TERM_TYPES(MAKE_SIG)

#undef MAKE_SIG

#define MAKE_ENUM_ITEM(_type, _method) _type,
enum class TermTy : int {
    FOR_TERM_TYPES(MAKE_ENUM_ITEM)
#undef MAKE_ENUM_ITEM
};

inline bool is_level_term(TermTy ty) {
    switch (ty) {
        case TermTy::LVar:
        case TermTy::LZero:
        case TermTy::LSuc:
        case TermTy::LMax:
            return true;
        default:
            return false;
    }
}
}

using term::Idx;
using term::TermTy;