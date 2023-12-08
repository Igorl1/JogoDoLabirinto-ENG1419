// O input na serial segue o formato x'{a},{b}'.
// Valores usados na serial para fazer tabuleiro tradicional:
// i'{1},{1}'
// f'{7},{7}'
// c'{5},{1}','{4},{5}'
// p'{2},{1}','{2},{2}','{2},{3}','{4},{1}','{4},{2}','{4},{3}','{6},{1}','{6},{2}','{6},{4}','{6},{5}','{6},{6}','{5},{6}','{5},{5}','{4},{6}','{3},{6}','{3},{5}','{2},{6}','{2},{5}'

#include <TouchScreen.h>
#include <JKSButton.h>
#include <MCUFRIEND_kbv.h>
#include <EEPROM.h>

typedef struct {
  int x[8], y[8];
} Coord;

typedef enum Modo { MENU = 1,
                    LAB = 2,
                    SEL = 3,
                    END = 4,
                    SCORE = 5 };

JKSButton botao_start, botao_niv, botao_menu, botao_n1, botao_n2, botao_n3, botao_score;
MCUFRIEND_kbv tela;
Coord coords;
TouchScreen touch(6, A1, A2, 7, 300);
int timemax = 60, cronometro, timefinal, modo, piaoX, piaoY, piaoXAnt, piaoYAnt, iniX = 1, iniY = 1, fimX = 7, fimY = 7, pm1X = 1, pm1Y = 6, pm2X = 6, pm2Y = 3, cX[2], cY[2], pX[18], pY[18];
const int piaoR = 15;
bool ganhou = false, comecou = false, ganhoufinal;
unsigned long instAnt = 0, instAnt2 = 0;

void setup(void) {
  Serial.begin(9600);
  tela.begin(tela.readID());
  Menu();
  Mapeamento();
  botao_start.setPressHandler(Labirinto);
  botao_niv.setPressHandler(Select);
  botao_menu.setPressHandler(Menu);
  botao_score.setPressHandler(Score);
  botao_n1.setPressHandler(N1);
  botao_n2.setPressHandler(N2);
  botao_n3.setPressHandler(N3);
}

void loop() {
  if (modo == MENU) {
    botao_start.process();
    botao_niv.process();
    botao_score.process();
  } else if (modo == LAB) {
    botao_menu.process();
  } else if (modo == SEL) {
    botao_n1.process();
    botao_n2.process();
    botao_n3.process();
  } else if (modo == END) {
    botao_start.process();
    botao_menu.process();
  } else if (modo == SCORE) {
    botao_menu.process();
  }

  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    if (input.startsWith("b")) {
      piaoX = input.substring(3, 4).toInt();
      piaoY = input.substring(7, 8).toInt();
      Serial.println("Bola movimentada");
    }
    if (input.startsWith("i")) {
      iniX = input.substring(3, 4).toInt();
      iniY = input.substring(7, 8).toInt();
      Serial.println("Casa inicial registrada");
    }
    if (input.startsWith("f")) {
      fimX = input.substring(3, 4).toInt();
      fimY = input.substring(7, 8).toInt();
      Serial.println("Casa final registrada");
    }
    if (input.startsWith("c")) {
      int a = 0;
      for (int i = 0; i < sizeof(cX) / sizeof(cX[0]); i++) {
        cX[i] = input.substring(3 + a, 4 + a).toInt();
        cY[i] = input.substring(7 + a, 8 + a).toInt();
        a = a + 10;
      }
      Serial.println("Casas checkpoint registradas");
    }
    if (input.startsWith("p")) {
      int a = 0;
      for (int i = 0; i < sizeof(pX) / sizeof(pX[0]); i++) {
        pX[i] = input.substring(3 + a, 4 + a).toInt();
        pY[i] = input.substring(7 + a, 8 + a).toInt();
        a = a + 10;
      }
      Serial.println("Casas parede registradas");
    }
  }

  unsigned long instAtual = millis();
  if (instAtual > instAnt + 1000) {
    UpdateCronometro();
    instAnt = instAtual;
  }

  unsigned long instAtual2 = millis();
  if (instAtual2 > instAnt2 + 250) {
    if (modo == LAB) {
      piaoXAnt = piaoX;
      piaoYAnt = piaoY;
      if (piaoX == fimX && piaoY == fimY) {
        ganhou = true;
        End();
      }
    }
    instAnt2 = instAtual2;
  }
  
  if (modo != LAB) {
    piaoX = iniX;
    piaoY = iniY;
    piaoXAnt = iniX;
    piaoYAnt = iniY;
  }

  if (modo == LAB) {
    if (piaoY > 7) {
      piaoY = 7;
    }
    if (piaoX > 7) {
      piaoX = 7;
    }
    if (piaoX < 0) {
      piaoX = 1;
    }
    if (piaoY < 0) {
      piaoY = 1;
    }
    for (int i = 0; i < sizeof(pX) / sizeof(pX[0]); i++) {
      if (pX[i] == piaoX && pY[i] == piaoY) {
        piaoX = piaoXAnt;
        piaoY = piaoYAnt;
      }
    } 
    if ((piaoX != piaoXAnt) || (piaoY != piaoYAnt)) {
      DrawPiao(piaoX, piaoY);
      DelPiao(piaoXAnt, piaoYAnt);
    }
  }
}

