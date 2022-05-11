#include "SplashScreenState.h"
#include "../utils/Arduboy2Ext.h"
#include "../images/Images.h"
#include "../utils/Enums.h"

static constexpr uint8_t ppotFrames = (1.6 *70); // 1.6s animation loop

// ----------------------------------------------------------------------------
//  Initialise state ..
//
void SplashScreenState::activate(StateMachine & machine) {

  auto & sound = machine.getContext().sound;
	auto & arduboy = machine.getContext().arduboy;

  sound.setOutputEnabled(arduboy.audio.enabled);
  sound.volumeMode(VOLUME_ALWAYS_NORMAL);
  
}


// ----------------------------------------------------------------------------
//  Handle state updates ..
//
void SplashScreenState::update(StateMachine & machine) {

  auto & sound = machine.getContext().sound;
	auto & arduboy = machine.getContext().arduboy;
	auto justPressed = arduboy.justPressedButtons();

  if (justPressed > 0 && this->counter > 0) {

    this->counter = 20;

  }

  if (justPressed > 0 && this->counter == 0) {

#ifdef PLAY_SOUNDS    
    sound.tone(NOTE_A2, 30);
    sound.tone(NOTE_F1, 30);
#endif

    this->counter = 1;

  }

  if (this->counter > 0) {

    this->counter++;

    if (this->counter == 21) machine.changeState(GameStateType::TitleScreen); 

  }

}


// ----------------------------------------------------------------------------
//  Render the state ..
//
void SplashScreenState::render(StateMachine & machine) {

	auto & arduboy = machine.getContext().arduboy;

  Sprites::drawOverwrite(32, 16, Images::PPOT, 0);

  uint8_t p = 28; // Default pixel position 0 is hidden in the image

  switch (arduboy.getFrameCount(ppotFrames)) {
  
    // 2nd quarter of frames
    case (ppotFrames*1/4) ... (ppotFrames*2/4)-1:
      p = 29; // Move pixel down to position 1
      /*-fallthrough*/

    // 1st quarter of frames
    case 0 ... (ppotFrames*1/4)-1:
      // Flash 'Play' arrow by clearing the image
      // Overwrite directly to the screen buffer
      arduboy.sBuffer[91 +(3*WIDTH)] = 0x00;
      arduboy.sBuffer[92 +(3*WIDTH)] = 0x00;
      arduboy.sBuffer[93 +(3*WIDTH)] = 0x00;
      arduboy.sBuffer[94 +(3*WIDTH)] = 0x00;
      arduboy.sBuffer[95 +(3*WIDTH)] = 0x00;
      break;

    // 3rd quarter of frames
    case (ppotFrames*2/4) ... (ppotFrames*3/4)-1:
      p = 30; // Move pixel down to position 2
      break;

    // 4th quarter of frames
    default:
      p = 31; // Move pixel down to position 3
      break;

  }

  // Draw pixel to represent the tape spooling
  // Render using pixel function
  arduboy.drawPixel(52, p, WHITE); // Positions 28, 29, 30, 31

  // On even steps of pixel movement, update the spindle image
  if (p % 2 == 0) { 
    // Render directly to the screen buffer
    arduboy.sBuffer[45 +(3*WIDTH)] = 0xA5;
    arduboy.sBuffer[46 +(3*WIDTH)] = 0x95;
  }

}
