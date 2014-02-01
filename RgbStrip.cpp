#include "RgbStrip.h"

void* pt2Object;

RgbStrip::RgbStrip(int redPin, int greenPin, int bluePin) {
	// Pin assignments
	_redPin = redPin;
	_greenPin = greenPin;
	_bluePin = bluePin;
	pinMode(_redPin, OUTPUT);
	pinMode(_greenPin, OUTPUT);
	pinMode(_bluePin, OUTPUT);

	// Set initial brightness and colour
	_brightness = DEFAULT_BRIGHTNESS;
	setTargetColour(OFF);
	
	// Set up transition events
	_transitionEventID = _timer.setInterval(DEFAULT_TRANSITION_PERIOD, transitionEvent_wrapper);
	disableTransitions();
	
	// Set up strobe events
	_strobeEventID = _timer.setInterval(DEFAULT_STROBE_PERIOD, strobeEvent_wrapper);
	disableStrobe();
}


// Colour control
/**
* Set the target colour of the RGB strip
* @param colour RGB colour code of the desired colour
*/
void RgbStrip::setTargetColour(RGB colour) {
	_targetColour = colour;
	
	// If transitions are not enabled, write the change in colour immediately
	if (isTransitionsEnabled() == false){
		setActiveColour(colour);
	}
}


/**
* Set the target colour of the RGB strip
* The colour indexes that are used in this method are found in COLOUR_INDEXES in RGB.h
* Worded colours are mapped to their corresponding indexes. See RGB.h for more detail.
* @param colourIndex The index of the desired colour according to the COLOUR_INDEXES enum.
*/
void RgbStrip::setTargetColour(int colourIndex){
	if (colourIndex < COLOUR_MAP.length()){
		RGB colour = COLOURS[colourIndex];
		setTargetColour(colour);
	}
}


/**
* Set the target colour of the RGB strip
* The colour indexes that are used in this method are found in COLOUR_MAP in RGB.h
* Single characters are mapped to colour index entries found in
* @param colourIndex The character index of the desired colour according to the COLOUR_MAP string in RGB.h
*/
void RgbStrip::setTargetColour(char colourCode){
	int colourIndex = COLOUR_MAP.indexOf(colourCode);
	if (colourIndex >= 0){
		setTargetColour(colourIndex);
	}
}


/**
* Set the active colour displayed directly (without transitioning)
* @param colour RGB colour code of the desired active colour
*/
void RgbStrip::setActiveColour(RGB colour) {
	_activeColour = colour;
	writeColour(_activeColour);
}


// Colour control - Private

/**
* Write the active colour to the RGB channels
*/
void RgbStrip::applyActiveColour() {
	
}


/**
* Write the specified colour to the RGB PWM outputs
* The global brightness factor is applied prior to writing the output to the colour channel pins
* @param colour RGB object containing the desired colour code
*/
void RgbStrip::writeColour(RGB colour) {
	byte adjustedRed;
	byte adjustedGreen;
	byte adjustedBlue;

	// Apply global brightness level
	adjustedRed = (colour.r * _brightness) / 100;
	adjustedGreen = (colour.g * _brightness) / 100;
	adjustedBlue = (colour.b * _brightness) / 100;

	analogWrite(_redPin, adjustedRed);
	analogWrite(_greenPin, adjustedGreen);
	analogWrite(_bluePin, adjustedBlue);
}


// Brightness
/**
* Set the global intensity of the lights as a percentage.
* Setting the brightness to zero will effectively turn the lights off.
* @param percentage The percentage intensity of the lights. 100% is full brightness, whilst 0% is off
*/
void RgbStrip::setBrightness(int percentage) {
	if (percentage > 100) {
		percentage = 100;
		} else if (percentage <= 0) {
		percentage = 0;
	}

	_brightness = percentage;
	applyActiveColour();
}

	
/**
* Set the brightness to a low level
* This level is defined in RgbStrip.h. Default: 30%
*/
void RgbStrip::setLowBrightness(){
	setBrightness(LOW_BRIGHTNESS);
}


/**
* Set the global brightness to full
*/
void RgbStrip::setFullBrightness(){
	setBrightness(FULL_BRIGHTNESS);
}


/**
* Increase the intensity of the LEDs by a fixed percentage
* By default, this is by steps of 10%
* Intensity will increase to a maximum of 100%
*/
void RgbStrip::increaseBrightness() {
	setBrightness(_brightness + BRIGHTNESS_INCREMENT);
}


/**
* Decrease the intensity of the LEDs by a fixed percentage
* By default, this is by steps of 10%
* Intensity will decrease to a minimum of 0%
*/
void RgbStrip::decreaseBrightness() {
	setBrightness(_brightness - BRIGHTNESS_INCREMENT);
}


/**
* Turn the lights off.
* This is accomplished via setting brightness, rather than colour
*/
void RgbStrip::lightsOff() {
	setBrightness(0);
}


/**
* Update timer to call events if needed  
*/
void RgbStrip::update(){
	_timer.run();
}

// Transitions
/**
* Transitions all colour channels towards the target colour
* Changes to the active colour are applied immediately
*/
void RgbStrip::stepTowardsTargetColour() {
	stepTowardsRedTarget();
	stepTowardsGreenTarget();
	stepTowardsBlueTarget();

	applyActiveColour();
}


/**
* Determines if the active colour is the same as target colour
* @return True if the active colour is the same as the target colour
*/
bool RgbStrip::isTargetColourReached() {
	bool targetReached = false;
	if (_targetColour.r == _activeColour.r) {
		if (_targetColour.g == _activeColour.g) {
			if (_targetColour.b == _activeColour.b) {
				targetReached = true;
			}
		}
	}

	return targetReached;
}


