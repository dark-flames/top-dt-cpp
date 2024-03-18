#pragma once

#include <Common/Visitor.h>

namespace value {
#define FOR_VALUE_TYPES(E) \
    E(Var, var)            \
    E(Lambda, lambda)      \
    E(LLambda, llambda) \
    E(Pi, pi) \
    E(LPi, lpi)            \
    E(Level, level) \
    E(Univ, univ) \
    E(UnivOmega, univ_omega) \
    E(Spine, spine)   \
    E(App, app)

#define MAKE_SIG(_type, _method) class _type;

FOR_VALUE_TYPES(MAKE_SIG)

#undef MAKE_SIG

#define MAKE_ENUM_ITEM(_type, _method) _type,
enum class ValueTy : int {
    FOR_VALUE_TYPES(MAKE_ENUM_ITEM)
#undef MAKE_ENUM_ITEM
};

}
using value::ValueTy;