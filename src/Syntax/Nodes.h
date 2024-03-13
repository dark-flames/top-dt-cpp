#pragma once

#include <Common/Visitor.h>

namespace syntax {
#define FOR_SYN_TYPES(E) \
    E(Ref, ref)          \
    E(Lambda, lambda)    \
    E(App, app)          \
    E(Pi, pi)            \
    E(Univ, univ)        \
    E(Level, level)      \
    E(LNat, lnat)        \
    E(LMax, lmax)        \
    E(LSuc, lsuc)

#define MAKE_SIG(_type, _method) class _type;

FOR_SYN_TYPES(MAKE_SIG)

#undef MAKE_SIG

#define MAKE_ENUM_ITEM(_type, _method) _type,
enum class SyntaxTy : int {
    FOR_SYN_TYPES(MAKE_ENUM_ITEM)
#undef MAKE_ENUM_ITEM
};
}