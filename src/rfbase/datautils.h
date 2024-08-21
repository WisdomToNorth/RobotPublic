
#pragma once
#ifndef DATA_UTILS_H
#define DATA_UTILS_H

#include <string>
#include <vector>

#include "rfdefine.h"

namespace rfbase
{
class DataUtils
{
public:
    // default locale is "%Y-%m-%d_%H-%M-%S"
    static std::string getTimeStr(const char *format = "%Y-%m-%d_%H-%M-%S");

    /**
     * @brief extractSubFolderAndFileName
     * Extract sub folder and file name from a path
     *
     * input: e.g. /home/user/folder1: sub_folder1, file1, file2, sub_folder1/file3
     * output: <is_folder, sub_folder/file_name>
     * output: e.g. node1:<true, sub_folder1>, node2: <false, file1>, node3: <false, file2>,
     * node1->subnode: <false, file3>(sub_folder1/file3)
     *
     * @param path
     * @return std::pair<bool, std::string>
     */
    typedef std::pair<bool, std::string> FileNode;
    static RfMultiTree<FileNode> *extractSubFolderAndFileName(const std::string &path,
                                                              RfMultiTree<FileNode> *root);

    static std::string bytesText(uint64_t sizeBytes);

    static bool isFileExist(const std::string &file_path);
    static bool isSameFileWithDiffLineDebug(const std::string &file1, const std::string &file2);

    static bool isSameFile(const std::string &file1, const std::string &file2);

    static std::string generateUuid();

    static std::string toLower(const std::string &str);

    static std::string getCurrentTimeStr();

    [[nodiscard]] static bool makeDir(const std::string &folder_path);

    [[nodiscard]] static int rmDir(const std::string &path);

    [[nodiscard]] static bool makeFile(const std::string &path);

    [[nodiscard]] static std::string getFileExtension(const std::string &filepath);

    [[nodiscard]] static std::string getFileName(const std::string &filepath);
    [[nodiscard]] static std::string getFilePath(const std::string &filepath);

    [[nodiscard]] static bool getAllFilesByDir(const std::string &_path, const std::string &suffix,
                                               bool relative_path, std::vector<std::string> &files);

    [[nodiscard]] static std::string loadFile(const std::string &path);

    enum class CppVersion
    {
        PreCpp17,
        Cpp17,
        Cpp20
    };

    static void printCppVersion()
    {
        if constexpr (__cplusplus == 201703L)
            printf("C++17\n");
        else if constexpr (__cplusplus == 202002L)
            printf("C++20\n");
        else
            printf("Pre C++17\n");
    }

    static constexpr CppVersion getCppVersion()
    {
        if constexpr (__cplusplus == 201703L)
            return CppVersion::Cpp17;
        else if constexpr (__cplusplus == 202002L)
            return CppVersion::Cpp20;
        else
            return CppVersion::PreCpp17;
    }
};
} // namespace rfbase

#endif
