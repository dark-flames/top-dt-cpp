//
// Created by darkflames on 3/8/24.
//

#include <common/visitor.h>

#include <string>
#include <utility>
#include <variant>
#include <memory>

namespace term {
    template<typename R> class IdxVisitor;
    class Idx;
    template<typename R> class LambdaVisitor;
    class Lambda;
    template<typename R> class AppVisitor;
    class App;
    template<typename R> class PiVisitor;
    class Pi;


    using Term = std::variant<Idx, Lambda, App, Pi>;

    using TermBox = std::shared_ptr<Term>;

    template<typename R>
    class TermVisitor: Visitor<Term, R>, LambdaVisitor<R>, AppVisitor<R>, PiVisitor<R>
    {
    public:
        R visit(std::shared_ptr<Term> target) override {
            std::visit([&](auto &&arg) {
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
            }, *target.get());
        }
    };

    // index

    class Idx{
    public:
        unsigned int v;
        Idx(unsigned int v) : v(v) {}
    };

    template<typename R>
    class IdxVisitor: Visitor<Idx, R> {
    protected:
    public:
        R visit(std::shared_ptr<Idx> target) override {
            return this->visitIdx(target);
        }
    protected:
        virtual R visitIdx(std::shared_ptr<Idx> target);
    };


    // lambda expression
    class Lambda {
    public:
        char* name;
        TermBox body;

        Lambda(char* name, TermBox body) : name(name), body(body) {}
    };

    template<typename R>
    class LambdaVisitor: Visitor<Lambda, R> {
    protected:
    public:
        R visit(std::shared_ptr<Lambda> target) override {
            return this->visitLambda(target);
        }
    protected:
        virtual R visitLambda(std::shared_ptr<Lambda> target);
    };

    // application
    class App {
    public:
        TermBox fun;
        TermBox param;

        App (TermBox fun, TermBox param) : fun(fun), param(param) {}
    };

    template<typename R>
    class AppVisitor: Visitor<App, R> {
    public:
        R visit(std::shared_ptr<App> target) override {
            return this->visitApp(target);
        }
    protected:
        virtual R visitApp(std::shared_ptr<App> target);
    };
    //pi type
    class Pi {
    public:
        char* name;
        TermBox domain;
        TermBox codomain;
        Pi (char* name, TermBox domain, TermBox codomain) : name(name), domain(domain), codomain(codomain) {}
    };

    template<typename R>
    class PiVisitor: Visitor<Pi, R> {
    public:
        R visit(std::shared_ptr<Pi> target) override {
            return this->visitPi(target);
        }
    protected:
        virtual R visitPi(std::shared_ptr<Pi> target);
    };
}
