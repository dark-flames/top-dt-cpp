#include <Value/Context.h>
#include <Exception/TypeCheck.h>

using namespace std;

optional<ValueWithSize> ContextConsNode::find(Identifier& ref) const {
    if (this->name == name) {
        return make_pair(this->ty->copy(), this->count);
    } else {
        return this->prev->find(ref);
    }
}

optional<ValueWithSize> ContextConsNode::find(Idx index) const {
    if (index == 0) {
        return make_pair(this->ty->copy(), this->count);
    } else {
        return this->prev->find(index - 1);
    }
}

optional<Size> ContextConsNode::find_level(Identifier& ref) const {
    if (this->name == name) {
        throw ContextFoundNonLevel(*this->ty);
    } else {
        return this->prev->find_level(ref);
    }
}

optional<Size> ContextConsNode::find_level(Idx index) const {
    if (index == 0) {
        throw ContextFoundNonLevel(*this->ty);
    } else {
        return this->prev->find_level(index - 1);
    }
}

optional<ValueWithSize> ContextConsLevelNode::find(Identifier& ref) const {
    if (this->name == name) {
        throw ContextFoundLevel();
    } else {
        return this->prev->find(ref);
    }
}

optional<ValueWithSize> ContextConsLevelNode::find(Idx index) const {
    if (index == 0) {
        throw ContextFoundLevel();
    } else {
        return this->prev->find(index - 1);
    }
}

optional<Size> ContextConsLevelNode::find_level(Identifier& ref) const {
    if (this->name == name) {
        return this->count;
    } else {
        return this->prev->find_level(ref);
    }
}

optional<Size> ContextConsLevelNode::find_level(Idx index) const {
    if (index == 0) {
        return this->count;
    } else {
        return this->prev->find_level(index - 1);
    }
}

template<class T>
optional<VTyWithIndex> Context::find(T& index) const {
    auto res = this->tail->find(index);
    if (res.has_value()) {
        std::pair<ValuePtr, Idx> r = std::make_pair(
            std::move(res.value().first),
            this->size() - res.value().second
        );
        return r;
    } else {
        return {};
    }
}

template<class T>
std::optional<Idx> Context::find_level(T& index) const {
    auto res = this->tail->find_level(index);
    if (res.has_value()) {
        return this->size() - res.value();
    } else {
        return {};
    }
}

Context Context::push(Identifier& name, VTyPtr& vty) {
    auto ptr = make_shared<ContextConsNode>(this->tail, vty, name);
    auto new_tail = std::static_pointer_cast<ContextNode>(ptr);

    return Context(new_tail);
}

Context Context::push_level(Identifier& name) {
    auto ptr = make_shared<ContextConsLevelNode>(this->tail, name);
    auto new_tail = std::static_pointer_cast<ContextNode>(ptr);

    return Context(new_tail);
}

Context Context::pop() {
    auto new_tail = this->tail->get_prev();

    return Context(new_tail);
}


