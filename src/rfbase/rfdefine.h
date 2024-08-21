/****************************************************************************
** Copyright (c) 2021, Fougue Ltd. <http://www.fougue.pro>
** All rights reserved.
** See license at https://github.com/fougue/mayo/blob/master/LICENSE.txt
****************************************************************************/
#pragma once
#ifndef DEFINE_H
#define DEFINE_H_

#include <random>

namespace rfbase
{
struct Pnt2i
{
    int x;
    int y;
};
struct Pnt3i
{
    int x;
    int y;
    int z;
};
struct Pnt2f
{
    double x;
    double y;
};
struct Pnt3f
{
    double x;
    double y;
    double z;
};

template <typename T>
struct RfBiTree
{
    RfBiTree *left;
    RfBiTree *right;
    T data;

    RfBiTree() : left(nullptr), right(nullptr)
    {
    }
    RfBiTree(const T &data) : left(nullptr), right(nullptr), data(data)
    {
    }
    RfBiTree(const T &data, RfBiTree *left, RfBiTree *right) : left(left), right(right), data(data)
    {
    }
};

template <typename T>
struct RfMultiTree
{
    T data;
    std::vector<RfMultiTree *> children;

    RfMultiTree() = default;
    RfMultiTree(const T &data) : data(data)
    {
    }
    RfMultiTree(const T &data, const std::vector<RfMultiTree *> &children) :
        data(data), children(children)
    {
    }
};
} // namespace rfbase
#endif
