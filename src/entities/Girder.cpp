#include "Girder.h"

#include "../utils/Enums.h"
#include "../map/Coordinates.h"

Girder::Girder() { }

uint8_t Girder::getPosition() {

  return this->position;

}

int8_t Girder::getXPosition() {

  return pgm_read_byte(&Coordinates::Girder[(this->position * 3)]);

}

int8_t Girder::getYPosition(uint8_t yOffset) {

  uint8_t y = pgm_read_byte(&Coordinates::Girder[(this->position * 3) + 1]);
  return y - yOffset;

}

uint8_t Girder::getImage() {

  return pgm_read_byte(&Coordinates::Girder[(this->position * 3) + 2]);

}

bool Girder::isEnabled() {

  return this->enabled;

}

void Girder::setPosition(uint8_t position) {

  this->position = position;

}

void Girder::setEnabled(bool enabled) {

  this->enabled = enabled;

}

void Girder::updatePosition() {

  if (!this->enabled) return;

  this->position++;

  uint8_t x = pgm_read_byte(&Coordinates::Girder[(this->position * 3)]);
  uint8_t y = pgm_read_byte(&Coordinates::Girder[(this->position * 3) + 1]);
 
  if (x == 0 && y == 0) {

    this->enabled = false;
    this->position = 0;

  }
}

Rect Girder::getRect(uint8_t yOffset) {

  int8_t x = pgm_read_byte(&Coordinates::Girder[(this->position * 3)]);
  uint8_t y = pgm_read_byte(&Coordinates::Girder[(this->position * 3) + 1]) - yOffset;

  return Rect{x + 1, y + 4, 20, 4 };

}
