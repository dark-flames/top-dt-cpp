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

    Closure(Environment env, TermPtr term) : env(env), term(std::move(term)) {}

    Closure copy() {
        return Closure{this->env.copy(), this->term->copy()};
    }
};
}

using value::Closure;
