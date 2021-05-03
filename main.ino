#include <ESP8266WiFi.h> //librería necesaria para la conexión wifi
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

//////------------------Variables y bibliotecas para el uso de Max7219--------------////
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
//------------PARA ESP82666-----------//
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 8
//--------Pines para ESP8266--------//
#define CLK_PIN   14
#define DATA_PIN  13
#define CS_PIN    2
//----Hardware SPI connection------//
MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

//----------------WiFi.begin("ssid", "password")-------------///;
const char* ssid = "MOVISTAR_382F";
const char* password = "77DBFD8164501F201C5C";

ESP8266WiFiMulti WiFiMulti;

void setup() {
  Serial.begin(115200);
  P.begin();// iniciamos el MAX7219
  
  /*for(uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();//Espera a que se complete la transmisión de los datos en serie salientes
    delay(1000);
  }*/
  WiFi.mode(WIFI_STA);///Para conectarla a una red Wi-Fi a través de un acces point (SSID)
  WiFiMulti.addAP(ssid,password);///Seleccionamos la red a la que nos queremos conectar (con esta librería podríamos conectarnos a varias)
}

void loop() {
  // put your main code here, to run repeatedly:
  if((WiFiMulti.run() == WL_CONNECTED)) {//Si se conecta bien a la red wifi(si hay varias se conectará a la más estable)

    ///----------------------------------------OBJETO 0: PRECIO ETHER---------------------////
    
    HTTPClient http;///Creamos un objeto HTTP (esta clase proporciona los métodos para crear y enviar la solicitud HTTP) 
    Serial.println("[HTTP] begin...");//Imprimimos por la pantalla que estamos conectados y empezando a hacer cosas. 
    http.begin("api.thingspeak.com", 80,"/apps/thinghttp/send_request?api_key=IIZMBB3Q8P1KM44Z"); //llamamos al método begin y le pasamos la url dónde queremos conectarnos.
    int httpCode = http.GET();//Le mandamosla solicitud http get y guardamos lo que recojamos en un entero.
    
    Serial.println("[HTTP] Get...");

    if(httpCode > 0){//Si tenemos un código  de respuesta
      Serial.printf("[HTTP] GET... code: %d",httpCode);//Imprimimos el código
      if(httpCode = HTTP_CODE_OK){///Si el código es diciendo que hemos obtenido respuesta
        Serial.print("Precio de Ether: ");
        String precio_ether = http.getString();
        Serial.println(http.getString());///Imprimimos la respuesta
        String Eth = "Eth:";
        Eth += precio_ether;
        P.print(Eth);
      }
    }
    
    else{
      Serial.printf("[HTTP] GET... Failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    delay(1000);
    
    ///----------------------------------------OBJETO 1: Mínimo 24 HORAS---------------------////

    HTTPClient http1;
    http1.begin("api.thingspeak.com", 80,"/apps/thinghttp/send_request?api_key=K9P2JQT9JN9HZJGZ");
    int httpCode1 =http1.GET();
    
    if(httpCode1 > 0){//Si tenemos un código  de respuesta
      Serial.printf("[HTTP] GET... code: %d",httpCode1);//Imprimimos el código
      if(httpCode1 = HTTP_CODE_OK){///Si el código es diciendo que hemos obtenido respuesta
        Serial.print("24 h:  ");
        String minim = http1.getString();
        String veinticuatrohoras = "Min:";
        Serial.println(http1.getString());///Imprimimos la respuesta
        veinticuatrohoras += minim;
        P.print(veinticuatrohoras);
      }
    }
    
    else{
      Serial.printf("[HTTP] GET... Failed, error: %s\n", http.errorToString(httpCode1).c_str());
    }
    delay(1000);
    ///----------------------------------------OBJETO 2 MÁXIMO 24 HORAS---------------------////
    HTTPClient http2;
    http2.begin("api.thingspeak.com", 80,"/apps/thinghttp/send_request?api_key=OC4DGPIUMTLYUVNC");
    int httpCode2 =http2.GET();
    
    if(httpCode2 > 0){//Si tenemos un código  de respuesta
      Serial.printf("[HTTP] GET... code: %d",httpCode2);//Imprimimos el código
      if(httpCode2 = HTTP_CODE_OK){///Si el código es diciendo que hemos obtenido respuesta
        Serial.print("Máximo 24 horas: ");
        Serial.println(http2.getString());///Imprimimos la respuesta
        String preciomaxi = http2.getString();
        String veinticuatrohoras = "Max:";
        Serial.println(http2.getString());///Imprimimos la respuesta
        veinticuatrohoras += preciomaxi;
        P.print(veinticuatrohoras);
      }
    }
    
    else{
      Serial.printf("[HTTP] GET... Failed, error: %s\n", http.errorToString(httpCode2).c_str());
    }
    delay(1000);

    ///----------------------------------------OBJETO 3: PRECIO AVAX---------------------////
    
    HTTPClient http3;///Creamos un objeto HTTP (esta clase proporciona los métodos para crear y enviar la solicitud HTTP) 
    Serial.println("[HTTP] begin...");//Imprimimos por la pantalla que estamos conectados y empezando a hacer cosas. 
    http3.begin("api.thingspeak.com", 80,"/apps/thinghttp/send_request?api_key=ADAFV6I3IH0QULO0"); //llamamos al método begin y le pasamos la url dónde queremos conectarnos.
    int httpCode3 = http3.GET();//Le mandamosla solicitud http get y guardamos lo que recojamos en un entero.
    
    Serial.println("[HTTP] Get...");

    if(httpCode3 > 0){//Si tenemos un código  de respuesta
      Serial.printf("[HTTP] GET... code: %d",httpCode3);//Imprimimos el código
      if(httpCode3 = HTTP_CODE_OK){///Si el código es diciendo que hemos obtenido respuesta
        Serial.print("Precio de Avax: ");
        String precio_avax = http3.getString();
        Serial.println(http3.getString());///Imprimimos la respuesta
        String AVAX = "AVAX: ";
        AVAX += precio_avax;
        P.print(AVAX);
      }
    }
    
    else{
      Serial.printf("[HTTP] GET... Failed, error: %s\n", http.errorToString(httpCode3).c_str());
    }
    delay(1000);
    
    ///----------------------------------------OBJETO 4: Mínimo 24 HORAS AVAX---------------------////

    HTTPClient http4;
    http4.begin("api.thingspeak.com", 80,"/apps/thinghttp/send_request?api_key=6N5D790YODVCB45Y");
    int httpCode4 =http4.GET();
    
    if(httpCode4 > 0){//Si tenemos un código  de respuesta
      Serial.printf("[HTTP] GET... code: %d",httpCode4);//Imprimimos el código
      if(httpCode1 = HTTP_CODE_OK){///Si el código es diciendo que hemos obtenido respuesta
        Serial.print("24 h:  ");
        String minim = http4.getString();
        String veinticuatrohoras = "Min: ";
        Serial.println(http4.getString());///Imprimimos la respuesta
        veinticuatrohoras += minim;
        P.print(veinticuatrohoras);
      }
    }
    
    else{
      Serial.printf("[HTTP] GET... Failed, error: %s\n", http.errorToString(httpCode4).c_str());
    }
    delay(1000);
    ///----------------------------------------OBJETO 5 MÁXIMO 24 HORAS AVAX---------------------////
    HTTPClient http5;
    http5.begin("api.thingspeak.com", 80,"/apps/thinghttp/send_request?api_key=ZWKWHCAP0XIIE94C");
    int httpCode5 =http5.GET();
    
    if(httpCode5 > 0){//Si tenemos un código  de respuesta
      Serial.printf("[HTTP] GET... code: %d",httpCode5);//Imprimimos el código
      if(httpCode5 = HTTP_CODE_OK){///Si el código es diciendo que hemos obtenido respuesta
        Serial.print("Máximo 24 horas: ");
        Serial.println(http5.getString());///Imprimimos la respuesta
        String preciomaxi = http5.getString();
        String veinticuatrohoras = "Max: ";
        Serial.println(http5.getString());///Imprimimos la respuesta
        veinticuatrohoras += preciomaxi;
        P.print(veinticuatrohoras);
      }
    }
    
    else{
      Serial.printf("[HTTP] GET... Failed, error: %s\n", http.errorToString(httpCode5).c_str());
    }
    delay(1000);

    ///----------------------------------------OBJETO 6: PRECIO ATOM---------------------////
    
    HTTPClient http6;///Creamos un objeto HTTP (esta clase proporciona los métodos para crear y enviar la solicitud HTTP) 
    Serial.println("[HTTP] begin...");//Imprimimos por la pantalla que estamos conectados y empezando a hacer cosas. 
    http6.begin("api.thingspeak.com", 80,"/apps/thinghttp/send_request?api_key=WWODXHX80FNQP2SE"); //llamamos al método begin y le pasamos la url dónde queremos conectarnos.
    int httpCode6 = http6.GET();//Le mandamosla solicitud http get y guardamos lo que recojamos en un entero.
    
    Serial.println("[HTTP] Get...");

    if(httpCode6 > 0){//Si tenemos un código  de respuesta
      Serial.printf("[HTTP] GET... code: %d",httpCode6);//Imprimimos el código
      if(httpCode6 = HTTP_CODE_OK){///Si el código es diciendo que hemos obtenido respuesta
        Serial.print("Precio de Atom: ");
        String precio_atom = http6.getString();
        Serial.println(http.getString());///Imprimimos la respuesta
        String ATOM = "ATOM:";
        ATOM += precio_atom;
        P.print(ATOM);
      }
    }
    
    else{
      Serial.printf("[HTTP] GET... Failed, error: %s\n", http.errorToString(httpCode6).c_str());
    }
    delay(1000);
    
    ///----------------------------------------OBJETO 7: Mínimo 24 HORAS ATOM---------------------////

    HTTPClient http7;
    http7.begin("api.thingspeak.com", 80,"/apps/thinghttp/send_request?api_key=RIWK5BRRHYFZR2WJ");
    int httpCode7 =http7.GET();
    
    if(httpCode7 > 0){//Si tenemos un código  de respuesta
      Serial.printf("[HTTP] GET... code: %d",httpCode7);//Imprimimos el código
      if(httpCode7 = HTTP_CODE_OK){///Si el código es diciendo que hemos obtenido respuesta
        Serial.print("24 h:  ");
        String minim = http7.getString();
        String veinticuatrohoras = "Min: ";
        Serial.println(http7.getString());///Imprimimos la respuesta
        veinticuatrohoras += minim;
        P.print(veinticuatrohoras);
      }
    }
    
    else{
      Serial.printf("[HTTP] GET... Failed, error: %s\n", http.errorToString(httpCode7).c_str());
    }
    delay(1000);
    ///----------------------------------------OBJETO 8 MÁXIMO 24 HORAS ATOM---------------------////
    HTTPClient http8;
    http8.begin("api.thingspeak.com", 80,"/apps/thinghttp/send_request?api_key=VTB5UX9G9P6BQSAA");
    int httpCode8 =http8.GET();
    
    if(httpCode8 > 0){//Si tenemos un código  de respuesta
      Serial.printf("[HTTP] GET... code: %d",httpCode8);//Imprimimos el código
      if(httpCode8 = HTTP_CODE_OK){///Si el código es diciendo que hemos obtenido respuesta
        Serial.print("Máximo 24 horas: ");
        Serial.println(http8.getString());///Imprimimos la respuesta
        String preciomaxi = http8.getString();
        String veinticuatrohoras = "Max: ";
        Serial.println(http8.getString());///Imprimimos la respuesta
        veinticuatrohoras += preciomaxi;
        P.print(veinticuatrohoras);
      }
    }
    
    else{
      Serial.printf("[HTTP] GET... Failed, error: %s\n", http.errorToString(httpCode8).c_str());
    }
    delay(1000);

    ///----------------------------------------OBJETO 9: PRECIO POLKADOT---------------------////
    
    HTTPClient http9;///Creamos un objeto HTTP (esta clase proporciona los métodos para crear y enviar la solicitud HTTP) 
    Serial.println("[HTTP] begin...");//Imprimimos por la pantalla que estamos conectados y empezando a hacer cosas. 
    http9.begin("api.thingspeak.com", 80,"/apps/thinghttp/send_request?api_key=F6UKDK0UGTRN770J"); //llamamos al método begin y le pasamos la url dónde queremos conectarnos.
    int httpCode9 = http9.GET();//Le mandamosla solicitud http get y guardamos lo que recojamos en un entero.
    
    Serial.println("[HTTP] Get...");

    if(httpCode9 > 0){//Si tenemos un código  de respuesta
      Serial.printf("[HTTP] GET... code: %d",httpCode9);//Imprimimos el código
      if(httpCode9 = HTTP_CODE_OK){///Si el código es diciendo que hemos obtenido respuesta
        Serial.print("Precio de polkadot: ");
        String precio_dot = http9.getString();
        Serial.println(http9.getString());///Imprimimos la respuesta
        String DOT = "DOT:";
        DOT += precio_dot;
        P.print(DOT);
      }
    }
    
    else{
      Serial.printf("[HTTP] GET... Failed, error: %s\n", http.errorToString(httpCode9).c_str());
    }
    delay(1000);
    
    ///----------------------------------------OBJETO 10: Mínimo 24 HORAS DOT---------------------////

    HTTPClient http10;
    http10.begin("api.thingspeak.com", 80,"/apps/thinghttp/send_request?api_key=6JX9DRNIRK8MH516");
    int httpCode10 =http10.GET();
    
    if(httpCode10 > 0){//Si tenemos un código  de respuesta
      Serial.printf("[HTTP] GET... code: %d",httpCode10);//Imprimimos el código
      if(httpCode1 = HTTP_CODE_OK){///Si el código es diciendo que hemos obtenido respuesta
        Serial.print("24 h:  ");
        String minim = http10.getString();
        String veinticuatrohoras = "Min:";
        Serial.println(http10.getString());///Imprimimos la respuesta
        veinticuatrohoras += minim;
        P.print(veinticuatrohoras);
      }
    }
    
    else{
      Serial.printf("[HTTP] GET... Failed, error: %s\n", http.errorToString(httpCode10).c_str());
    }
    delay(1000);
    ///----------------------------------------OBJETO 11 MÁXIMO 24 HORAS DOT---------------------////
    HTTPClient http11;
    http11.begin("api.thingspeak.com", 80,"/apps/thinghttp/send_request?api_key=8V30REV567BLVZS4");
    int httpCode11 =http11.GET();
    
    if(httpCode11 > 0){//Si tenemos un código  de respuesta
      Serial.printf("[HTTP] GET... code: %d",httpCode11);//Imprimimos el código
      if(httpCode11 = HTTP_CODE_OK){///Si el código es diciendo que hemos obtenido respuesta
        Serial.print("Máximo 24 horas: ");
        Serial.println(http11.getString());///Imprimimos la respuesta
        String preciomaxi = http11.getString();
        String veinticuatrohoras = "Max:";
        Serial.println(http11.getString());///Imprimimos la respuesta
        veinticuatrohoras += preciomaxi;
      P.print(veinticuatrohoras);
    }
  }
  
  else{
    Serial.printf("[HTTP] GET... Failed, error: %s\n", http.errorToString(httpCode11).c_str());
  }
  delay(1000);
  
  http.end();
  http1.end();
  http2.end();
  http3.end();
  http4.end();
  http5.end();
  http6.end();
  http7.end();
  http8.end();
  http9.end();
  http10.end();
  http11.end();

}
delay(1000);
}