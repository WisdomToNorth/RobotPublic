
#include "datautils.h"

#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace rfbase
{

std::string DataUtils::getTimeStr(const char *format /* = "%Y-%m-%d_%H-%M-%S"*/)
{
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&now_c), format);
    return ss.str();
}

typedef std::pair<bool, std::string> FileNode;
RfMultiTree<FileNode> *DataUtils::extractSubFolderAndFileName(const std::string &path,
                                                              RfMultiTree<FileNode> *root)
{
    std::filesystem::path l_path(path);
    if (!std::filesystem::exists(l_path))
    {
        return nullptr;
    }

    if (root == nullptr)
    {
        root = new RfMultiTree<FileNode>(FileNode(true, l_path.filename().string()));
    }

    for (const std::filesystem::directory_entry &entry :
         std::filesystem::directory_iterator(l_path))
    {
        if (entry.is_directory())
        {
            RfMultiTree<FileNode> *sub_folder =
                new RfMultiTree<FileNode>(FileNode(true, entry.path().filename().string()));
            root->children.push_back(sub_folder);
            extractSubFolderAndFileName(entry.path().string(), sub_folder);
        }
        else
        {
            root->children.push_back(
                new RfMultiTree<FileNode>(FileNode(false, entry.path().filename().string())));
        }
    }
    return root;
}

std::string DataUtils::bytesText(uint64_t sizeBytes)
{
    constexpr int oneMB = 1024 * 1024;
    std::stringstream ss;

    if (sizeBytes < 1024)
    {
        ss << sizeBytes << "B";
    }
    else if (sizeBytes < oneMB)
    {
        ss << std::fixed << std::setprecision(2) << (static_cast<double>(sizeBytes) / 1024.0)
           << "KB";
    }
    else if (sizeBytes < (10 * oneMB))
    {
        ss << std::fixed << std::setprecision(2)
           << (static_cast<double>(sizeBytes) / static_cast<double>(oneMB)) << "MB";
    }
    else if (sizeBytes < (100 * oneMB))
    {
        ss << std::fixed << std::setprecision(1)
           << (static_cast<double>(sizeBytes) / static_cast<double>(oneMB)) << "MB";
    }
    else
    {
        ss << (sizeBytes / oneMB) << "MB";
    }

    return ss.str();
}

bool DataUtils::isFileExist(const std::string &file_path)
{
    return std::filesystem::exists(file_path);
}

bool DataUtils::isSameFile(const std::string &file1, const std::string &file2)
{
    std::ifstream f1(file1, std::ifstream::binary | std::ifstream::ate);
    std::ifstream f2(file2, std::ifstream::binary | std::ifstream::ate);

    if (f1.fail() || f2.fail())
    {
        return false; // file problem
    }

    if (f1.tellg() != f2.tellg())
    {
        return false; // size mismatch
    }

    // seek back to beginning and use std::equal to compare contents
    f1.seekg(0, std::ifstream::beg);
    f2.seekg(0, std::ifstream::beg);

    return std::equal(std::istreambuf_iterator<char>(f1.rdbuf()), std::istreambuf_iterator<char>(),
                      std::istreambuf_iterator<char>(f2.rdbuf()));
}

