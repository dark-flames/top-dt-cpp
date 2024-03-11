#include<Value/Env.h>

using namespace value;
using namespace std;

ValuePtr EnvConsNode::find(Lvl l) const {
    if (this->count - 1 == l) {
        return this->value;
    } else {
        return this->prev->find(l);
    }
}

Environment Environment::push(ValuePtr& value) {
    auto new_tail = static_pointer_cast<EnvNode>(
        make_shared<EnvConsNode>(this->tail, value)
    );

    return Environment(new_tail);
}