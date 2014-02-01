/*
* RgbStrip.h
*
* Created: 21/12/2013 11:49:37
*  Author: Leenix
*/


#ifndef RGBSTRIP_H_
#define RGBSTRIP_H_

// Include
#include <Arduino.h>
#include "RGB.h"
#include "SimpleTimer.h"

#define TRANSITION_STEP 1	// Transition step in levels
#define TRANSITION_PERIOD_STEP 2	// Step for adjusting transition timer event period
#define DEFAULT_TRANSITION_PERIOD 10    // How often (in ms) transition steps occur

#define BRIGHTNESS_INCREMENT 10	// Brightness increment in percentage
#define DEFAULT_BRIGHTNESS 100	// Default brightness in percentage

#define LOW_BRIGHTNESS 30	// Low brightness value in percentage
#define FULL_BRIGHTNESS 100	// Full brightness value in percentage

#define STROBE_STEP 5	// Strobe period increment step in ms
#define DEFAULT_STROBE_PERIOD 100
#define MINIMUM_STROBE_PERIOD 20	// Minimum half-cycle strobe period in ms. This translates to 25 Hz

class RgbStrip
{
	public:
	// Constructor
	RgbStrip(int redPin, int greenPin, int bluePin);
	
	// Set the target colour. Colour will change instantly if transitions are disabled
	void setTargetColour(RGB colour);
	void setTargetColour(char colourCode);
	void setTargetColour(int colourIndex);

	// Set the brightness of the led strip
	void setBrightness(int percentage);
	
	// Set the brightness of the led strip to a low level
	void setLowBrightness();
	
	// Set the brightness of the led strip to the maximum level
	void setFullBrightness();
	
	// Set the brightness of the led strip to zero. (Turn them off)
	void lightsOff();
	
	// Increase the brightness of the led strip by BRIGHTNESS_INCREMENT
	void increaseBrightness();
	
	// Decrease the brightness of the led strip by BRIGHTNESS_INCREMENT
	void decreaseBrightness();
	
	// Enable transition timer events
	void enableTransitions();
	
	// Disabled transition timer events
	void disableTransitions();
	
	// Set the period for the transition timer events
	void setTransitionPeriod(long period);
	
	// Increase the period for transition timer events
	void increaseTransitionPeriod();
	
	// Decrease the period for transition timer events
	void decreaseTransitionPeriod();
	
	// Determine if transition timer events are enabled
	bool isTransitionsEnabled();
	
	// Enable strobe timer events
	void enableStrobe();
	
	// Disable strobe timer events
	void disableStrobe();
	
	// Set the toggle period for transition timer events
	void setStrobePeriod(long period);
	
	// Increase the period for strobe timer events by a fixed amount
	void increaseStrobePeriod();
	
	// Decrease the period for strobe timer events by a fixed amount
	void decreaseStrobePeriod();
	
	// Determine if strobe timer events are enabled
	bool isStrobeEnabled();
	
	// Update timer status
	void update();
	
	private:
	
	// Directly set the colour for the led strip to display
	void setActiveColour(RGB colour);
	
	// Update the active colour to the led strip
	void applyActiveColour();
	
	// Write the specified colour to the led strip. Uses global brightness settings
	void writeColour(RGB colour);
	
	// Step the active colour towards the target colour by TRANSITION_STEP levels
	void stepTowardsTargetColour();
	
	// Step the active colour towards the target colour by TRANSITION_STEP levels by channel
	void stepTowardsRedTarget();
	void stepTowardsGreenTarget();
	void stepTowardsBlueTarget();
	
	// Determine if the active colour is the same as the target colour
	bool isTargetColourReached();
	
	// Transition timer callback. Step the active colour towards the target.
	void transitionEvent();
	static void transitionEvent_wrapper();
	
	// Strobe timer callback.
	void strobeEvent();
	static void strobeEvent_wrapper();
	
	
	int	_redPin;
	int	_greenPin;
	int _bluePin;
	int _brightness;
	int _strobeBrightness;
	RGB _activeColour;
	RGB _targetColour;
	SimpleTimer	_timer;
	int _transitionEventID;
	int _strobeEventID;
};



#endif /* RGBSTRIP_H_ */