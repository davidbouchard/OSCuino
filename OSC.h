#ifndef OSC_h 
#define OSC_h

#include "OSCMessage.h"
#include "SLIPEncodedSerial.h"
#include "Udp.h"
#include <HardwareSerial.h>

//=============================================================================
class OscWrapper {
	
protected:	
	OscMessage msgIN;

public:
	virtual void send(OscMessage &) = 0;
	virtual void listen() = 0;
};

//=============================================================================
class OscSerial : public OscWrapper {	

private:
	SLIPEncodedSerial *slip;

public:	
	OscSerial(HardwareSerial &);
	void send(OscMessage &);
	void listen();
};

//=============================================================================
class OscUDP : public OscWrapper {

private:
	UDP *udp;	

public:
	OscUDP(UDP &);		
	void send(OscMessage &);
	void listen();
};

//=============================================================================
class Osc {
	
private:
	OscWrapper *wrapper;
	
public:
	void begin(HardwareSerial &);
	void begin(UDP &);
	void listen();
	void send(OscMessage &);
};

#endif