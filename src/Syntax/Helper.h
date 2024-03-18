#pragma once
#include <Syntax/SyntaxNode.h>
#include <Syntax/RefNode.h>

namespace syntax {

inline SyntaxPtr ref(std::string entry) {
    return make_syn_ptr<Ref>(entry);
}

inline SyntaxPtr lambda(std::string bind, SyntaxPtr body) {
    return make_syn_ptr<Lambda>(bind, body);
}

inline SyntaxPtr app(SyntaxPtr f, SyntaxPtr p) {
    return make_syn_ptr<App>(f, p);
}

inline SyntaxPtr pi(std::string name, SyntaxPtr domain, SyntaxPtr codomain) {
    return make_syn_ptr<Pi>(name, domain, codomain);
}

inline SyntaxPtr l_nat(MetaNat l) {
    return make_syn_ptr<LNat>(l);
}

inline SyntaxPtr l_suc(SyntaxPtr level) {
    return make_syn_ptr<LSuc>(level);
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