/**
* Steps the active red channel towards the target colour
*/
void RgbStrip::stepTowardsRedTarget() {
	// If the difference between current and target level is less than the transition step, just apply the target level
	if (abs(_targetColour.r - _activeColour.r) > TRANSITION_STEP) {
		if (_targetColour.r > _activeColour.r) {
			_activeColour.r += TRANSITION_STEP;
			} else {
			_activeColour.r -= TRANSITION_STEP;
		}
		} else {
		_activeColour.r = _targetColour.r;
	}

}


/**
* Steps the active green channel towards the target colour
*/
void RgbStrip::stepTowardsGreenTarget() {
	if (abs(_targetColour.g - _activeColour.g) > TRANSITION_STEP) {
		if (_targetColour.g > _activeColour.g) {
			_activeColour.g += TRANSITION_STEP;
			} else {
			_activeColour.g -= TRANSITION_STEP;
		}
		} else {
		_activeColour.g = _targetColour.g;
	}
}


/**
* Steps the active blue channel towards the target colour
*/
void RgbStrip::stepTowardsBlueTarget() {
	if (abs(_targetColour.b - _activeColour.b) > TRANSITION_STEP) {
		if (_targetColour.b > _activeColour.b) {
			_activeColour.b += TRANSITION_STEP;
			} else {
			_activeColour.b -= TRANSITION_STEP;
		}
		} else {
		_activeColour.b = _targetColour.b;
	}
}


/**
* Callback event for the transition timer event
* Steps the active colour one increment towards the target colour
*/
void RgbStrip::transitionEvent(){
	stepTowardsTargetColour();
}


/**
* Static method wrapper for transition timer events
*/
void RgbStrip::transitionEvent_wrapper(){
	RgbStrip* thisInstance = (RgbStrip*) pt2Object;
	
	thisInstance->transitionEvent();
}


/**
* Enable timed transition events to occur  
*/
void RgbStrip::enableTransitions(){
	_timer.enable(_transitionEventID);
}


/**
* Disable timed transition events  
*/
void RgbStrip::disableTransitions(){
	_timer.disable(_transitionEventID);
}


/**
* Set the period for transition events
* @param period Time between transition events in ms.  
*/
void RgbStrip::setTransitionPeriod(long period){
	if (period < TRANSITION_STEP){
		period = TRANSITION_STEP;
	}
	
	_timer.setTimerPeriod(_transitionEventID, period);
}


/**
* Increase the time between transition events by a set amount 
* (default/TRANSITION_PERIOD_STEP: 2ms)  
*/
void RgbStrip::increaseTransitionPeriod(){
	long transitionPeriod = _timer.getTimerPeriod(_transitionEventID);
	setTransitionPeriod(transitionPeriod + TRANSITION_PERIOD_STEP);
}


/**
* Decrease the time between transition events by a set amount
* (default/TRANSITION_PERIOD_STEP: 2ms)
*/  
void RgbStrip::decreaseTransitionPeriod(){
	long transitionPeriod = _timer.getTimerPeriod(_transitionEventID);
	setTransitionPeriod(transitionPeriod - TRANSITION_STEP);
}


/**
* Determine if transition events are enabled
* @return True if events are allowed to occur; otherwise false  
*/
bool RgbStrip::isTransitionsEnabled(){
	return _timer.isEnabled(_transitionEventID);
}


//Strobe
/**
* Callback for the strobe timer event
* The strobe event toggles the brightness between off and its initial value.  
*/
void RgbStrip::strobeEvent(){
	if (_brightness == _strobeBrightness){
		lightsOff();
		}else{
		setBrightness(_strobeBrightness);
	}
}


/**
* Static method wrapper for strobe timer events  
*/
void RgbStrip::strobeEvent_wrapper(){
	RgbStrip* thisInstance = (RgbStrip*) pt2Object;
	
	thisInstance->strobeEvent();
}


/**
* Enable strobe timer events to occur  
*/
void RgbStrip::enableStrobe(){
	_timer.enable(_strobeEventID);
	_strobeBrightness = _brightness;
}


/**
* Disable strobe timer events from occuring  
*/
void RgbStrip::disableStrobe(){
	_timer.disable(_strobeEventID);
}


/**
* Set the period between strobe timer events
* @param period Time between strobe events in ms.  
*/
void RgbStrip::setStrobePeriod(long period){
	if(period < MINIMUM_STROBE_PERIOD){
		period = MINIMUM_STROBE_PERIOD;
	}
	_timer.setTimerPeriod(_strobeEventID, period);
}


/**
* Increase the time between strobe timer events by a fixed amount
* By default, increases are in 5ms increments (STROBE_STEP)   
*/
void RgbStrip::increaseStrobePeriod(){
	long strobePeriod = _timer.getTimerPeriod(_strobeEventID);
	
	setStrobePeriod(strobePeriod + STROBE_STEP);
}


/**
* Decrease the time between strobe timer events by a fixed amount
* By default, decreases are in 5ms increments (STROBE_STEP)
*/
void RgbStrip::decreaseStrobePeriod(){
	long strobePeriod = _timer.getTimerPeriod(_strobeEventID);
	
	strobePeriod -= STROBE_STEP;
	if(strobePeriod < MINIMUM_STROBE_PERIOD){
		strobePeriod = MINIMUM_STROBE_PERIOD;
	}
	
	setStrobePeriod(strobePeriod);
}


/**
* Determine whether strobe events are enabled
* @return True if strobe events are enabled to occur; otherwise false.  
*/
bool RgbStrip::isStrobeEnabled(){
	return _timer.isEnabled(_strobeEventID);
}
