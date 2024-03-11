#include<Value/Helper.h>

#include<memory>
#include<map>

using namespace std;

namespace value {

ValuePtr empty_spine() {
    return make_value_ptr<Spine>();
}

ValuePtr var(Lvl l) {
    return make_value_ptr<Var>(l, empty_spine());
}

ValuePtr neutral_app(ValuePtr s, ValuePtr p) {
    auto ty = s->ty();
    if (ty == ValueTy::Var) {
        auto ptr = specialize_value_ptr<Var>(s);

        auto spine = make_value_ptr<App>(ptr->spine, p);
        ptr->spine = spine;

        return generalize_value_ptr(ptr);
    } else {
        throw runtime_error("Impossible apply: unexpected function node");
    }
}

ValuePtr lambda(Closure body) {
    return make_value_ptr<Lambda>(body);
}

ValuePtr l_lambda(Closure body) {
    return make_value_ptr<LLambda>(body);
}

ValuePtr pi(ValuePtr domain, Closure codomain) {
    return make_value_ptr<Pi>(domain, codomain);
}

ValuePtr l_pi(Closure codomain) {
    return make_value_ptr<LPi>(codomain);
}

ValuePtr l_nat(MetaNat l) {
    return make_value_ptr<Level>(l);
}

ValuePtr l_incr(ValuePtr l, Lvl c) {
    auto level = specialize_value_ptr<Level>(l);
    level->pure += c;

    return generalize_value_ptr(level);
}

ValuePtr l_var(Lvl l) {
    map<Lvl, MetaNat> m;
    m[l] = 0;

    return make_value_ptr<Level>(m, l);
}

ValuePtr l_max(ValuePtr l, ValuePtr r) {
    auto l_level = specialize_value_ptr<Level>(l);
    auto r_level = specialize_value_ptr<Level>(r);

    l_level -> pure = max(l_level->pure, r_level->pure);

    for (const auto& pair : r_level->m) {
        auto l_v = l_level->m.find(pair.first);

        if(l_v == l_level->m.end()) {
            l_level->m.insert(pair);
        } else if (l_v->second > pair.second) {
            l_v->second = pair.second;
        }
    }

    return generalize_value_ptr(l_level);
}

ValuePtr univ(ValuePtr level) {
    return make_value_ptr<Univ>(level);
}

ValuePtr univ_omega() {
    return make_value_ptr<UnivOmega>();
}
}