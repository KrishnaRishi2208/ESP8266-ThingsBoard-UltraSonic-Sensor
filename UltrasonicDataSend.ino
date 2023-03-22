#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define WIFI_AP ""
#define WIFI_PASSWORD ""
#define TOKEN ""

// DHT
#define trigPin 5
#define echoPin 4

char mqttServer[] = "demo.thingsboard.io";
char clientId[] = "ESP8266Client";
char outTopic[] = "v1/devices/me/telemetry";

WiFiClient wifiClient;
PubSubClient pubSubClient(wifiClient);

int status = WL_IDLE_STATUS;
unsigned long lastSend;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(115200);
  InitWiFi();
  lastSend = 0;
}

void loop() {
  if (!pubSubClient.connected()) {
    reconnect();
  }

  if (millis() - lastSend > 5000) { // Update and send only after 1 seconds
    getAndSendDistanceData();
    lastSend = millis();
  }

  pubSubClient.loop();
}

void getAndSendDistanceData() {
  Serial.println("Connecting to ultrasonic sensor")  ;
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  delay(500);

  Serial.println("Sending data to ThingsBoard:");
  Serial.print("Distance: ");
  Serial.print(distance);

  char payload[50];
  sprintf(payload, "{\"distance\":%ld}", distance);
  pubSubClient.publish(outTopic, payload);
}

void InitWiFi() {
  Serial.println("Connecting to AP ...");

  WiFi.begin(WIFI_AP, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to AP");
  
  pubSubClient.setServer(mqttServer, 1883);
  pubSubClient.setCallback(callback);
}

void reconnect() {
  while (!pubSubClient.connected()) {
    status = WiFi.status();
    if (status != WL_CONNECTED) {
      WiFi.begin(WIFI_AP, WIFI_PASSWORD);
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }
      Serial.println("Connected to AP");
    }
    Serial.print("Connecting to ThingsBoard node ...");
    if (pubSubClient.connect(clientId, TOKEN, NULL)) {
      Serial.println("[DONE]");
      pubSubClient.subscribe("v1/devices/me/rpc/request/+");
    } else {
      Serial.print("[FAILED]");
      Serial.println(" : retrying in 5 seconds]");
      delay(5000);
    }
  }
}