/*
 slave Node 01 - Servo motor ==
*/
#include <RF24.h>
#include <RF24Network.h>
#include <SPI.h>

RF24 radio(10, 9);               // nRF24L01 (CE,CSN)10,9
RF24Network network(radio);      // Include the radio in the network
const uint16_t this_node = 01;   // Address of our node in Octal format ( 04,031, etc)
const uint16_t master00 = 00;   // Address of master node to send data in Octal format ( 04,031, etc)

const unsigned long interval = 10;  //ms  // How often to send data to the other unit
unsigned long last_sent;            // When did we last send?

void setup() {
  SPI.begin();
  radio.begin();
  network.begin(90, this_node); //(channel, node address)
  radio.setDataRate(RF24_2MBPS);
  Serial.begin(9600);
}

void loop() {
  network.update();
  while ( network.available() ) {     // Is there any incoming data?
    RF24NetworkHeader header;
    unsigned long incomingData[2];
    network.read(header, &incomingData, sizeof(incomingData)); // Read the incoming data

    Serial.print("data from master node ");
    Serial.print(header.from_node);
    Serial.print(": ");
    Serial.print(incomingData[1]);
    Serial.println("");
  }

   unsigned long now = millis();
  if (now - last_sent >= interval) {   // If it's time to send a data, send it!
    last_sent = now;
    unsigned long potValue = analogRead(A0);
    unsigned long angle = map(potValue, 0, 1023, 0, 255);
    RF24NetworkHeader header(master00);   // (Address where the data is going)
    bool ok = network.write(header, &angle, sizeof(angle)); // Send the data
  }
}
