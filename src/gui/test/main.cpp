#include <QApplication>

#include <gtest/gtest.h>

int main(int argc, char *argv[])
{
#ifdef ENABLE_GUI
    QApplication a(argc, argv);
#endif
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
