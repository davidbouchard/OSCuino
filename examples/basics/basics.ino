#include <OSC.h>

Osc osc;

void setup() {
  Serial.begin(9600);
  osc.begin(Serial); 
}

void loop() {
  delay(100);
  OscMessage msg("/test");
  msg.add(0); 
  osc.send(msg);   
}

void oscEvent(OscMessage &m) {
  // receiver 
  m.plug("/steve", fromSteve); 
}

void fromSteve(OscMessage &m) {
  int value = m.getInt(0); 
}

