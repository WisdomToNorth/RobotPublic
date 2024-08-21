#include <chrono>
#include <iostream>
#include <thread>

#include <gtest/gtest.h>

#include "generated/config.h"
#include "rflog/rfLog.h"

using namespace rflog;

TEST(frlog, logger_1)
{
    FRLog *logger = FRLog::instancePtr();
    logger->registerLogger(REG_OUTPUT_FOLDER + "/frlog/", "FR");

    logger->traceEntry("id1");
    logger->trace("trace");
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    logger->info("info");
    logger->traceExit("id1");

    warn("warn");
    debug("debug");
    error("error");
    logger->deRegisterLogger();
}

TEST(frlog, logger_without_register)
{
    trace("trace");
    info("info");
    debug("debug");
    warn("warn");
    error("error");
}
