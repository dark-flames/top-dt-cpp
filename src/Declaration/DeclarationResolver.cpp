#include <Declaration/DeclarationResolver.h>
#include <TypeCheck/TypeChecker.h>
#include <stdexcept>

using namespace std;

Declaration* DeclarationResolver::get_decl(Entry& entry) {
    auto res = this->declarations.find(entry);

    if (res == this->declarations.end()) {
        throw runtime_error("Cannot find entry " + entry);
    } else {
        return res->second.get();
    }
}

bool DeclarationResolver::dependency_detect(Entry& from, Entry& to) {
    auto current = this->waiting_for.find(from);

    while (current != this->waiting_for.end()) {
        if (current->second == to) {
            return true;
        } else {
            current = this->waiting_for.find(current->second);
        }
    }

    return false;
}

WithSignature* DeclarationResolver::try_check_signature(Entry& entry) {
    auto original = std::move(this->current_entry);
    this->current_entry = entry;
    auto decl = this->get_decl(entry);
    UncheckedDeclaration* unchecked = dynamic_cast<UncheckedDeclaration*>(decl);
    auto res = this->type_checker->check_ty(*unchecked->type);
    auto vty = this->type_checker->eval(*res.first);

    DeclarationPtr new_dec = make_unique<CheckedSignature>(
        vty,
        unchecked->body
    );

    this->declarations[std::move(entry)] = std::move(new_dec);

    this->current_entry = std::move(original);

    auto new_decl = this->get_decl(entry);
    return dynamic_cast<WithSignature*>(new_decl);
}

CheckedDeclaration* DeclarationResolver::try_check(Entry& entry) {
    auto original = std::move(this->current_entry);
    this->current_entry = entry;
    auto decl = this->get_decl(entry);

    auto s = dynamic_cast<CheckedSignature*>(decl);
    auto res = this->type_checker->check_expr(*s->body, s->type.get());

    DeclarationPtr new_dec = make_unique<CheckedDeclaration>(
        s->type,
        res
    );

    this->declarations[entry] = std::move(new_dec);

    this->current_entry = std::move(original);

    auto new_decl = this->get_decl(entry);
    return dynamic_cast<CheckedDeclaration*>(new_decl);
}

DeclarationResolver& DeclarationResolver::push_unchecked_body(UncheckedBody& body) {
    auto res = this->declarations.find(body.name);
    if (res == this->declarations.end()) {
        this->declarations.insert(
            std::make_pair(
                body.name,
                raw_body(body.name, std::move(body.body))
            ));
    } else if ((*res).second->state() == DeclarationState::UncheckedSignature) {
        UncheckedSignature* signature = dynamic_cast<UncheckedSignature*>((*res).second.get());
        this->declarations[body.name] = unchecked(std::move(signature->type), std::move(body.body));
    } else {
        throw CannotPushDecl(body.name, "unchecked body");
    }
    return *this;
}

DeclarationResolver& DeclarationResolver::push_unchecked_signature(UncheckedSignature& signature) {
    auto res = this->declarations.find(signature.name);
    if (res == this->declarations.end()) {
        this->declarations.insert(
            std::make_pair(
                signature.name,
                raw_signature(signature.name, std::move(signature.type))
            ));
    } else if ((*res).second->state() == DeclarationState::UncheckedBody) {
        UncheckedBody* body = dynamic_cast<UncheckedBody*>((*res).second.get());
        this->declarations[signature.name] = unchecked(std::move(signature.type), std::move(body->body));
    } else {
        throw CannotPushDecl(signature.name, "unchecked signature");
    }
    return *this;
}

DeclarationResolver& DeclarationResolver::push_unchecked(UncheckedPtr& decl) {
    if (decl->state() == DeclarationState::UncheckedSignature) {
        UncheckedSignature* signature = dynamic_cast<UncheckedSignature*>(decl.get());
        return this->push_unchecked_signature(*signature);
    } else if (decl->state() == DeclarationState::UncheckedBody) {
        UncheckedBody* body = dynamic_cast<UncheckedBody*>(decl.get());
        return this->push_unchecked_body(*body);
    } else {
        throw ImpossibleException(
            "Only unchecked signature or body can be pushed to `DeclarationResolver`"
        );
    }
}

WithSignature* DeclarationResolver::get_signature_or_waiting(Entry& entry) {
    auto decl = this->get_decl(entry);

    if (decl->state() == DeclarationState::SignatureOnly ||
        decl->state() == DeclarationState::Finished) {
        return dynamic_cast<WithSignature*>(decl);
    } else {
        this->waiting_for[this->current_entry] = entry;
        auto result = this->try_check_signature(entry);
        this->waiting_for.erase(this->current_entry);
        return result;
    }
}

CheckedDeclaration* DeclarationResolver::get_body_or_waiting(Entry& entry) {
    auto decl = this->get_decl(entry);

    if (decl->state() == DeclarationState::Finished) {
        return dynamic_cast<CheckedDeclaration*>(decl);
    } else if (decl->state() == DeclarationState::SignatureOnly) {
        this->waiting_for[this->current_entry] = entry;
        auto result = this->try_check(entry);
        this->waiting_for.erase(this->current_entry);
        return result;
    } else {
        this->waiting_for[this->current_entry] = entry;
        this->try_check_signature(entry);
        auto result = this->try_check(entry);
        this->waiting_for.erase(this->current_entry);
        return result;
    }
}