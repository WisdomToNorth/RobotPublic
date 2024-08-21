/****************************************************************************
** Copyright (c) 2021, Fougue Ltd. <http://www.fougue.pro>
** All rights reserved.
** See license at https://github.com/fougue/mayo/blob/master/LICENSE.txt
****************************************************************************/

#pragma once
#ifndef RF_FILEPATH_H
#define RF_FILEPATH_H

#include <filesystem>
namespace std_filesystem = std::filesystem;

namespace rfapp
{

using FilePath = std_filesystem::path;

// Exception-safe version of std::filesystem::file_size()
inline uintmax_t filepathFileSize(const FilePath &fp)
{
    try
    {
        return std_filesystem::file_size(fp);
    }
    catch (...)
    { // fs::file_size() might throw on non-existing files
        return 0;
    }
}

// Exception-safe version of std::filesystem::canonical()
inline FilePath filepathCanonical(const FilePath &fp)
{
    try
    {
        return std_filesystem::canonical(fp);
    }
    catch (...)
    { // fs::canonical() might throw on non-existing files
        return fp;
    }
}

// Exception-safe version of std::filesystem::equivalent()
inline bool filepathExists(const FilePath &fp)
{
    try
    {
        return std_filesystem::exists(fp);
    }
    catch (...)
    { // fs::exists() might throw on non-existing files
        return false;
    }
}

// Exception-safe version of std::filesystem::equivalent()
inline bool filepathEquivalent(const FilePath &lhs, const FilePath &rhs)
{
    try
    {
        if (lhs == rhs)
            return true;
        else
            return std_filesystem::equivalent(lhs, rhs);
    }
    catch (...)
    { // fs::equivalent() might throw on non-existing files
        return false;
    }
}

// Exception-safe version of std::filesystem::is_regular_file()
inline bool filepathIsRegularFile(const FilePath &fp)
{
    try
    {
        return std_filesystem::is_regular_file(fp);
    }
    catch (...)
    {
        return false;
    }
}

// Exception-safe version of std::filesystem::last_write_time()
inline std_filesystem::file_time_type filepathLastWriteTime(const FilePath &fp)
{
    try
    {
        return std_filesystem::last_write_time(fp);
    }
    catch (...)
    {
        return {};
    }
}

} // namespace rfapp
#endif
