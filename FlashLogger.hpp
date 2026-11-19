/*
 * FlashLogger.hpp
 *
 *  Created on: Oct 24, 2026
 *      Author: josh wall
 */

#ifndef FLASHLOGGINGMODULE_INC_FLASHLOGGER_HPP_
#define FLASHLOGGINGMODULE_INC_FLASHLOGGER_HPP_

#include "FlashCursor.hpp"
#include "LogConsts.hpp"
#include "LogChunk.hpp"

class FlashLogger
{
public:
    FlashLogger();

    LoggingStatus init();

    template <typename T>
    LoggingStatus logData(const LogChunk<T>& chunk) // takes in a LogChunk object reference to make it easier for users
    {
        return appendBytes(chunk.getChunkBytes(), chunk.getChunkSize());
    }

    bool loadCursor();  // Employ FlashCursorStore functions
    bool storeCursor(); // ^

    void startLogging();
    void stopLogging();

private:
    LoggingStatus const appendBytes(uint8_t *bytes, uint8_t chunkSize);

    uint8_t ramBuffer[RAM_LOG_SIZE] = {};
    uint32_t ramHead = 0;
    bool loggingEnabled = false;
};

#endif