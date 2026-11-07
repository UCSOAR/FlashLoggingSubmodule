/*
 * LogChunk.hpp
 *
 *  Created on: Oct 24, 2026
 *      Author: josh wall
 */


#ifndef FLASHLOGGINGMODULE_INC_LOGCHUNK_HPP_
#define FLASHLOGGINGMODULE_INC_LOGCHUNK_HPP_

#include <cstdint>
#include "LogConsts.hpp"


template <typename T>
class LogChunk {
    public:
        LogChunk(const T& dataReference, const LoggingData dataTypeID) 
            : data(dataReference), dataType(dataTypeID) {}
            
        LoggingData getType() const { return LogChunkArr[0]; }
        const T& getData() const { return LogChunkArr[1]; }
        uint8_t getDataSize() const { return LogChunkArr[2]; }  // bytes
        uint8_t getChunkSize() const { return sizeof(LogChunkArr)}

    private:
        const T data;   // store a copy of data cus ptrs may get overwritten
        const LoggingData dataType;
        const uint8_t dataSize = sizeof(T);
        uint8_t LogChunkArr[sizeof(T) + 2] = {dataType, data, dataSize};
    
};

#endif