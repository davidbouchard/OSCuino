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

void OscSerial::send(OscMessage &msg) {
	msg.send(*slip);
	slip->endPacket();
	msg.empty();
}

void OscSerial::send(OscMessage &msg, IPAddress _ip, int _port){
   Serial.println(" Not available for serial connections :: remove IP and PORT") ;
}

void OscSerial::send(OscMessage &msg, NetAddress &na){
    Serial.println(" Not available for serial connections :: remove IP and PORT") ;
}

//=============================================================================
// OSC_UDP Wrapper
//=============================================================================

// TODO: write similar wrapper class using the UDP interface 
// waiting to solve the blocking kink on the Serial one first

OscUDP::OscUDP(EthernetUDP &u){
    
    udp = &u;  // &u ? u
    
}

void OscUDP::send(OscMessage &msg) {
    Serial.println("Not available for UDP connections ::  IP and PORT required") ;
}

void OscUDP::send(OscMessage &msg, NetAddress &na){
    OscUDP::send (msg, na.getIP(), na.getPort() );
}

void OscUDP::send(OscMessage &msg, IPAddress outIp, int outPort) {

    // SEND BASED ON THIS :
    // http://cnmat.berkeley.edu/library/oscuino/omessage
    
    // we need to do some magic here
    udp->beginPacket(outIp, outPort);
    msg.send(*udp); // send the bytes to the SLIP stream
    udp->endPacket(); // mark the end of the OSC Packet
    msg.empty(); // free space occupied by message
    
}

void OscUDP::listen() {
    
    // need a non-blocking method to read bytes from the UDP stream
    // parsePacket us analogous to available
    int UDPpacketSize;
  
	if( (UDPpacketSize = udp->parsePacket())  > 0) {
		while(UDPpacketSize--) msgIN.fill(udp->read());
	} else {
        return; // i am not sure that works but lets see
                //  i think it says if packet is <= 0 return
    }
    // this is slip serial specific
	//if (!Udp->endofPacket()) return;
    
    
	
	if (!msgIN.hasError()) {
		oscEvent(msgIN);
	}
	
	msgIN.reset();
    

}



//=============================================================================
// Base OSC interface 
//=============================================================================

void Osc::begin(HardwareSerial &s) {
	wrapper = new OscSerial(s);
}

void Osc::begin(EthernetUDP &u) {
    wrapper = new OscUDP(u);
  
}

void Osc::send(OscMessage &msg) {
	wrapper->send(msg);
}


void Osc::send(OscMessage &msg, NetAddress &addr) {
    wrapper->send(msg, addr);
}


void Osc::send(OscMessage &msg, IPAddress _ip, int _port) {
	wrapper->send(msg, _ip, _port);
}


void Osc::listen() {
	wrapper->listen();
}

//=============================================================================
// NetAddress interface
//=============================================================================


NetAddress  :: NetAddress   (){;}  // sets up object

void NetAddress::set(IPAddress _ip, int _port) {
    destinationIP = _ip;
    destinationPort = _port;
}

IPAddress NetAddress :: getIP (){
    return destinationIP;
}

int NetAddress :: getPort(){
    return destinationPort;
}

