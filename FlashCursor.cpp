/*
 * FlashCursor.cpp
 *
 *  Created on: Oct 24, 2026
 *      Author: josh wall
 */

#include "FlashCursor.hpp"


uint16_t FlashCursor::getCursorSectorAddress() const
{
    return flashSectorAddress;
}

uint8_t FlashCursor::getCursorSectorIndex() const
{
    return flashSectorIndex;
}

bool FlashCursor::advanceCursor()
{
    if (flashSectorAddress == SECTORS_IN_USE && flashSectorIndex == INDEXES_IN_USE)
    {
        SOAR_PRINT("Cursor position: (%u, %u).\n", (unsigned int)flashSectorAddress, (unsigned int)flashSectorIndex);
        SOAR_PRINT("Flash storage is full.");

        return false;
    }

    if (flashSectorIndex == INDEXES_IN_USE)
    {
        flashSectorIndex = 0;
        flashSectorAddress++;
        SOAR_PRINT("Cursor position: (%u, %u).\n", (unsigned int)flashSectorAddress, (unsigned int)flashSectorIndex);
    }

    else
    {
        flashSectorIndex++;
        SOAR_PRINT("Cursor position: (%u, %u).\n", (unsigned int)flashSectorAddress, (unsigned int)flashSectorIndex);
    }

    return true;
}
