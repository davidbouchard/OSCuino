import oscP5.*;
import processing.serial.*;

Serial port;
OscSerial osc;


void setup() {
  println(Serial.list());
  port = new Serial(this, Serial.list()[0], 9600);
  osc = new OscSerial(this, port);
  
  osc.plug(this,"plugTest", "/sensor");
}

void draw() {
}

void mousePressed() {
  OscMessage msg = new OscMessage("/hi");
  msg.add(frameCount);
  osc.send(msg);  
}

void plugTest(int value) {
  println("Plugged from /sensor: " + value);
}

void oscEvent(OscMessage theMessage) {
  println("Message: " + theMessage);  
}
