#pragma once

#include <Common/Visitor.h>

namespace syntax {
#define FOR_TERM_TYPES(E) \
    E(Var, var) \
    E(Lambda, lambda)     \
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
enum class SyntaxTy : int {
    FOR_TERM_TYPES(MAKE_ENUM_ITEM)
#undef MAKE_ENUM_ITEM
};
}