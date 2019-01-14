/****************************************************************************************************************************************************
 * Title: 'An environment supporting end-user driven composition of collaborative web and IoT services'
 * Group-2:'Towards actuator sensor and web service modules for a middleware supporting end-user driven collaborative service composition'
 * Participated By 'Abu Sayeed Bin Mozahid'
 * Profile: http://ambientintelligencelab.com/memberDetails/4
 * Authorized By 'Ambient Intelligence Lab'
 * Website: http://ambientintelligencelab.com/
 * Daffodil International University 
 * Updated Date: 16-06-2018
*****************************************************************************************************************************************************/


#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "admin";                         // Set the "User Network Name"
const char* password =  "password";                 // Set the "User Network Password" 
const char* mqttServer = "m10.cloudmqtt.com";       // Set the cloudmqtt instances Server
const int mqttPort = 17635;                         // Set the cloudmqtt instances Port 
const char* mqttUser = "ewpjantq";                  // Set the cloudmqtt instances User Name
const char* mqttPassword = "Rfnyu4oiB8WO";          // Set the cloudmqtt instances User Password
const String MAC = "/ac0wtc23";                     // Create an manutal MAC address for identify the machine
/*
 * AC- Actuator
 * 0- Actuator Name
 * LS- Lower Sensor
 * WTC32 - WTC project name and 32 project number
*/

String topic;                         // declare a String type variable
int actuatorOutput = 2;                  // Set the motor pin
char message[100];                    // declare an array type variable


//int i=0;/
WiFiClient espClient;                 // Creating WiFiClient object
PubSubClient client(espClient);       // Creating PubSubClient  object and it take WiFiClient object as a parameter

//********************** AGENT FUNCTION **************************//

void waterPumpControl(char actuatorDecision)
{
  char trueValue;
  trueValue='1';
  Serial.print("Payload: ");
  Serial.println(actuatorDecision);
  Serial.print("trueValue ");
  Serial.println(trueValue);
  if(actuatorDecision == trueValue)
  {
    /*
     * Relay will work active to LOW. When relay get LOW 
     * that means it will run otherwise it will stop.
    */
    digitalWrite(actuatorOutput,LOW);      
    Serial.println("Motor ON");
  }
  else{
    digitalWrite(actuatorOutput,HIGH);
    Serial.println("Motor OFF");
  }
}
//******************** <AGENT_FUNCTION> ****************************//

//*************************** WiFI Connection *************************//
void wifiConnection()
{
  // WiFi.begin function will take ssid and password as parameters for establish the connection 
  WiFi.begin(ssid, password);
  
 /*
  * If system can't establish connection with the wifi network, 
  * system stuck in this while loop and always show  
  * "Connecting to WiFi..."
  */
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.println("Connecting to WiFi...");
  }

   /* 
   *  If system can connection with wifi network
   *  then System will show "Connected to the WiFi network"
   */
  Serial.println("Connected to the WiFi Network");
}
//*************************** <Wifi Connection> ***********************//

//*************************** Reconnect *******************************//
void reconnect(){
  while(!client.connected()){
    Serial.println("Connecting to MQTT");

    if (client.connect("ESP8266Client", mqttUser, mqttPassword)){
      Serial.println("Connected");
      client.subscribe("/ac0wtc23"); 
//       client.subscribe("/#"); 
    }else{
      Serial.print("Failed with State");
      Serial.print(client.state());
      delay(2000);
    }
  }
}
//*************************** <Reconnect> *****************************//

//*************************** callBack ********************************//
void callBack(char* topic, byte* payload, unsigned int length) {
 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
 
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  waterPumpControl(payload[0]);
  Serial.println();
  Serial.println("-----------------------");
 
}
//*************************** <callBack> ******************************//

//****************************** Setup *******************************//
// Setup function will have been only once time run after system get connection from power supply

void setup() {
  
  Serial.begin(115200);                       // Open Serial monitor for debugging
  wifiConnection();           
  pinMode(motorOutput,OUTPUT);                // initialize that, motorOutput pin work for OUTPUT
  
  /*
  * client.setServer function will take mqttServer and mqttPort as parameters 
  * for establish the connection with CloudMQTT 
  */
  
  client.setServer(mqttServer, mqttPort); 
  
  /*
   * Relay will work active to LOW. When relay get LOW 
   * that means it will run otherwise it will stop.
   */
  digitalWrite(motorOutput,HIGH);
}
//****************************** <Setup> *******************************//


//****************************** LOOP *********************************//
void loop() 
{
  if (!client.connected())
  {
    Serial.println("Enter the IF");
    reconnect();
    client.setCallback(callBack);
  }
  else
  {
    client.setCallback(callBack);
    client.loop();
    delay(500); 
  }
  
}
//**************************** <LOOP> **********************************//
