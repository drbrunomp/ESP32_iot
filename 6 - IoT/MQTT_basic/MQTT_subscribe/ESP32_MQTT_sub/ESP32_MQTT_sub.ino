#include <ArduinoJson.h>
#include <EspMQTTClient.h>

char ValueDigital[100];
char ValueVoltage[100];
char ValueLed[100];
const char* payload[100];

int       VdigAdc = 0,
          StateLed = 0,
          LedMqtt = 0;
const int Led2 = 2;
float     VoltageAdc = 0;

EspMQTTClient client( "A71Bruno",                                   // Rede Wi-Fi
                      "a71bruno",                                   // Senha da rede Wi-Fi
                      "mqtt.tago.io",                               // MQTT Broker server
                      "Default",                                    // Usuário
                      "2b91ddf4-61af-4359-a174-f5941bab13de",       // Código do Token 
                      "Esp32DEV1",                                  // Nome do cliente (Especifico por dispositivo)
                      1883                                          // Porta MQTT, padrão em 1883.
                    );

void setup()
{
 analogReadResolution(12);
 analogSetAttenuation(ADC_11db);
 analogSetWidth(12);
 pinMode(Led2, OUTPUT);
}

void onConnectionEstablished()
{
  
   client.subscribe("info/Subled", [](const String & payload)
   {
     
    if (payload == '1')
    {
      digitalWrite(Led2, HIGH);
    } 
    else if (payload == '2') 
    {
      digitalWrite(Led2, LOW);
    }
   });    

    
}

void controlLogic()
{
  VdigAdc = analogRead(32);
  VoltageAdc = VdigAdc * 0.80586e-3; 
}



void publishMQTT()
{
  StaticJsonDocument<300> Publish_vdig;
  Publish_vdig["variable"] = "Digital";
  Publish_vdig["value"] = VdigAdc;
  serializeJson(Publish_vdig, ValueDigital);
  
  StaticJsonDocument<300> Publish_volt;
  Publish_volt["variable"] = "Voltage";
  Publish_volt["value"] = VoltageAdc;
  serializeJson(Publish_volt, ValueVoltage);

  StaticJsonDocument<300> Publish_led;
  Publish_led["variable"] = "Led";
  Publish_led["value"] = StateLed;
  serializeJson(Publish_led, ValueLed);


  client.publish("info/ValueDigital", ValueDigital); 
  client.publish("info/ValueVoltage", ValueVoltage); 
  client.publish("info/ValueLed", ValueLed);   
  delay(1000);

  client.loop();
}

void loop()
{
  client.loop();
  controlLogic();
  publishMQTT(); 
}