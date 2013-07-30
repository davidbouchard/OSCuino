#include <OSC.h>

//=============================================================================
// OSC_Serial Wrapper
//=============================================================================

OSC_Serial::OSC_Serial(HardwareSerial &s) {
	slip = new SLIPEncodedSerial(s);	 
}

void OSC_Serial::listen() {

	int size;
	while ((size = slip->available()) > 0) {
		char c = slip->read();
		slip->print(c);
		msgIN.fill(c);
		if (slip->endofPacket()) {
			if (!msgIN.hasError()) {
				//oscEvent(msgIN);
				char buf[20];
   				msgIN.getAddress(buf, 0);
    			slip->println(buf);
				msgIN.reset();
			}
			else {
				//slip->println("MSG ERROR");	
			}
		}		
	}	
	
	/*
	// THIS WORKS 
	int size;
	while(!slip->endofPacket()) {
		if( (size = slip->available()) > 0) {
		   while(size--) msgIN.fill(slip->read());
		}  
	}
	
	if(!msgIN.hasError()) {
		slip->println("MSG IN");
		
		char buf[20];
   		msgIN.getAddress(buf, 0);
    	slip->println(buf);

	}

	else {
		slip->println("MSG ERROR");
	}
	
	// get the OSC message ready for the next one
    msgIN.reset();
    */
}

void OSC_Serial::send(OSCMessage &msg) {
	msg.send(*slip);
	slip->endPacket();
	msg.empty();
}

//=============================================================================
// OSC_UDP Wrapper
//=============================================================================



//=============================================================================
// Base OSC interface 
//=============================================================================

void OSC::begin(HardwareSerial &s) {
	wrapper = new OSC_Serial(s);
}

void OSC::send(OSCMessage &msg) {
	wrapper->send(msg);
}


void OSC::listen() {
	wrapper->listen();
}
