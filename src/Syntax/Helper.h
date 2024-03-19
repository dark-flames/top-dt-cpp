#pragma once
#include <Syntax/SyntaxNode.h>
#include <Syntax/RefNode.h>
#include <Syntax/PiNode.h>
#include <Syntax/UnivNode.h>

namespace syntax {

inline SyntaxPtr ref(Id entry) {
    return make_syn_ptr<Ref>(entry);
}

inline SyntaxPtr lambda(Id bind, SyntaxPtr body) {
    return make_syn_ptr<Lambda>(bind, body);
}

inline SyntaxPtr bind(std::vector<Id>& vars, SyntaxPtr body) {
    auto result = std::move(body);
    for (auto name = vars.rbegin(); name != vars.rend(); ++name) {
        result = lambda(*name, std::move(result));
    }

    return result;
}

inline SyntaxPtr app(SyntaxPtr f, SyntaxPtr p) {
    return make_syn_ptr<App>(f, p);
}

inline SyntaxPtr pi(Id name, SyntaxPtr domain, SyntaxPtr codomain) {
    return make_syn_ptr<Pi>(name, domain, codomain);
}

inline SyntaxPtr l_nat(MetaNat l) {
    return make_syn_ptr<LNat>(l);
}

inline SyntaxPtr l_incr(SyntaxPtr level, MetaNat l) {
    auto result = std::move(level);
    while (l > 0) {
        result = make_syn_ptr<LSuc>(std::move(result));
        --l;
    }
    return result;
}

inline SyntaxPtr l_max(SyntaxPtr l, SyntaxPtr r) {
    return make_syn_ptr<LMax>(l, r);
}

inline SyntaxPtr univ(SyntaxPtr level) {
    return make_syn_ptr<Univ>(level);
}

inline SyntaxPtr level() {
    return make_syn_ptr<Level>();
}

}