#include <Value/Context.h>
#include <Exception/TypeCheck.h>
#include <iostream>
using namespace std;

optional<ValueWithSize> ContextConsNode::find(Id& ref) {
    if (this->name == ref) {
        return make_pair(this->ty->copy(), this->count);
    } else {
        return this->prev->find(ref);
    }
}

optional<ValueWithSize> ContextConsNode::find(DBIndex index) {
    if (index == 0) {
        return make_pair(this->ty->copy(), this->count);
    } else {
        return this->prev->find(index - 1);
    }
}

optional<Size> ContextConsNode::find_level(Id& ref) {
    if (this->name == ref) {
        throw ContextFoundNonLevel(*this->ty);
    } else {
        return this->prev->find_level(ref);
    }
}

optional<Size> ContextConsNode::find_level(DBIndex index) {
    if (index == 0) {
        throw ContextFoundNonLevel(*this->ty);
    } else {
        return this->prev->find_level(index - 1);
    }
}

optional<ValueWithSize> ContextConsLevelNode::find(Id& ref) {
    if (this->name == ref) {
        throw ContextFoundLevel();
    } else {
        return this->prev->find(ref);
    }
}

optional<ValueWithSize> ContextConsLevelNode::find(DBIndex index) {
    if (index == 0) {
        throw ContextFoundLevel();
    } else {
        return this->prev->find(index - 1);
    }
}

optional<Size> ContextConsLevelNode::find_level(Id& ref) {
    if (this->name == ref) {
        return this->count;
    } else {
        return this->prev->find_level(ref);
    }
}

optional<Size> ContextConsLevelNode::find_level(DBIndex index) {
    if (index == 0) {
        return this->count;
    } else {
        return this->prev->find_level(index - 1);
    }
}

optional<VTyWithIndex> Context::find(Id& index) {
    auto res = this->tail->find(index);
    if (res.has_value()) {
        std::pair<ValuePtr, DBIndex> r = std::make_pair(
            std::move(res.value().first),
            this->size() - res.value().second
        );
        return r;
    } else {
        return {};
    }
}

std::optional<DBIndex> Context::find_level(Id& index) {
    auto res = this->tail->find_level(index);
    if (res.has_value()) {
        return this->size() - res.value();
    } else {
        return {};
    }
}

Context Context::push(Id& name, VTyPtr& vty) {
    auto ptr = make_shared<ContextConsNode>(this->tail, vty, name);
    auto new_tail = std::static_pointer_cast<ContextNode>(ptr);

    return Context(new_tail);
}

Context Context::push_level(Id& name) {
    auto ptr = make_shared<ContextConsLevelNode>(this->tail, name);
    auto new_tail = std::static_pointer_cast<ContextNode>(ptr);

    return Context(new_tail);
}

Context Context::pop() {
    auto new_tail = this->tail->get_prev();

    return Context(new_tail);
}

