/**
 * Example of ABP device
 * Authors: 
 *        Ivan Moreno
 *        Eduardo Contreras
 *  June 2019
 * 
 * This code is beerware; if you see me (or any other collaborator 
 * member) at the local, and you've found our code helpful, 
 * please buy us a round!
 * Distributed as-is; no warranty is given.
 */
#include <lorawan.h> // Library Beelan http://librarymanager/All#Beelan

const char *devEui = "0000000000000000";
const char *appEui = "0000000000000000";
const char *appKey = "00000000000000000000000000000000";

const unsigned long interval = 120000; // 10 s interval to send message
unsigned long previousMillis = 0;     // will store last time message sent
unsigned int counter = 0;             // message counter

char myStr[50];
char outStr[255];
byte recvStatus = 0;

const sRFM_pins RFM_pins = {
    .CS = SS,
    .RST = RFM_RST,
    .DIO0 = RFM_DIO0,
    .DIO1 = RFM_DIO1,
    .DIO2 = RFM_DIO2,
    .DIO5 = RFM_DIO5,
};

void setup()
{
  // Setup loraid access
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(2000);                       // wait for a second

  Serial.begin(9600);
  while (!Serial){
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW); 
  }

   digitalWrite(LED_BUILTIN, LOW);

  if (!lora.init())
  {
    Serial.println("RFM95 not detected");
    delay(5000);
    return;
  }

  pinMode(RFM_TCX_ON, OUTPUT);
  pinMode(RFM_SWITCH, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  // Set LoRaWAN Class change CLASS_A or CLASS_C
  lora.setDeviceClass(CLASS_A);

  // Set Data Rate
  lora.setDataRate(SF7BW125);

  // set channel to random
  lora.setChannel(MULTI);

  // Put OTAA Key and DevAddress here
  lora.setDevEUI(devEui);
  lora.setAppEUI(appEui);
  lora.setAppKey(appKey);
  bool isJoined;
  do
  {
    Serial.println("Joining...");
    isJoined = lora.join();

    //wait for 10s to try again
    delay(10000);
  } while (!isJoined);
  Serial.println("Joined to network");
}

void loop()
{
  // Check interval overflow
  if (millis() - previousMillis > interval)
  {
    previousMillis = millis();

    sprintf(myStr, "Counter-%d", counter);

    Serial.print("Sending: ");
    Serial.println(myStr);

    lora.sendUplink(myStr, strlen(myStr), 0, 1);
    counter++;
  }

  recvStatus = lora.readData(outStr);
  if (recvStatus)
  {
    Serial.println(outStr);
  }

  // Check Lora RX
  lora.update();
}