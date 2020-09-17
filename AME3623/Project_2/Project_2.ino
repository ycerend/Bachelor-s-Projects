/*Project 2
  2/15/2018
  Group 7
  Hannah Phillips, Travis Barnett and Yagmur Dardagan

  Coder: Yagmur Dardagan
*/
//includes "project.h" to use variable DistanceSensor
#include "project.h"



// Define analog pin mapping for yellow and red
const int analogRightYellow = 37;
const int analogLeftRed = 38;

//defining variable for sensor
DistanceSensor sensor;

//to read values of sensor
float valueForYellow;
float valueForRed;

void setup() {
  // Initialize the serial port connected to the PC's USB port
  Serial.begin(57600);
  //initializing the variable for sensors
  sensor = DISTANCE_LEFT;
}
/*The method read_distance takes the input, side,  for the sensors.
   Then it checks which sensor is being read, checks the value, and returns the distance in cm.
  If neıther DISTANCE_LEFT or DISTANCE_RIGHT ıs evaluated as true zero ıs returned.
  Input= DistanceSensor side
  Output= float in cm
*/

float read_distance(DistanceSensor side) {
  //if the DistanceSensor side is DISTANCE_LEFT, run this code.
  if ( side == DISTANCE_LEFT) {
    //reads the distance from sensor
    valueForRed = analogRead(analogLeftRed);
    //if the distance is greater than 80 cm , run this code.
    if (valueForRed < 178) {
      //If the distance is greater than 80 cm, the value of the sensor must be equal to 80

      return 80;
    }
    //calculates the distance in cm
    float distForLeftRed = 5867.2 / (valueForRed - 105.02);
    //returns the distance in cm
    return distForLeftRed;
  }

  //if the DistanceSensor side is DISTANCE_RIGHT, run this code.
  if ( side == DISTANCE_RIGHT) {
    //reads the distance from sensor
    valueForYellow = analogRead(analogRightYellow);
    //if the distance is greater than 80 cm , run this code.
    if (valueForYellow < 137) {
      //If the distance is greater than 80 cm, the value of the sensor must be equal to 80
      return 80;
    }
    //calculates the distance in cm
    float distForRightYellow = 5847.2 / (valueForYellow - 64.425);

    //returns the distance in cm
    return distForRightYellow;

  }
  //If neıther DISTANCE_LEFT or DISTANCE_RIGHT ıs evaluated as true zero ıs returned.
  return 0;

}

void loop()
{

  //Switching sensor to distance left
  sensor = DISTANCE_LEFT;

  //Callıng the read_dıstance Function and storing the returned distance
  float RealDistanceForYellow = read_distance(sensor);

  //Switching sensor to distance right
  sensor = DISTANCE_RIGHT;

  //Callıng the read_dıstance Function and storing the returned distance
  float RealDistanceForRed = read_distance(sensor);

  //prints the sensor and its distance in cm for left and right
  Serial.print("\tRight Yellow:\t");
  Serial.print(RealDistanceForYellow);
  Serial.print("\tLeft Red:\t");
  Serial.println(RealDistanceForRed);

  //delay for 500 ms
  delay(500);

}
