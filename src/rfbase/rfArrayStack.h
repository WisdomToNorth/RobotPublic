#ifndef FRBASE_FRARRAYSTACK_H
#define FRBASE_FRARRAYSTACK_H
#include <vector>

namespace rfbase
{
template <typename T>
class ArrayStack
{
private:
    int level = -1;
    std::vector<T> node_arr;

    void reallocate()
    {
        size_t newSize = node_arr.size() * 4;
        node_arr.resize(newSize);
    }

public:
    ArrayStack(int max_stack_depth)
        : node_arr(max_stack_depth)
    {
    }

    void reset()
    {
        level = -1;
    }

    void push(const T &element)
    {
        ++level;
        if (level >= node_arr.size())
        {
            reallocate();
        }
        node_arr[level] = element;
    }

    T pop()
    {
        if (level < 0)
        {
            return T(); // or throw an exception
        }
        T result = node_arr[level];
        --level;
        return result;
    }
};
} // namespace rfbase
#endif // FRBASE_FRARRAYSTACK_H
