/*
  ESP8266 SMART LOCK MQTT 

  AUTHORS: ANDRE MARCOS PEREZ NUSP 8006891
           LEONARDO NUNES PARENTE NUSP 7987352

  SEL373 PROJETOS EM SISTEMAS DIGITAIS
  
  It connects to an MQTT server then:
  - publishes "c" to the topic "home/hallSensor" when the sensor state changes from HIGH to LOW (door closed).
  - publishes "o" to the topic "home/hallSensor" when the sensor state changes from LOW to HIGH (door opened).
  - publishes "m" to the topic "home/pirSensor" when the PIR sensor state changes from LOW to HIGH (movement detected).
  - subscribes to the topic "home/lock", NB - it assumes the received payloads are strings not binary.
  - If the first character of the topic "home/lock" is a 0, the motor is turned clockwise closing the door.
  - If the first character of the topic "home/lock" is an 1, the motor is turned anticlockwise opening the door.
  - If the first character of the topic "home/lock" is an 3, the battery level is measured and published to the topic 
    "home/battery".

  It will reconnect to the server if the connection is lost using a blocking
  reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
  achieve the same result without blocking the main loop.

*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//Connect to router
const char* ssid = "LabMicros";
const char* password = "abc01234def";
const char* mqtt_server = "192.168.1.111";
bool hall_flag;
bool motion_flag;

WiFiClient espClient;
PubSubClient client(espClient);
char msg[10];

//Blink the blue led when something is published or read
void commandLED(){
  digitalWrite(D1, HIGH);
  delayMicroseconds(15000);
  digitalWrite(D1, LOW);
}


void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

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

/*----------- Subscribe to home/lock -----------*/
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the MOTOR if an 1 was received as first character
  if ((char)payload[0] == '0') {
    digitalWrite(D6, HIGH);   // OPEN THE DOOR
    delay(500);
    digitalWrite(D6, LOW);
    commandLED();
  } 
  if ((char)payload[0] == '1'){
    digitalWrite(D7, HIGH);  // CLOSE THE DOOR
    delay(500);
    digitalWrite(D7, LOW);
    commandLED();
  }
  //Publish battery level
  if ((char)payload[0] == '2'){
      //float value = ((analogRead(A0)-4)*5)/235;
      int value = analogRead(A0);
      if (value <= 3.5){
        digitalWrite(D1, HIGH);  
      }
      char result[6];
      dtostrf(value, 6, 2, result); 
      snprintf (msg, 10, result);
      client.publish("home/battery", msg);
      commandLED();
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, resubscribe
      client.subscribe("home/lock");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}



void setup() {
  pinMode(D3, INPUT);     // Initialize D3 pin as an INPUT (Hall sensor)
  pinMode(D8, INPUT);     // Initialize D5 pin as an INPUT (PIR sensor)
  pinMode(D6, OUTPUT);     // Initialize D6 pin as an OUTPUT MOTOR CLOCKWISE
  pinMode(D7, OUTPUT);     // Initialize D7 pin as an OUTPUT MOTOR ANTICLOCKWISE
  pinMode(D1, OUTPUT);     // Initialize D1 pin as the BLUE COMMAND LED
  pinMode(D2, OUTPUT);     // Initialize D2 pin as the LOW BATTERY INDICADOR LED (RED)
  digitalWrite(D6, LOW);   // Initialize Motor as Stoped
  digitalWrite(D7, LOW);   // Initialize Motor as Stoped
  if (digitalRead(D3) == HIGH) {
    hall_flag = true;
  }
  else {
    hall_flag = false;
  }
  if (digitalRead(D8) == HIGH) {
    motion_flag = true;
  }
  else {
    motion_flag = false;
  }
  
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}


/*--------------- MAIN LOOP -----------*/
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  //check hall sensor
  if (digitalRead(D3) == HIGH) {
    if (hall_flag == false) {
      snprintf (msg, 10, "o");
      client.publish("home/hallSensor", msg);
      Serial.println("opened");
      commandLED();
    }
    hall_flag = true;
  }
  else {
    if (hall_flag == true) {
      snprintf (msg, 10, "c");
      client.publish("home/hallSensor", msg);
      Serial.println("closed");
      commandLED();
    }
    hall_flag = false;
  }
  //check motion sensor
  if (digitalRead(D8) == HIGH) {
    if (motion_flag == false) {
      snprintf (msg, 10, "m");
      client.publish("home/pirSensor", msg);
      Serial.println("motion");
      motion_flag = true;
      commandLED();
    }
  }
  else {
    if (motion_flag) {
      Serial.println("ended");
      motion_flag = false;
    }
  }
  client.loop();
}


