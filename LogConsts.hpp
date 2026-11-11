/*
 * LogConsts.hpp
 *
 *  Created on: Nov 2, 2026
 *      Author: josh wall
 */


#ifndef FLASHLOGGINGMODULE_INC_LOGCONSTS_HPP_
#define FLASHLOGGINGMODULE_INC_LOGCONSTS_HPP_

#include <cstdint>
#include "CubeDefines.hpp"

constexpr uint32_t RAM_LOG_SIZE = 512; // bytes
constexpr uint32_t SECTOR_READ = 4096;
constexpr uint32_t NUM_SECTORS = 4096;
constexpr uint32_t SLOTS_IN_SECTORS = (NUM_SECTORS / RAM_LOG_SIZE); 
constexpr uint32_t SECTORS_IN_USE = (NUM_SECTORS - 3); 		// -2 last two sectors for testing and flash cursor; -1 for zero based indexing
constexpr uint32_t INDEXES_IN_USE = (SLOTS_IN_SECTORS - 1); // -1 for zero based indexing
constexpr uint32_t TOTAL_LOGBUF_SLOTS = (NUM_SECTORS * 8);

enum class LoggingData		// should this be general so more things not just sensor types can be logged?
{
	IMU32G,
	IMU16G,
	MAG,
	BARO07,
	BARO11,
	GPS,
	FILTER

};

enum class LoggingStatus 
{
	LOG_SUCCESS,
	LOG_ERR

};

#endif