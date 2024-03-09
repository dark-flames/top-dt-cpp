#include <common/visitor.h>

#include <string>

using PrettyPrintResult = std::string;

class PrettyPrint : Operation<PrettyPrintResult> {
private:
    PrettyPrintResult content;
public:
    PrettyPrintResult perform() {
        return this->content;
    }

    PrettyPrint(PrettyPrintResult c): content(c) {}

    PrettyPrint empty() {
        return PrettyPrint("");
    }

    PrettyPrint& operator>> (const PrettyPrint& r) {
        this->content += r.content;
        return *this;
    }

    PrettyPrint& operator>> (const std::string& r) {
        this->content += r;
        return *this;
    }
    PrettyPrint& operator>> (const char* r) {
        this->content += r;
        return *this;
    }

    PrettyPrint& operator>>= (const PrettyPrint& r) {
        this->content += (" " + r.content);
        return *this;
    }

    PrettyPrint& operator>>= (const char* r) {
        this->content += " ";
        this->content += r;
        return *this;
    }

    PrettyPrint& operator>>= (const std::string& r) {
        this->content += (" " + r);
        return *this;
    }

    PrettyPrint& operator/ (const PrettyPrint& r) {
        this->content += ("\n" + r.content);
        return *this;
    }
};

PrettyPrint lambda() {
    return PrettyPrint("λ");
}

PrettyPrint pi_ty() {
    return PrettyPrint("Π");
}

PrettyPrint dot() {
    return PrettyPrint(".");
}

PrettyPrint colon() {
    return PrettyPrint(":");
}


PrettyPrint paren(PrettyPrint inner) {
    return PrettyPrint("(") >> inner >> ")";
}