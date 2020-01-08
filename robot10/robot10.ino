#include <Timer1.h>

/* 
 * rosserial Subscriber Example
 * Blinks an LED on callback
 */

#include <ros.h>
#include <geometry_msgs/Twist.h>
#include <SoftwareSerial.h>


// USB-config
ros::NodeHandle  nh;

// Start Bluetooth-config
// Comment below if needed to switch to USB-connection
// If uncommented, comment USB-config

//class NewHardware : public ArduinoHardware {
//  public: NewHardware():ArduinoHardware(&Serial1, 57600){};
//}; 
//ros::NodeHandle_<NewHardware> nh;

// End Bluetooth-config


int forwardLeft = 6;
int forwardRight = 2;
int reverseLeft = 7;
int reverseRight = 3;

double time;
int enableLeft = 24;
int enableRight = 25;

int triggerPin = 23;
int echoPin = 22;

void messageCb( const geometry_msgs::Twist& msg){
  if(msg.linear.x > 0) {
    analogWrite(reverseLeft, 0);
    analogWrite(reverseRight, 0);
    analogWrite(forwardLeft, 255);
    analogWrite(forwardRight, 255);
  } else if(msg.linear.x < 0) {
    analogWrite(forwardLeft, 0);
    analogWrite(forwardRight, 0);
    analogWrite(reverseLeft, 255);
    analogWrite(reverseRight, 255);
  }  else if(msg.angular.z > 0){
    analogWrite(forwardLeft, 62);
    analogWrite(forwardRight, 255);
    analogWrite(reverseLeft, 0);
    analogWrite(reverseRight, 0);
  } else if(msg.angular.z < 0){
    analogWrite(forwardLeft, 255);
    analogWrite(forwardRight, 62);
    analogWrite(reverseLeft, 0);
    analogWrite(reverseRight, 0);
  } else if(msg.linear.x == 0 && msg.angular.z == 0){
    analogWrite(forwardLeft, 0);
    analogWrite(forwardRight, 0);
    analogWrite(reverseLeft, 0);
    analogWrite(reverseRight, 0);
  }
}

ros::Subscriber<geometry_msgs::Twist> sub("cmd_vel", &messageCb );

void setup()
{ 
  pinMode(forwardLeft, OUTPUT);
  pinMode(forwardRight, OUTPUT);
  pinMode(reverseLeft, OUTPUT);
  pinMode(reverseRight, OUTPUT);
  pinMode(enableLeft, OUTPUT);
  pinMode(enableRight, OUTPUT);
  pinMode(13, OUTPUT);
  
  digitalWrite(enableLeft, HIGH);
  digitalWrite(enableRight, HIGH);
  
  nh.initNode();
  nh.subscribe(sub);
  
  startTimer1(1000L); //Prepare timer1 to check for messages every 1ms
  time = millis();
  digitalWrite(enableRight, HIGH);
  digitalWrite(enableLeft, HIGH);

}

void loop()
{    
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
// Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
// Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
// Calculating the distance
  distance= duration*0.034/2;
  
  if (distance >= 5) {
    digitalWrite(enableLeft, LOW);
    digitalWrite(enableRight, LOW);
  }
    
  double currentTime = millis();
  //if nothing happened for 1 second.
  if (currentTime - time > 1000) {
    digitalWrite(enableLeft, LOW);
    digitalWrite(enableRight, LOW);
  }
  
}

ISR(timer1Event)
{
  resetTimer1(); //reset timer1
  time = millis();
  np.spinOnce(); //look for twist messages and call callback
}

