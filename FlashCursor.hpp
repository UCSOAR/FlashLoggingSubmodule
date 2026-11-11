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
        SOAR_ASSERT(sectorAddress <= SECTORS_IN_USE, "Invalid sector address.\n");
        SOAR_ASSERT(sectorIndex <= INDEXES_IN_USE, "Invalid sector index.\n");
    }

    uint16_t getCursorSectorAddress() const;
    uint8_t getCursorSectorIndex() const;
    bool advanceCursor();

private:
    uint16_t flashSectorAddress = 0; // VALID: [0, SECTORS_IN_USE]
    uint8_t flashSectorIndex = 0;    // VALID: [0, INDEXES_IN_USE]
};

#endif
