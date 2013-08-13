#include <OSC.h>

OSC osc;

void setup() {
  Serial.begin(9600);
  osc.begin(Serial); 
}

void loop() {
  delay(100);

  OSCMessage msg("/test");
  msg.add(0); 
  osc.send(msg);   
}

void oscEvent(OSCMessage &m) {
  // receiver 
}

