# MIDI USB Controller com Encoders, Matriz de Botões e Display OLED

## Visão Geral
Este projeto implementa um controlador MIDI USB utilizando Arduino Micro, com:
- 4 encoders rotativos enviando mensagens MIDI CC
- 1 matriz de botões 5x4 enviando notas MIDI
- 1 display OLED I2C 0,96" exibindo uma barra de volume (0 a 100%) baseada no primeiro encoder

---

## Componentes Utilizados

- **Arduino Micro** (ou outro compatível com MIDI USB)
- **4x Encoders rotativos** com pinos A/B
- **Matriz de 20 botões** (5 linhas x 4 colunas)
- **Display OLED 0,96"** (I2C - SSD1306)
- **Cabos e jumpers**

---

## Ligações

### Display OLED (I2C)
| Display | Arduino Micro |
|---------|----------------|
| VCC     | 5V             |
| GND     | GND            |
| SDA     | D2             |
| SCL     | D3             |

### Encoders
| Encoder | Pin A | Pin B | CC Number |
|---------|-------|-------|-----------|
| 1       | D13   | D19   | 21        |
| 2       | D20   | D21   | 22        |
| 3       | D8    | D7    | 23        |
| 4       | D6    | D5    | 24        |

### Matriz de Botões
- **Linhas**: D22, D23, D1, D0, D4
- **Colunas**: D12, D11, D10, D9


---

## Funcionalidades

### Encoders
- Enviam mensagens MIDI CC de 0 a 127
- Cada encoder possui um canal CC diferente (21 a 24)
- O valor é incrementado/decrementado em passos de 5

### Matriz de Botões
- 20 botões organizados em 5 linhas por 4 colunas
- Cada botão envia um Note On/Off
- Canal MIDI 1

### Display OLED
- Exibe o volume em porcentagem baseado no primeiro encoder (0% a 100%)
- Mostra uma barra gráfica representando o volume
- Atualiza apenas quando o encoder 1 é girado


---

## Bibliotecas Utilizadas

- [Control Surface](https://github.com/tttapa/Control-Surface) (v1.2.0)
- Adafruit GFX
- Adafruit SSD1306
- Wire (I2C)


---

## Como Usar

1. Conecte todos os componentes conforme o esquema acima.
2. Instale as bibliotecas necessárias no Arduino IDE.
3. Compile e envie o código para o Arduino Micro.
4. Conecte o Arduino ao computador e abra um software compatível com MIDI (Ableton, FL Studio etc).
5. Gire o primeiro encoder para ver a barra de volume se mover no display.


---

## Expansões Futuras

- Mostrar todos os encoders no display
- Adicionar botão para alternar o encoder visível
- Salvar presets em EEPROM
- Integração com controladores mais complexos ou várias telas


---

## Licença

Este projeto é de uso livre para fins educacionais e pessoais.

