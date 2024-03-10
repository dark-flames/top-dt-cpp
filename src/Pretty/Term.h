#pragma once

#include <Pretty/Block.h>
#include <Term/Nodes.h>
#include <Term/RefNode.h>
#include <Term/TermVisitor.h>
#include <string>
#include <memory>
#include <utility>

using namespace term;

class TermPrettyPrinterState {
    vector<std::string> name_stack;
public:
    friend class TermPrettyPrinter;
};



using StatePtr = shared_ptr<TermPrettyPrinterState>;

class TermPrettyPrinter : TermVisitor<void> {
private:
    BlockPtr result;
    StatePtr state;
public:
    explicit TermPrettyPrinter(StatePtr s) {
        result = make_shared<Block>(BlockWrapper::None);
        state = std::move(s);
    }

    void visit_var(NodePtr<Var> &target) final;

    void visit_lambda(NodePtr<Lambda> &target) final;

    void visit_app(NodePtr<App> &target) final;

    void visit_pi(NodePtr<Pi> &target) final;

    TermPrettyPrinter& push_name(string& n) {
        this->state->name_stack.push_back(n);
        return *this;
    }

    TermPrettyPrinter& pop_name() {
        this->state->name_stack.pop_back();
        return *this;
    }

    string get_name(Idx i) {
        return this->state->name_stack.at(-i);
    }

    BlockPtr sub_pretty(TermPtr& term) {
        auto printer = new TermPrettyPrinter(this->state);

        printer->visit(term);

        return printer->result;
    }

    static BlockPtr pretty(TermPtr& term) {
        auto state = make_shared<TermPrettyPrinterState>();
        auto printer = new TermPrettyPrinter(state);

        printer->visit(term);

        return printer->result;
    }
};


