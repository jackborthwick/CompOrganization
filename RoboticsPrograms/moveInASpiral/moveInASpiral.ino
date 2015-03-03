#include <Servo.h>                           // Include servo library
 
Servo servoLeft;                             // Declare left and right servos
Servo servoRight;
 
void setup()                                 // Built-in initialization block
{
  delay(1000);                               // Delay to finish tone

  servoLeft.attach(13);                      // Attach left signal to pin 13
  servoRight.attach(12);                     // Attach right signal to pin 12
 
  for(int speed = 0; speed <= 400; speed += 1) // Ramp up to full speed.
  {
    servoLeft.writeMicroseconds(1500+speed*2);   // us = 1500,1502,...1598,1600
    servoRight.writeMicroseconds(1500-speed*(.25));  // us = 1500,1498,...1402,1400
    delay(200);                                 // 20 ms at each speed
  }
   
                                 // Full speed for 1.5 seconds
  servoLeft.detach();                          // Stop sending servo signals
  servoRight.detach();
}

void loop()                                    // Main loop auto-repeats
{                                              // Empty, nothing to repeat
}
