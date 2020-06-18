/*
 This sketch test is based on the code developed by aDavid del Peral
 Tested on Arduino MKR 1400 GSM/4G
*/

// libraries
#include <MKRGSM.h>


const char PINNUMBER[] = "";
char login[50]="";
char password[20]="";
char apn[50]="ndo";
bool use_proxy = false;
char proxy[100] = "";

// messages for serial monitor response
String oktext = "OK";
String errortext = "ERROR";

// initialize the library instance
GSM gsmAccess;        // GSM access: include a 'true' parameter for debug enabled
GPRS gprsAccess;  // GPRS access
GSMClient client;  // Client service for TCP connection


// URL and path (for example: example.org)
char url[] = "https://example.org";
char urlproxy[] = "http://www.example.org";
char path[] = "/";

// variable for save response obtained
String response = "";

void setup() {
  // initialize serial communications and wait for port to open:
  Serial.begin(9600);
  //while (!Serial) {
  //  ; // wait for serial port to connect. Needed for Leonardo only
  //}
}

void loop() {
  use_proxy = false;

  char message[50];
  Serial.print("Enter your message: ");
  readSerial(message);
  
  // start GSM shield
  // if your SIM has PIN, pass it as a parameter of begin() in quotes
  Serial.print("Connecting GSM network...");
  if (gsmAccess.begin(PINNUMBER) != GSM_READY) {
    Serial.println(errortext);
  }
  Serial.println(oktext);


  // attach GPRS
  Serial.println("Attaching to GPRS with your APN...");
  if (gprsAccess.attachGPRS(apn, login, password) != GPRS_READY) {
    Serial.println(errortext);
  } else {

    Serial.println(oktext);

    // if user introduced a proxy, asks them for proxy port
    int pport;
    if (proxy[0] != '\0') {
      // read proxy port introduced by user
      char proxyport[10];
      Serial.print("Enter the proxy port: ");
      readSerial(proxyport);
      // cast proxy port introduced to integer
      pport = (int) proxyport;
      use_proxy = true;
      Serial.println(proxyport);
    }

    // connection with example.org and realize HTTP request
    Serial.print("Connecting and sending GET request to...");
    int res_connect;

    // if use a proxy, connect with it
    if (use_proxy) {
      res_connect = client.connect(proxy, pport);
    } else {
      res_connect = client.connect(url, 80);
    }
    Serial.print(res_connect);
    
    if (res_connect) {
      // make a HTTP 1.0 GET request (client sends the request)
      //client.print("GET /hab.php");
      client.print(message);

      // if use a proxy, the path is example.org URL
      if (use_proxy) {
        client.print(urlproxy);
      } else {
        client.print(path);
      }

      client.println(" HTTP/1.1\r\nHost:emuhab.ngrok.io");
      client.println();
      Serial.println(oktext);
    } else {
      // if you didn't get a connection to the server
      Serial.println(errortext);
    }
    Serial.print("Receiving response...");

    bool test = true;
    while (test) {
      // if there are incoming bytes available
      // from the server, read and check them
      if (client.available()) {
        char c = client.read();
        response += c;

        // cast response obtained from string to char array
        char responsechar[response.length() + 1];
        response.toCharArray(responsechar, response.length() + 1);

        // if response includes a "200 OK" substring
        if (strstr(responsechar, "200 OK") != NULL) {
          Serial.println(oktext);
          Serial.println("TEST COMPLETE!");
          test = false;
        }
      }

      // if the server's disconnected, stop the client:
      if (!client.connected()) {
        Serial.println();
        Serial.println("disconnecting.");
        client.stop();
        test = false;
      }
    }
  }
}

/*
  Read input serial
 */
int readSerial(char result[]) {
  int i = 0;
  while (1) {
    while (Serial.available() > 0) {
      char inChar = Serial.read();
      if (inChar == '\n') {
        result[i] = '\0';
        return 0;
      }
      if (inChar != '\r') {
        result[i] = inChar;
        i++;
      }
    }
  }
}
