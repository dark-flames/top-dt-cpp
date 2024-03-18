#pragma once

#include <Pretty/Block.h>
#include <Term/Nodes.h>
#include <Term/RefNode.h>
#include <Term/TermVisitor.h>
#include <string>
#include <memory>
#include <utility>

class LevelPrettyPrinter;

class TermPrettyPrinterState {
    std::vector<std::string> name_stack;
    Indentation block_style;
    bool block_tabs;
public:
    TermPrettyPrinterState(
        Indentation block_style = Indentation::Inline,
        bool block_tabs = false
    ) : block_style(block_style), block_tabs(block_tabs) {}

    friend class TermPrettyPrinter;
};

using StatePtr = std::shared_ptr<TermPrettyPrinterState>;

class TermPrettyPrinter : public TermVisitor<void> {
private:
    BlockPtr result;
    StatePtr state;

    TermPrettyPrinter& push_name(std::string& n);

    TermPrettyPrinter& pop_name();

    std::string get_name(DBIndex i) const;

    BlockPtr sub_pretty(Term& term);

    LevelPrettyPrinter create_level_printer();

protected:
    void visit_var(term::Var& target) final;

    void visit_lambda(term::Lambda& node) final;

    void visit_llambda(term::LLambda& node) final;

    void visit_app(term::App& node) final;

    void visit_pi(term::Pi& node) final;

    void visit_lpi(term::LPi& node) final;

    void visit_univ(term::Univ& node) final;

    void visit_univ_omega(term::UnivOmega& node) final;

public:
    explicit TermPrettyPrinter(StatePtr& s) {
        state = s;
        result = create_block();
    }

    BlockPtr create_block() {
        return std::make_unique<Block>(
            BlockWrapper::None,
            this->state->block_style,
            this->state->block_tabs
        );
    }

    static BlockPtr pretty_inline(TermPtr& term) {
        auto state = std::make_shared<TermPrettyPrinterState>();
        auto printer = TermPrettyPrinter(state);

        printer.visit(*term);

        return std::move(printer.result);
    }

    friend class LevelPrettyPrinter;
};

class LevelPrettyPrinter : public TermVisitor<void> {
private:
    TermPrettyPrinter* term_printer;

    unsigned int offset;
    std::optional<BlockPtr> base;

    LevelPrettyPrinter& clear_result() {
        this->offset = 0;
        this->base = {};

        return *this;
    }

    BlockPtr sub_pretty(Term& term);

public:
    explicit LevelPrettyPrinter(
        TermPrettyPrinter* term_printer
    ) : term_printer(term_printer), offset(0), base({}) {}

    BlockPtr get_result();

protected:
    void visit_lmax(term::LMax& node) final;

    void visit_lzero(term::LZero& node) final;

    void visit_lsuc(term::LSuc& node) final;

    void visit_lvar(term::LVar& node) final;
};


