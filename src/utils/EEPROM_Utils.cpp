#include "EEPROM_Utils.h"
#include "Arduboy2Ext.h"
#include "Enums.h"

#include <avr/eeprom.h>

/* ----------------------------------------------------------------------------
 *   Is the EEPROM initialised?
 *
 *   Looks for the characters 'D' and 'K' in the first two bytes of the EEPROM
 *   memory range starting from byte EEPROM_STORAGE_SPACE_START.  If not found,
 *   it resets the settings ..
 */

constexpr uint8_t letter1 = 'D';
constexpr uint8_t letter2 = 'K';

namespace EEPROM_Utils {

  void initialiseEEPROM() {

    uint8_t * const eepromStartChar1 = reinterpret_cast<uint8_t *>(eepromStart_C1);
    uint8_t * const eepromStartChar2 = reinterpret_cast<uint8_t *>(eepromStart_C2);

    const byte c1 = eeprom_read_byte(eepromStartChar1);
    const byte c2 = eeprom_read_byte(eepromStartChar2);

    if ((c1 != letter1) || (c2 != letter2)) {

      clearEEPROM();

    }

  }

  void clearEEPROM() {

    uint8_t * const eepromStartChar1 = reinterpret_cast<uint8_t *>(eepromStart_C1);
    uint8_t * const eepromStartChar2 = reinterpret_cast<uint8_t *>(eepromStart_C2);

    eeprom_update_byte(eepromStartChar1, letter1);
    eeprom_update_byte(eepromStartChar2, letter2);

    SaveEntry * const saveEntries = reinterpret_cast<SaveEntry *>(saveEntriesStart);
    
    for(uint8_t saveIndex = 0; saveIndex < saveEntriesTotal; ++saveIndex) {
    
      SaveEntry entry { 0, "" };

      const char letter = ('A' + saveIndex);
      for (uint8_t index = 0; index < SaveEntry::nameCount; ++index) {

        entry.name[index] = letter;
      
      }

      entry.name[SaveEntry::nameCount] = '\0';

      eeprom_update_block(&entry, &saveEntries[saveIndex], sizeof(SaveEntry));

    }

  }

  uint8_t findScore(uint16_t newScore, const SaveEntry * saveEntries) {

    for (uint8_t i = 0; i < saveEntriesCount; ++i) {

      const uint16_t oldScore = eeprom_read_word(&saveEntries[i].score);

      if (newScore >= oldScore) {

        return i;

      }

    }

    return NO_WINNER;

  }

  /* -----------------------------------------------------------------------------
   *   Save score if it is in the top 3, return slot number (or NO_WINNER) ..
   */
  uint8_t saveScore(GameMode gameMode, uint16_t newScore) {

    const size_t saveEntriesIndex = (gameMode == GameMode::Easy) ? saveEntriesEasyStart : saveEntriesHardStart;
    SaveEntry * saveEntries = reinterpret_cast<SaveEntry *>(saveEntriesIndex);

    const uint8_t targetIndex = findScore(newScore, saveEntries);

    if (targetIndex == NO_WINNER) {

      return NO_WINNER;

    }

    // New High Score ..

    for (uint8_t index = (saveEntriesCount - 1); index > targetIndex; --index) {

      const uint8_t previousIndex = (index - 1);

      SaveEntry entry;
      eeprom_read_block(&entry, &saveEntries[previousIndex], sizeof(SaveEntry));
      eeprom_update_block(&entry, &saveEntries[index], sizeof(SaveEntry));

    }

    // Save new name and score ..
    SaveEntry entry { newScore, { '?', '?', '?', '\0' } };
    eeprom_update_block(&entry, &saveEntries[targetIndex], sizeof(SaveEntry));

    return targetIndex;

  }

  void saveChar(GameMode gameMode, uint8_t saveIndex, uint8_t charIndex, char newChar) {

    const size_t saveEntriesIndex = (gameMode == GameMode::Easy) ? saveEntriesEasyStart : saveEntriesHardStart;
    SaveEntry * saveEntries = reinterpret_cast<SaveEntry *>(saveEntriesIndex);
    uint8_t * const bytePointer = reinterpret_cast<uint8_t *>(&saveEntries[saveIndex].name[charIndex]);
    eeprom_update_byte(bytePointer, newChar);

  }

  void readSaveEntry(GameMode gameMode, SaveEntry & entry, uint8_t saveIndex) {

    const size_t saveEntriesIndex = (gameMode == GameMode::Easy) ? saveEntriesEasyStart : saveEntriesHardStart;
    const SaveEntry * saveEntries = reinterpret_cast<const SaveEntry *>(saveEntriesIndex);
    eeprom_read_block(&entry, &saveEntries[saveIndex], sizeof(SaveEntry));

  }

  void readSaveEntryName(GameMode gameMode, char (&name)[SaveEntry::nameSize], uint8_t saveIndex) {

    const size_t saveEntriesIndex = (gameMode == GameMode::Easy) ? saveEntriesEasyStart : saveEntriesHardStart;
    const SaveEntry * saveEntries = reinterpret_cast<const SaveEntry *>(saveEntriesIndex);
    eeprom_read_block(&name, &saveEntries[saveIndex].name, SaveEntry::nameSize);

  }

  uint16_t readSaveEntryScore(GameMode gameMode, uint8_t saveIndex) {

    const size_t saveEntriesIndex = (gameMode == GameMode::Easy) ? saveEntriesEasyStart : saveEntriesHardStart;
    const SaveEntry * saveEntries = reinterpret_cast<const SaveEntry *>(saveEntriesIndex);
    return eeprom_read_word(&saveEntries[saveIndex].score);

  }

  void writeSaveEntry(GameMode gameMode, const SaveEntry & entry, uint8_t saveIndex) {

    const size_t saveEntriesIndex = (gameMode == GameMode::Easy) ? saveEntriesEasyStart : saveEntriesHardStart;
    SaveEntry * saveEntries = reinterpret_cast<SaveEntry *>(saveEntriesIndex);
    eeprom_update_block(&entry, &saveEntries[saveIndex], sizeof(SaveEntry));

  }

  void writeSaveEntryName(GameMode gameMode, const char (&name)[SaveEntry::nameSize], uint8_t saveIndex) {

    const size_t saveEntriesIndex = (gameMode == GameMode::Easy) ? saveEntriesEasyStart : saveEntriesHardStart;
    SaveEntry * saveEntries = reinterpret_cast<SaveEntry *>(saveEntriesIndex);
    eeprom_update_block(&name, &saveEntries[saveIndex].name, SaveEntry::nameSize);

  }

  void writeSaveEntryScore(GameMode gameMode, uint16_t score, uint8_t saveIndex) {

    const size_t saveEntriesIndex = (gameMode == GameMode::Easy) ? saveEntriesEasyStart : saveEntriesHardStart;
    SaveEntry * saveEntries = reinterpret_cast<SaveEntry *>(saveEntriesIndex);
    eeprom_update_word(&saveEntries[saveIndex].score, score);

  }

}