#include "unionset_basic.h"

#include <iostream>

namespace rfbase
{

UnionFind::UnionFind(int n) : parent_(n), rank_(n)
{
    // O(N)
    for (int a = 0; a < n; ++a)
    {
        parent_[a] = a;
    }
}

int UnionFind::find(int index)
{
    if (index != parent_[index])
    {
        parent_[index] = find(parent_[index]);
    }
    return parent_[index];
}

bool UnionFind::merge(int _a, int _b)
{
    int roota = find(_a);
    int rootb = find(_b);

    if (roota == rootb)
    {
        return true;
    }
    else
    {
        // Merge children
        if (rank_[roota] < rank_[rootb]) // 按秩合并
        {
            std::swap(roota, rootb);
        }
        parent_[rootb] = roota;
        // 如果秩相等，将父节点rootx秩 + 1
        if (rank_[roota] == rank_[rootb])
        {
            rank_[roota]++;
        }
        return true;
    }
}

void UnionFind::combine(UnionFind &rhs)
{
    // O(N)
    for (int i = 0; i < (int)parent_.size(); ++i)
    {
        merge(this->find(i), rhs.find(i));
    }
}

void UnionFind::printUnion()
{
    std::cout << '\n';
    for (int i = 0; i < (int)parent_.size(); ++i)
    {
        std::cout << "##:" << this->find(i);
    }
}

bool UnionFind::isConnected(int _a, int _b)
{
    return find(_a) == find(_b);
}

void UnionFind::getResult(std::vector<std::pair<int, std::unordered_set<int>>> &final_set_)
{
    std::unordered_map<int, std::unordered_set<int>> final_set;
    // O(2N)
    for (auto it = parent_.begin(); it != parent_.end(); ++it)
    {
        final_set[find((*it).first)].insert((*it).first);
    }
    for (auto sit = final_set.begin(); sit != final_set.end(); ++sit)
    {
        final_set_.emplace_back((*sit).first, (*sit).second);
    }
}

} // namespace rfbase
