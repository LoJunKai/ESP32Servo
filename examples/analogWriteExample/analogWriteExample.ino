/*
 Mega analogWrite() test

 This sketch fades LEDs up and down one at a time on digital pins 2 through 13.
 This sketch was written for the Arduino Mega, and will not work on previous boards.

 The circuit:
 * LEDs attached from pins 2 through 13 to ground.

 created 8 Feb 2009
 by Tom Igoe

 This example code is in the public domain.

 */
// These constants won't change.  They're used to give names
// to the pins used:
const int lowestPin = 3;
const int highestPin = 32;
#include <ESP32Servo.h>
Servo myservo;

void setup() {
	Serial.begin(115200);
	// set pins 2 through 13 as outputs:
	for (int thisPin = lowestPin; thisPin <= highestPin; thisPin++) {
		if (ESP32PWM::hasPwm(thisPin))// Only enable PWM capible pins as analog write
			pinMode(thisPin, OUTPUT);
	}

}

void loop() {
	if (!myservo.attached()) {
		myservo.setPeriodHertz(50);// standard 50 hz servo
		myservo.attach(33, 1000, 2000);
	}
	myservo.write(0);
	// iterate over the pins:
	for (int thisPin = lowestPin; thisPin <= highestPin; thisPin++) {
		if (ESP32PWM::hasPwm(thisPin) &&  // Is it possible for this pin to PWM
				(ESP32PWM::channelsRemaining() > 0 || // New channels availible to allocate
						pwmFactory(thisPin) != NULL || // already allocated this pin in the factory
						thisPin == 25 || // one of the 2 DAC outputs, no timer needed
						thisPin == 26)) { // one of the 2 DAC outputs, no timer needed

			// fade the LED on thisPin from off to brightest:
			for (int brightness = 0; brightness < 255; brightness++) {
				analogWrite(thisPin, brightness);
				delay(1);
				myservo.write(brightness);
			}
			// fade the LED on thisPin from brithstest to off:
			for (int brightness = 255; brightness >= 0; brightness--) {
				analogWrite(thisPin, brightness);
				myservo.write(brightness);
				delay(1);
			}

		}
	}
	myservo.detach();// Turn the servo off for a while
	delay(2000);

}