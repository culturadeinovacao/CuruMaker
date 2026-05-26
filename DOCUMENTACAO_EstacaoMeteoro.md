# Estação Meteorológica Maker — Guia do Colaborador

> **Projeto:** Rede Ciência Cidadã · Parque Tecnológico de Santo André  
> **Autor original:** Matheus Valadares Teixeira  
> **Plataforma:** Arduino UNO R4 (compatível com R3 com ajustes)  
> **Arquivo principal:** `Arduino_R4_test.ino`

---

## Índice

1. [Visão Geral](#1-visão-geral)
2. [Configuração e Instalação](#2-configuração-e-instalação)
3. [Exemplos de Uso](#3-exemplos-de-uso)
4. [Referência da API](#4-referência-da-api)
5. [Armadilhas Comuns e FAQ](#5-armadilhas-comuns-e-faq)
6. [Solução de Problemas](#6-solução-de-problemas)
7. [Sugestões de Melhoria para Contribuidores](#7-sugestões-de-melhoria-para-contribuidores)

---

## 1. Visão Geral

### O que este código faz

Este firmware transforma um Arduino em uma **estação meteorológica de baixo custo**, capaz de medir e exibir em tempo real:

| Grandeza             | Sensor          | Saída          |
|----------------------|-----------------|----------------|
| Temperatura do ar    | DHT11           | °C (1 decimal) |
| Umidade relativa do ar | DHT11         | % (1 decimal)  |
| Umidade do solo      | Capacitivo + A0 | % (inteiro)    |
| Luminosidade         | GY-30 / BH1750  | lux (inteiro)  |

Todos os dados são exibidos em um **display OLED 128×64** e transmitidos pela **porta serial** (para log ou integração futura).

### Por que este código existe

O projeto faz parte da **Rede Ciência Cidadã**, uma iniciativa de ciência participativa que distribui estações meteorológicas maker para escolas e espaços públicos. O objetivo é coletar dados ambientais hiper-locais — algo que as redes meteorológicas oficiais raramente fornecem em nível de bairro.

O código foi projetado com simplicidade intencional para ser replicável por educadores e estudantes com pouca experiência em eletrônica. Contribuidores são encorajados a expandir funcionalidades (conectividade Wi-Fi, armazenamento em SD card, envio para APIs) sem quebrar essa acessibilidade.

### Fluxo de execução resumido

```
setup()
  └─ Inicializa Serial, Wire (I2C), OLED, DHT11, BH1750

loop() — a cada 2 segundos:
  ├─ Lê temperatura e umidade do ar (DHT11)
  ├─ Lê luminosidade (BH1750 via I2C)
  ├─ Lê umidade do solo (ADC + mapeamento linear)
  ├─ Valida leituras (isnan check)
  ├─ Atualiza display OLED
  └─ Aguarda 2000 ms
```

---

## 2. Configuração e Instalação

### 2.1 Pré-requisitos de hardware

| Componente                   | Modelo testado       | Interface | Pino / Endereço |
|------------------------------|----------------------|-----------|-----------------|
| Microcontrolador             | Arduino UNO R4       | —         | —               |
| Sensor de temperatura/umidade| DHT11                | Digital   | Pino 2          |
| Sensor de luminosidade       | GY-30 (BH1750)       | I2C       | 0x23 (padrão)   |
| Display OLED                 | SSD1306 128×64       | I2C       | 0x3C (padrão)   |
| Sensor de umidade do solo    | Capacitivo (genérico)| Analógico | A0              |

> **Atenção:** Módulos com endereço I2C diferente (ex.: display em `0x3D`) exigem alteração no código. Ver [Solução de Problemas](#6-solução-de-problemas).

### 2.2 Diagrama de conexões

```
Arduino         DHT11
D2 ──────────── DATA
5V ──────────── VCC
GND ─────────── GND

Arduino         SSD1306 (OLED)     GY-30 (BH1750)
A4 (SDA) ─────── SDA ──────────── SDA
A5 (SCL) ─────── SCL ──────────── SCL
3.3V ──────────── VCC ──────────── VCC
GND ─────────── GND ──────────── GND

Arduino         Sensor de Solo (Capacitivo)
A0 ──────────── AOUT
5V ──────────── VCC
GND ─────────── GND
```

> No Arduino UNO R4 WiFi/Minima, os pinos I2C permanecem em A4/A5 para compatibilidade com shields R3.

### 2.3 Dependências de software

Instale as seguintes bibliotecas pela **Arduino IDE** (Sketch → Incluir Biblioteca → Gerenciar Bibliotecas):

| Biblioteca         | Versão mínima testada | Instalação (nome exato no gerenciador) |
|--------------------|-----------------------|----------------------------------------|
| `Adafruit GFX`     | 1.11.x                | `Adafruit GFX Library`                 |
| `Adafruit SSD1306` | 2.5.x                 | `Adafruit SSD1306`                     |
| `DHT sensor library` | 1.4.x               | `DHT sensor library` (by Adafruit)     |
| `BH1750`           | 1.3.x                 | `BH1750` (by Christopher Laws)         |
| `Wire`             | —                     | Nativa do Arduino (não instalar)       |

### 2.4 Calibração do sensor de solo (obrigatória)

O sensor capacitivo de solo **precisa ser calibrado para o seu hardware específico** antes do primeiro uso.

**Passo a passo:**

1. Faça o upload do código e abra o **Monitor Serial** (9600 baud).
2. Deixe o sensor suspenso no ar por 30 segundos e anote o valor impresso para `soloRaw`.
3. Mergulhe o sensor em um copo de água até a linha de marcação e anote o novo valor.
4. Edite as constantes no início do arquivo:

```cpp
const int VALOR_SECO  = 1020;  // ← substitua pelo valor lido no ar
const int VALOR_UMIDO = 410;   // ← substitua pelo valor lido na água
```

> Valores fora do intervalo `[0, 100]` são automaticamente limitados pela função `constrain()`, mas leituras fora da faixa calibrada indicam necessidade de recalibração.

---

## 3. Exemplos de Uso

### Exemplo 1 — Operação básica (sem alterações)

Faça o upload do código com as configurações padrão e abra o Monitor Serial. Você verá as leituras a cada 2 segundos:

**Saída esperada no Serial:**
```
GY-30 inicializado com sucesso.
```
*(Leituras de erro do DHT11, se houver, aparecerão aqui também)*

**Saída no display OLED:**
```
   MONITOR DE SENSORES
────────────────────────
Temp. Ar: 24.5 C
Umid. Ar: 68.0 %
Umid. Solo: 42 %
Luminos.: 312 lx
```

---

### Exemplo 2 — Adicionando log detalhado no Serial

Para depuração ou registro em CSV, adicione as seguintes linhas ao final do `loop()`, após o bloco de atualização do OLED:

```cpp
// Adicionar no final de loop(), antes de delay(2000)
Serial.print("TEMP:");
Serial.print(tempAr, 1);
Serial.print(",UMID_AR:");
Serial.print(umidadeAr, 1);
Serial.print(",UMID_SOLO:");
Serial.print(umidadeSolo);
Serial.print(",LUX:");
Serial.println(lux, 0);
```

**Saída no Serial:**
```
TEMP:24.5,UMID_AR:68.0,UMID_SOLO:42,LUX:312
TEMP:24.6,UMID_AR:67.8,UMID_SOLO:43,LUX:305
```

Esse formato facilita a importação em planilhas ou sistemas de monitoramento como o **Node-RED** e o **Grafana** via serial-to-MQTT bridge.

---

### Exemplo 3 — Ajustando o intervalo de leitura e habilitando alertas

Para aplicações que precisam de mais dados por minuto (ex.: experimento em sala de aula) ou de alertas visuais, modifique o `loop()`:

```cpp
// Troque o delay fixo por uma constante configurável no topo do arquivo:
#define INTERVALO_MS 2000  // DHT11 requer mínimo 2000 ms

// No loop():
if (umidadeSolo < 20) {
  // Alerta visual: pisca a tela 3 vezes se solo muito seco
  for (int i = 0; i < 3; i++) {
    display.invertDisplay(true);
    delay(200);
    display.invertDisplay(false);
    delay(200);
  }
}

delay(INTERVALO_MS);
```

> **Atenção:** O DHT11 tem taxa de amostragem máxima de 1 Hz (uma leitura por segundo). Valores abaixo de `1000 ms` no delay causarão leituras `NaN` consecutivas. Use o DHT22 se precisar de leituras mais frequentes.

---

## 4. Referência da API

Este firmware não expõe uma API pública no sentido convencional — toda a lógica está contida nas duas funções padrão do Arduino. A documentação abaixo descreve os pontos de extensão mais relevantes para contribuidores.

---

### `setup()` — Inicialização do sistema

**Descrição:** Executada uma única vez ao ligar o Arduino. Inicializa todos os periféricos na seguinte ordem: Serial → I2C (Wire) → OLED → DHT11 → BH1750.

**Comportamento em falha:**

| Falha                        | Comportamento                                           |
|------------------------------|---------------------------------------------------------|
| Display OLED não encontrado  | Trava em loop infinito (`for(;;)`) + mensagem no Serial |
| BH1750 não encontrado        | Mensagem de erro no Serial, execução continua          |
| DHT11 com leitura inválida   | Detectado no `loop()`, não interrompe o `setup()`      |

**Variáveis configuradas nesta função:**

```cpp
display.setTextColor(SSD1306_WHITE);  // Cor fixa para display monocromático
display.setTextSize(1);               // Tamanho de fonte padrão (6×8 px por caractere)
```

---

### `loop()` — Ciclo principal de leitura e exibição

**Descrição:** Executada continuamente. A cada iteração: lê os três sensores, valida os dados, atualiza o OLED e aguarda 2 segundos.

**Variáveis locais relevantes:**

| Variável      | Tipo    | Fonte                   | Descrição                                  |
|---------------|---------|-------------------------|--------------------------------------------|
| `umidadeAr`   | `float` | `dht.readHumidity()`    | Umidade relativa do ar em %                |
| `tempAr`      | `float` | `dht.readTemperature()` | Temperatura do ar em °C                    |
| `lux`         | `float` | `lightMeter.readLightLevel()` | Luminosidade em lux                  |
| `soloRaw`     | `int`   | `analogRead(SOIL_PIN)`  | Valor ADC bruto (0–1023)                   |
| `umidadeSolo` | `int`   | `map()` + `constrain()` | Umidade do solo mapeada para 0–100%        |

**Tratamento de erro do DHT11:**

```cpp
if (isnan(umidadeAr) || isnan(tempAr)) {
    // Valores são zerados — o display mostrará 0.0 em vez de NaN
    tempAr    = 0;
    umidadeAr = 0;
}
```

> **Nota para contribuidores:** Zerar os valores em caso de erro pode ser enganoso para o usuário final (0 °C e 0% são valores plausíveis). Considere exibir `"ERR"` no display nesses casos.

---

### Constantes e pinos configuráveis

```cpp
#define DHTPIN        2       // Pino digital do DHT11
#define DHTTYPE       DHT11   // Modelo: DHT11 ou DHT22
#define SCREEN_WIDTH  128     // Largura do OLED em pixels
#define SCREEN_HEIGHT 64      // Altura do OLED em pixels
#define OLED_RESET    -1      // -1 = compartilha reset com o Arduino

const int SOIL_PIN    = A0;   // Pino analógico do sensor de solo
const int VALOR_SECO  = 1020; // Valor ADC no ar seco (calibrar!)
const int VALOR_UMIDO = 410;  // Valor ADC na água (calibrar!)
```

---

## 5. Armadilhas Comuns e FAQ

### ❓ O display OLED não acende e o código trava

**Causa mais comum:** O endereço I2C do display não é `0x3C`. Alguns módulos usam `0x3D`.

**Diagnóstico:** Faça o upload do seguinte sketch de escaneamento I2C e verifique o endereço no Serial:

```cpp
#include <Wire.h>
void setup() {
  Wire.begin();
  Serial.begin(9600);
  for (byte addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0)
      Serial.println(addr, HEX);
  }
}
void loop() {}
```

**Solução:** Altere `0x3C` para o endereço encontrado na linha:
```cpp
display.begin(SSD1306_SWITCHCAPVCC, 0x3C)  // ← altere aqui
```

---

### ❓ O sensor de solo sempre mostra 0% ou 100%

**Causa:** Os valores `VALOR_SECO` e `VALOR_UMIDO` não foram calibrados para o hardware em uso.

**Solução:** Siga o [procedimento de calibração](#24-calibração-do-sensor-de-solo-obrigatória). Cada par sensor/placa tem variação diferente de fábrica — os valores padrão (1020/410) são apenas um ponto de partida.

---

### ❓ O DHT11 exibe "Falha na leitura" frequentemente

**Causas possíveis:**

1. Cabo entre Arduino e DHT11 comprido demais (>50 cm sem resistor pull-up).
2. `delay(2000)` foi reduzido abaixo de 2 segundos.
3. Alimentação instável (3.3V em vez de 5V).
4. Sensor com defeito ou pinos invertidos.

**Solução para cabo longo:** Adicione um resistor de 4.7 kΩ entre o pino DATA e o VCC do DHT11.

---

### ❓ Posso usar DHT22 em vez de DHT11?

Sim. Altere as duas linhas abaixo:

```cpp
#define DHTPIN  2
#define DHTTYPE DHT22   // ← era DHT11
```

O DHT22 oferece maior precisão (±0.5°C vs ±2°C) e permite leituras a cada segundo (vs 2 segundos no DHT11). Neste caso, o `delay(2000)` pode ser reduzido para `delay(1000)`.

---

### ❓ O BH1750 retorna `0 lx` mesmo com luz ambiente

**Causa:** O módulo GY-30 pode estar com o jumper `ADDR` solto, resultando em endereço I2C `0x5C` em vez de `0x23` (padrão da biblioteca).

**Solução:** Especifique o endereço na inicialização:

```cpp
lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, 0x5C);
```

---

## 6. Solução de Problemas

### 🔴 "Falha ao inicializar a tela OLED" — código travado

**Sintomas:** O LED do Arduino pisca uma vez e o código não prossegue.

**Checklist:**
- [ ] Cabos SDA/SCL conectados corretamente (A4=SDA, A5=SCL)?
- [ ] Display alimentado em 3.3V ou 5V conforme especificação do módulo?
- [ ] Endereço I2C correto? (execute o scanner I2C descrito acima)
- [ ] Outro dispositivo I2C em conflito de endereço? (improvável com OLED+BH1750)

---

### 🟡 "Erro ao inicializar o GY-30" — execução continua mas sem luminosidade

**Sintomas:** Serial exibe `"Erro ao inicializar o GY-30."`, display mostra `0 lx`.

**Checklist:**
- [ ] O GY-30 compartilha os mesmos fios SDA/SCL do OLED?
- [ ] Alimentação: o módulo aceita 3.3V ou 5V?
- [ ] Tente `lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, 0x5C)`.

---

### 🟡 Leituras do DHT11 sempre 0.0°C e 0.0%

**Sintomas:** Nenhuma mensagem de erro visível no display, mas valores são sempre zero.

**Causa:** O tratamento de erro atual substitui valores inválidos por `0` silenciosamente.

**Diagnóstico:** Abra o Monitor Serial. Se aparecer `"Falha na leitura do sensor DHT11!"` repetidamente, o sensor não está sendo lido corretamente. Siga as dicas da [FAQ acima](#-o-dht11-exibe-falha-na-leitura-frequentemente).

---

### 🟡 Umidade do solo com valores negativos ou acima de 100%

**Sintomas:** O `constrain()` esconde o problema, mas o valor bruto no Serial indica leitura fora da faixa.

**Causa:** Sensor inserido em solo mais úmido do que a referência de calibração (copo de água), ou sensor danificado.

**Solução:** Recalibre com o solo mais úmido que você espera encontrar como referência `VALOR_UMIDO`.

---

### 🟡 Display com texto cortado ou deslocado

**Sintomas:** Última linha de dados não aparece no OLED.

**Causa:** O display SSD1306 de 128×64 com fonte tamanho 1 comporta no máximo **8 linhas de texto** (8 px por linha). O layout atual usa 6 linhas com espaçamento, mas `setCursor` mal calculado pode causar overflow.

**Solução:** Verifique as coordenadas Y em `setCursor(x, y)`. Cada linha de texto ocupa 8 pixels. A linha mais baixa visível começa em `y = 56`.

---

## 7. Sugestões de Melhoria para Contribuidores

As observações abaixo foram identificadas durante a documentação deste código. Elas não são bloqueantes, mas tornariam o projeto mais robusto e legível:

### 7.1 Nomenclatura inconsistente (mistura de português e inglês)

Variáveis como `lux`, `soloRaw` e `SOIL_PIN` misturam inglês com `umidadeAr`, `tempAr` e `VALOR_SECO` em português. Para um projeto de ciência cidadã voltado a educadores brasileiros, recomenda-se padronizar tudo em português:

```cpp
// Atual (misto):
float lux = lightMeter.readLightLevel();
int   soloRaw = analogRead(SOIL_PIN);

// Sugerido (consistente):
float luminosidade = lightMeter.readLightLevel();
int   leituraBrutaSolo = analogRead(PINO_SOLO);
```

### 7.2 Tratamento de erro silencioso no DHT11

Substituir valores inválidos por `0` sem aviso visual no display pode passar despercebido. Sugestão:

```cpp
if (isnan(umidadeAr) || isnan(tempAr)) {
    display.print("Temp: ERRO");  // Em vez de mostrar 0.0
    Serial.println(F("Falha na leitura do sensor DHT11!"));
    return;  // Pula atualização do display para este ciclo
}
```

### 7.3 Ausência de timestamp nas leituras

Para uso científico, cada leitura deveria ter um registro de tempo. Como o Arduino R4 possui RTC interno, é possível adicionar data/hora ao log serial sem hardware adicional:

```cpp
#include "RTC.h"  // Disponível no Arduino UNO R4
// Permite registrar: "2024-06-01 14:32:05,TEMP:24.5,..."
```

### 7.4 Nome do arquivo pouco descritivo

`Arduino_R4_test.ino` sugere código temporário de teste. Para um repositório open source, recomenda-se renomear para algo como `estacao_meteorologica.ino` ou `weather_station_r4.ino`.

---

*Documentação gerada para contribuidores do projeto Rede Ciência Cidadã — Parque Tecnológico de Santo André.*
