#pragma once

#include <Pretty/Document.h>
#include <Term/Nodes.h>
#include <Term/RefNode.h>
#include <Term/TermVisitor.h>
#include <string>
#include <memory>
#include <utility>
#include <functional>

class LevelPrettyPrinter;

class LambdaPrettyPrinter;

enum class Precedence : int {
    Doc = 0,
    Abs = 1,
    Op = 5,
    App = 10,
    Atom = 114514
};

enum class Associativity : int {
    Left,
    Right,
    None
};

class TermPrettyPrinterState {
public:
    Precedence precedence;
    Associativity position;

    TermPrettyPrinterState(Precedence pred, Associativity pos);
};

class TermPrettyPrinter :
    public TermVisitor<DocumentPtr>, public WithState<TermPrettyPrinterState> {
private:
    std::vector<std::string> name_stack;

    [[nodiscard]] std::string get_name(DBIndex i) const;

    DocumentPtr sub_pretty(Precedence pred, Associativity pos, Term& term);

    DocumentPtr pretty_print_level(Term& node);

    DocumentPtr with_precedence(Precedence pred, Associativity assoc, std::function<void(Document&)> callback);
protected:
    DocumentPtr visit_var(term::Var& target) final;

    DocumentPtr visit_def_ref(term::DefRef& doc) override;

    DocumentPtr visit_lambda(term::Lambda& node) final;

    DocumentPtr visit_llambda(term::LLambda& node) final;

    DocumentPtr visit_app(term::App& doc) final;

    DocumentPtr visit_pi(term::Pi& doc) final;

    DocumentPtr visit_lpi(term::LPi& node) final;

    DocumentPtr visit_univ(term::Univ& node) final;

    DocumentPtr visit_univ_omega(term::UnivOmega& node) final;

    DocumentPtr visit_lmax(term::LMax& node) override;

    DocumentPtr visit_lzero(term::LZero& node) override;

    DocumentPtr visit_lvar(term::LVar& node) override;

    DocumentPtr visit_lsuc(term::LSuc& node) override;

public:
    explicit TermPrettyPrinter(
        Precedence pred = Precedence::Doc,
        Associativity assoc = Associativity::Left
    );

    TermPrettyPrinter& push_name(std::string& n);

    TermPrettyPrinter& pop_name();

    TermPrettyPrinter& clean_names();

    friend class LevelPrettyPrinter;

    friend class LambdaPrettyPrinter;
};

class LambdaPrettyPrinter : public TermVisitor<DocumentPtr> {
private:
    TermPrettyPrinter* term_printer;
    std::vector<std::string> bind_list;

    DocumentPtr finish(Term& body);

protected:
    DocumentPtr visit_lambda(term::Lambda& node) override;

    DocumentPtr visit_llambda(term::LLambda& node) override;

public:
    explicit LambdaPrettyPrinter(
        TermPrettyPrinter* term_printer
    ) : term_printer(term_printer) {}
};

class LevelPrettyPrinter : public TermVisitor<void> {
private:
    TermPrettyPrinter* term_printer;
    int offset;
    std::optional<DocumentPtr> base;

    DocumentPtr sub_pretty(Term& term, Associativity pos);
public:
    explicit LevelPrettyPrinter(
        TermPrettyPrinter* term_printer
    ) : term_printer(term_printer),
        offset(0),
        base({}) {}

    DocumentPtr get_result();

protected:
    void visit_lmax(term::LMax& node) final;

    void visit_lzero(term::LZero& node) final;

    void visit_lsuc(term::LSuc& node) final;

    void visit_lvar(term::LVar& node) final;
};


