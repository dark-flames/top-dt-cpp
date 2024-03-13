#pragma once
#include <Value/ValueNode.h>
#include <Term/Nodes.h>
#include <Syntax/RefNode.h>

#include <optional>
#include <vector>
#include <memory>
#include <utility>

class Context;

class ContextNode;
using ContextNodePtr = std::shared_ptr<ContextNode>;
using ValueWithSize = std::pair<VTyPtr, Size>;

class ContextNode {
public:
    virtual Size size() const {
        return 0;
    }

    virtual std::optional<ValueWithSize> find(Identifier& ref) const {
        return {};
    }

    virtual std::optional<ValueWithSize> find(Idx index) const {
        return {};
    }

    virtual std::optional<Size> find_level(Identifier& ref) const {
        return {};
    }

    virtual std::optional<Size> find_level(Idx index) const {
        return {};
    }

    virtual ContextNodePtr get_prev() const {
        throw std::runtime_error("No previous segment.");
    }
};

class ContextConsNode : public ContextNode {
private:
    ContextNodePtr prev;
    Size count;
    Identifier name;
    VTyPtr ty;
public:
    ContextConsNode(ContextNodePtr& prev, VTyPtr& ty, Identifier& name) :
        prev(prev), count(prev->size() + 1), name(name), ty(std::move(ty)) {}

    Size size() const final {
        return this->count;
    }

    ContextNodePtr get_prev() const final {
        return this->prev;
    }

    virtual std::optional<ValueWithSize> find(Identifier& ref) const final;

    virtual std::optional<ValueWithSize> find(Idx index) const final;

    virtual std::optional<Size> find_level(Identifier& ref) const final;

    virtual std::optional<Size> find_level(Idx index) const final;

    friend class Context;
};

class ContextConsLevelNode : public ContextNode {
private:
    ContextNodePtr prev;
    Size count;
    std::string name;
public:
    ContextConsLevelNode(ContextNodePtr& prev, Identifier& name) :
        prev(prev), count(prev->size() + 1), name(name) {}

    Size size() const final {
        return this->count;
    }

    ContextNodePtr get_prev() const final {
        return this->prev;
    }

    virtual std::optional<ValueWithSize> find(Identifier& ref) const final;

    virtual std::optional<ValueWithSize> find(Idx index) const final;

    virtual std::optional<Size> find_level(Identifier& ref) const final;

    virtual std::optional<Size> find_level(Idx index) const final;

    friend class Context;
};

using VTyWithIndex = std::pair<VTyPtr, Idx>;

/**
 * The any instance of the class environment contains a path on the overall context tree.
 * **Warning:** never modify any things on the environment tree;
 */
class Context {
private:
    ContextNodePtr tail;

    explicit Context(ContextNodePtr& tail) : tail(tail) {};
public:
    Context() {
        this->tail = std::make_shared<ContextNode>();
    }

    [[nodiscard]] Size size() const {
        return this->tail->size();
    }

    template<class T>
    std::optional<VTyWithIndex> find(T& index) const;

    template<class T>
    std::optional<Idx> find_level(T& index) const;

    /**
     * This function will now affect *this, but just return the new environment.
     */
    Context push(Identifier & name, VTyPtr& vty);

    Context push_level(Identifier & name);

    Context pop();
};