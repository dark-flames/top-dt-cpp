#pragma once

#include <Declaration/Common.h>
#include <Declaration/Checked.h>
#include <Declaration/Unchecked.h>

#include <map>
#include <set>
#include <string>

class TypeChecker;

using Entry = std::string;

class DeclarationResolver {
private:
    Entry current_entry;
    std::map<Entry, DeclarationPtr> declarations;
    std::map<Entry, Entry> waiting_for;
    TypeChecker* type_checker;

    Declaration* get_decl(Entry& entry);

    bool dependency_detect(Entry& from, Entry& to);

    WithSignature* try_check_signature(Entry& entry);

    CheckedDeclaration* try_check(Entry& entry);

public:
    DeclarationResolver& push_unchecked(Entry& entry, UncheckedPtr& decl);

    WithSignature* get_signature_or_waiting(Entry& entry);

    CheckedDeclaration* get_body_or_waiting(Entry& entry);

    DeclarationResolver() {}

    DeclarationResolver(TypeChecker* type_checker)
        : type_checker(type_checker) {}
};