#include <WiFi.h>
#include <PubSubClient.h>
#define ADCpin 1  
#define LED 12 

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
  //analogSetWidth(12);
  pinMode(LED, OUTPUT);

  Serial.begin(9600);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }
  Serial.println("Conectado ao WiFi");

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  while (!client.connected())
  {
    Serial.println("Conectando ao MQTT...");    
    if (client.connect("userid"))
    {    
      Serial.println("Conectando ao MQTT");
      client.subscribe("LED");            
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

void callback(String topic, byte* payload, unsigned int length)
{
  Serial.println();
  Serial.print("Mensagem recebida no tópico ");
  Serial.println(topic);
  Serial.println("Mensagem: ");
  String msg;

  if (topic == "LED")
  {
    for (int i = 0; i < length; i++)
    {
      Serial.print((char)payload[i]);
      msg += (char)payload[i];
    }
    Serial.println(msg);
    Serial.print("LED: ");
    if (msg == "ON")
    { 
      digitalWrite(LED, HIGH);
      Serial.println("ON"); 
    }
    else if (msg == "OFF")
    { 
      digitalWrite(LED, LOW);
      Serial.println("OFF");
    }
  }
  Serial.println();
  delay(10);   
}

