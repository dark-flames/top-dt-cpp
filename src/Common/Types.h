#pragma once
#include <string>

using DBIndex = int;
using DBLevel = int;
using Id = std::string;

using MetaNat = int;
using Size = int;

template<typename S>
class WithState {
private:
    S state;
protected:
    explicit WithState(S& state) : state(state) {}

    explicit WithState(S&& state) : state(state) {}

public:
    S& get_state() {
        return this->state;
    }

    S swap_state(S&& s) {
        auto old = this->state;
        this->state = s;
        return old;
    }

    void recovery(S& s) {
        this->state = s;
    }

    void recovery(S&& s) {
        this->state = s;
    }
};