void UpdateCronometro() {  // Atualiza o cronometro (dentro do loop para atualizar a cada segundo)
  if (modo == LAB) {
    tela.setCursor(15, 250);
    tela.setTextColor(TFT_WHITE);
    tela.setTextSize(2);
    tela.print("Tempo: ");
    tela.fillRect(86, 245, 30, 100, TFT_BLUE);
    tela.setCursor(90, 250);
    tela.setTextColor(TFT_WHITE);
    tela.setTextSize(2);
    tela.print(cronometro);
    if (cronometro == 0) {
      End();
    }
    cronometro = cronometro - 1;
    timefinal = cronometro;
  }
}

void ResetCronometro() {
  cronometro = timemax;
}

void Menu() {  // Inicializa a tela menu
  modo = MENU;
  comecou = false;
  ResetCronometro();
  tela.fillScreen(TFT_BLACK);
  botao_start.init(&tela, &touch, 120, 110, 170, 50, TFT_BLUE, TFT_BLUE, TFT_WHITE, "Jogar", 2);
  botao_score.init(&tela, &touch, 120, 190, 170, 50, TFT_BLUE, TFT_BLUE, TFT_WHITE, "Historico", 2);
  botao_niv.init(&tela, &touch, 120, 270, 170, 50, TFT_BLUE, TFT_BLUE, TFT_WHITE, "Config", 2);
  tela.fillRect(18, 18, 204, 32, TFT_BLUE);
  tela.setCursor(20, 20);
  tela.setTextColor(TFT_WHITE);
  tela.setTextSize(2);
  tela.println("Jogo do Labirinto");
  tela.setCursor(93, 40);
  tela.setTextColor(TFT_WHITE);
  tela.setTextSize(1);
  tela.println("Turma 3VA");
}

void Score() {  // Inicializa a tela de score
  char str[5];
  modo = SCORE;
  tela.fillScreen(TFT_BLACK);
  botao_menu.init(&tela, &touch, 120, 160, 170, 50, TFT_BLUE, TFT_BLUE, TFT_WHITE, "Menu", 2);
  tela.fillRect(0, 75, 185, 25, TFT_BLUE);
  tela.setTextColor(TFT_WHITE);
  tela.setTextSize(3);
  tela.setCursor(20, 40);
  tela.println("Ultimo jogo");
  tela.setTextSize(2);
  tela.setCursor(20, 80);
  if (isnan(EEPROM.read(0)) || isnan(EEPROM.read(3)) || isnan(EEPROM.read(6))) {
    tela.print("Nao registrado");
  } else {
    if (EEPROM.read(6) == 1) {
      tela.print("Vitoria: ");
    } else {
      tela.print("Derrota: ");
    }
    sprintf(str, "%d:%.2d", EEPROM.read(0), EEPROM.read(3));
    tela.print(str);
  }
}

