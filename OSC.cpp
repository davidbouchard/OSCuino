#include <OSC.h>

void oscEvent(OscMessage &);

//=============================================================================
// OSC_Serial Wrapper
//=============================================================================

OscSerial::OscSerial(HardwareSerial &s) {
	slip = new SLIPEncodedSerial(s);	 
}

// Non-blocking version
void OscSerial::listen() {
	int size;
	if( (size = slip->available()) > 0) {
		while(size--) msgIN.fill(slip->read());
	}  
	if (!slip->endofPacket()) return;
	
	if (!msgIN.hasError()) {
		oscEvent(msgIN);
	}	
	
	msgIN.reset();
}

void OscSerial::send(OscMessage &msg) {
	msg.send(*slip);
	slip->endPacket();
	msg.empty();
}


/*
// Blocking Version -- do not use, left there for reference
void OscSerial::listen() {	
	int size;
	while(!slip->endofPacket()) {
		if( (size = slip->available()) > 0) {
		   while(size--) msgIN.fill(slip->read());
		}  
	}
		
	if(!msgIN.hasError()) {
		oscEvent(msgIN);
	}

	// get the OSC message ready for the next one
    msgIN.reset();
}
*/


//=============================================================================
// OSC_UDP Wrapper
//=============================================================================

void Osc::OscUDP(UDP &u) {
	// save a handle to the UDP object
	udp = &u;
}

void Osc::OscUDP listen() {
	// TOOD: code to build an OSC packet object from UDP reads
}

void Osc::OscUDP send(OscMessage &msg) { 
	// TODO 
}


//=============================================================================
// Base OSC interface 
//=============================================================================

void Osc::begin(HardwareSerial &s) {
	wrapper = new OscSerial(s);
}

void Osc::begin(UDP &u) {
	wrapper = new OscUDP(u);
}

void Osc::send(OscMessage &msg) {
	wrapper->send(msg);
}


void Osc::listen() {
	wrapper->listen();
}