bool DataUtils::isSameFileWithDiffLineDebug(const std::string &file1, const std::string &file2)
{
    std::ifstream f1(file1, std::ifstream::binary | std::ifstream::ate);
    std::ifstream f2(file2, std::ifstream::binary | std::ifstream::ate);

    if (f1.fail() || f2.fail())
    {
        return false; // file problem
    }

    if (f1.tellg() != f2.tellg())
    {
        return false; // size mismatch
    }

    // seek back to beginning and use std::equal to compare contents
    f1.seekg(0, std::ifstream::beg);
    f2.seekg(0, std::ifstream::beg);

    unsigned int checked_line = 0;
    while (!f1.eof() && !f2.eof())
    {
        std::string line1, line2;
        std::getline(f1, line1);
        std::getline(f2, line2);
        if (line1 != line2)
        {
            std::cout << "line " << checked_line << " is different" << '\n';
            std::cout << "line1: " << line1 << '\n';
            std::cout << "line2: " << line2 << '\n';
            return false;
        }
        checked_line++;
    }

    return std::equal(std::istreambuf_iterator<char>(f1.rdbuf()), std::istreambuf_iterator<char>(),
                      std::istreambuf_iterator<char>(f2.rdbuf()));
}
std::string DataUtils::generateUuid()
{
    boost::uuids::random_generator gen;
    boost::uuids::uuid uuid = gen();
    return boost::uuids::to_string(uuid);
}

std::string DataUtils::toLower(const std::string &str)
{
    std::string res = str;
    std::transform(res.begin(), res.end(), res.begin(),
                   [](unsigned char c) -> char { return static_cast<char>(std::tolower(c)); });

    return res;
}

std::string DataUtils::getCurrentTimeStr()
{
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

bool DataUtils::makeDir(const std::string &folder_path)
{
    std::filesystem::path dir(folder_path);
    return std::filesystem::create_directories(dir);
}

int DataUtils::rmDir(const std::string &path)
{
    return (int)std::filesystem::remove_all(path);
}

bool DataUtils::makeFile(const std::string &path)
{
    std::filesystem::path filePath(path);
    if (std::filesystem::exists(filePath))
    {
        return true;
    }

    std::filesystem::path parentDir = filePath.parent_path();
    if (!std::filesystem::exists(parentDir))
    {
        bool created = std::filesystem::create_directories(parentDir);
        if (!created)
        {
            return false;
        }
    }

    std::ofstream fileStream(filePath);
    if (fileStream.is_open())
    {
        fileStream.close();
        return true;
    }
    else
    {
        std::cerr << "Error opening file: " << std::strerror(errno) << '\n';
        return false;
    }
}

std::string DataUtils::loadFile(const std::string &path)
{
    std::ifstream file(path);
    if (!file)
    {
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

bool DataUtils::getAllFilesByDir(const std::string &_path, const std::string &suffix,
                                 bool relative_path, std::vector<std::string> &files)
{
    std::filesystem::path l_dir_path(_path);
    // is exists
    if (!std::filesystem::exists(l_dir_path))
    {
        std::cout << _path << ", not exists..." << '\n';
        return false;
    }
    if (!std::filesystem::is_directory(l_dir_path))
    {
        std::cout << _path << ", not dir...." << '\n';
        return false;
    }

    // std::cout << "Files found in " << _path << ":\n";

    std::filesystem::directory_entry l_dir(l_dir_path);
    for (std::filesystem::directory_iterator end, begin(l_dir); begin != end; ++begin)
    {
        if (!std::filesystem::is_directory(begin->path()))
        {
            if (begin->path().extension().string() == suffix)
            {
                if (relative_path)
                {
                    files.push_back(begin->path().filename().string());
                }
                else
                {
                    files.push_back(begin->path().string());
                }
            }
        }
    }
    return true;
};

std::string DataUtils::getFileExtension(const std::string &filepath)
{
    size_t pos = filepath.find_last_of('.');
    if (pos != std::string::npos)
    {
        return filepath.substr(pos + 1);
    }
    return "";
}

// input: /home/robot/robot.json
// output: /home/robot/
std::string DataUtils::getFilePath(const std::string &filepath)
{
    size_t pos = filepath.find_last_of("/\\");
    if (pos != std::string::npos)
    {
        return filepath.substr(0, pos + 1);
    }
    return "";
}

std::string DataUtils::getFileName(const std::string &filepath)
{
    size_t pos = filepath.find_last_of("/\\");
    if (pos != std::string::npos)
    {
        return filepath.substr(pos + 1);
    }
    return "";
}
} // namespace rfbase