void Select() {  // Inicializa a tela de seleção
  modo = SEL;
  tela.fillScreen(TFT_BLACK);
  botao_n1.init(&tela, &touch, 120, 110, 170, 50, TFT_BLUE, TFT_BLUE, TFT_WHITE, "90 Seg", 2);
  botao_n2.init(&tela, &touch, 120, 190, 170, 50, TFT_BLUE, TFT_BLUE, TFT_WHITE, "60 Seg", 2);
  botao_n3.init(&tela, &touch, 120, 270, 170, 50, TFT_BLUE, TFT_BLUE, TFT_WHITE, "30 Seg", 2);
  tela.fillRect(0, 38, 185, 20, TFT_BLUE);
  tela.setCursor(20, 40);
  tela.setTextColor(TFT_WHITE);
  tela.setTextSize(2);
  tela.println("Tempo de Jogo");
}

void End() {  // Inicializa a tela final
  modo = END;
  comecou = false;
  char str[5];
  int timedecorrido = timemax - timefinal, minutos = timedecorrido / 60, segundos = timedecorrido % 60;
  ResetCronometro();
  tela.fillScreen(TFT_BLACK);
  botao_start.init(&tela, &touch, 120, 150, 170, 50, TFT_BLUE, TFT_BLUE, TFT_WHITE, "Novo Jogo", 2);
  botao_menu.init(&tela, &touch, 120, 230, 170, 50, TFT_BLUE, TFT_BLUE, TFT_WHITE, "Menu", 2);
  tela.fillRect(0, 75, 165, 25, TFT_BLUE);
  tela.setTextColor(TFT_WHITE);
  tela.setTextSize(3);
  tela.setCursor(20, 40);
  tela.println(ganhou ? "Vitoria" : "Derrota");
  ganhoufinal = ganhou;
  tela.setTextSize(2);
  tela.setCursor(20, 80);
  tela.print("Tempo: ");
  sprintf(str, "%d:%.2d", minutos, segundos);
  tela.print(str);
  EEPROM.write(0, minutos);
  EEPROM.write(3, segundos);
  EEPROM.write(6, ganhoufinal);
}

void N1() {
  timemax = 90;
  Menu();
}

void N2() {
  timemax = 60;
  Menu();
}

void N3() {
  timemax = 30;
  Menu();
}

void Mapeamento() {  // Transforma coordenas puras no lcd nas coordenas do tabuleiro (1,1) até (7,7)
  int coordx = 0, coordy = 0, x_ini = 7, y_ini = 7;
  for (int i = 0; i < 7; i++) {
    coordx += 30;
    coordy += 30;
    coords.x[x_ini] = coordx;
    coords.y[y_ini] = coordy;
    x_ini = x_ini - 1;
    y_ini = y_ini - 1;
  }
}

void DrawTile(String str, int x, int y) {  // Desenha o quadrado colorido especificado
  if (str == "parede") {
    tela.fillRect(coords.x[x] - 15, coords.y[y] - 15, 30, 30, TFT_BLACK);
    tela.drawRect(coords.x[x] - 15, coords.y[y] - 15, 30, 30, TFT_BLUE);
  } else if (str == "checkpoint") {
    tela.fillRect(coords.x[x] - 15, coords.y[y] - 15, 30, 30, TFT_RED);
    tela.drawRect(coords.x[x] - 15, coords.y[y] - 15, 30, 30, TFT_BLUE);
  } else if (str == "final") {
    tela.fillRect(coords.x[x] - 15, coords.y[y] - 15, 30, 30, TFT_YELLOW);
    tela.drawRect(coords.x[x] - 15, coords.y[y] - 15, 30, 30, TFT_BLUE);
  } else if (str == "inicial") {
    tela.fillRect(coords.x[x] - 15, coords.y[y] - 15, 30, 30, TFT_GREEN);
    tela.drawRect(coords.x[x] - 15, coords.y[y] - 15, 30, 30, TFT_BLUE);
  } else if (str == "normal") {
    tela.fillRect(coords.x[x] - 15, coords.y[y] - 15, 30, 30, TFT_WHITE);
    tela.drawRect(coords.x[x] - 15, coords.y[y] - 15, 30, 30, TFT_BLUE);
  }
}

