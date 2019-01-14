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
#include <ESP8266WiFi.h>

 
const char* ssid = "admin";                     // Set the "User Network Name"
const char* password =  "password";             // Set the "User Network Password" 
const char* mqttServer = "m10.cloudmqtt.com";   // Set the cloudmqtt instances Server
const int mqttPort = 13964 ;                    // Set the cloudmqtt instances Port 
const char* mqttUser = "hvgihpll";              // Set the cloudmqtt instances User Name
const char* mqttPassword = "CTMRYdz5vSx4";      // Set the cloudmqtt instances User Password
const String MAC = "/S07USWTC23";         
/*
 * S-Sensor
 * 07-Sensor Name
 * LS- Lower Sensor
 * WTC32 - WTC project name and 32 project number
*/

char topicValue[10];        // declare an array type variable
char mac [15];              // declare an array type variable
int sensorValue = A0;       // Set the sensor pin
int value;                  // declare an integer type variable


WiFiClient espClient;                           // Creating WiFiClient object
PubSubClient client(espClient);                 // Creating PubSubClient  object and it take WiFiClient object as a parameter




/************************************* Setup Function Start ************************************************/
// Setup function will have been only once time run after system get connection from power supply

void setup() {
 
  Serial.begin(115200);   // Open Serial monitor for debugging

  WiFi.begin(ssid, password);   // WiFi.begin function will take ssid and password as parameters for establish the connection 

 /*
  * If system can't establish connection with the wifi network, 
  * system stuck in this while loop and always show  
  * "Connecting to WiFi..."
  */
  while (WiFi.status() != WL_CONNECTED)  
  {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  
  /* 
   *  If system can connection with wifi network
   *  then System will show "Connected to the WiFi network"
   */
  Serial.println("Connected to the WiFi network");

 /*
  * client.setServer function will take mqttServer and mqttPort as parameters 
  * for establish the connection with CloudMQTT 
  */
  client.setServer(mqttServer, mqttPort);
  /*
   * client.setCallback take callback function as a parameter. 
   * Here callback function called when a message arrives for a subscription created by this client.
   * Reference: https://pubsubclient.knolleary.net/api.html#setcallback 
  */  
  client.setCallback(callback);  


 
 /*
  * If system can't establish connection with the CloudMQTT server, 
  * system stuck in this while loop untill connecte with CloudMQTT server and always show  
  * "Connecting to MQTT..."
  */
  while (!client.connected())
  {
    Serial.println("Connecting to MQTT...");

    /*System will be trying agian to establish connetcion  with CloudMQTT server by clientID, CloudMQTT username, CloudMQTT password
     * Reference: https://pubsubclient.knolleary.net/api.html#connect3
     */
    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) 
    {
      Serial.println("connected");  
 
    } 
    else
    {
      Serial.print("Failed with state ");
      /*
       * Returns the current state of the client. If a connection attempt fails, 
       * this can be used to get more information about the failure.
       * Reference: https://pubsubclient.knolleary.net/api.html#state
      */
      Serial.print(client.state());
      delay(2000); 
    }
  }
 
}

/************************************* Setup Function End ************************************************/


/************************************* Reconnect Function Start ******************************************/
void reconnect()
{
    while (!client.connected()) 
    {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) 
    { 
      Serial.println("connected");  
    }
    else 
    {
      Serial.print("Failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

/************************************* Reconnect Function End ******************************************/


/**************************************** Call Back Function Star **********************************************/ 
void callback(char* topic, byte* payload, unsigned int length) {
 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
 
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
 
  Serial.println();
  Serial.println("-----------------------");
 
}
 
/**************************************** Call Back Function End **********************************************/ 

/**************************************** Loop Function Start ************************************************/
void loop() {

   value = analogRead(sensorValue);     //take sensor value

   Serial.println(value);
   String payload = "";
   payload += value;                     // convert sensor value Integer to String
   payload.toCharArray(topicValue,10);   // convert sensor value String to Character
   MAC.toCharArray(mac,15);              // convert MAC value String to Character

   if (!client.connected()) 
   {
    reconnect();                         // Call reconnect function when client.connected() is False
   }  

   /*
    * This should be called regularly to allow the client to process incoming messages 
    * and maintain its connection to the server.
    * Reference: https://pubsubclient.knolleary.net/api.html#loop
   */
   client.loop();                                           
   client.publish(mac, topicValue);
   delay(2000);
}

/**************************************** Loop Function End ************************************************/
