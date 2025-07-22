// ===== Bibliotecas =====
#include <Wire.h>                 // Comunicação RTC via I2C (Inter-Integrated Circuit)
#include <DS3231.h>               // Biblioteca para manipular o módulo RTC e depende do Wire.h
#include <Adafruit_GFX.h>         // Biblioteca gráfica (texto, linha, circulo,...)
#include <Adafruit_ST7735.h>      // Biblioteca que controla o display, é mais adequada para spi(Serial Peripheral Interface), então é conectado aos pinos: os pinos de controle: CS, DC, RST e o mosi D11 e sck D13
#include <SPI.h>                  // protocolo de comunicação SPI para o display com o arduino
#include <SoftwareSerial.h>       // Permite usar pinos digitais como porta serial extra
#include <math.h>                 // Biblioteca matemática (usada para seno/cosseno no relógio analógico)

// ====== Pinos do Bluetooth HC-05 ======
SoftwareSerial mySerial(9, 8);    // RX, TX

// ====== Pinos do LED e Botão ======
#define ledPin 2                 // Led para teste
#define buttonPin 12             // Botão para mudar os modos

// ====== Pinos do Display TFT ST7735 ======
#define TFT_CS   10              // Chip Select do display
#define TFT_RST  6               // Reset do display
#define TFT_DC   7               // Data/Command do display

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// ====== RTC ======
DS3231 rtc(SDA, SCL);            // Usa os pinos SDA/SCL padrão do Arduino para I2C
Time now;                        // Objeto que vai guardar hora atual lida do RTC

// ====== Variáveis de Controle ======
char command = 0;               // Comando recebido via Bluetooth
bool lastButtonState = HIGH;    // Estado anterior do botão
unsigned long lastButtonPress = 0;  // Momento do último clique
bool waitingForDoubleClick = false; // Se está esperando um segundo clique
bool longPressDetected = false;     // Se já detectou clique longo


enum Mode { MODE_OI, MODE_MODERN, MODE_ANALOG, MODE_STOPWATCH };
Mode currentMode = MODE_MODERN;     // Modo inicial do relógio

// ====== Cronômetro ======
bool stopwatchRunning = false;      // Se o cronômetro está rodando
unsigned long stopwatchStart = 0;   // Momento em que foi iniciado
unsigned long stopwatchElapsed = 0;  // Tempo acumulado

// ====== Controle de troca de modo por botão ======
unsigned long lastModeChange = 0;         // Última troca de modo
const unsigned long debounceDelay = 300;  // Tempo mínimo entre cliques (em ms)

void setup() {
  Serial.begin(115200);                   // Porta Serial para depuração
  mySerial.begin(9600);                   // Porta Serial para Bluetooth

  pinMode(ledPin, OUTPUT);               // Led
  pinMode(buttonPin, INPUT_PULLUP);      // Ativa resistor interno de pull-up no botão
  digitalWrite(ledPin, LOW);             // Garante que o LED começa desligado

  tft.initR(INITR_BLACKTAB);             // Inicializa o display
  tft.setRotation(0);                    // Define orientação da tela
  tft.fillScreen(ST77XX_BLACK);          // Limpa a tela
  tft.setTextSize(2);                    // Define tamanho padrão do texto
  tft.setTextColor(ST77XX_WHITE);        // Cor padrão do texto

  rtc.begin();                           // Inicia o RTC


  // ===== Ajusta o RTC AQUI =====
  // rtc.setDOW(WEDNESDAY);               // Apenas uma vez e comentar depois
  // rtc.setTime(18, 29, 0);              // Apenas uma vez e comentar depois
  // rtc.setDate(11, 6, 2025);            // Apenas uma vez e comentar depois
  // ------------------------------

  Serial.println("Comandos:");
  Serial.println("  '1' - Ligar LED");
  Serial.println("  '0' - Desligar LED");
  Serial.println("  '2' - Modo previsão (animação)");
  Serial.println("  '4' - Modo moderno");
  Serial.println("  '5' - Modo analógico");
  Serial.println("  '3' - Modo cronômetro");
  Serial.println("  'A' - Iniciar/pausar cronômetro");
  Serial.println("  'B' - Zerar cronômetro");
}

