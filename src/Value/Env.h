#pragma once
#include <Value/ValueNode.h>

#include <optional>
#include <vector>
#include <memory>
#include <iostream>

class Environment;

class EnvNode {
public:
    virtual unsigned int size() const {
        return 0;
    }

    virtual ValuePtr find(Lvl l) const {
        throw std::runtime_error("Unknown Term Ptr.");
    }
};

using EnvNodePtr = std::shared_ptr<EnvNode>;

class EnvConsNode : public EnvNode {
private:
    EnvNodePtr prev;
    unsigned int count;
    ValuePtr value;
public:
    EnvConsNode(EnvNodePtr& prev, ValuePtr& value) :
        prev(prev), value(value), count(prev->size() + 1) {}

    virtual unsigned int size() const {
        return this->count;
    }

    ValuePtr find(Lvl l) const final;

    friend class Environment;
};

/**
 * The any instance of the class environment contains a path on the overall environment tree.
 * **Warning:** never modify any things on the environment tree;
 */
class Environment {
private:
    EnvNodePtr tail;

    Environment(EnvNodePtr& tail) : tail(tail) {};

public:
    Environment() {
        this->tail = std::make_shared<EnvNode>();
    }

    ValuePtr find(Lvl l) const {
        return this->tail->find(l);
    }

    unsigned int size() const {
        return this->tail->size();
    }

    /**
     * This function will now affect *this, but just return the new environment.
     */
    Environment push(ValuePtr& value);
};