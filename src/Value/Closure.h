#pragma once
#include <Term/TermNode.h>
#include <Value/Env.h>

#include <functional>

namespace value {

class Closure {
public:
    Environment env;
    term::TermPtr term;

    Closure(Environment& env, term::TermPtr& term) : env(env), term(term) {}
};
}
