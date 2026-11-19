/*
 * FlashCursorStore.hpp
 *
 *  Created on: Nov 7, 2026
 *      Author: Jacob Hailemariam
 */


 /*
 this file in short---->

 //1 Load() function


//2 Save(cursor) function

//3 Clear() function
 
 
 */

#ifndef FLASHLOGGINGMODULE_INC_FLASHCURSORSTORE_HPP_
#define FLASHLOGGINGMODULE_INC_FLASHCURSORSTORE_HPP_

#include <cstdint>           // gives us uint32_t (exactly 32 bits on every compiler)

#include "FlashCursor.hpp"
#include "LogConsts.hpp"

class FlashCursorStore {
public:
    //-------What the other classes are allowed to call

    // Reads the store sector, finds the newest valid record and returns it as a cursor.
    //FlashLogger calls this once at startup. LogReader calls it to know where the dump ends.
    //If nothing valid is saved (first boot or after a clear), it returns a cursor at 0
    FlashCursor Load();


    //Writes the cursor into the next slot, which is passed by const&
    //Returns true if the read-back matched what we wrote, false if the write failed
    bool Save(const FlashCursor& cursor);

    // Erases the store sector and starts the slot cout over.
    // Used when someone runs flash_clear to wipe old logs
    void Clear();

private:
    //Only this class's own functions can touch it.

    //-------------------Constants----------------
    // "static constexpr" means: one shared copy for the whole class (static), and the value is known at compile time (constexpr), so it costs no RAM

    // Sectors are numbered 0 to NUM_SECTORS - 1, so this is the very last sector.
    //the logger must never write sensor data here, it belongs to te store.
    static constexpr uint32_t STORE_SECTOR = NUM_SECTORS - 1;

    //A fixed "signature" written at the start of every record, Erased flash reads
    //0xFFFFFFFF 
    // we see it we know the slot holds a real record. 0x4C4F4731 is "LOG1"
    static constexpr uint32_t MAGIC = 0x4C4F4731;

    //-------What one saved record looks like on flash ----------------
    struct CursorRecord {
        uint32_t magic;
        uint32_t buffer;
        uint32_t checksum;
        uint32_t reserved;
    };

    // Checked at compile time: if someone changes the struct and it stops being 
    // 16 bytes, the build fails here instead of the slot math silently breaking.
    static_assert(sizeof(CursorRecord) == 16, "CursorRecord must be 16 bytes");

    // 4096 B sector / 16 B record = 256 slots. 16 divides 4096 evenly, so no slot
    // ever hangs off the end of the sector. That's the reason for the filler field
    static constexpr uint32_t RECORDS_PER_SECTOR = SECTOR_READ / sizeof(CursorRecord);

    // Value of a slot that has never been written since the last erase.
    // NOR flash erases to all 1s, so an untouched uint32_t reads 0xFFFFFFFF
    static constexpr uint32_t ERASED_WORD = 0xFFFFFFFF;

    //---------------Helpers------------
    // static because they only look at the record passed in,
    // not at nextSlot_ or slotKnown_, so they don't need an object to run

    //Mixes the record's fields into 1 number to catch half-written records
    static uint32_t CheckSum(const CursorRecord& r);

    //True only if magic == MAGIC and checksum matches
    static bool IsValid(const CursorRecord& r);

    // Not static: it changes nextSlot_ and slotKnown_
    //Walks the sector, fills 'newest' wit the last valid record

    bool ScanSector(CursorRecord& newest);

    //-------------------State----------------
    // Index (0 to 255) of the first blank slot, where the next Save goes.
    // If it hits RECORDS_PER_SECTOR the sector is full and Save erases first
    uint32_t nextSlot_ = 0;

    // false until the sector has been scanned once, so Save knows
    // not to trust nextSlot_ yet
    bool slotKnown_ = false;


    
};

#endif // FLASHLOGGINGMODULE_INC_FLASHCURSORSTORE_HPP_





