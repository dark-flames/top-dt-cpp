//
// Created by darkflames on 3/11/24.
//
#include <Value/Nodes.h>
#include <Value/ValueNode.h>
#include <Value/Closure.h>
#include <Value/NeutralNode.h>
#include <Value/PiNode.h>
#include <Value/UnivNode.h>

namespace value {

inline ValuePtr empty_spine();

inline ValuePtr var(Lvl l);

ValuePtr neutral_app(ValuePtr s, ValuePtr p);

inline ValuePtr lambda(Closure body);

inline ValuePtr l_lambda(Closure body);

inline ValuePtr pi(ValuePtr domain, Closure codomain);

inline ValuePtr l_pi(Closure codomain);

inline ValuePtr l_nat(MetaNat l);

inline ValuePtr l_incr(ValuePtr l, Lvl c);

inline ValuePtr l_var(Lvl l);

ValuePtr l_max(ValuePtr l, ValuePtr r);

inline ValuePtr univ(ValuePtr level);

inline ValuePtr univ_omega();
}