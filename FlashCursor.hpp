/*
 * FlashCursor.hpp
 *
 *  Created on: Oct 24, 2026
 *      Author: josh wall
 */

#ifndef FLASHLOGGINGMODULE_INC_FLASHCURSOR_HPP_
#define FLASHLOGGINGMODULE_INC_FLASHCURSOR_HPP_

#include <cstdint>
#include "CubeDefines.hpp"

class FlashCursor
{
public:
    FlashCursor() = default;
    FlashCursor(uint16_t sectorAddress, uint8_t sectorIndex)
        : flashSectorAddress(sectorAddress), flashSectorIndex(sectorIndex)
    {
        SOAR_ASSERT(sectorAddress <= 4093, "Invalid sector address !(0-4093).\n");
        SOAR_ASSERT(sectorIndex <= 7, "Invalid sector index !(0-7).\n");
    }

    uint16_t getCursorSectorAddress() const;
    uint8_t getCursorSectorIndex() const;
    bool advanceCursor();

private:
    uint16_t flashSectorAddress = 0; // 0 - 4093 with 4095 reserved for flash cursor and 4094 for testing
    uint8_t flashSectorIndex = 0;    // 0 - 7
};

#endif
