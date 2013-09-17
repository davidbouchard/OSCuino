#ifndef OSC_h 
#define OSC_h

#include "OSCMessage.h"
#include "SLIPEncodedSerial.h"
#include "Udp.h"             //what is this ?
#include <HardwareSerial.h>

// added this for out address in UDP
#include <Ethernet.h>
#include <EthernetUdp.h>



//=============================================================================

class NetAddress {
    
private:
    
    
public:
    
    IPAddress destinationIP;
    int       destinationPort;
    
    NetAddress(void);
    void set (IPAddress, int);
    IPAddress getIP();
    int getPort();

    
};



//=============================================================================
class OscWrapper {
	
protected:	
	OscMessage msgIN;
   // NetAddress destination;
    
    // add the outgoing IP and PORT
    IPAddress outIP;
    int       outPort;

    

public:
	virtual void send(OscMessage &) = 0;
	virtual void listen() = 0;
    
    // add this virtual function 
    virtual void send(OscMessage &, NetAddress & ) = 0;
    virtual void send(OscMessage &, IPAddress outIP, int outPort) = 0;  // may not need
};

//=============================================================================
class OscSerial : public OscWrapper {	

private:
	SLIPEncodedSerial *slip;

public:	
	OscSerial(HardwareSerial &);
    
    void send(OscMessage &, IPAddress, int);
    void send(OscMessage &, NetAddress  &);
	void send(OscMessage &); // not used
	void listen();
};

//=============================================================================
class OscUDP : public OscWrapper {

private:
	EthernetUDP *udp; // *udp
    
    // add a struct for storing
    // NetAddress -- it may have to go in the wrapper.
    // IPAddress (ip) , int (port)
    
    //struct Location {
    //    IPAddress outIP;
   //     int  outPort;
   // } NetAddress;
    
public:
    
	OscUDP(EthernetUDP &);
    
    void send(OscMessage &, NetAddress  &);
    void send(OscMessage &, IPAddress, int);
	void send(OscMessage &); // not used
	void listen();
    
};

//=============================================================================


class Osc {
	
private:
	OscWrapper *wrapper;
    NetAddress *destination;

	
public:
	void begin(HardwareSerial &);
	void begin(EthernetUDP &);
    
    void send(OscMessage & );
    void send(OscMessage &, NetAddress  &);
	void send(OscMessage &, IPAddress, int );
    
    void listen();
};




#endif