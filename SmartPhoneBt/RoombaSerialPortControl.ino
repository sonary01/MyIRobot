#include <SoftwareSerial.h>
#define clamp(value, min, max) (value < min ? min : value > max ? max : value)

#define ON 1
#define OFF 0

//Motor Drive
# define MAX_SPEED 220 // 70% of top speed ==> 256
# define MIN_SPEED 70 

int motorSpeed = 100;

char state = 0;

int buttonPin = 12;
int ledPin=13;
int ddPin=5; //device detect

//bool debrisLED;
//bool spotLED;
//bool dockLED;
//bool warningLED;
//byte color;
//byte intensity;
SoftwareSerial Roomba(10, 11); //rx,tx

 
 
// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(ddPin,  OUTPUT);
  pinMode(13, OUTPUT);
  //Serial.begin(57600);
  //Roomba.begin(57600);

   Serial.begin(57600);
  Roomba.begin(57600);
    
    //Use Device Detect to wake Roomba
    wakeUp ();
    //Initialize Roomba SCI
    // Start SCI
    StartSafe ();

}
 
// the loop function runs over and over again forever
void loop() {
  
  
 if(Serial.available())  
  {  
   char c = Serial.read(); 
   if (c == '1')
    {
      driveWheels(0,0);
      Serial.print('1');
    }
    else if(c == '2')
    {
      Serial.print("2");
      driveWheels(100,100);
      delay(2000);
      driveStop();
       
    }
    else if(c == '3')
    {
      Serial.print("3");
      turnCCW(4,10);
    }
    
    else if(c == '4')
    {
       Serial.print("4");
       turnCW(4,10);
    }
  }
  
}//loop




void wakeUp (void)
{
  
  digitalWrite(ddPin, HIGH);
  delay(100);
  digitalWrite(ddPin, LOW);
  delay(500);
  digitalWrite(ddPin, HIGH);
  delay(2000);
} 

void StartSafe (void)

{

  Roomba.write(128);
  delay(100);
  // Enable control
  Roomba.write(130);  
  delay(100);
  // Enable full control, no safety, all commands
  Roomba.write(132);
  delay(100);
  
}


 
//void greenStatusOn() {
//  mySerial.write(139);       // control LEDs opcode
//  mySerial.write(32);        // Status On Green 00100000 Binary = 32 Decimal
//  mySerial.write((byte)0);   // Power Color Green
//  mySerial.write(255);       // Intensity set to full
//}
// 
//void redStatusOn() {
//  mySerial.write(139);        // control LEDs opcode
//  mySerial.write(16);         // Status On Green 00010000 Binary = 16 Decimal
//  mySerial.write(255);        // Power Color Red
//  mySerial.write(255);        // Intensity set to full
//}
void drive(int velocity, int radius)
{
  clamp(velocity, -500, 500); //def max and min velocity in mm/s
  clamp(radius, -2000, 2000); //def max and min radius in mm
  
  Roomba.write(137);
  Roomba.write(velocity >> 8);
  Roomba.write(velocity);
  Roomba.write(radius >> 8);
  Roomba.write(radius);
}

//---------------------------------------------------------------

void driveWheels(int right, int left)
{
  clamp(right, -500, 500);
  clamp(left, -500, 500);
  
  Roomba.write(145);
  Roomba.write(right >> 8);
  Roomba.write(right);
  Roomba.write(left >> 8);
  Roomba.write(left);
  }

//---------------------------------------------------------------
void driveWheelsPWM(int rightPWM, int leftPWM)
{
  clamp(rightPWM, -255, 255);
  clamp(leftPWM, -255, 255);
  
  Roomba.write(146);
  Roomba.write(rightPWM >> 8);
  Roomba.write(rightPWM);
  Roomba.write(leftPWM >> 8);
  Roomba.write(leftPWM);
}

//---------------------------------------------------------------
void turnCW(unsigned short velocity, unsigned short degrees)
{
  drive(velocity, -1);
  clamp(velocity, 0, 500);
  delay(6600);
  //delay((1580 + 2.25*velocity)/velocity*degrees);
  //delay((-0.03159720835 * velocity + 21.215270835) * degrees);
  drive(0,0);
}

//---------------------------------------------------------------
void turnCCW(unsigned short velocity, unsigned short degrees)
{
  drive(velocity, 1); 
  clamp(velocity, 0, 500);
  delay(6600);
  //delay(2708.3333/velocity*degrees);
  //delay((1580 + 2.25*velocity)/velocity*degrees);
  //delay((-0.03159720835 * velocity + 21.215270835) * degrees);
  drive(0,0);
}

//---------------------------------------------------------------
void driveStop(void)
{
  drive(0,0);
}

//---------------------------------------------------------------
void driveLeft(int left)
{
  driveWheels(left, 0);
}

//---------------------------------------------------------------
void driveRight(int right)
{
  driveWheels(0, right);
}
