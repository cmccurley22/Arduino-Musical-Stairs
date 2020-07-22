#include "WString.h"
#include <FatReader.h>
#include <avr/pgmspace.h>
#include "WaveUtil.h"
#include "WaveHC.h"


// create class for sensors
class UltrasonicSensor {
  //variables
  public:
    int trigPin;
    int echoPin;
    long duration, distance;
    bool steppedOn;
    UltrasonicSensor(int trig, int echo);
    long getDistance();
    void checkDistance();
    int stairDist;
};

// function declarations
void playFile(char* name);
void initWave();
bool isSteppedOn(UltrasonicSensor sensor);
int numSteppedOn(UltrasonicSensor sensors[]);

//global variables 
extern SdReader card; // holds info for SD card
extern FatVolume vol; // holds info for SD card partition
extern FatReader root; // holds info for SD card file system
extern FatReader f; // holds info for file being played
extern WaveHC wave; // wave object