//receiver with push and pot and foue joystick 
//connect to arduino uno

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

#define SIGNAL_TIMEOUT 500  // This is signal timeout in milli seconds. We will reset the data if no signal

const uint64_t pipeIn = 0xF9E8F0F0E1LL;
RF24 radio(8, 9); 
unsigned long lastRecvTime = 0;

struct PacketData
{
  byte lxAxisValue;
  byte lyAxisValue;
  byte rxAxisValue;
  byte ryAxisValue;
  byte wxAxisValue;
  byte wyAxisValue;
  byte txAxisValue;
  byte tyAxisValue;

  byte lPotValue;  
  byte rPotValue;    
  byte switch1Value;
  byte switch2Value;
  byte switch3Value; 
};
PacketData receiverData;

Servo servo1;     //Pin D2
Servo servo2;     //Pin D3
Servo servo3;     //Pin D4
Servo servo4;     //Pin D5
Servo servo5;     //Pin D6
Servo servo6;     //Pin D7
Servo servo7;     //Pin D8
Servo servo8;     //Pin D9

Servo servo9;     //Pin A4
Servo servo10;     //Pin A5

int   led1 = A4;
int   led2 = A5;
int   led3 = 10;

//Assign default input received values
void setInputDefaultValues()
{
  // The middle position for joystick. (254/2=127)
  receiverData.lxAxisValue = 127;
  receiverData.lyAxisValue = 127;
  receiverData.rxAxisValue = 127;
  receiverData.ryAxisValue = 127;   
  receiverData.wxAxisValue = 127;
  receiverData.wyAxisValue = 127;
  receiverData.txAxisValue = 127;
  receiverData.tyAxisValue = 127;

  receiverData.lPotValue = 0;  
  receiverData.rPotValue = 0;    
  receiverData.switch1Value = LOW;
  receiverData.switch2Value = LOW;
  receiverData.switch3Value = LOW;
}

void mapAndWriteValues()
{
  servo1.write(map(receiverData.lxAxisValue, 0, 254, 0, 180));
  servo2.write(map(receiverData.lyAxisValue, 0, 254, 0, 180));
  servo3.write(map(receiverData.rxAxisValue, 0, 254, 0, 180));
  servo4.write(map(receiverData.ryAxisValue, 0, 254, 0, 180));
  servo5.write(map(receiverData.wxAxisValue, 0, 254, 0, 180));
  servo6.write(map(receiverData.wyAxisValue, 0, 254, 0, 180));
  servo7.write(map(receiverData.txAxisValue, 0, 254, 0, 180));
  servo8.write(map(receiverData.tyAxisValue, 0, 254, 0, 180));

  servo9.write(map(receiverData.lPotValue, 0, 254, 0, 180));
  servo10.write(map(receiverData.rPotValue, 0, 254, 0, 180));
  
  digitalWrite(led1, receiverData.switch1Value);
  digitalWrite(led2, receiverData.switch2Value);
  digitalWrite(led3, receiverData.switch3Value);
}

void setup()
{
  radio.begin();
  radio.setDataRate(RF24_250KBPS);
  radio.openReadingPipe(1,pipeIn);
  radio.startListening(); //start the radio receiver 

  servo1.attach(A0);
  servo2.attach(A1);
  servo3.attach(A2);
  servo4.attach(A3);
  servo5.attach(9);
  servo6.attach(6);
  servo7.attach(3);
  servo8.attach(2);
  servo9.attach(5);
  servo10.attach(4);

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);   
 
  setInputDefaultValues();
  mapAndWriteValues();
}

void loop()
{
  // Check if RF is connected and packet is available 
  if(radio.isChipConnected() && radio.available())
  {
    radio.read(&receiverData, sizeof(PacketData)); 
    lastRecvTime = millis(); 
  }
  else
  {
    //Check Signal lost.
    unsigned long now = millis();
    if ( now - lastRecvTime > SIGNAL_TIMEOUT ) 
    {
      setInputDefaultValues();
    }
  }
  mapAndWriteValues();         
}
