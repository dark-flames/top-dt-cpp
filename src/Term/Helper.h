#pragma  once

#include <Term/TermNode.h>
#include <Term/PiNode.h>
#include <Term/RefNode.h>
#include <Term/PiNode.h>
#include <Term/UnivNode.h>
#include <string>
#include <vector>
#include <utility>

namespace term {
inline TermPtr var(DBIndex i) {
    return make_term_ptr<Var>(i);
}

inline TermPtr def_ref(Id entry) {
    return make_term_ptr<DefRef>(entry);
}

inline TermPtr lambda(Id n, TermPtr b) {
    return make_term_ptr<Lambda>(n, b);
}

inline TermPtr abs(std::vector<Id>&& v, TermPtr core) {
    for (auto it = v.rbegin(); it != v.rend(); ++it) {
        core = lambda(*it, std::move(core));
    }

    return core;
}

inline TermPtr l_lambda(Id n, TermPtr b) {
    return make_term_ptr<LLambda>(n, b);
}

inline TermPtr l_abs(std::vector<Id>&& v, TermPtr core) {
    for (auto it = v.rbegin(); it != v.rend(); ++it) {
        core = l_lambda(*it, std::move(core));
    }

    return core;
}

inline TermPtr app(TermPtr fun, TermPtr param) {
    return make_term_ptr<App>(fun, param);
}

inline TermPtr apps(TermPtr fun, std::vector<TermPtr>& params) {
    for (auto& p : params) {
        fun = app(std::move(fun), std::move(p));
    }

    return fun;
}

inline TermPtr apps(TermPtr fun, std::vector<DBIndex>&& params) {
    for (auto p : params) {
        fun = app(std::move(fun), var(p));
    }

    return fun;
}

inline TermPtr pi(std::string n, TermPtr domain, TermPtr codomain) {
    return make_term_ptr<Pi>(n, domain, codomain);
}

inline TermPtr forall(
    std::vector<std::pair<Id, TermPtr>> v,
    TermPtr core
) {
    for (auto it = v.rbegin(); it != v.rend(); ++it) {
        core = pi(
            it->first,
            std::move(it->second),
            std::move(core)
        );
    }

    return core;
}

inline TermPtr l_pi(std::string n, TermPtr codomain) {
    return make_term_ptr<LPi>(n, codomain);
}

inline TermPtr l_forall(
    std::vector<Id>&& v,
    TermPtr core
) {
    for (auto it = v.rbegin(); it != v.rend(); ++it) {
        core = l_pi(*it, std::move(core));
    }

    return core;
}

inline TermPtr l_var(DBIndex i) {
    return make_term_ptr<LVar>(i);
}

inline TermPtr l_zero() {
    return make_term_ptr<LZero>();
}

inline TermPtr l_suc(TermPtr level) {
    return make_term_ptr<LSuc>(level);
}

inline TermPtr l_plus_nat(TermPtr level, unsigned int l) {
    while (l > 0) {
        level = l_suc(std::move(level));
        --l;
    }

    return level;
}

inline TermPtr l_nat(MetaNat l) {
    return l_plus_nat(l_zero(), l);
}

inline TermPtr l_max(TermPtr l, TermPtr r) {
    return make_term_ptr<LMax>(l, r);
}

inline TermPtr univ(TermPtr level) {
    return make_term_ptr<Univ>(level);
}

inline TermPtr univ_omega() {
    return make_term_ptr<UnivOmega>();
}
}