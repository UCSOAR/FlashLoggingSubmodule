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

        const T& getData() const { return data; }
        uint32_t getSize() const { return sizeof(T); }  // bytes
        LoggingData getType() const { return dataType; }

    private:
        const T data;   // store a copy of data cus ptrs may get overwritten
        const LoggingData dataType;
    
};

#endif