void loop() {
  if (mySerial.available()) {           //  verifica se Recebeu comandos Bluetooth
    command = mySerial.read();          // lê o caracter que veio do bluetooth
    switch (command) {                  // Escolhe o que fazer com base no comando recebido
      case '1': digitalWrite(ledPin, HIGH); break;
      case '0': digitalWrite(ledPin, LOW); break;
      case '2': tft.fillScreen(ST77XX_BLACK); currentMode = MODE_OI; break;
      case '4': tft.fillScreen(ST77XX_BLACK); currentMode = MODE_MODERN; break;
      case '5': tft.fillScreen(ST77XX_BLACK); currentMode = MODE_ANALOG; break;
      case '3': tft.fillScreen(ST77XX_BLACK); currentMode = MODE_STOPWATCH; break;
      case 'A': toggleStopwatch(); break;
      case 'B': resetStopwatch(); break;
    }
  }

  // ===== Leitura e ação do botão
  bool buttonState = digitalRead(buttonPin);         //  cria a variável e Lê o estado atual do pino onde o botão está conectado.
  unsigned long nowMillis = millis();                // cria a variável, armazena o tempo em millis 

  if (buttonState == LOW && lastButtonState == HIGH) {  //Verifica se o botão acabou de ser pressionado
    digitalWrite(ledPin, HIGH);                         // Liga led
    if (waitingForDoubleClick && nowMillis - lastButtonPress < 400) { // duplo clique, se o tempo entre cliques for menor que 400 ms
      toggleStopwatch();                                // para iniciar ou pausar o cronômetro.
      waitingForDoubleClick = false;                    // não foi clicado antes
    } else {
      waitingForDoubleClick = true;                     // já houve um clique anterior
      lastButtonPress = nowMillis;                      // atualiza o tempo do ultimo click
    }
    if (nowMillis - lastModeChange > debounceDelay) {
      nextMode();                                 // Troca de modo se não for clique longo
      lastModeChange = nowMillis;                 // atualiza o tempo do ultimo click
    }
  } else if (buttonState == LOW && !longPressDetected && nowMillis - lastButtonPress > 2000) {    // Se o botão ainda está precionado
    resetStopwatch();                            //Se for clique longo (mais de 2s): zera cronômetro 
    longPressDetected = true;
  } else if (buttonState == HIGH && lastButtonState == LOW) {  // Detecta se o botão foi solto
    digitalWrite(ledPin, LOW);                  // Desliga led
    longPressDetected = false;                  // Reseta a flag de clique longo, para que ele possa ser detectado novamente na próxima vez que o botão for pressionado.
  }

  lastButtonState = buttonState;              // atualiza o estado do botão para que na próxima verificação o código possa saber se o botão mudou de estado

  // ===== Permite digitar comandos no monitor serial e enviá-los ao HC-05 =====
  if (Serial.available()) {                   // se tiver um dispositivo na porta serial, retornar o bytes para leitura
    mySerial.write(Serial.read());            // lê os bytes que chegam do serial e envia para o modulo
  }
  
  // ===== Escolhe o modo =====
  switch (currentMode) {                           // CurrentMode fala qual modo esta, chama a função relacionada e impede que o proximo seja executado com o break. 
    case MODE_OI: showOi(); break;                 
    case MODE_MODERN: drawModernStyle(); break;    // Draw - desenha
    case MODE_ANALOG: drawAnalogClock(); break;
    case MODE_STOPWATCH: drawStopwatch(); break;
  }
}

void nextMode() {
  currentMode = (Mode)(((int)currentMode + 1) % 4);   // Incrementa modo circularmente
  tft.fillScreen(ST77XX_BLACK);                       // Limpa tela
}

