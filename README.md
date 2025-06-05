# 🌤️ Projeto ESP32 - Monitoramento Climático via HTTP

Este projeto utiliza um ESP32 fisico conectado à internet para consultar dados da API OpenWeatherMap e enviar os valores de **temperatura**, **umidade**, **pressão atmosférica** e **velocidade do vento** para a plataforma **ThingSpeak** e para o **Serial Monitor**.

## 📋 Funcionalidades

- Conexão Wi-Fi com ESP32
- Requisição HTTP para API OpenWeatherMap
- Extração de dados em formato JSON
- Envio de dados para ThingSpeak via HTTP
- Monitoramento periódico a cada 5 minutos

## 🚀 Configuração

### Pré-requisitos

- Arduino IDE
- Placa ESP32 configurada
- Bibliotecas:
  - WiFi
  - HTTPClient
  - Arduino_JSON

### Configuração do Código

1. Alterar a rede wifi e a senha.
2. Criar um novo canal no thingspeak.
   2.2. adicionar os campos: temperatura, umidade, pressão e velocidade do vento.
3. Após isso alterar as API Keys do codigo, para se conectar com o seu thingspeak.
   
