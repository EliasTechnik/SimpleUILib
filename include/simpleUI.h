#pragma once
#include <Arduino.h>
#include <U8g2lib.h>

//include all ui components (this will be converted to one headerfile later)
#include "uiText.h" //All for Text
#include "uiInteractive.h" //legacy interactions handler (remove later)
#include "uiGraphics.h" //geometric drawing functions
#include "uiBasics.h" //basic classes to make everything work
#include "uiPage.h" //pagination
#include "uiRoot.h" //entry and also pagination
#include "uiHardwareInputDriver.h" //handles hardware input and conects hardware inputs to the ui tree
#include "uiPrebuilds.h" //preconfigureds uiElements to build upon
#include "uiButtons.h" //buttons :)
#include "uiInputs.h" //uiElements to handle specific user Inputs
#include "uiImage.h" //all about images (bitmap and xbm)
#include "uiCheckbox.h" //checkboxes
#include "uiRadio.h" //radiobuttons