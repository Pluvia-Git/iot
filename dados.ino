#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
 
// === CREDENCIAIS WI-FI ===
const char* SECRET_SSID = "Redmi Note 11S"; // Sua Internet
const char* SECRET_PW = "18042006"; // Sua Senha 
 
// === API KEYS ===
String apiKey = "03a99da0a4af3fad38e4a44d0a540046";      // OpenWeatherMap
String thingSpeakApiKey = "X0VWC4IT66F7BLGO";              // ThingSpeak
 
// === URLs ===
const char* thingSpeakServer = "http://api.thingspeak.com/update";
 
// === Timer ===
unsigned long lastTime = 0;
unsigned long timerDelay = 300000;  // 5 minutos (em milissegundos)
 
// === JSON buffer ===
String jsonBuffer;
 
void setup() {
  Serial.begin(115200);
  WiFi.begin(SECRET_SSID, SECRET_PW);
  Serial.println("Conectando...");
  verificaWiFi();
  Serial.println("Timer programado para 5 minutos. Aguarde esse tempo para a leitura...");
}
 
void loop() {
  if ((millis() - lastTime) > timerDelay) {
    if (WiFi.status() == WL_CONNECTED) {
      // Requisição para OpenWeatherMap (em °C)
      String serverPath = "http://api.openweathermap.org/data/2.5/weather?q=S%C3%A3o%20Paulo,BR&APPID=" + apiKey + "&units=metric";
      jsonBuffer = httpGETRequest(serverPath.c_str());
      JSONVar myObject = JSON.parse(jsonBuffer);
 
      if (JSON.typeof(myObject) == "undefined") {
        Serial.println("Falha no formato dos dados!");
        return;
      }
 
      // Extração dos dados
      float temperatura = (double)myObject["main"]["temp"];
      int umidade = (int)myObject["main"]["humidity"];
      float vento = (double)myObject["wind"]["speed"];
      int pressao = (int)myObject["main"]["pressure"];
 
      Serial.println("========================================================================");
      Serial.print("Temperatura (°C): ");
      Serial.println(temperatura);
      Serial.print("Umidade (%): ");
      Serial.println(umidade);
      Serial.print("Velocidade do Vento (m/s): ");
      Serial.println(vento);
      Serial.print("Pressão (hPa): ");
      Serial.println(pressao);
      Serial.println("========================================================================");
 
      // Monta a URL para enviar ao ThingSpeak
      String url = String(thingSpeakServer) +
                   "?api_key=" + thingSpeakApiKey +
                   "&field1=" + String(temperatura) +
                   "&field2=" + String(umidade) +
                   "&field3=" + String(vento) +
                   "&field4=" + String(pressao);
 
      // Envia requisição GET para ThingSpeak
      String response = httpGETRequest(url.c_str());
      Serial.print("Resposta do ThingSpeak: ");
      Serial.println(response);
    } else {
      Serial.println("WiFi desconectado");
    }
    lastTime = millis();  // Reinicia o timer
  }
}
 
// === Função genérica para requisições HTTP GET ===
String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
  http.begin(client, serverName);
  int httpResponseCode = http.GET();
  String payload = "{}";
 
  if (httpResponseCode > 0) {
    Serial.print("Código de resposta HTTP: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  } else {
    Serial.print("Erro na requisição: ");
    Serial.println(httpResponseCode);
  }
  http.end();
  return payload;
}
 
// === Verifica conexão WiFi e reconecta se necessário ===
void verificaWiFi() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Tentando conectar à rede SSID: ");
    Serial.println(SECRET_SSID);
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(SECRET_SSID, SECRET_PW);
      Serial.print(".");
      delay(2000);
    }
    Serial.println("\nConectado com sucesso!");
    Serial.print("Endereço IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("MAC Address: ");
    Serial.println(WiFi.macAddress());
    Serial.println();
  }
}
 
 