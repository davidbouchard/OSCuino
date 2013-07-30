import java.nio.ByteBuffer;

public class OscSerial {

  // SLIP Serial characters
  final int END     = 0300;
  final int ESC     = 0333;
  final int ESC_END = 0334;
  final int ESC_ESC = 0335;

  ArrayList serialBuffer;  
  int count = 0;

  Serial device;

  //--------------------------------------------------------------------------
  OscSerial(PApplet parent, Serial device) {
    this.device = device;
    serialBuffer = new ArrayList();
    parent.registerPre(this);
  }  

  void pre() {
    listen();  
  }

  //--------------------------------------------------------------------------
  // Listen to data on the Serial port and assemble packets 
  // There doesn't seem to be a way to hook into the serialEvent() callback directly
  // so this method will need to be placed inside serialEvent()
  void listen() {
    try {
      while (device.available () > 0) {
        int buffer = device.read();
        switch (buffer) {
        case END:
          if (count > 0) {
            count = 0;
            oscDataReady(); // <-- process the packet
            device.clear();
            serialBuffer.clear();
            return;
          }
          break;
        case ESC:
          buffer = device.read(); // <-- immediately read the next character
          switch (buffer) {
          case ESC_END:
            buffer = END;
            break;
          case ESC_ESC:
            buffer = ESC;
            break;
          }
        default:
          serialBuffer.add(buffer);
          count++;
        }
      }
    }

    catch(Exception e) {
      e.printStackTrace();
    }
  }

  //--------------------------------------------------------------------------
  void send(OscMessage msg) {
    sendSLIP(msg.getBytes());
  }

  //--------------------------------------------------------------------------
  // TODO: needs to handle Bundles Vs Message
  // right now, assumes we are always getting Messages
  
  void oscDataReady() {
    String address = "";
    int pos = 0;
    int b = (Integer)serialBuffer.get(pos++);
    ArrayList argumentTypes = new ArrayList();
    while (b!=0) {
      address += char(b);
      b = (Integer) serialBuffer.get(pos++);
    }
    // Skip address zeros and the comma for the parameters
    pos += 4 - ((address.length()) % 4);
    b = (Integer) serialBuffer.get(pos++);
    while (b != 0) {
      argumentTypes.add(char(b));
      b = (Integer) serialBuffer.get(pos++);
    }

    //println(address);

    //println(pos);
    // Skip parameter zeros
    pos--;
    pos += 4 - ((argumentTypes.size()+1) % 4);

    OscMessage oscMsg = new OscMessage(address);
    int data = -1;
    for (int i=0; i<argumentTypes.size(); i++) {
      char type = (Character)argumentTypes.get(i);
      switch (type) {
      case 'i':
        int[] intArr = new int[4];
        intArr[0] = (Integer) serialBuffer.get(pos++);
        intArr[1] = (Integer) serialBuffer.get(pos++);
        intArr[2] = (Integer) serialBuffer.get(pos++);
        intArr[3] = (Integer) serialBuffer.get(pos++);
        data = byteArrayToInt(intArr, 0);
        oscMsg.add(data);
        break;
      case 'f':
        byte[] byteArr = new byte[4];
        byteArr[0] = byte((Integer) serialBuffer.get(pos++));
        byteArr[1] = byte((Integer) serialBuffer.get(pos++));
        byteArr[2] = byte((Integer) serialBuffer.get(pos++));
        byteArr[3] = byte((Integer) serialBuffer.get(pos++));
        float f = arr2float(byteArr, 0);
        oscMsg.add(f);
        break;
      case 's':
        String str = "";
        char c = char((Integer)serialBuffer.get(pos++));
        while (c != 0x00) {
          str += c;
          c = char((Integer)serialBuffer.get(pos++));
        }

        int zeros = 4 - (str.length() % 4);
        //Skip zeros
        for (int j=0; j<zeros-1; j++) {
          pos++;
        }
        oscMsg.add(str);
        break;
      }
    }
    
    // Trigger an oscEvent function. 
    // in the Library version, this needs to be done like in ControlP5
    oscEvent(oscMsg);
  }

  //--------------------------------------------------------------------------
  int byteArrayToInt(int[] b, int offset) {
    int value = 0;
    for (int i = 0; i < 4; i++) {
      int shift = (4 - 1 - i) * 8;
      value += (b[i + offset] & 0x000000FF) << shift;
    }
    return value;
  }

  //--------------------------------------------------------------------------
  float arr2float (byte[] buf, int pos) {
    ByteBuffer bb = ByteBuffer.allocate(4);
    bb.put(buf, pos, 4);
    return bb.getFloat(0);
  }


  //--------------------------------------------------------------------------
  void sendSLIP(byte[] packet) {
    int len = packet.length;
    device.write(END);
    for (int i=0; i<packet.length; i++) {
      switch (packet[i]) {
        case (byte)END:
          device.write(ESC);
          device.write(ESC_END);
          break;
        case (byte)ESC:
          device.write(ESC);
          device.write(ESC_END);
          break;
      default:
        device.write(packet[i]);        
      }
    }
    device.write(END);
  }
}