// ===== Modo animado ===== 
void showOi() {                                     // Animação
  static unsigned long lastUpdate = 0;              // controla o tempo de atualizalação 
  static int cloudX = 0;                            // posição da nuvem
  static bool moonBright = true;                    // troca a cor da lua
  static int frame = 0;                             // conta os quadros

  if (millis() - lastUpdate < 1500) return;         // Atualiza a tela
  lastUpdate = millis();

  tft.fillScreen(ST77XX_BLACK);                     // coloca a tela de preto antes dos proximos elementos

  // Nome da cidade centralizado
  tft.setTextColor(ST77XX_WHITE);                   // Cor do texto
  tft.setTextSize(2);                               // Tamanho do texto
  int cityX = (128 - (8 * 2 * 6)) / 2;              // centraliza o nome MESQUITA de acordo com o tamanho da tela, quantos letras tem e o tamanho da fonte
  tft.setCursor(cityX, 5);                          // cursor no ponto 5 no eixo X
  tft.print("Mesquita");                            // Escreve Mesquita

  // Lua piscando
  moonBright = (frame % 20 < 10);                   
  uint16_t moonColor = moonBright ? ST77XX_YELLOW : ST77XX_WHITE;
  tft.fillCircle(64, 30, 10, moonColor);            // Desenha um circulo

  // Nuvem animada
  int cx = 39 + cloudX;                             // posição inicial
  tft.fillCircle(cx, 50, 6, ST77XX_WHITE);          // Desnha um circulo
  tft.fillCircle(cx + 8, 50, 6, ST77XX_WHITE);      // Desenha um circulo
  tft.fillRect(cx - 2, 50, 14, 6, ST77XX_WHITE);    // Desenha um retangulo
  cloudX += 1;                                      // Aumenta a cada chamada, indo da esquerda para direita
  if (cloudX > 50) cloudX = -20;                    // quando chega em 50 ela volta para a posição 20, fazendo um loop

  // Temperatura
  tft.setTextColor(ST77XX_WHITE);                   // Cor do texto
  tft.setTextSize(3);                               // Tamanho do texto
  int tempX = (128 - 36) / 2;                       // 2 dígitos grandes + espaço pro símbolo
  tft.setCursor(tempX, 75);                         // posição do cursor nas coordenadas x e y
  tft.print("19");
  tft.drawCircle(tempX + 40, 80, 2, ST77XX_WHITE);  // Desenha o simbolo de grau ° 

  // Mínima / Máxima
  tft.setTextSize(2);
  int minmaxX = (128 - 72) / 2;                    // Determina onde o elemento vai vicar no eixo x (centralizado)
  tft.setCursor(minmaxX, 105);                     // Posição no eixo x e y
  tft.print("23 / 15");                            // Mostra a informação

  // Hora
  int timeX = (128 - 60) / 2;           
  tft.setCursor(timeX, 130);
  tft.print("19:00");

  frame++;                                        // A da frame aumenta 1 na função lá em cima em show()
}

