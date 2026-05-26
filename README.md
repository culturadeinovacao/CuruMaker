# 🌦️ Estação Meteorológica Maker

<div align="center">

![Arduino](https://img.shields.io/badge/Arduino-UNO_R4-00979D?style=for-the-badge&logo=arduino&logoColor=white)
![License](https://img.shields.io/badge/Licença-MIT-green?style=for-the-badge)
![Status](https://img.shields.io/badge/Status-Ativo-brightgreen?style=for-the-badge)

**Firmware open source para monitoramento ambiental de baixo custo**  
Parte da **Rede Ciência Cidadã** · Parque Tecnológico de Santo André

</div>

---

## 📖 Sobre o Projeto

A **Estação Meteorológica Maker** é um dispositivo embarcado que coleta e exibe dados ambientais em tempo real usando sensores acessíveis e um Arduino UNO R4. O projeto integra a **Rede Ciência Cidadã**, iniciativa que distribui estações para escolas e espaços públicos de Santo André com o objetivo de mapear dados climáticos em nível de bairro.

```
┌─────────────────────────────────────┐
│        MONITOR DE SENSORES          │
│─────────────────────────────────────│
│  Temp. Ar:    24.5 C                │
│  Umid. Ar:    68.0 %                │
│  Umid. Solo:  42 %                  │
│  Luminos.:    312 lx                │
└─────────────────────────────────────┘
```

---

## ✨ Funcionalidades

- 🌡️ **Temperatura do ar** — via sensor DHT11 (precisão ±2°C)
- 💧 **Umidade relativa do ar** — via sensor DHT11 (precisão ±5%)
- 🌱 **Umidade do solo** — via sensor capacitivo com calibração ajustável
- ☀️ **Luminosidade** — via módulo GY-30 / BH1750 (1–65535 lux)
- 🖥️ **Display OLED 128×64** — exibição local em tempo real
- 📡 **Log serial** — saída para integração com computadores ou gateways

---

## 🛒 Hardware Necessário

| Componente                    | Qtd | Estimativa (R$) |
|-------------------------------|-----|-----------------|
| Arduino UNO R4 (Minima ou WiFi) | 1 | ~R$ 120         |
| Sensor DHT11                  | 1   | ~R$ 10          |
| Módulo GY-30 (BH1750)         | 1   | ~R$ 15          |
| Display OLED 128×64 (SSD1306) | 1   | ~R$ 25          |
| Sensor de solo capacitivo     | 1   | ~R$ 12          |
| Jumpers e protoboard          | —   | ~R$ 15          |
| **Total estimado**            |     | **~R$ 197**     |

---

## ⚡ Início Rápido

### 1. Clone o repositório

```bash
git clone https://github.com/seu-usuario/estacao-meteorologica-maker.git
cd estacao-meteorologica-maker
```

### 2. Instale as bibliotecas na Arduino IDE

Vá em **Sketch → Incluir Biblioteca → Gerenciar Bibliotecas** e instale:

- `Adafruit GFX Library`
- `Adafruit SSD1306`
- `DHT sensor library` (by Adafruit)
- `BH1750` (by Christopher Laws)

### 3. Monte o circuito

```
Arduino D2   ──── DHT11 DATA
Arduino A4   ──── SDA (OLED + GY-30)
Arduino A5   ──── SCL (OLED + GY-30)
Arduino A0   ──── Sensor de solo (AOUT)
```

### 4. Calibre o sensor de solo

Edite as constantes no topo do arquivo `Arduino_R4_test.ino`:

```cpp
const int VALOR_SECO  = 1020;  // Valor lido no ar seco
const int VALOR_UMIDO = 410;   // Valor lido em água
```

> Veja o [Guia do Contribuidor](DOCUMENTACAO_EstacaoMeteoro.md) para instruções detalhadas de calibração.

### 5. Faça o upload

Selecione a placa **Arduino UNO R4 Minima** (ou WiFi) e a porta correta, depois clique em **Upload**.

---

## 📁 Estrutura do Repositório

```
estacao-meteorologica-maker/
├── Arduino_R4_test.ino          # Firmware principal
├── README.md                    # Este arquivo
├── DOCUMENTACAO_EstacaoMeteoro.md  # Guia completo do contribuidor
└── LICENSE
```

---

## 🗺️ Roadmap

- [x] Leitura de temperatura e umidade do ar (DHT11)
- [x] Leitura de luminosidade (BH1750)
- [x] Leitura de umidade do solo (capacitivo)
- [x] Exibição em display OLED
- [ ] Log em cartão SD
- [ ] Envio de dados via Wi-Fi (MQTT / HTTP)
- [ ] Timestamp com RTC integrado do R4
- [ ] Dashboard web local
- [ ] Suporte ao DHT22 como opção configurável

---

## 🤝 Como Contribuir

Contribuições são muito bem-vindas! Este projeto é voltado para educadores, estudantes e makers.

1. Faça um **fork** do repositório
2. Crie uma branch para sua feature: `git checkout -b feature/minha-melhoria`
3. Commit suas mudanças: `git commit -m 'Add: minha melhoria'`
4. Push para a branch: `git push origin feature/minha-melhoria`
5. Abra um **Pull Request**

Antes de contribuir, leia o [Guia do Contribuidor](DOCUMENTACAO_EstacaoMeteoro.md) para entender a arquitetura do código e as armadilhas conhecidas.

---

## 👤 Autor

**Matheus Valadares Teixeira** 
- Engenheiro Mecânico com formação complementar em Engenharia de Automação e Controle, Robótica Educacional e Educação para o Ensino Fundamental II e Médio. Possui base sólida em tecnologia e ensino formal e não formal, com foco na integração entre engenharia, robótica e práticas educacionais, voltado ao desenvolvimento de soluções pedagógicas inovadoras.
- Parque Tecnológico de Santo André · Rede Ciência Cidadã

---

## 📄 Licença

Distribuído sob a licença MIT. Veja `LICENSE` para mais informações.

---

<div align="center">
  Feito com ❤️ para a comunidade maker de Santo André
</div>
