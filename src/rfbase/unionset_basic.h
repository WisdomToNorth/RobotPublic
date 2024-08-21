#pragma once
#ifndef UNIONSET_BASIC_H
#define UNIONSET_BASIC_H

#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace rfbase
{
class UnionFind
{
public:
    std::unordered_map<int, int> parent_;
    std::unordered_map<int, int> rank_;

public:
    UnionFind(int n);
    ~UnionFind() = default;

    [[nodiscard]] int find(int index);

    [[nodiscard]] bool isConnected(int _a, int _b);

    bool merge(int _a, int _b);

    void combine(UnionFind &rhs);

    void printUnion();

    void getResult(std::vector<std::pair<int, std::unordered_set<int>>> &final_set_);
};
} // namespace rfbase
#endif
