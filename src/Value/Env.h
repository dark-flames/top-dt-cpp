#pragma once
#include <Value/ValueNode.h>
#include <Term/Nodes.h>
#include <optional>
#include <vector>
#include <memory>
#include <iostream>

class Environment;

class EnvNode;
using EnvNodePtr = std::shared_ptr<EnvNode>;

class EnvNode {
public:
    virtual unsigned int size() const {
        return 0;
    }

    virtual ValuePtr find(term::Idx  l) const {
        throw std::runtime_error("cannot find level.");
    }

    virtual ValuePtr last() const {
        throw std::runtime_error("No last element on this env.");
    }

    virtual EnvNodePtr get_prev() const {
        throw std::runtime_error("No previous segment.");
    }
};

class EnvConsNode : public EnvNode {
private:
    EnvNodePtr prev;
    unsigned int count;
    ValuePtr value;
public:
    EnvConsNode(EnvNodePtr& prev, ValuePtr& value) :
        prev(prev), value(std::move(value)), count(prev->size() + 1) {}

    virtual unsigned int size() const {
        return this->count;
    }

    ValuePtr find(term::Idx l) const final {
        if (l == 0) {
            return this->value->copy();
        } else {
            return this->prev->find(l - 1);
        }
    }

    ValuePtr last() const final {
        return this->value->copy();
    }

    EnvNodePtr get_prev() const final {
        return this->prev;
    }

    friend class Environment;
};

/**
 * The any instance of the class environment contains a path on the overall environment tree.
 * **Warning:** never modify any things on the environment tree;
 */
class Environment {
private:
    EnvNodePtr tail;

    explicit Environment(EnvNodePtr& tail) : tail(tail) {};

public:
    Environment() {
        this->tail = std::make_shared<EnvNode>();
    }

    ValuePtr find(term::Idx l) const  {
        return this->tail->find(l);
    }

    unsigned int size() const {
        return this->tail->size();
    }

    /**
     * This function will now affect *this, but just return the new environment.
     */
    Environment push(ValuePtr& value) {
        auto ptr = std::make_shared<EnvConsNode>(this->tail, value);
        auto new_tail = std::static_pointer_cast<EnvNode>(ptr);

        return Environment(new_tail);
    }


    [[nodiscard]] ValuePtr last() const {
        return this->tail->last();
    }


    [[nodiscard]] Environment pop() const {
        auto new_tail = this->tail->get_prev();

        return Environment(new_tail);
    }

    Environment copy() {
        return Environment(this->tail);
    }
};