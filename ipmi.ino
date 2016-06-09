#include <UIPEthernet.h>

EthernetServer server = EthernetServer(80);
#define TOGGLE_PIN 4
void setup()
{
  pinMode(TOGGLE_PIN, OUTPUT);
  uint8_t mac[6] = {0x00,0x01,0x02,0x03,0x04,0x05};
  Ethernet.begin(mac);
  server.begin();
}

void loop()
{
  size_t size;

  if (EthernetClient client = server.available())
    {
      uint8_t* msg = 0;
      size_t total_read = 0;
      while((size = client.available()) > 0)
        {
          msg = (uint8_t*)malloc(size);
          size = client.read(msg,size);
          total_read += size;
        }
       
      client.print(
      "HTTP/1.1 200 OK\r\n"
      "Accept-Encoding: identity\r\n"
      "\r\n"
      "<form action=\"power\"><input type=\"submit\" value=\"Power Button\"/></form>");
      
      uint8_t *url_start = msg + 5, *t;
      for (t=url_start; *t != ' '; t++);
      *t = 0;
      
      if (msg[0] == 'G' && msg[1] == 'E' && msg[2] == 'T' && 
          url_start[0] == 'p' && url_start[1] == 'o' && url_start[2] == 'w' &&
          url_start[3] == 'e' && url_start[4] == 'r') {
         client.write("Power toggled!");
         digitalWrite(TOGGLE_PIN, LOW);
         delay(200);
         digitalWrite(TOGGLE_PIN, HIGH);
      }
      
      
      if (msg) {
        client.write(msg, total_read);
        free(msg);
      }
      client.stop();
    }
}
