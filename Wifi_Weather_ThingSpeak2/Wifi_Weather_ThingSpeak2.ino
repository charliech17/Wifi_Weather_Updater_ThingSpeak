#include <ESP8266WiFi.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#define dhtPin 13
#define dhtType DHT11
DHT dht(dhtPin,dhtType);
String temp,humi;
char buf[3];

/*Put your SSID & Password*/
const char* ssid = "";    // Enter SSID here
const char* password = "";  //Enter Password here

const char* server = "api.thingspeak.com";

/* Set GET link with channel ID */
const char* _getLink = "https://api.thingspeak.com/update?api_key=????????????&field1=";  //enter updata key (????????????)

WiFiClient client;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Connecting to ");
  Serial.println(ssid);

  //connect to your local wi-fi network
  WiFi.begin(ssid, password);

  //check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}

void loop() 
{
  if (client.connect(server,80))     // "184.106.153.149" or api.thingspeak.com
  { 
    //讀取濕度與溫度
     float h=dht.readHumidity();
     buf[0]=0x30+(int)h/10;
     buf[1]=0x30+(int)h%10;
     humi=(String(buf)).substring(0,2);
     float t=dht.readTemperature();
     buf[0]=0x30+(int)t/10;
     buf[1]=0x30+(int)t%10;
     temp=(String(buf)).substring(0,2);
     

     Serial.println("");
     Serial.print("Temperature ");
     Serial.print(t);
     Serial.println("C");
  
     Serial.print("Humidity: ");
     Serial.print(h);
     Serial.println("%");
     
     if(isnan(h)|| isnan(t)){
      Serial.println("Failed to read from DHT sensor");
      return;
      }

    //連線  
    String getStr = _getLink+temp+"&field2="+humi;

    client.print("GET "+getStr+"\n");
    client.print("HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n\n\n");

    while(client.available()){
  char c = client.read();
  Serial.print(c);
    }
    Serial.println("");
  }
  client.stop();
  Serial.println("Waiting…");
  delay(1800000); // Updata every half hour
}
