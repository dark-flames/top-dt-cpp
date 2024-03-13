#pragma once

#include <Exception/Common.h>
#include <vector>
#include <string>

class DeclarationException : public Exception {
};

class CyclingDependency : public DeclarationException {
public:
    std::vector<std::string> cycle;

    CyclingDependency(std::vector<std::string>& cycle) : cycle(cycle) {}
};

class UnknownEntry : public DeclarationException {
public:
    std::string entry;

    UnknownEntry(std::string& entry) : entry(entry) {}
};