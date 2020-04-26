#include "arduino.h"
//#include "HardwareSerial.h"
#define HAL_UART_MODULE_ENABLED
int ADValue;
HardwareSerial Serial2(PA_10, PA_9);
void setup()
{
	//
	Serial2.begin(115200);
	//Serial1.println("Hello World");
	pinMode(PB_9,OUTPUT);
	pinMode(PA_0,INPUT_ANALOG);
}

void loop()
{
   ADValue=analogRead(PA_0);
	 Serial2.print("ADValue=");
	 Serial2.println(ADValue);
	 digitalWrite(PB_9,HIGH);
	 delay(10);
	 digitalWrite(PB_9,LOW);
	 delay(10);
}