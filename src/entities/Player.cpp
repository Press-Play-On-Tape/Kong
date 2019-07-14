#include "Player.h"

#include "../utils/Enums.h"
#include "../map/Coordinates.h"
#include "../sounds/Sounds.h"

#define JUMP_POSITIONS 37

const uint8_t PROGMEM jumpPositions[JUMP_POSITIONS] = { 
0, 2, 4, 6,
8, 9, 10, 10,
11, 11, 12, 12, 

12, 13, 13, 13, 
13, 13, 13, 13, 
13, 13, 13, 13, 

//13, 13, 13, 13, 
13, 12, 12, 12, 
11, 11, 10, 9, 

8, 6, 4, 2, 
0
};

Player::Player() { 

  this->movements = pgm_read_byte(&Coordinates::Player[(this->position * 5) + 4]);
  this->yOffset = pgm_read_byte(&Coordinates::Player[(this->position * 5) + 2]);

}

uint16_t Player::getPosition() {

  return this->position;

}

uint8_t Player::getJumpPosition() {

  return this->jumpPosition;

}

int8_t Player::getXPosition(bool updatePrevPosition) {

  if (updatePrevPosition) {
    this->prevXPosition = this->currXPosition;
  }

  this->currXPosition = static_cast<int8_t>(pgm_read_byte(&Coordinates::Player[(this->position * 5)]));
  return this->currXPosition;

}

int8_t Player::getYPosition() {

  int8_t y = pgm_read_byte(&Coordinates::Player[(this->position * 5) + 1]) - this->yOffset - pgm_read_byte(&jumpPositions[this->jumpPosition]);
  return y;

}

uint8_t Player::getMovements() {

  return this->movements;

}

uint8_t Player::getYOffset() {

  return this->yOffset;

}

bool Player::isDead() {

  return this->dead;

}

bool Player::isLeaping() {

  return this->leaping;

}

bool Player::isFalling() {

  return this->falling;

}

void Player::setSound(ArduboyTonesExt *sound) {

  this->sound = sound;

}

void Player::setPosition(uint16_t position) {

  this->position = position;
  this->movements = pgm_read_byte(&Coordinates::Player[(this->position * 5) + 4]);
  this->yOffset = pgm_read_byte(&Coordinates::Player[(this->position * 5) + 2]);
  this->jumpPosition = 0;

}

void Player::setJumpPosition(uint8_t jumpPosition) {

  this->jumpPosition = jumpPosition;

}

void Player::setMovements(uint8_t movements) {

  this->movements = movements;

}

void Player::setYOffset(uint8_t yOffset) {

  this->yOffset = yOffset;

}

void Player::setDead(bool dead) {

  this->dead = dead;

}

void Player::setLeaping(bool leaping) {

  this->leaping = leaping;

}

void Player::setFalling(bool falling) {

  this->falling = falling;

}

void Player::incPlayerPosition() {

  this->position++;
  this->movements = pgm_read_byte(&Coordinates::Player[(this->position * 5) + 4]);
  this->yOffset = pgm_read_byte(&Coordinates::Player[(this->position * 5) + 2]);

}

void Player::decPlayerPosition() {

  this->position--;
  this->movements = pgm_read_byte(&Coordinates::Player[(this->position * 5) + 4]);
  this->yOffset = pgm_read_byte(&Coordinates::Player[(this->position * 5) + 2]);

}

bool Player::canMove(Movements movement) {

  return (this->movements & static_cast<uint8_t>(movement));

}

void Player::updateJump() {

  if (this->jumpPosition < JUMP_POSITIONS - 1) {
    this->jumpPosition++;
  }
  else {
    this->jumpPosition = 0;
  }

}

void Player::startJump() {

#ifdef PLAY_SOUNDS 
  this->sound->tones(Sounds::jump);
#endif
  this->jumpPosition = 1;

}

bool Player::isJumping() {

  return this->jumpPosition > 0;

}

uint8_t Player::getImage() {

  Stance stance = static_cast<Stance>(pgm_read_byte(&Coordinates::Player[(this->position * 5) + 3]));
  
  if (this->jumpPosition == 0) {

    switch  (stance) {

      case Stance::Normal:

        if (this->currXPosition == this->prevXPosition) {

          if (this->runMovement == Movements::Left) {
            return static_cast<uint8_t>(Stance::Normal);
          }
          else {
            return static_cast<uint8_t>(Stance::Normal_RHS);
          }

        }

        if (this->currXPosition > this->prevXPosition) {

          this->runCounter++;
          this->runMovement = Movements::Right;
          if (this->runCounter == 16) this->runCounter = 0;
          return static_cast<uint8_t>(Stance::Normal) + ((this->runCounter / 4) * 2) + 3;

        }

        if (this->currXPosition < this->prevXPosition) {

          this->runCounter++;
          this->runMovement = Movements::Left;
          if (this->runCounter == 16) this->runCounter = 0;
          return static_cast<uint8_t>(Stance::Normal) + ((this->runCounter / 4) * 2) + 2;

        }

        break;

      default:

        return static_cast<uint8_t>(stance);

    }

  }
  else {

    if (this->currXPosition == this->prevXPosition) {

      if (this->runMovement == Movements::Left) {
        return static_cast<uint8_t>(Stance::Jump);
      }
      else {
        return static_cast<uint8_t>(Stance::Jump_RHS);
      }

    }

  }

  return static_cast<uint8_t>(Stance::Normal);

}


Rect Player::getRect() {

  uint8_t x = pgm_read_byte(&Coordinates::Player[(this->position * 5)]);
  int8_t y = pgm_read_byte(&Coordinates::Player[(this->position * 5) + 1]) - this->yOffset - pgm_read_byte(&jumpPositions[this->jumpPosition]);
  return Rect{x, y, 7, 11 };

}

void Player::reset() {

  this->setPosition(0);
  this->setJumpPosition(0);
  this->dead = false;
  this->leaping = false;
  this->falling = false;
  this->currXPosition = static_cast<int8_t>(pgm_read_byte(&Coordinates::Player[(this->position * 5)]));
  this->prevXPosition = -1;//static_cast<int8_t>(pgm_read_byte(&Coordinates::Player[(this->position * 5)]));
  this->runMovement = Movements::Right;

}