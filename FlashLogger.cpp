/*
 * FlashLogger.cpp
 *
 *  Created on: Oct 24, 2026
 *      Author: josh wall
 */

#include "FlashLogger.hpp"

namespace
{

}

FlashLogger::FlashLogger() {}

LoggingStatus FlashLogger::init()
{
    // load and validate cursor, ram buffer, and flash state
    // if no issues
    return LoggingStatus::FLASH_READY;
}

LoggingStatus const FlashLogger::appendBytes(const uint8_t *bytes, uint8_t chunkSize)
{
    if (!loggingEnabled)
    {
        return LoggingStatus::LOGGING_DISABLED;
    }
}

bool FlashLogger::loadCursor()
{
    // FlashCursorStore function
}

bool FlashLogger::storeCursor()
{
    // FlashCursorStore function
}

void FlashLogger::startLogging()
{
    loggingEnabled = true;
}

void FlashLogger::stopLogging()
{
    if (!loggingEnabled)
    {
        return;
    }

    loggingEnabled = false;
    storeCursor();
}
