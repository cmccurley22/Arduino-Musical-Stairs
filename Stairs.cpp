#include "TheStairs.h"
#include "Arduino.h"
#include "String.h"
#include <FatReader.h>
#include <avr/pgmspace.h>
#include "WaveUtil.h"
#include "WaveHC.h"

// constructor 
UltrasonicSensor::UltrasonicSensor(int trig, int echo)
{
	trigPin = trig;
	echoPin = echo;
	//set pin modes
	pinMode(trigPin, OUTPUT);
	pinMode(echoPin, INPUT);
	//set range of stairs
	stairDist = 5;
}

// get the distance from a sensor to the nearest object
long UltrasonicSensor::getDistance()
{
	// send ping
	digitalWrite(trigPin, LOW); // trigger off
	delayMicroseconds(10); 
  	digitalWrite(trigPin, HIGH); // trigger on
  	delayMicroseconds(15);
	digitalWrite(trigPin, LOW); // trigger off
	
	// receive ping, record duration of ping
	duration = pulseIn(echoPin, HIGH);
	
	// calculate distance
	long distance = duration/148;

   	return distance;
}

//update ultrasonic sensor data
void UltrasonicSensor::checkDistance()
{
	distance = getDistance();
	Serial.print("sensor distance: "); Serial.println(distance);
	if (distance < stairDist)
	{
		Serial.println("sensor distance is less than stair distance");
		steppedOn = true;
	}
	else {
		steppedOn = false;
	}
}

// check if a specific sensor is stepped on
bool isSteppedOn(UltrasonicSensor sensor)
{
	if (sensor.distance < sensor.stairDist) // is there something closer than the stair
	{
		return true;
	}
	else
	{
		return false;
	}
}

// check how many sensors are stepped on
int numSteppedOn(UltrasonicSensor sensors[])
{
	int sensorsOn = 0;
	for (int i = 0; i < sizeof(sensors); i++)
	{
		if (sensors[i].steppedOn)
		{
			sensorsOn++;
		}
	}
	return sensorsOn;
}

// play a file from the wave shield
void playFile(char* name)
{
  // check if a file is playing, stop it if it is
  if (wave.isplaying)
  {
    wave.stop();
  }
  // find file
  if (!f.open(root, name))
  {
    Serial.println("Couldn't open file "); Serial.print(name);
    return;
  }
  if (!wave.create(f))
  {
    Serial.println("Not a valid WAV");
    return;
  }
 
  // play file
  Serial.print("Playing file "); Serial.println(name);
  wave.play();
}

// initialize the wave shield and SD card, print errors
void initWave()
{
  // prints amount of free RAM -- for debugging
  Serial.println("Free RAM: ");
  Serial.println(FreeRam());

  // try to initialize SD card
  if (!card.init())
  {
    Serial.println("Card init. failed!");
  }

  // try to find FAT partition
  uint8_t part;
  for (part = 0; part < 5; part++)
  {
    if (vol.init(card, part)) 
      break;
  }
  if (part == 5)
  {
    Serial.println("No valid FAT partition!");
  }

  // try to open root directory
  if (!root.openRoot(vol)) {
    Serial.println("Can't open root dir!");
  }
}