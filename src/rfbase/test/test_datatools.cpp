#include <filesystem>

#include <gtest/gtest.h>

#include "generated/config.h"
#include "rfbase/datautils.h"

namespace rfbase
{
TEST(TestDataUtils, makefile)
{
    std::string filename = REG_OUTPUT_FOLDER + "/testfile.txt";
    bool succ = DataUtils::makeFile(filename);
    EXPECT_TRUE(succ);

    // Use std::filesystem to check if the file exists and is a regular file
    std::filesystem::path filePath(filename);
    EXPECT_TRUE(std::filesystem::exists(filePath) && std::filesystem::is_regular_file(filePath));

    std::string filename2 = REG_OUTPUT_FOLDER + "/rflog_2024-03-16_22:32:05.log";
    succ = DataUtils::makeFile(filename2);

#ifdef _WIN32
    // NOTE: ':' is not allowed in filename in windows
    EXPECT_FALSE(succ);
#elif __linux__
    EXPECT_TRUE(succ);
#elif __APPLE__
    EXPECT_TRUE(succ);
#elif defined(__ANDROID__)
    EXPECT_TRUE(succ);
#endif
}

TEST(TestDataUtils, make_remove_dir)
{
    std::string foldername = APP_PATH + "/testfolder";
    bool succ_make = DataUtils::makeDir(foldername);
    EXPECT_TRUE(succ_make);

    std::filesystem::path folder_path1(foldername);
    EXPECT_TRUE(std::filesystem::is_directory(folder_path1));

    int delete_cnt = DataUtils::rmDir(foldername);
    EXPECT_EQ(delete_cnt, 1);

    std::filesystem::path folder_path2(foldername);
    EXPECT_FALSE(std::filesystem::is_directory(folder_path2));
}

TEST(TestDataUtils, make_remove_dir_recursive)
{
    // make file recursive
    std::string foldername1 = APP_PATH + "/testfolder_r/subfolder/subsubdir";
    bool succ = DataUtils::makeDir(foldername1);
    EXPECT_TRUE(succ);

    std::filesystem::path folder_path1(foldername1);
    EXPECT_TRUE(std::filesystem::is_directory(folder_path1));

    // make dir recursive
    std::string filename2 = APP_PATH + "/testfolder_r/subfolder_r/test.txt";

    succ = DataUtils::makeFile(filename2);
    EXPECT_TRUE(succ);

    std::filesystem::path folder_path2(filename2);
    EXPECT_TRUE(std::filesystem::is_directory(folder_path1));

    // remove dir recursive
    std::string foldername3 = APP_PATH + "/testfolder_r";

    int delete_cnt = DataUtils::rmDir(foldername3);
    EXPECT_EQ(delete_cnt, 5);

    std::filesystem::path folder_path3(foldername3);
    EXPECT_FALSE(std::filesystem::is_directory(folder_path3));
}

TEST(TestDataUtils, test_get_all_files)
{
    std::string foldername = APP_PATH + "/testfolder_getfiles/";

    bool succ_make;
    std::string filename1 = foldername + "test1.txt";
    std::string filename2 = foldername + "test2.txt";
    std::string filename3 = foldername + "test3.txt";
    std::string filename4 = foldername + "test4.txt";
    std::string filename5 = foldername + "test5.txt";

    succ_make = DataUtils::makeFile(filename1);
    EXPECT_TRUE(succ_make);
    succ_make = DataUtils::makeFile(filename2);
    EXPECT_TRUE(succ_make);
    succ_make = DataUtils::makeFile(filename3);
    EXPECT_TRUE(succ_make);
    succ_make = DataUtils::makeFile(filename4);
    EXPECT_TRUE(succ_make);
    succ_make = DataUtils::makeFile(filename5);
    EXPECT_TRUE(succ_make);

    std::vector<std::string> files;
    bool succ = DataUtils::getAllFilesByDir(foldername, ".txt", true, files);
    EXPECT_TRUE(succ);
    EXPECT_EQ(files.size(), 5);

    int delete_cnt = DataUtils::rmDir(foldername);
    EXPECT_EQ(delete_cnt, 6);

    std::filesystem::path folder_path(foldername);
    EXPECT_FALSE(std::filesystem::is_directory(folder_path));
}

TEST(uuid, test_uuid)
{
    std::string uuid1 = DataUtils::generateUuid();
    std::string uuid2 = DataUtils::generateUuid();
    std::cout << uuid1 << '\n';
    std::cout << uuid2 << '\n';
    EXPECT_NE(uuid1, uuid2);
}

TEST(testpath, test_getfolder)
{
    std::string path = "/home/robot/robotframework/test.txt";
    std::string folder = DataUtils::getFilePath(path);
    EXPECT_EQ(folder, "/home/robot/robotframework/");
}

static std::string getSpace(int level)
{
    std::string space = "  ";
    for (int i = 0; i < level; i++)
    {
        space += "  ";
    }
    return space;
}

[[maybe_unused]] static void printTree(RfMultiTree<DataUtils::FileNode> *tree, int level)
{
    if (tree == nullptr)
    {
        return;
    }

    std::string space = getSpace(level);
    std::string folder_mark = tree->data.first ? "./" : "";
    std::cout << space << folder_mark << tree->data.second << '\n';
    for (auto &child : tree->children)
    {
        printTree(child, level + 1);
    }
}

TEST(testpath, extractSubFolderAndFileName)
{
    std::string path = REG_FILE_FOLDER + "/resources";
    RfMultiTree<DataUtils::FileNode> *tree = DataUtils::extractSubFolderAndFileName(path, nullptr);
    // printTree(tree, 0);
    EXPECT_EQ(tree->data.second, "resources");
    EXPECT_EQ(tree->children.size(), 12);
    delete tree;
}

TEST(testpath, test_getsuffix)
{
    std::string path = "/home/robot/robotframework/test.txt";
    std::string folder = DataUtils::getFileExtension(path);
    EXPECT_EQ(folder, "txt");
}

TEST(testpath, test_getfilename)
{
    std::string path = "/home/robot/robotframework/test.txt";
    std::string folder = DataUtils::getFileName(path);
    EXPECT_EQ(folder, "test.txt");
}

TEST(testtolower, charonly)
{
    std::string input{"ABCdE"};
    std::string output = DataUtils::toLower(input);
    EXPECT_EQ("abcde", output);
}

TEST(testtolower, charwithnumber)
{
    std::string input{"ABCdE123"};
    std::string output = DataUtils::toLower(input);
    EXPECT_EQ("abcde123", output);
}

} // namespace rfbase