void DrawPiao(int piaoX, int piaoY) {  // Desenha na posicao desejada do piao
  tela.fillCircle(coords.x[piaoX], coords.y[piaoY], piaoR, TFT_BLUE);
  tela.drawCircle(coords.x[piaoX], coords.y[piaoY], piaoR, TFT_BLACK);
}

void DelPiao(int piaoX, int piaoY) {  // Deleta posicao desejada do piao
  if (piaoXAnt == iniX && piaoYAnt == iniY) {
    tela.fillRect(coords.x[piaoX] - 15, coords.y[piaoY] - 15, 30, 30, TFT_GREEN);
    tela.drawRect(coords.x[piaoX] - 15, coords.y[piaoY] - 15, 30, 30, TFT_BLUE);
  } else if (piaoXAnt == fimX && piaoYAnt == fimY) {
    tela.fillRect(coords.x[piaoX] - 15, coords.y[piaoY] - 15, 30, 30, TFT_YELLOW);
    tela.drawRect(coords.x[piaoX] - 15, coords.y[piaoY] - 15, 30, 30, TFT_BLUE);
  } else {
    tela.fillRect(coords.x[piaoX] - 15, coords.y[piaoY] - 15, 30, 30, TFT_WHITE);
    tela.drawRect(coords.x[piaoX] - 15, coords.y[piaoY] - 15, 30, 30, TFT_BLUE);
  }
  for (int i = 0; i < sizeof(cX) / sizeof(cX[0]); i++) {
    if (piaoXAnt == cX[i] && piaoYAnt == cY[i]) {
      tela.fillRect(coords.x[piaoX] - 15, coords.y[piaoY] - 15, 30, 30, TFT_RED);
      tela.drawRect(coords.x[piaoX] - 15, coords.y[piaoY] - 15, 30, 30, TFT_BLUE);
    }
  }
}

void Labirinto() {  // Desenha o labirinto
  ganhou = false;
  comecou = true;
  Serial.println("O jogo comecou");
  int aumento = 0;
  modo = LAB;
  tela.fillScreen(TFT_BLACK);

  botao_menu.init(&tela, &touch, 180, 275, 80, 50, TFT_BLUE, TFT_BLUE, TFT_WHITE, "Menu", 2);

  tela.fillRect(15, 15, 210, 210, TFT_WHITE);

  for (int i = 0; i < 7; i++) {
    tela.drawRect(15, 15 + aumento, 30, 30, TFT_BLUE);
    tela.drawRect(45, 15 + aumento, 30, 30, TFT_BLUE);
    tela.drawRect(75, 15 + aumento, 30, 30, TFT_BLUE);
    tela.drawRect(105, 15 + aumento, 30, 30, TFT_BLUE);
    tela.drawRect(135, 15 + aumento, 30, 30, TFT_BLUE);
    tela.drawRect(165, 15 + aumento, 30, 30, TFT_BLUE);
    tela.drawRect(195, 15 + aumento, 30, 30, TFT_BLUE);
    aumento = aumento + 30;
  }
  for (int i = 0; i < sizeof(pX) / sizeof(pX[0]); i++) {
    if (pX[i] == NULL && pY[i] == NULL) {
      DrawTile("parede", 10, 10);
    } else {
      DrawTile("parede", pX[i], pY[i]);
    }
  }
  for (int i = 0; i < sizeof(cX) / sizeof(cX[0]); i++) {
    if (cX[i] == NULL && cY[i] == NULL) {
      DrawTile("checkpoint", 10, 10);
    } else {
      DrawTile("checkpoint", cX[i], cY[i]);
    }
  }
  DrawTile("final", fimX, fimY);
  DrawTile("inicial", iniX, iniY);
  DrawPiao(piaoX, piaoY);
}
