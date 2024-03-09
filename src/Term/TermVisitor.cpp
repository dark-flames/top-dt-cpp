#pragma once

#include <Term/TermVisitor.h>
#include <memory>
#include <variant>
#include <stdexcept>


namespace term {
template<typename R>
R TermVisitor<R>::visit(std::shared_ptr<Term> target) {
    return std::visit([&](auto &&arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, Idx>)
            return static_cast<IdxVisitor<R> *>(this) -> visit(target);
        else if constexpr (std::is_same_v<T, Lambda>)
            return static_cast<LambdaVisitor<R> *>(this) -> visit(target);
        else if constexpr (std::is_same_v<T, App>)
            return static_cast<AppVisitor<R> *>(this) -> visit(target);
        else if constexpr (std::is_same_v<T, Pi>)
            return static_cast<PiVisitor<R> *>(this) -> visit(target);
        else
        throw std::logic_error("Impossible Term");
    }, *target);
}
}