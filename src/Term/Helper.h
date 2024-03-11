#pragma  once
#include <Term/TermNode.h>
#include <Term/PiNode.h>
#include <Term/RefNode.h>
#include <Term/PiNode.h>
#include <Term/UnivNode.h>
#include <string>

namespace term {
inline TermPtr var(Idx i) {
    return make_term_ptr<Var>(i);
}

inline TermPtr lambda(std::string n, TermPtr b) {
    return make_term_ptr<Lambda>(n, b);
}

inline TermPtr l_lambda(std::string n, TermPtr b) {
    return make_term_ptr<LLambda>(n, b);
}

inline TermPtr app(TermPtr fun, TermPtr param) {
    return make_term_ptr<App>(fun, param);
}

inline TermPtr pi(std::string n, TermPtr domain, TermPtr codomain) {
    return make_term_ptr<Pi>(n, domain, codomain);
}

inline TermPtr l_pi(std::string n, TermPtr codomain) {
    return make_term_ptr<LPi>(n, codomain);
}

inline TermPtr l_var(Idx i) {
    return make_term_ptr<LVar>(i);
}

inline TermPtr l_zero() {
    return make_term_ptr<LZero>();
}

inline TermPtr l_suc(TermPtr level) {
    return make_term_ptr<LSuc>(level);
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