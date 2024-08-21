#include <chrono>
#include <thread>

#include <gtest/gtest.h>

#include "rfbase/rfTimer.h"
namespace rfbase
{

// TODO: in linux, the time is accurate; but in mac and windows, the time is not accurate
// We set eps is 10% of expected time

#ifdef __linux__
TEST(KTimerTest, TimeFromBegin)
{
    FrTimer timer;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_NEAR(timer.timeFromBegin<std::milli>(), 100, 3);

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    EXPECT_NEAR(timer.timeFromLastSee<std::milli>(), 200, 20);
    EXPECT_NEAR(timer.timeFromBegin<std::milli>(), 300, 30);
    timer.reset();
    EXPECT_NEAR(timer.timeFromBegin<std::milli>(), 0, 1);
}

TEST(KTimerTest, test_stop) // read time while not stop
{
    FrTimer timer;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    timer.stop();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    timer.restore();
    EXPECT_NEAR(timer.timeFromBegin<std::milli>(), 100, 10);

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    timer.stop();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    timer.restore();

    EXPECT_NEAR(timer.timeFromLastSee<std::milli>(), 200, 20);
    EXPECT_NEAR(timer.timeFromBegin<std::milli>(), 300, 30);
}

TEST(KTimerTest, test_stop_2) // read time while stop
{
    FrTimer timer;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    timer.stop();
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // skip
    EXPECT_NEAR(timer.timeFromBegin<std::milli>(), 100, 10);
    timer.restore();

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    timer.stop();
    EXPECT_NEAR(timer.timeFromLastSee<std::milli>(), 300, 30);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    EXPECT_NEAR(timer.timeFromLastSee<std::milli>(), 300, 30);
    EXPECT_NEAR(timer.timeFromBegin<std::milli>(), 300, 30);
    timer.restore();

    EXPECT_NEAR(timer.timeFromLastSee<std::milli>(), 300, 30);
    EXPECT_NEAR(timer.timeFromBegin<std::milli>(), 300, 30);
    EXPECT_NEAR(timer.timeFromLastSee<std::milli>(), 0, 1);
}

TEST(KTimerTest, test_timer_data_benchmark)
{
    FrTimer timer;
    for (int i = 0; i < 100; ++i)
    {
        for (int j = 0; j < 10; ++j)
        {
            timer.startSubTimer(std::to_string(j));
            for (int k = 10; k < 15; ++k)
            {
                FrTimer *subtimer = timer.getSubTimerByName(std::to_string(j));
                subtimer->restore();
                subtimer->startSubTimer(std::to_string(k));
                subtimer->stopSubTimer(std::to_string(k));
            }
            timer.stopSubTimer(std::to_string(j));
        }
    }
    EXPECT_TRUE(timer.checkValid());
    // timer.printTimeReport<std::milli>();
    // timer.printTimeReport<std::micro>("us");
}

static FrTimer g_timer;
void taskOne(int ms = 10)
{
    g_timer.startSubTimer("task one");
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    g_timer.stopSubTimer("task one");
};

void taskTwo(int ms = 15)
{
    g_timer.startSubTimer("task two");
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    g_timer.stopSubTimer("task two");
};

bool taskThree(int ms = 20)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    return true;
}

bool taskFour(int ms = 25)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));

    return true;
}

void taskFive()
{
    FrTimer *timer = g_timer.startSubTimer("task five");

    for (int i = 0; i < 4; ++i)
    {
        FrTimer *sub_timer = timer->startSubTimer("task three");
        taskThree();
        sub_timer->stop();

        FrTimer *sub4_timer = timer->startSubTimer("task four");
        taskFour();
        sub4_timer->stop();
    }

    timer->stop();
};

void taskSeven() // 270
{
    FrTimer *loc_timer = g_timer.startSubTimer("task seven");

    for (int i = 0; i < 2; ++i)
    {
        loc_timer->startSubTimer("task three");
        if (taskThree()) // 20
        {
            loc_timer->stopSubTimer("task three");
            loc_timer->startSubTimer("task four");
            taskFour(); // 25
            loc_timer->stopSubTimer("task four");
        }
        for (int j = 0; j < 2; ++j)
        {
            loc_timer->startSubTimer("task three");
            taskThree(); // 20
            loc_timer->stopSubTimer("task three");

            loc_timer->startSubTimer("task four");
            taskFour(); // 25
            loc_timer->stopSubTimer("task four");
        }
    }

    loc_timer->stop();
};

void taskSix(int ms = 10)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
};

TEST(KTimerTest, test_timer)
{
    FrTimer timer;

    for (int i = 0; i < 2; ++i)
    {
        timer.startSubTimer("one");
        taskOne();
        timer.stopSubTimer("one");

        timer.startSubTimer("five");
        taskFive();
        timer.stopSubTimer("five");

        timer.startSubTimer("two");
        taskTwo();
        timer.stopSubTimer("two");
    }

    EXPECT_NEAR(timer.timeFromBegin(), 410, 40);
    EXPECT_TRUE(timer.checkValid());
    timer.printTimeReport("");
}

TEST(KTimerTest, test_add_sub)
{
    g_timer.reset();
    for (int i = 0; i < 4; ++i)
    {
        taskOne();
        taskFive();
        taskTwo();
        taskSix();
    }

    EXPECT_NEAR(g_timer.timeFromBegin(), 860, 80);
    EXPECT_TRUE(g_timer.checkValid());
    g_timer.printTimeReport("");
}

TEST(KTimerTest, test_time_valid_api)
{
    FrTimer timer;
    for (int i = 0; i < 2; ++i)
    {
        timer.startSubTimer("l_one");
        taskOne();

        timer.startSubTimer("l_five");
        taskFive();

        timer.startSubTimer("l_two");
        taskTwo();

        timer.stopSubTimer("l_one");
        timer.stopSubTimer("l_two");
        timer.stopSubTimer("l_five");
    }

    EXPECT_NEAR(timer.timeFromBegin(), 410, 40);
    EXPECT_FALSE(timer.checkValid());
    timer.printTimeReport("");
}

TEST(KTimerTest, test_add_sub_2)
{
    g_timer.reset();
    for (int i = 0; i < 2; ++i)
    {
        taskOne();   // 10
        taskSeven(); // 270
        taskTwo();   // 15
        taskSix();   // 10
    }

    EXPECT_NEAR(g_timer.timeFromBegin(), 610, 60);
    EXPECT_TRUE(g_timer.checkValid());
    g_timer.printTimeReport("");
}
#endif
} // namespace rfbase
