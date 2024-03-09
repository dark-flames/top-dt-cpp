//
// Created by darkflames on 3/8/24.
//

#include <any>
#include <memory>

template<typename R>
class Operation {
public:
    virtual R perform ();
};

template<typename N, typename R>
class Visitor {
public:
    virtual R visit(std::shared_ptr<N> target);
};








