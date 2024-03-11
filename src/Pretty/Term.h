#pragma once

#include <Pretty/Block.h>
#include <Term/Nodes.h>
#include <Term/RefNode.h>
#include <Term/TermVisitor.h>
#include <string>
#include <memory>
#include <utility>

using namespace term;

class LevelPrettyPrinter;

class TermPrettyPrinterState {
    vector<std::string> name_stack;
    Indentation block_style;
    bool block_tabs;
public:
    TermPrettyPrinterState(
        Indentation block_style = Indentation::Inline,
        bool block_tabs = false
    ) : block_style(block_style), block_tabs(block_tabs) {}

    friend class TermPrettyPrinter;
};

using StatePtr = shared_ptr<TermPrettyPrinterState>;

class TermPrettyPrinter : public TermVisitor<void> {
private:
    BlockPtr result;
    StatePtr state;

    TermPrettyPrinter& push_name(string& n);

    TermPrettyPrinter& pop_name();

    string get_name(Idx i) const;

    BlockPtr sub_pretty(TermPtr& term);

    LevelPrettyPrinter create_level_printer();
protected:
    void visit_var(NodePtr<Var>& target) final;

    void visit_lambda(NodePtr<Lambda>& node) final;

    void visit_llambda(NodePtr<LLambda>& node) final;

    void visit_app(NodePtr<App>& node) final;

    void visit_pi(NodePtr<Pi>& node) final;

    void visit_lpi(NodePtr<LPi>& node) final;

    void visit_univ(NodePtr<Univ>& node) final;

    void visit_univ_omega(NodePtr<UnivOmega>& node) final;

public:
    explicit TermPrettyPrinter(StatePtr s) {
        state = std::move(s);
        result = create_block();
    }

    BlockPtr create_block() {
        return make_shared<Block>(
            BlockWrapper::None,
            this->state->block_style,
            this->state->block_tabs
        );
    }

    static BlockPtr pretty_inline(TermPtr& term) {
        auto state = make_shared<TermPrettyPrinterState>();
        auto printer = new TermPrettyPrinter(state);

        printer->visit(term);

        return printer->result;
    }
    friend class LevelPrettyPrinter;
};

class LevelPrettyPrinter : public TermVisitor<void> {
private:
    TermPrettyPrinter* term_printer;

    unsigned int offset;
    optional<BlockPtr> base;

    LevelPrettyPrinter& clear_result() {
        this->offset = 0;
        this->base = {};

        return *this;
    }

    BlockPtr sub_pretty(TermPtr& term);
public:
    explicit LevelPrettyPrinter(
        TermPrettyPrinter* term_printer
    ) : term_printer(term_printer), offset(0), base({}) {}

    BlockPtr get_result();
protected:
    void visit_lmax(NodePtr<LMax>& node) final;

    void visit_lzero(NodePtr<LZero>& node) final;

    void visit_lsuc(NodePtr<LSuc>& node) final;

    void visit_lvar(NodePtr<LVar>& node) final;
};


