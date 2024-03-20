#pragma once

#include <Syntax/SyntaxVisitor.h>
#include <Pretty/Document.h>
#include <Pretty/TermPrettyPrinter.h>

class SyntaxPrettyPrinterState {
public:
    Precedence precedence;
    Associativity associativity;

    SyntaxPrettyPrinterState(Precedence pred, Associativity assoc);
};

class SyntaxPrettyPrinter :
    public SyntaxVisitor<DocumentPtr>,
    public WithState<SyntaxPrettyPrinterState> {
private:
    DocumentPtr sub_pretty(Precedence pred, Associativity assoc, Syntax& node);

    DocumentPtr with_precedence(Precedence pred, Associativity pos, std::function<void(Document&)> callback);

protected:
    DocumentPtr visit_lmax(syntax::LMax& node) override;

    DocumentPtr visit_lambda(syntax::Lambda& node) override;

    DocumentPtr visit_pi(syntax::Pi& node) override;

    DocumentPtr visit_univ(syntax::Univ& node) override;

    DocumentPtr visit_lnat(syntax::LNat& node) override;

    DocumentPtr visit_level(syntax::Level& node) override;

    DocumentPtr visit_ref(syntax::Ref& node) override;

    DocumentPtr visit_app(syntax::App& node) override;

    DocumentPtr visit_lsuc(syntax::LSuc& node) override;

    friend class SyntaxLambdaPrettyPrinter;

public:
    SyntaxPrettyPrinter(
        Precedence precedence = Precedence::Doc,
        Associativity associativity = Associativity::Left
    ) : WithState(SyntaxPrettyPrinterState(precedence, associativity)) {}
};

class SyntaxLambdaPrettyPrinter : public SyntaxVisitor<DocumentPtr> {
private:
    SyntaxPrettyPrinter* printer;
    std::vector<std::string> bind_list;

    DocumentPtr finish(Syntax& doc);

protected:
    DocumentPtr visit_lambda(syntax::Lambda& node) override;

public:
    explicit SyntaxLambdaPrettyPrinter(
        SyntaxPrettyPrinter* printer
    ) : printer(printer) {}
};
