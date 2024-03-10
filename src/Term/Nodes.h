#pragma once

#include <Common/Visitor.h>

namespace term {
#define FOR_TERM_TYPES(E) \
    E(Var, var) \
    E(Lambda, lambda) \
    E(App, app) \
    E(Pi, pi)

#define MAKE_SIG(_type, _method) class _type;

FOR_TERM_TYPES(MAKE_SIG)

#undef MAKE_SIG

#define MAKE_ENUM_ITEM(_type, _method) _type,
enum class TermTy : int {
    FOR_TERM_TYPES(MAKE_ENUM_ITEM)
#undef MAKE_ENUM_ITEM
};

}