

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

const char* ssid = "mywifi"; // replace with ssid
const char* password = "password"; // replace with wifi password

const char* host = "api.pushbullet.com";
const int httpsPort = 443;
const char* PushBulletAPIKEY = "API_KEY"; //get it from your pushbullet account

// Use web browser to view and copy
// SHA1 fingerprint of the certificate
//const char* fingerprint = "2C BC 06 10 0A E0 6E B0 9E 60 E5 96 BA 72 C5 63 93 23 54 B3"; //got it using https://www.grc.com/fingerprints.htm
const char* fingerprint = "E7 06 F1 30 B1 5F 25 72 00 4D 99 F0 ED 90 4D F9 60 D3 FB 75";
const int greenLEDPin = 13;
const int redLEDPin = 14;
const int blueLEDPin = 12;

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);

  // set the digital pins as outputs
  pinMode(greenLEDPin, OUTPUT);
  pinMode(redLEDPin, OUTPUT);
  pinMode(blueLEDPin, OUTPUT);

  //init state is blue
  digitalWrite(greenLEDPin, LOW);
  digitalWrite(redLEDPin, LOW);
  digitalWrite(blueLEDPin, HIGH);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
 // digitalWrite(blueLEDPin, LOW);
 // digitalWrite(greenLEDPin, HIGH);
 // digitalWrite(redLEDPin, HIGH);

  // Use WiFiClientSecure class to create TLS connection
  WiFiClientSecure client;
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");

    //error state is red
     digitalWrite(greenLEDPin, LOW);
    digitalWrite(redLEDPin, HIGH);
    digitalWrite(blueLEDPin, LOW);
     Serial.println("stop");
    return;
  }

  if (client.verify(fingerprint, host)) {
    Serial.println("certificate matches");
  } else {
    Serial.println("certificate doesn't match");
  }
  String url = "/v2/pushes";
  String messagebody = "{\"type\": \"note\", \"title\": \"Maison\", \"body\": \"Je suis rentre!\"}\r\n";
  Serial.print("requesting URL: ");
  Serial.println(url);

  client.print(String("POST ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Authorization: Bearer " + PushBulletAPIKEY + "\r\n" +
               "Content-Type: application/json\r\n" +
               "Content-Length: " +
               String(messagebody.length()) + "\r\n\r\n");
  client.print(messagebody);



  Serial.println("request sent");

  //success is green
    digitalWrite(greenLEDPin, HIGH);
    digitalWrite(redLEDPin, LOW);
    digitalWrite(blueLEDPin, LOW);

    //loop loop loop
  while (client.available() == 0);

  while (client.available()) {
    String line = client.readStringUntil('\n');
    Serial.println(line);
  }
}

void loop() {
}
