#include <Arduino.h>
#include "SensorSerial.h"

//SENSOR COLLECTOR DEVICE INFORMATION
SensorSerial Channel1(D1, 1); // mendfefinisikan sensor channel1 dengan pin D1 dan nomor channel 1
SensorSerial Channel2(D2, 2); // mendfefinisikan sensor channel2 dengan pin D2 dan nomor channel 2
SensorSerial Channel3(D3, 3); // mendfefinisikan sensor channel3 dengan pin D3 dan nomor channel 3

//KONFIGURASI WIFI
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
const char* ssid = "JGEF";
const char* password = "bintangdelapankali";
const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient;
PubSubClient client(espClient);
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
char outTopic[MSG_BUFFER_SIZE];

// untuk menyambungkan ke jaringan wifi
void setup_wifi() {

  delay(10);
  
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

//untuk menyambungkan device ke broker mqtt
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "eFisherySensorCollector";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


//untuk mempublish data sensor ke broker
void PublsihChannel(int channel, String message){
  message.toCharArray(msg,MSG_BUFFER_SIZE);
  ("SensorCollector/" + String(channel)).toCharArray(outTopic,MSG_BUFFER_SIZE);
  client.publish(outTopic, msg);
}

void setup()
{
  Serial.begin(9600);
  Serial.println("Start Program");

  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop()
{
  //mengupdate data sensor
  Channel1.CollectSensorData();
  Channel2.CollectSensorData();
  Channel3.CollectSensorData();
  if (!client.connected()) {
    //untuk menjaga agar device tetap terhubung ke broker
    reconnect();
  }
  else{
    //publish data ke broker
    PublsihChannel(Channel1.GetChannelNumber(),Channel1.GetSensorInfo());
    PublsihChannel(Channel2.GetChannelNumber(),Channel2.GetSensorInfo());
    PublsihChannel(Channel3.GetChannelNumber(),Channel3.GetSensorInfo());
  }
  client.loop();
}