// ===== Digital Moderno =====
void drawModernStyle() {                         // Função desenhar
  static unsigned long lastUpdate = 0;           // guarda a última vez em que a tela foi atualizada.
  if (millis() - lastUpdate < 3000) return;      // se ainda não passaram 3 segundos (3000 ms) desde a última atualização, a função termina
  lastUpdate = millis();                         // evita que a tela fique sendo redesenhada rapidamente

  now = rtc.getTime();                          // Chama o módulo RTC (DS3231) para obter a hora atual e salvar em NOW
  tft.fillScreen(ST77XX_BLACK);                 // preenche a tela com a cor selecionada

  tft.setTextColor(ST77XX_WHITE);               // Cor do texto
  tft.setTextSize(5);                           // Tamanho do texto
  tft.setCursor(10, 10);                        // posição na tela
  if (now.hour < 10) tft.print("0");            // Se a hora for menor que 10, imprime um 0 antes (por exemplo: 09).
  tft.print(now.hour);                          // Mostra a hora em relação ao valor obtido em NOW

  tft.setTextColor(ST77XX_YELLOW);              // Cor do texto
  tft.setCursor(10, 60);                        // posição do texto
  if (now.min < 10) tft.print("0");             // se os minutos for menor que 10 coloca o 0 na frente
  tft.print(now.min);                           // mostra os minutos obtidos em NOW

  tft.fillRoundRect(85, 60, 35, 25, 5, ST77XX_YELLOW);     // Desenha um retangulo arredondado preenchido pelo FILL e de cor amarela
  tft.setTextColor(ST77XX_BLACK);                          // Cor do texto
  tft.setTextSize(2);                                      // Tamanho do texto
  tft.setCursor(92, 65);                                   // Posição
  tft.print(now.date);                                     // Mostra a data obtida em NOW

  tft.fillCircle(15, 110, 6, ST77XX_WHITE);                // desenha um circulo branco preenchido  (nuvem)
  tft.fillCircle(20, 105, 4, ST77XX_WHITE);                // desenha um circulo branco preenchido  (nuvem)
  tft.fillCircle(26, 103, 3, ST77XX_YELLOW);               // desenha um circulo amarelo preenchido (sol)

  tft.setTextColor(ST77XX_YELLOW);                         // Cor do texto
  tft.setTextSize(2);                                      // Tamanho do texto
  tft.setCursor(40, 105);                                  // Posição
  tft.print((int)rtc.getTemp());                           // Coverte o valor para inteiro
  tft.fillCircle(64, 108, 2, ST77XX_YELLOW);               // desenha o simbolo de grau
}

// ===== Modo analógico =====
void drawAnalogClock() {
  static unsigned long lastUpdate = 0;                    // armazena o valor da ultima atualização, iniciando o valor em 0
  if (millis() - lastUpdate < 1000) return;               // Atualiza a tela
  lastUpdate = millis();

  now = rtc.getTime();                                    // Lê a hora atual do módulo RTC (DS3231).
  tft.fillScreen(ST77XX_BLACK);                           // tela preta

  // Definindo o centro do relógio
  const int centerX = 64;
  const int centerY = 64;
  const int radius = 60;                                  // define o tamanho do círculo externo do relógio
  
  // Desenha circulos
  tft.drawCircle(centerX, centerY, radius, ST77XX_WHITE);
  tft.drawCircle(centerX, centerY, radius - 1, ST77XX_WHITE);

  // Números das horas
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_WHITE);
  for (int i = 1; i <= 12; i++) {                        // Calcula o ângulo onde cada número deve ser desenhado. Subtrai 3 para alinhar o número "12" ao topo
    float angle = (i - 3) * 30 * DEG_TO_RAD;             // Define 30 graus para cada numero (360/12) e converte de graus para radianos (necessário para cos e sin)
    int tx = centerX + cos(angle) * (radius - 10);       // Calcula a posição (x, y) para desenhar o número, 10 pixels antes da borda do relógio
    int ty = centerY + sin(angle) * (radius - 10);       // Calcula a posição (x, y) para desenhar o número, 10 pixels antes da borda do relógio
    tft.setCursor(tx - (i < 10 ? 3 : 6), ty - 3);        // Ajusta ligeiramente a posição do texto, dependendo se o número é de 1 dígito (1–9) ou 2 (10–12)
    tft.print(i);                                        // mostra os numeros
  }

  // Ponteiros
  // formula
  // x = centroX + cos(θ) * r
  // y = centroY + sin(θ) * r

  float angleHour = ((now.hour % 12) + now.min / 60.0) * 30 * DEG_TO_RAD;    // Calcula o ângulo das horas com fração de minutos (ex: 10:30 → ponteiro entre 10 e 11) multiplica por 30 graus para cada hora
  int hx = centerX + cos(angleHour) * (radius - 30);                         // Calcula a posição da ponta do ponteiro das horas e desenha uma linha até lá
  int hy = centerY + sin(angleHour) * (radius - 30);                         // Calcula a posição da ponta do ponteiro das horas e desenha uma linha até lá
  tft.drawLine(centerX, centerY, hx, hy, ST77XX_WHITE);                      // desenha na tela

  float angleMin = now.min * 6 * DEG_TO_RAD;                                // now indica o minuto atual e se move 6° por minuto (360°/60) e converte de graus para radius para ser usado por cos() e sen()
  int mx = centerX + cos(angleMin) * (radius - 15);                         // mx é a posição X final do ponteiro dos minutos e para calcular a posição horizontal (X) da ponta do ponteiro e Multiplica pelo tamanho do ponteiro: radius - 15 → o ponteiro é menor que o raio total, para caber bem no relógio.
  int my = centerY + sin(angleMin) * (radius - 15);
  tft.drawLine(centerX, centerY, mx, my, ST77XX_YELLOW);                    // Desenha uma linha do centro do relógio até (mx, my), que é a ponta do ponteiro.

  float angleSec = now.sec * 6 * DEG_TO_RAD;                                // mesmo processo dos minutos
  int sx = centerX + cos(angleSec) * (radius - 5);
  int sy = centerY + sin(angleSec) * (radius - 5);
  tft.drawLine(centerX, centerY, sx, sy, ST77XX_RED);

  tft.fillCircle(centerX, centerY, 3, ST77XX_WHITE);                        // Desenha um circulo branco no centro do relógio
}

