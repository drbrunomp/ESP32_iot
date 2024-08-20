#include <WiFi.h>
#include <PubSubClient.h>
#define ADCpin 1   

const char* ssid = "A71Bruno";
const char* password = "a71bruno";

const char* mqttServer = "test.mosquitto.org";
const int mqttPort = 1883;
const char* mqttUser = "";
const char* mqttPassword = "";

int ValueToSend,
    ADC;

char payload[10]; 


WiFiClient espClient;
PubSubClient client(espClient);

void setup()
{
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);
 // analogSetWidth(12);
  
  Serial.begin(9600);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }
  Serial.println("Conectado ao WiFi");
  client.setServer(mqttServer, mqttPort); 

  while (!client.connected())
  {
    Serial.println("Conectando ao MQTT...");    
    if (client.connect("userid"))
    {    
      Serial.println("Conectado ao MQTT");        
    } 
    else 
    {
      Serial.print("Falha de estado ");
      Serial.print(client.state());
      delay(2000);
    }
  } 
}

void loop() 
{
  ADC = analogRead(ADCpin);
  ValueToSend = ADC;
  Serial.println(ValueToSend);
  
  snprintf(payload, sizeof(payload), "%d", ValueToSend);
  client.publish("pubtopic", payload);     
  client.loop();
  Serial.println();
  delay(10);   
}