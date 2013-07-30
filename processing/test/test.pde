/**
 * OSC Slip Serial parser
 *
 * TODO: implement the other functionality from OscP5 class 
 * (mainly the plug() feature)
 * TODO: make into a standalone JAR file 
 */

import processing.serial.*;
import oscP5.*;
import netP5.*;

Serial device;
OscSerial osc;

void setup() { 
  device = new Serial(this, Serial.list()[0], 9600);  
  osc = new OscSerial(this, device);
}

void draw() {
  background(0);
}

void oscEvent(OscMessage theOscMessage) {
  println(theOscMessage);
}


void serialEvent(Serial myPort) {
  // used to troubleshoot with the arduino.. just print 
  // what is received 
  print((char)myPort.read());
}


void keyPressed() {
  if (key == '0') {
    OscMessage m = new OscMessage("/led");
    m.add(0);
    osc.send(m);
  }
  if (key == '1') {
    OscMessage m = new OscMessage("/led");
    m.add(1);
    osc.send(m);
  }
  if (key == 'm') {
    OscMessage m = new OscMessage("/motor");
    m.add(67);
    osc.send(m);
  }
}





