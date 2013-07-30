#ifndef OSC_h 
#define OSC_h

#include "OSCMessage.h"
#include "SLIPEncodedSerial.h"
#include "Udp.h"
#include <HardwareSerial.h>

//=============================================================================
class OSC_Wrapper {
	
protected:	
	OSCMessage msgIN;

public:
	virtual void send(OSCMessage &) = 0;
	virtual void listen() = 0;
};

//=============================================================================
class OSC_Serial : public OSC_Wrapper {	

private:
	SLIPEncodedSerial *slip;

public:	
	OSC_Serial(HardwareSerial &);
	void send(OSCMessage &);
	void listen();
};

//=============================================================================
class OSC_UDP : public OSC_Wrapper {

private:
	UDP *udp;	

public:
	OSC_UDP(UDP &);		
	void send(OSCMessage &);
	void listen();
};

//=============================================================================
class OSC {
	
private:
	OSC_Wrapper *wrapper;
	
public:
	void begin(HardwareSerial &);
	void begin(UDP &);
	void listen();
	void send(OSCMessage &);
};

#endif