#pragma once


enum class SplitType : int {
    ForceSingleLine = 0,
    NewLine = 1,
    WrappedBlock = 3,
    AfterArrow = 4,
    AfterOperator = 5,
    Application = 6,
    Space = 7
};

namespace token {
constexpr const char* lambda = "\\";
constexpr const char* pi = "Π";
constexpr const char* colon = ":";
constexpr const char* dot = ".";
constexpr const char* add = "+";
constexpr const char* arrow = "->";
constexpr const char* left_paren = "(";
constexpr const char* right_paren = ")";

constexpr const char* univ = "Univ";
constexpr const char* under_line = "_";
constexpr const char* lmax = "∩";
constexpr const char* omega = "Ω";
constexpr const char* level = "Level";
constexpr const char* comma = ",";

constexpr const SplitType block_split = SplitType::WrappedBlock;
constexpr const SplitType endl = SplitType::NewLine;
constexpr const SplitType after_arrow = SplitType::AfterArrow;
constexpr const SplitType after_op = SplitType::AfterOperator;
constexpr const SplitType app_split = SplitType::Application;
constexpr const SplitType space = SplitType::Space;
}