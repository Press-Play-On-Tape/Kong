#include "Girder.h"

#include "../utils/Enums.h"
#include "../map/Coordinates.h"

Girder::Girder() { }

uint8_t Girder::getPosition() {

  return this->position;

}

int8_t Girder::getXPosition() {

  return girderData.x;

}

int8_t Girder::getYPosition(uint8_t yOffset) {

  return girderData.y - yOffset;

}

uint8_t Girder::getImage() {

  return static_cast<uint8_t>(girderData.rotation);

}

bool Girder::isEnabled() {

  return this->enabled;

}

void Girder::setPosition(uint8_t position) {

  this->position = position;
  Coordinates::readGirderData(girderData, this->position);

}

void Girder::setEnabled(bool enabled) {

  this->enabled = enabled;

}

void Girder::updatePosition() {

  if (!this->enabled) return;

  this->position++;
  Coordinates::readGirderData(girderData, this->position);
 
  if (girderData.x == 0 && girderData.y == 0) {

    this->enabled = false;
    this->position = 0;

  }

}

Rect Girder::getRect(uint8_t yOffset) {

  return Rect{girderData.x + 1, girderData.y + 4, 20, 4 };

}
