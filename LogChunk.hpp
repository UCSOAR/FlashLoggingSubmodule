/*
 * LogChunk.hpp
 *
 *  Created on: Oct 24, 2026
 *      Author: josh wall
 */

#ifndef FLASHLOGGINGMODULE_INC_LOGCHUNK_HPP_
#define FLASHLOGGINGMODULE_INC_LOGCHUNK_HPP_

#include <cstdint>
#include <string.h>
#include "LogConsts.hpp"
#include "CubeDefines.hpp"

template <typename T>
class LogChunk
{
public:
    LogChunk(const T &data, const LoggingData dataTypeID, const uint32_t timeStamp)
    {
        SOAR_ASSERT(sizeof(T) + 6 <= UINT8_MAX, "LogChunk exceeds maximum size of 255 bytes.\n");

        logChunkArr[0] = static_cast<uint8_t>(dataTypeID);
        logChunkArr[1] = static_cast<uint8_t>(sizeof(T));
        memcpy(logChunkArr + 2, &timeStamp, sizeof(timeStamp));
        memcpy(logChunkArr + 6, &data, sizeof(data));
    }

    const uint8_t *getChunkBytes() const { return logChunkArr; }
    LoggingData getType() const { return static_cast<LoggingData>(logChunkArr[0]); }
    const uint8_t *getSensorData() const { return logChunkArr + 6; }
    uint8_t getDataSize() const { return logChunkArr[1]; }
    uint32_t getTimeStamp() const
    {
        uint32_t timeStamp_;
        memcpy(&timeStamp_, logChunkArr + 2, sizeof(timeStamp_));
        return timeStamp_;
    }
    uint8_t getChunkSize() const { return sizeof(logChunkArr); }

private:
    uint8_t logChunkArr[sizeof(T) + 6] = {0};
};

#endif
