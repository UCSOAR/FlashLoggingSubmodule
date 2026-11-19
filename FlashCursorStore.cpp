/*
 * FlashCursorStore.cpp
 *
 * Created on: Nov 7, 2026
 *      Author: Jacob Hailemariam
 *
 *
*/


#include "FlashCursorStore.hpp"
#include "mx66xx_qspi.hpp"


// -----------1. CheckSum---------------

//Return one number built from r.magic and r.buffer
// XOR (^) them together
// This is a simple checksum that will catch most errors, but not all. It is not cryptographically secure.
uint32_t FlashCursorStore::CheckSum(const CursorRecord& r)
{

    // You could also ^ 0xA5A55A5A (the old LoggingService does), but it's not necessary:
    // XOR with a constant doesn't catch any extra errors, and the magic check in IsValid
    // already rejects all-0x00 and all-0xFF records.
    return r.magic ^ r.buffer;
}

// -----------2. IsValid---------------

// A record only counts if BOTH checks pass:
//   magic matches     -> it's a real record, not blank (0xFF) or garbage
//   checksum matches  -> the whole record was written, not cut off by a power loss
bool FlashCursorStore::IsValid(const CursorRecord& r)
{
    return r.magic == MAGIC && r.checksum == CheckSum(r);   // == binds tighter than &&, so no brackets needed
}

// -----------3. Clear---------------

// Wipes every saved cursor. Used by flash_clear.
// After this, Load returns 0 until the next Save.
void FlashCursorStore::Clear()
{
    MX66xxQSPI_EraseSector(STORE_SECTOR);   // all 256 slots back to 0xFF
    nextSlot_ = 0;                          // slot 0 is blank now
    slotKnown_ = true;                      // we just erased, so we KNOW slot 0 is blank, no scan needed
}

// -----------4. ScanSector---------------

// Walks the store sector from slot 0 upward and does two jobs in one pass:
//   1. remembers the newest valid record (copied into 'newest')
//   2. finds the first blank slot (stored in nextSlot_) so Save knows where to write
// Returns true if at least one valid record was found.
bool FlashCursorStore::ScanSector(CursorRecord& newest)
{
    bool found = false;

    for (uint32_t slot = 0; slot < RECORDS_PER_SECTOR; slot++)
    {
        // Read one 16 B slot straight into a struct
        CursorRecord rec{};
        MX66xxQSPI_ReadSector(reinterpret_cast<uint8_t*>(&rec), STORE_SECTOR,
                              slot * sizeof(CursorRecord), sizeof(CursorRecord));

        // Blank slot: records are only ever appended, so everything after this is blank too.
        // Stop here: this is where the next Save goes.
        if (rec.magic == ERASED_WORD)
        {
            nextSlot_ = slot;
            slotKnown_ = true;
            return found;
        }

        // Valid record: keep it. Slots are in write order, so a later valid one
        // overwrites an earlier one and 'newest' ends up holding the latest.
        // Invalid (half-written) records just get skipped.
        if (IsValid(rec))
        {
            newest = rec;
            found = true;
        }
    }

    // Loop finished without a blank slot: all 256 are used.
    // Setting nextSlot_ to 256 tells Save to erase before the next write.
    nextSlot_ = RECORDS_PER_SECTOR;
    slotKnown_ = true;
    return found;
}

// -----------5. Load---------------

// Returns the newest saved cursor, or a cursor at buffer 0 if nothing valid is saved
// (first boot, or right after a Clear).
FlashCursor FlashCursorStore::Load()
{
    CursorRecord rec{};
    if (ScanSector(rec))
    {
        return FlashCursor(rec.buffer);
    }
    return FlashCursor(0);
}

// -----------6. Save---------------

// Appends the cursor into the next blank slot of the store sector.
// Returns true if reading it back gives the same value we wrote.
bool FlashCursorStore::Save(const FlashCursor& cursor)
{

    // Step 1: if nobody has scanned the sector yet (e.g. Save called before Load),
    // we don't know where the next blank slot is, so scan now.
    if(!slotKnown_)
    {
        CursorRecord unused{};   // ScanSector needs a record to fill, but we don't need it here
        ScanSector(unused);      // we only want its side effect: setting nextSlot_ and slotKnown_
    }



    // Step 2: all 256 slots used -> wipe the sector and start again at slot 0.
    // This is the ONLY place the store erases, which keeps erase cycles low.
    if (nextSlot_ >= RECORDS_PER_SECTOR)
    {
        MX66xxQSPI_EraseSector(STORE_SECTOR);   // whole 4 KB sector back to 0xFF
        nextSlot_ = 0;                           // first slot is blank again
    }



    // Step 3: build the record
    // Make a CursorRecord called rec{} and fill 4 fields:
    CursorRecord rec{};
    rec.magic = MAGIC;
    rec.buffer = cursor.GetBuffer();
    rec.reserved = ERASED_WORD;     // filler stays 0xFF, same as erased flash
    rec.checksum = CheckSum(rec);   // LAST: fingerprint of the fields above

    // Step 4: write it
    const uint32_t offset = nextSlot_ * sizeof(CursorRecord);   // 0, 16, 32, ... 4080
    MX66xxQSPI_WriteSector(reinterpret_cast<uint8_t*>(&rec), STORE_SECTOR, offset, sizeof(rec));

    // Step 5: move to the next slot, even if the write turns out bad.
    // A bad slot fails IsValid on the next Load and just gets skipped, so stepping past it is safe.
    // If we didn't move on, the next Save would try to write on top of half-programmed bits.
    nextSlot_++;

    // Step 6: read the same 16 B back into a SEPARATE record and check it.
    // Comparing against rec would only compare RAM to RAM; reading into check proves it's in flash.
    CursorRecord check{};
    MX66xxQSPI_ReadSector(reinterpret_cast<uint8_t*>(&check), STORE_SECTOR, offset, sizeof(check));
    return IsValid(check) && (check.buffer == rec.buffer);
}