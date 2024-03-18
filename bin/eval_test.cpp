#include <syntax.h>
#include <term.h>
#include <normalize.h>
#include <exception.h>
#include <declaration.h>
#include <type_check.h>
#include <pretty.h>
#include <parser.h>
#include <iostream>
#include <utility>

using namespace std;
using namespace term;


TermPtr id() {
    return lambda("a", var(0));
}

TermPtr zero() {
    return l_lambda("l", abs({"A", "zero", "suc"}, var(1)));
}

TermPtr suc() {
    vector<TermPtr> v;
    v.push_back(l_var(3));
    v.push_back(var(2));
    v.push_back(var(1));
    v.push_back(var(0));

    return lambda(
        "n", l_lambda(
            "l", abs(
                {"A", "zero", "suc"},
                app(var(0), apps(var(4), v))
            )));
}

TermPtr nat(unsigned int i) {
    auto r = zero();
    while (i > 0) {
        r = app(suc(), std::move(r));
        i--;
    }

    return r;
}

TermPtr nat_plus() {
    vector<TermPtr> u;
    u.push_back(l_var(3));
    u.push_back(var(2));
    u.push_back(var(1));
    u.push_back(var(0));

    vector<TermPtr> v;
    v.push_back(l_var(3));
    v.push_back(var(2));
    v.push_back(apps(var(4), u));
    v.push_back(var(0));
    return abs(
        {"n", "m"}, l_lambda(
            "l", abs(
                {"A", "zero", "suc"},
                apps(var(5), v)
            )));
}

int main(int argc, char* argv[]) {
    auto term = app(app(nat_plus(), nat(114)), nat(514));
    auto result = nat(628);

    auto value = eval_term(term);
    auto value_result = eval_term(result);

    auto eq = compare_value(value, value_result);

    if (eq == Equality::Eq) {
        cout << "Equal" << endl;
    } else if (eq == Equality::ConditionallyEq) {
        cout << "Conditionally Equal" << endl;
    } else {
        cout << "UnEqual" << endl;
    }

    return 0;
}