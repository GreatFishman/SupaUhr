#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

class NTPClient {
  public:
    NTPClient();

    unsigned long getTime();

  private:    
    void sendNTPPacket(IPAddress& address);

  private:
    IPAddress timeServerIP;
    const char* ntpServerName = "time.nist.gov";

    unsigned int localPort = 2390;      // local port to listen for UDP packets

    static int NTP_PACKET_SIZE;

    byte packetBuffer[48]; //buffer to hold incoming and outgoing packets
    
    // A UDP instance to let us send and receive packets over UDP
    WiFiUDP udp;
};


