#pragma once

#include "../arduboyTones/ArduboyTonesPitches.h"
#include "../arduboyTones/ArduboyTonesExt.h"
#include "../utils/Arduboy2Ext.h"
#include "../Utils/Enums.h"
#include "Base.h"

class Hook : public Base {

  public:

    Hook();


    // Methods ..

    void decCounter();
    void setSound(ArduboyTonesExt *sound);

  protected:

    ArduboyTonesExt *sound;

};

