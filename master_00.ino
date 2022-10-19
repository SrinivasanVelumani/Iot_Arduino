/*master server*/
#include <RF24.h>
#include <RF24Network.h>
#include <SPI.h>

RF24 radio(10, 9);               // nRF24L01 (CE,CSN)10,9
RF24Network network(radio);      // Include the radio in the network
const uint16_t this_node = 00;   // Address of this node in Octal format ( 04,031, etc)
const uint16_t node01 = 01;      

void setup() {
  SPI.begin();
  radio.begin();
  network.begin(90, this_node);  //(channel, node address)
  radio.setDataRate(RF24_2MBPS);
  Serial.begin(9600);
}

void loop()
{
  network.update();

   while ( network.available() ) {     // Is there any incoming data?
    RF24NetworkHeader header;
    unsigned long incomingData;
    network.read(header, &incomingData, sizeof(incomingData)); // Read the incoming data
    
    Serial.print("Data from client node ");
    Serial.print(header.from_node);
    Serial.print(": ");
    Serial.print(incomingData);
    Serial.println("");
  }
  
  unsigned long potValue = analogRead(A0);  // Read the potentiometer value
  unsigned long angleValue[2];
  angleValue[0] = 01;
  angleValue[1] = map(potValue, 0, 1023, 0, 180); // Convert the value to 0-180
  RF24NetworkHeader header(node01);     // (Address where the data is going)
  bool ok = network.write(header, &angleValue, sizeof(angleValue)); // Send the data
  //Serial.println(angleValue);
  //Serial.println(ok);
  //delay(500);
}
