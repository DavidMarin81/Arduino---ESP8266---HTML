### Conectar Arduino con ESP8266 y archivo HTML

- Arduino: "LOLIN(WEMOS) D1 mini Pro"
- ESP8266
- Sensor Ultrasónico

Se incluyen las librerias:

```c++
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
```

Se indica SSID y PASSWORD:

```c++
// Definición de los objetos para WiFi y servidor web
const char* ssid = "Nombre de la Wifi";
const char* password = "Contraseña";
```

Se crea una instancia de un servidor web en un ESP8266
```c++
// Para manejar solicitudes HTTP entrantes y enviar respuestas HTTP salientes
ESP8266WebServer server(80);
```


Se definen las variables
```c++
// Definición de los pines del sensor ultrasónico
const int TRIG_PIN = D7;
const int ECHO_PIN = D6;

// Variables para la distancia medida
long distancia = 0;
```

Se crea un archivo HTML con el método R"x()x";
```c++
// Archivo HTML como una cadena de caracteres
// R"x()x"; -> Se usa para no escapar caracteres especiales
// La x puede ser cualquier etiqueta. Por convencion, pondremos html
const char* paginaHTML = R"html(
  <!DOCTYPE html>
  <html>
  <head>
    <title>Distancia del sensor</title>
    <meta http-equiv="refresh" content="1"> <!-- Esto recargará la página cada segundo -->
  </head>
  <body>
    <h1>Distancia medida:</h1>
    <p id="distancia">%DISTANCIA%</p>
  </body>
  </html>
)html";
```

### En el setup()

Se inicia la conexión Wifi
```c++
// Inicialización de la conexión WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
```

Esperamos a que la conexión esté establecida 
    
 - Puede ser:
    - WL_CONNECTED
    - WL_DISCONNECTED
    - WL_CONNECT_FAILED
```c++
// Espera a que la conexión WiFi esté establecida
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
```

Si se conecta, mostramos lo siguiente:
```c++
    Serial.println("");
    Serial.println("WiFi conectado");
    Serial.println("Dirección IP: ");
    Serial.println(WiFi.localIP());
```

Configuramos los pines del sensor ultrasonico
```c++
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
```

Configuramos la ruta para mostrar la distancia en la pagina web

```c++
  server.on("/", HTTP_GET, handleRoot);
  server.begin();
  Serial.println("Servidor web iniciado");
```

### Se crean dos funciones

Función para obtener la pagina HTML con la distancia actualizada
```c++
// Función para obtener la página HTML con la distancia actualizada
String obtenerPaginaHTML() {
  // Reemplazar la etiqueta %DISTANCIA% con el valor actual de la distancia medida
  String pagina = paginaHTML;
  pagina.replace("%DISTANCIA%", String(distancia));
  return pagina;
}
```

Función para manejar la petición de la página web
```c++
// Función para manejar la petición de la página web
void handleRoot() {
  server.send(200, "text/html", obtenerPaginaHTML());
}
```

### En el loop()
```c++
void loop() {
  // Lectura de la distancia del sensor ultrasónico
  long duracion;
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duracion = pulseIn(ECHO_PIN, HIGH);
  distancia = duracion * 0.034 / 2; // Convertir a centímetros

  server.send(200, "text/html", obtenerPaginaHTML());

  // Control del servidor web
  server.handleClient();
}
```









