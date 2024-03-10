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

class TermPrettyPrinter : public TermVisitor<void> {
private:
    BlockPtr result;
    StatePtr state;
public:
    explicit TermPrettyPrinter(StatePtr s) {
        result = make_shared<Block>(BlockWrapper::None);
        state = std::move(s);
    }

    TermPrettyPrinter& push_name(string& n);

    TermPrettyPrinter& pop_name();

    string get_name(Idx i);

    BlockPtr sub_pretty(TermPtr& term);

    static BlockPtr pretty(TermPtr& term) {
        auto state = make_shared<TermPrettyPrinterState>();
        auto printer = new TermPrettyPrinter(state);

        printer->visit(term);

        return printer->result;
    }
protected:
    void visit_var(NodePtr<Var> &target) final;

    void visit_lambda(NodePtr<Lambda> &target) final;

    void visit_app(NodePtr<App> &target) final;

    void visit_pi(NodePtr<Pi> &target) final;
};


