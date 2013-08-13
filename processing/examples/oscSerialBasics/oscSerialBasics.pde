import oscP5.*;
import netP5.Logger;
import processing.serial.*;

Serial port;
OscSerial osc;


void setup() {
  println(Serial.list());
  port = new Serial(this, Serial.list()[4], 9600);
  osc = new OscSerial(this, port);
 
  osc.plug(this,"plugTest", "/test");  
}

void draw() {
}
 
void mousePressed() {
  OscMessage msg = new OscMessage("/hi");
  msg.add(frameCount);
  osc.send(msg);  
}

void plugTest(int value) {
  println("Plugged from /test: " + value);
}

void oscEvent(OscMessage theMessage) {
  println("Message: " + theMessage + ", " + theMessage.isPlugged());  
}
