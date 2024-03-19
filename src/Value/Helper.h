#pragma once

#include <Value/Nodes.h>
#include <Value/ValueNode.h>
#include <Value/Closure.h>
#include <Value/NeutralNode.h>
#include <Value/PiNode.h>
#include <Value/UnivNode.h>

#include <map>

namespace value {
inline ValuePtr empty_spine() {
    return make_value_ptr<Spine>();
}

inline ValuePtr var(DBLevel l) {
    return make_value_ptr<Var>(l, empty_spine());
}

inline ValuePtr neutral_app(ValuePtr s, ValuePtr p) {
    auto ty = s->ty();
    if (ty == ValueTy::Var) {
        auto s_var = dynamic_cast<Var*>(s.get());

        s_var->spine = make_value_ptr<App>(std::move(s_var->spine), p);

        return s;
    } else {
        throw std::runtime_error("Impossible apply: unexpected function node");
    }
}

inline ValuePtr lambda(Id& name, Closure body) {
    return make_value_ptr<Lambda>(name, body);
}

inline ValuePtr l_lambda(Id& name, Closure body) {
    return make_value_ptr<LLambda>(name, body);
}

inline ValuePtr pi(Id& name, ValuePtr domain, Closure codomain) {
    return make_value_ptr<Pi>(name, domain, codomain);
}

inline ValuePtr l_pi(Id& name, Closure codomain) {
    return make_value_ptr<LPi>(name, codomain);
}

inline ValuePtr l_nat(MetaNat l) {
    return make_value_ptr<Level>(l);
}

inline ValuePtr l_incr(ValuePtr l, DBLevel c) {
    auto level = dynamic_cast<Level*>(l.get());
    level->pure += c;

    return l;
}

inline ValuePtr l_var(DBLevel l) {
    std::map<DBLevel, MetaNat> m;
    m[l] = 0;

    return make_value_ptr<Level>(m, 0);
}

inline ValuePtr l_max(ValuePtr l, ValuePtr r) {
    auto l_level = dynamic_cast<Level*>(l.get());
    auto r_level = dynamic_cast<Level*>(r.get());

    l_level->pure = std::max(l_level->pure, r_level->pure);

    for (const auto& pair : r_level->m) {
        auto l_v = l_level->m.find(pair.first);

        if (l_v == l_level->m.end()) {
            l_level->m.insert(pair);
        } else if (l_v->second > pair.second) {
            l_v->second = pair.second;
        }
    }

    return l;
}

inline ValuePtr univ(ValuePtr level) {
    return make_value_ptr<Univ>(level);
}

inline ValuePtr univ_omega() {
    return make_value_ptr<UnivOmega>();
}
}