// ===== Cronômetro =====
void drawStopwatch() {
  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate < 900) return;             // Atualiza a tela a cada 900 millis
  lastUpdate = millis();

  tft.fillScreen(ST77XX_BLACK);                        // tela em preto

  unsigned long elapsed = stopwatchElapsed;            // é o tempo acumulado antes da última pausa
  if (stopwatchRunning) {                              // se estiver iniciado
    elapsed += millis() - stopwatchStart;              // adiciona o tempo que foi iniciado em millis
  }

  // Converte de millis para segundo, minuto e centésimos de segundo
  unsigned long seconds = (elapsed / 1000) % 60;       // cada 1.000 ms é 1 segundo.
  unsigned long minutes = (elapsed / 60000) % 60;      // cada 60.000 ms é 1 minuto.
  unsigned long millisec = (elapsed % 1000) / 10;      // mostra centésimos de segundo (/10, já que 1000/10 = 100 intervalos)

  #define DARKGREY 0x5294                              // Cor que não está na bibliteca

  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);
  tft.setCursor(1, 10);
  tft.print("Cronometro");

  tft.fillRoundRect(8, 35, 112, 88, 10, DARKGREY);    // Desenha um retângulo aredondado de cor cinza escuro (8, 35) - posição, 112 - largura, 88 - altura e 10 o raio para aredondar as bordas

  tft.setTextColor(ST77XX_CYAN);
  tft.setCursor(20, 60);
  if (minutes < 10) tft.print("0");                   // Mostra os minutos
  tft.print(minutes);                                 
  tft.print(":");
  if (seconds < 10) tft.print("0");                   // mostra os segundos
  tft.print(seconds);

  tft.setTextColor(ST77XX_YELLOW);
  tft.setTextSize(1);
  tft.setCursor(30, 80);
  if (millisec < 10) tft.print("0");                 // Mostra os centésimos
  tft.print(millisec);
  tft.print(" ms");

  tft.fillCircle(100, 75, 5, stopwatchRunning ? ST77XX_GREEN : ST77XX_RED);  // Desenha um circulo na tela que muda de cor dependendo no modo ativado (x, y, raio, cor)
}

// ===== Pausa e Iniciar do cronômetro =====
void toggleStopwatch() {
  if (stopwatchRunning) {                             // ===== Se estiver contando =====
    stopwatchElapsed += millis() - stopwatchStart;    // Salva o tempo decorrido
    stopwatchRunning = false;                         // Para de contar
  } else {                                            // ===== Se está parado =====
    stopwatchStart = millis();                        // marca o momento que parou
    stopwatchRunning = true;                          // Volta a contar
  }
}

// ===== Reset do cronômetro =====
void resetStopwatch() {
  stopwatchRunning = false;                           // Para o cronômetro
  stopwatchElapsed = 0;                               // zera o tempo acumulado
}
