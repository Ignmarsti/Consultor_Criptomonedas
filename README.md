### Consultor criptomonedas

El propósito de este proyecto es crear un dispositivo que muestre por una matriz de LED´s el precio actual, el mínino y el máximo en 24 horas, de las criptomonedas que queramos. En mi caso mostrará los datos de ETHER, AVAX, ATOM y DOT.
Mis nociones de IoT son bastante básicas, por lo que si alguién tiene alguna sugerencia o comentario será bienvenido en [esta sección](https://github.com/Ignmarsti/Consultor_Criptomonedas/issues "esta sección").

# Introducción
Para este proyecto vamos a tener que tener claros unos conceptos básicos tales como qué es una API y como usarla.

Una API sería algo así como un manual de instrucciones que te dice como usar un servicio o un producto. Una buena analogía sería la del camarero, el cocinero y el cliente de un restaurante. En un restaurante, el camarero no sabe cocinar los platos de la carta, pero si sabe trasmitir el mensaje desde el cliente hasta el cocinero, y llevar el resultado del mensaje desde el cocinero hasta el cliente. En esta analogía tú serías el cliente (client), el cocinero sería el servidor (server) y el camarero sería la API.

En nuestro caso, la API funcionara a través de internet, por lo que entra en juego el protocolo HTTP. Este procolo nos permite transmitir información por internet usando un sistema de peticiones y respuestas.

No vamos a profundizar mucho en el tema de HTTP porque es bastante extenso. Nosotros como clientes solo utilizaremos para este proyecto la solicitud GET, que se usan solo para recuperar datos.

# Materiales
-NodeMcu: ESP8266 12-E
-MAX7219 de 8x8 LED´s (x8)
-Wire Jumpers hembra-hembra (x5)

# Librerías
1. ESP8266WiFi.h
		Librería necesaria para conectarse a la red WiFi. Puedes descargarla [aquí](https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/src/ESP8266WiFi.h "aquí")
1. ESP8266WiFiMulti.h
		Librería por si queremos tener varias redes posibles a las que conectarnos y que se conecte automáticamente a la que mejor señal tenga. Puedes descargarla [aquí](https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/src/ESP8266WiFiMulti.h "aquí").
1. ESP8266HTTPClient.h
		Nos permitirá usar ESP8266 como cliente. Puedes descargarla [aquí](https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266HTTPClient "aquí")
1. MD_Parola.h
		Librería para controlar matrices de LED´s conectadas entre si. Puedes descargarla [aquí](https://github.com/MajicDesigns/MD_Parola "aquí")
1. MD_MAX72xx.h
		Librería para controlar una matriz de LED´s de 8x8. Puedes descargarla [aquí](https://github.com/MajicDesigns/MD_MAX72XX "aquí")
1. SPI.h
		Librería necesaria para la comunicación con dispositivos SPI

# Código
Primero comenzaremos incluyendo las librerías que controlan la conexión WiFi y HTTP:

```C++
#include <ESP8266WiFi.h> 
#include <ESP8266WiFiMulti.h> 
#include <ESP8266HTTPClient.h> 
            ```
Seguimos incluyendo las libreías para el control de la matriz de LED´s:
```C++
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
```

Vamos a definir las variables necesarias para el correcto funcionamiento de las bibliotecas, así como los puertos en los que conectaremos la matriz de LED´s al NodeMcu. El tipo de hardware que tenemos lo podemos consultar en la librería MD_MAX7219. Puedes consultarla [aquí](file:///C:/Users/stief/Documents/Arduino/libraries/MD_MAX72XX/docs/class_m_d___m_a_x72_x_x.html "aquí"). Recordad que una matriz de 8x8 se considera un solo dispositivo.
```C++
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 8// 
//--------Pines para ESP8266--------//
#define CLK_PIN   14
#define DATA_PIN  13
#define CS_PIN    2
```
Creamos ahora un objeto para la comunicación con el hardware:
```C++
MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);```


Definimos las variables que almacenaran el nombre y la contraseña de la red WiFi a la que nos conectaremos y creamos el objeto ESP8266WiFiMulti.
```C++
const char *ssid     = "Nombre de tu red";
const char *password = "Contraseña de tu red";
ESP8266WiFiMulti WiFiMulti;
```
En la función setup() iniciaremos la comunicación serial, aunque solo la usaremos a modo de depuración. También iniciaremos comunicación con la matriz de LED´s, conectaremos el NodeMcu a la red WiFi (recordad que con la librería WifiMulti podríamos poner varias redes a las que conectarnos):
```C++
void setup() {
  Serial.begin(115200);
  P.begin();
  
  for(uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(ssid,password);
}
```

En la función loop() vamos a seguir la siguiente dinámica. Vamos a crear un objeto HTTPClient por cada consulta que queramos hacer (uno para el precio actual del ether, otro para el precio máximo, otro para el mínimo...).
Para obtener una url con los datos que queremos, hemos hecho uso de thigspeak. En un anexo explicaré detalladamente como lo he realizado.
Una vez tenemos creado el objeto y la url, llamamos al método begin y le pasamos la url a la que nos conectaremos. Le mandamos una solicitud http get. La respuesta del servidor incluirá un código numérico que nos dará información sobre si la solicitud a culminado correctamente (código 200) o si ha habído algun error. Los códigos posibles de respuesta los podéis encontrar [aquí](https://developer.mozilla.org/es/docs/Web/HTTP/Status "aquí"). Si todo ha ido bien y el código que hemos recibido es satisfactorio, guardaremos en una variable string el dato que hemos pedido al servidor haciendo uso de la función getString() y lo imprimiremos por la matriz de LED´s y por serial (a modo de depuración). Tal y como he indicado anteriormente, repetiremos este proceso cada vez que queramos tomar un dato de un servidor.
```C++
void loop() {
  if((WiFiMulti.run() == WL_CONNECTED)) {//Si se conecta bien a la red wifi(si hay varias se conectará a la más estable)

    ///----------------------------------------OBJETO 0: PRECIO ETHER---------------------////
    
    HTTPClient http;
    Serial.println("[HTTP] begin...");
    http.begin("api.thingspeak.com", 80,"/apps/thinghttp/send_request?api_key=IIZMBB3Q8P1KM44Z"); 
    int httpCode = http.GET();
    
    Serial.println("[HTTP] Get...");

    if(httpCode > 0){
      Serial.printf("[HTTP] GET... code: %d",httpCode);
      if(httpCode = HTTP_CODE_OK){ 
        Serial.print("Precio de Ether: ");
        String precio_ether = http.getString();
        Serial.println(http.getString());
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
    
    if(httpCode1 > 0){
      Serial.printf("[HTTP] GET... code: %d",httpCode1);
      if(httpCode1 = HTTP_CODE_OK){
        Serial.print("24 h:  ");
        String minim = http1.getString();
        String veinticuatrohoras = "Min:";
        Serial.println(http1.getString());
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
    
    if(httpCode2 > 0){
      Serial.printf("[HTTP] GET... code: %d",httpCode2);
      if(httpCode2 = HTTP_CODE_OK){
        Serial.print("Máximo 24 horas: ");
        Serial.println(http2.getString());
        String preciomaxi = http2.getString();
        String veinticuatrohoras = "Max:";
        Serial.println(http2.getString());
        veinticuatrohoras += preciomaxi;
        P.print(veinticuatrohoras);
      }
    }
    
    else{
      Serial.printf("[HTTP] GET... Failed, error: %s\n", http.errorToString(httpCode2).c_str());
    }
    delay(1000);

    ///----------------------------------------OBJETO 3: PRECIO AVAX---------------------////
    
    HTTPClient http3; 
    Serial.println("[HTTP] begin...");
    http3.begin("api.thingspeak.com", 80,"/apps/thinghttp/send_request?api_key=ADAFV6I3IH0QULO0");
    int httpCode3 = http3.GET();.
    
    Serial.println("[HTTP] Get...");

    if(httpCode3 > 0){
      Serial.printf("[HTTP] GET... code: %d",httpCode3);
      if(httpCode3 = HTTP_CODE_OK){
        Serial.print("Precio de Avax: ");
        String precio_avax = http3.getString();
        Serial.println(http3.getString());
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
    
    if(httpCode4 > 0){
      Serial.printf("[HTTP] GET... code: %d",httpCode4);o
      if(httpCode1 = HTTP_CODE_OK){
        Serial.print("24 h:  ");
        String minim = http4.getString();
        String veinticuatrohoras = "Min: ";
        Serial.println(http4.getString());
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
    
    if(httpCode5 > 0){
      Serial.printf("[HTTP] GET... code: %d",httpCode5)
      if(httpCode5 = HTTP_CODE_OK){
        Serial.print("Máximo 24 horas: ");
        Serial.println(http5.getString());
        String preciomaxi = http5.getString();
        String veinticuatrohoras = "Max: ";
        Serial.println(http5.getString());
        veinticuatrohoras += preciomaxi;
        P.print(veinticuatrohoras);
      }
    }
    
    else{
      Serial.printf("[HTTP] GET... Failed, error: %s\n", http.errorToString(httpCode5).c_str());
    }
    delay(1000);

    ///----------------------------------------OBJETO 6: PRECIO ATOM---------------------////
    
    HTTPClient http6;
    Serial.println("[HTTP] begin...");
    http6.begin("api.thingspeak.com", 80,"/apps/thinghttp/send_request?api_key=WWODXHX80FNQP2SE"); 
    int httpCode6 = http6.GET();
    
    Serial.println("[HTTP] Get...");

    if(httpCode6 > 0){
      Serial.printf("[HTTP] GET... code: %d",httpCode6);
      if(httpCode6 = HTTP_CODE_OK){
        Serial.print("Precio de Atom: ");
        String precio_atom = http6.getString();
        Serial.println(http.getString());
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
    
    if(httpCode7 > 0){
      Serial.printf("[HTTP] GET... code: %d",httpCode7);
      if(httpCode7 = HTTP_CODE_OK){
        Serial.print("24 h:  ");
        String minim = http7.getString();
        String veinticuatrohoras = "Min: ";
        Serial.println(http7.getString());
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
    
    if(httpCode8 > 0){
      Serial.printf("[HTTP] GET... code: %d",httpCode8);
      if(httpCode8 = HTTP_CODE_OK){
        Serial.print("Máximo 24 horas: ");
        Serial.println(http8.getString());
        String preciomaxi = http8.getString();
        String veinticuatrohoras = "Max: ";
        Serial.println(http8.getString());
        veinticuatrohoras += preciomaxi;
        P.print(veinticuatrohoras);
      }
    }
    
    else{
      Serial.printf("[HTTP] GET... Failed, error: %s\n", http.errorToString(httpCode8).c_str());
    }
    delay(1000);

    ///----------------------------------------OBJETO 9: PRECIO POLKADOT---------------------////
    
    HTTPClient http9;
    Serial.println("[HTTP] begin...");
    http9.begin("api.thingspeak.com", 80,"/apps/thinghttp/send_request?api_key=F6UKDK0UGTRN770J"); 
    int httpCode9 = http9.GET();
    
    Serial.println("[HTTP] Get...");

    if(httpCode9 > 0){
      Serial.printf("[HTTP] GET... code: %d",httpCode9);
      if(httpCode9 = HTTP_CODE_OK){
        Serial.print("Precio de polkadot: ");
        String precio_dot = http9.getString();
        Serial.println(http9.getString());
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
    
    if(httpCode10 > 0){
      Serial.printf("[HTTP] GET... code: %d",httpCode10);
      if(httpCode1 = HTTP_CODE_OK){
        Serial.print("24 h:  ");
        String minim = http10.getString();
        String veinticuatrohoras = "Min:";
        Serial.println(http10.getString());
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
    
    if(httpCode11 > 0){
      Serial.printf("[HTTP] GET... code: %d",httpCode11);
      if(httpCode11 = HTTP_CODE_OK){
        Serial.print("Máximo 24 horas: ");
        Serial.println(http11.getString());
        String preciomaxi = http11.getString();
        String veinticuatrohoras = "Max:";
        Serial.println(http11.getString());
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
```

