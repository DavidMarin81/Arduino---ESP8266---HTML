//Placa: "LOLIN(WEMOS) D1 mini Pro"

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Definición de los objetos para WiFi y servidor web
const char* ssid = "MIWIFI_2G_VXSS";
const char* password = "rQdpFtNL";
ESP8266WebServer server(80);

// Definición de los pines del sensor ultrasónico
const int TRIG_PIN = D7;
const int ECHO_PIN = D6;

// Variables para la distancia medida
long distancia = 0;

// Archivo HTML como una cadena de caracteres
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

void setup() {
  Serial.begin(115200);

  // Inicialización de la conexión WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Espera a que la conexión WiFi esté establecida
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("Dirección IP: ");
  Serial.println(WiFi.localIP());

  // Configuración de los pines del sensor ultrasónico
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Configuración de la ruta para mostrar la distancia en la página web
  server.on("/", HTTP_GET, handleRoot);
  server.begin();
  Serial.println("Servidor web iniciado");
}

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

// Función para obtener la página HTML con la distancia actualizada
String obtenerPaginaHTML() {
  // Reemplazar la etiqueta %DISTANCIA% con el valor actual de la distancia medida
  String pagina = paginaHTML;
  pagina.replace("%DISTANCIA%", String(distancia));
  return pagina;
}

// Función para manejar la petición de la página web
void handleRoot() {
  server.send(200, "text/html", obtenerPaginaHTML());
}
