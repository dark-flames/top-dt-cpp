#pragma once

#include <Term/TermNode.h>
#include <Value/Env.h>

#include <functional>
#include <utility>

namespace value {

class Closure {
public:
    Environment env;
    TermPtr term;

    Closure(Environment& env, TermPtr& term) : env(env), term(std::move(term)) {}

    Closure copy() {
        auto e = this->env.copy();
        auto t = this->term->copy();
        return Closure(e, t);
    }
};
}

using value::Closure;
