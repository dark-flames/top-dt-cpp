#include <Exception/TypeCheck.h>
#include <Pretty/Tokens.h>
#include <Exception/TypeCheckTracer.h>

using namespace std;

DocumentPtr UnknownReference::format(TypeCheckTracer& tracer) {
    auto res = std::make_unique<Document>();;
    *res << "UnknownReference: `" << this->ref << "`.";
    return res;
}

DocumentPtr CyclingDependencyReference::format(TypeCheckTracer& tracer) {
    auto res = std::make_unique<Document>();;
    *res << "CyclingDependencyReference:" << token::endl;
    bool first = true;
    for (auto& name : this->cycle) {
        if (first) {
            first = false;
        } else {
            *res << token::arrow;
        }
        *res << name;
    }
    return res;
}

DocumentPtr NonPi::format(TypeCheckTracer& tracer) {
    auto res = std::make_unique<Document>();

    auto syn = tracer.pretty_syntax(*this->term);
    auto vty = tracer.pretty_value(*this->ty);

    *res << "Check the lambda expression:" << token::endl;
    *res << syn << token::endl;
    *res << "as a non-Pi type:" << token::endl;
    *res << vty;

    return res;
}

DocumentPtr ApplyNonPi::format(TypeCheckTracer& tracer) {
    auto res = std::make_unique<Document>();

    auto syn = tracer.pretty_syntax(*this->fun);
    auto vty = tracer.pretty_value(*this->ty);
    *res << "Use :" << token::endl;
    *res << syn << token::endl;
    *res << "as a function, but it doesn't typed as:" << token::endl;
    *res << vty;

    return res;
}

DocumentPtr NotType::format(TypeCheckTracer& tracer) {
    auto res = std::make_unique<Document>();

    auto syn = tracer.pretty_syntax(*this->ty);
    auto vty = tracer.pretty_value(*this->tty);
    *res << "Use :" << token::endl;
    *res << syn << token::endl;
    *res << "as a type, but it doesn't typed as:" << token::endl;
    *res << vty;

    return res;
}


DocumentPtr UnificationException::format(TypeCheckTracer& tracer) {
    auto res = std::make_unique<Document>();

    auto syn = tracer.pretty_syntax(*this->term);
    auto a_ty = tracer.pretty_value(*this->ty);
    auto e_ty = tracer.pretty_value(*this->expect);
    *res << "Expression :" << token::endl;
    *res << syn << token::endl;
    *res << "has type:" << token::endl;
    *res << a_ty << token::endl;
    *res << "but expect to be:" << token::endl;
    *res << e_ty << token::endl;

    return res;
}
