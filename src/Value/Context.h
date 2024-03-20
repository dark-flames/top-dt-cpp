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
    ContextNode() {}

    virtual Size size() {
        return 0;
    }

    virtual std::optional<ValueWithSize> find(Id& ref) {
        return {};
    }

    virtual std::optional<ValueWithSize> find(DBIndex index) {
        return {};
    }

    virtual std::optional<Size> find_level(Id& ref) {
        return {};
    }

    virtual std::optional<Size> find_level(DBIndex index) {
        return {};
    }

    virtual ContextNodePtr get_prev() {
        throw std::runtime_error("No previous segment.");
    }

    virtual std::string get_name() {
        throw std::runtime_error("No name.");
    }

    virtual std::optional<VTyPtr> get_ty() {
        throw std::runtime_error("No name.");
    }
};

class ContextConsNode : public ContextNode {
private:
    ContextNodePtr prev;
    Size count;
    Id name;
    VTyPtr ty;
public:
    ContextConsNode(ContextNodePtr& prev, VTyPtr& ty, Id& name) :
        prev(prev), count(prev->size() + 1), name(name), ty(std::move(ty)) {}

    [[nodiscard]] Size size() final {
        return this->count;
    }

    ContextNodePtr get_prev() final {
        return this->prev;
    }

    std::optional<ValueWithSize> find(Id& ref) final;

    std::optional<ValueWithSize> find(DBIndex index) final;

    std::optional<Size> find_level(Id& ref) final;

    std::optional<Size> find_level(DBIndex index) final;

    std::string get_name() final {
        return this->name;
    }

    std::optional<VTyPtr> get_ty() final {
        return this->ty->copy();
    }

    friend class Context;
};

class ContextConsLevelNode : public ContextNode {
private:
    ContextNodePtr prev;
    Size count;
    std::string name;
public:
    ContextConsLevelNode(ContextNodePtr& prev, Id& name) :
        prev(prev), count(prev->size() + 1), name(name) {}

    Size size() final {
        return this->count;
    }

    ContextNodePtr get_prev() final {
        return this->prev;
    }

    virtual std::optional<ValueWithSize> find(Id& ref) final;

    virtual std::optional<ValueWithSize> find(DBIndex index) final;

    virtual std::optional<Size> find_level(Id& ref) final;

    virtual std::optional<Size> find_level(DBIndex index) final;

    std::string get_name() final {
        return this->name;
    }

    std::optional<VTyPtr> get_ty() final {
        return {};
    }

    friend class Context;
};

using VTyWithIndex = std::pair<VTyPtr, DBIndex>;

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

    [[nodiscard]] Size size() {
        return this->tail->size();
    }

    ContextNodePtr get_tail() {
        return this->tail;
    }

    std::optional<VTyWithIndex> find(Id& index);

    std::optional<DBIndex> find_level(Id& index);

    /**
     * This function will now affect *this, but just return the new environment.
     */
    Context push(Id& name, VTyPtr& vty);

    Context push_level(Id& name);

    Context pop();
};