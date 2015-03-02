#include <Servo.h>
Servo servoLeft;
Servo servoRight;
void setup()
{
  pinMode(7, INPUT);                         // Set right whisker pin to input
  pinMode(5, INPUT); 
  servoLeft.attach(13);
  servoRight.attach(12);
  //spinDegrees(180);
  //lTurn();
  //rTurn();
  //delay(1000);
  //spinDegrees(180);
  //rTurn();
  //lTurn();
  //manuver(100,-100,2460);
  //spin(3);
  manuver(-115,-100,4000);
  r_turn();
  manuver(-115,-100,4000);
  r_turn();
  manuver(-115,-100,4000);
  r_turn();
  manuver(-115,-100,4000);
  manuver(0,0,-1);

}
void loop(){
}
void forward(int time)                       // Forward function
{
  servoLeft.writeMicroseconds(1700);         // Left wheel counterclockwise
  servoRight.writeMicroseconds(1300);        // Right wheel clockwise
  delay(time);                               // Maneuver for time ms
}

void turnLeft(int time)                      // Left turn functurntion
{
  servoLeft.writeMicroseconds(1300);         // Left wheel clockwise
  servoRight.writeMicroseconds(1300);        // Right wheel clockwise
  delay(time);                               // Maneuver for time ms
}

void turnRight(int time)                     // Right turn function
{
  servoLeft.writeMicroseconds(1700);         // Left wheel counterclockwise
  servoRight.writeMicroseconds(1700);        // Right wheel counterclockwise
  delay(time);                               // Maneuver for time ms
}

void backward(int time)                      // Backward function
{
  servoLeft.writeMicroseconds(1300);         // Left wheel clockwise
  servoRight.writeMicroseconds(1700);        // Right wheel counterclockwise
  delay(time);                               // Maneuver for time ms
}
void r_turn(){
  manuver(35,-35,1370);
}
void l_turn(){
  manuver(-35,35,1370);
}
void spin(int times){
  for(int i=0;i<times;i++){
    manuver(100,-100,2460);
  }
  manuver(0,0,1);
}
void spin_degrees(int deg){
  manuver(35,-35,((1450*4)/360)*deg);
}

void manuver(int speedLeft,int speedRight,int msTime){
  // speedLeft, speedRight ranges: Backward  Linear  Stop  Linear   Forward
  //                               -200      -100......0......100       200
  servoLeft.writeMicroseconds(1500 + speedLeft);   // Set Left servo speed
  servoRight.writeMicroseconds(1500 - speedRight); // Set right servo speed
  if(msTime==-1)                                   // if msTime = -1
  {                                  
    servoLeft.detach();                            // Stop servo signals
    servoRight.detach();   
  }
  delay(msTime);              
}

