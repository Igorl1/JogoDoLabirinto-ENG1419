#include <TouchScreen.h>
#include <JKSButton.h>
#include <MCUFRIEND_kbv.h>

typedef struct {
  int x[8];
  int y[8];
} Coord;

JKSButton botao_start, botao_niv, botao_menu, botao_n1, botao_n2, botao_n3;
MCUFRIEND_kbv tela;
TouchScreen touch(6, A1, A2, 7, 300);
int timemax = 60, cronometro, timefinal, modo, piaoX = 1, piaoY = 1, piaoXAnt = 1, piaoYAnt = 1;
const int piaoR = 15;
bool ganhou = false;
unsigned long instAnt = 0, instAnt2 = 0;
Coord coords;

void setup(void) {
  Serial.begin(9600);
  tela.begin(tela.readID());
  Menu();
  Mapeamento();
  botao_start.setPressHandler(Labirinto);
  botao_niv.setPressHandler(Select);
  botao_menu.setPressHandler(Menu);
  botao_n1.setPressHandler(N1);
  botao_n2.setPressHandler(N2);
  botao_n3.setPressHandler(N3);
}

void loop() {
  if (modo == 1) {  // Menu
    botao_start.process();
    botao_niv.process();
  } else if (modo == 2) {  // Labirinto
    botao_menu.process();
  } else if (modo == 3) {  // Select
    botao_n1.process();
    botao_n2.process();
    botao_n3.process();
  } else if (modo == 4) {  // End
    botao_start.process();
    botao_menu.process();
  }

  if (Serial.available() >= 4) {
    piaoX = Serial.parseInt();
    piaoY = Serial.parseInt();
  }

  unsigned long instAtual = millis();
  if (instAtual > instAnt + 1000) {
    UpdateCronometro();
    instAnt = instAtual;
  }

  unsigned long instAtual2 = millis();
  if (instAtual2 > instAnt2 + 250) {
    if (modo == 2) {
      piaoXAnt = piaoX;
      piaoYAnt = piaoY;
    } else {
      piaoX = 1;
      piaoY = 1;
      piaoXAnt = 1;
      piaoYAnt = 1;
    }
    instAnt2 = instAtual2;
  }

  if (piaoY > 7) {
    piaoY = 7;
  }
  if (piaoX > 7) {
    piaoX = 7;
  }
  if ((piaoX != piaoXAnt && modo == 2) || (piaoY != piaoYAnt && modo == 2)) {
    DrawPiao(piaoX, piaoY);
    DelPiao(piaoXAnt, piaoYAnt);
  }
}

void UpdateCronometro() {
  if (modo == 2) {
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

void Menu() {
  ResetCronometro();
  modo = 1;
  tela.fillScreen(TFT_BLACK);
  botao_start.init(&tela, &touch, 120, 140, 170, 50, TFT_BLUE, TFT_BLUE, TFT_WHITE, "Jogar", 2);
  botao_niv.init(&tela, &touch, 120, 220, 170, 50, TFT_BLUE, TFT_BLUE, TFT_WHITE, "Config", 2);
  tela.fillRect(18, 38, 204, 32, TFT_BLUE);
  tela.setCursor(20, 40);
  tela.setTextColor(TFT_WHITE);
  tela.setTextSize(2);
  tela.println("Jogo do Labirinto");
  tela.setCursor(93, 60);
  tela.setTextColor(TFT_WHITE);
  tela.setTextSize(1);
  tela.println("Turma 3VA");
}

void Select() {
  modo = 3;
  tela.fillScreen(TFT_BLACK);
  botao_n1.init(&tela, &touch, 120, 110, 170, 50, TFT_BLUE, TFT_BLUE, TFT_WHITE, "90 Seg", 2);
  botao_n2.init(&tela, &touch, 120, 190, 170, 50, TFT_BLUE, TFT_BLUE, TFT_WHITE, "60 Seg", 2);
  botao_n3.init(&tela, &touch, 120, 270, 170, 50, TFT_BLUE, TFT_BLUE, TFT_WHITE, "30 Seg", 2);
  tela.fillRect(0, 38, 185, 20, TFT_BLUE);
  tela.setCursor(19, 40);
  tela.setTextColor(TFT_WHITE);
  tela.setTextSize(2);
  tela.println("Tempo de Jogo");
}

void End() {
  char str[1];
  int timedecorrido = timemax - timefinal, minutos = timedecorrido / 60, segundos = timedecorrido % 60;
  ResetCronometro();
  modo = 4;
  tela.fillScreen(TFT_BLACK);
  botao_start.init(&tela, &touch, 120, 150, 170, 50, TFT_BLUE, TFT_BLUE, TFT_WHITE, "Novo Jogo", 2);
  botao_menu.init(&tela, &touch, 120, 230, 170, 50, TFT_BLUE, TFT_BLUE, TFT_WHITE, "Menu", 2);
  tela.fillRect(0, 75, 165, 25, TFT_BLUE);
  tela.setTextColor(TFT_WHITE);
  tela.setTextSize(3);
  tela.setCursor(20, 40);
  tela.println(ganhou ? "Vitoria" : "Derrota");
  tela.setTextSize(2);
  tela.setCursor(20, 80);
  tela.print("Tempo: ");
  sprintf(str, "%d:%.2d", minutos, segundos);
  tela.print(str);
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

void Mapeamento() {
  int coordx = 0, coordy = 0, x_ini = 7, y_ini = 7;
  for (int i = 0; i < 7; ++i) {
    coordx += 30;
    coordy += 30;
    coords.x[x_ini] = coordx;
    coords.y[y_ini] = coordy;
    x_ini = x_ini - 1;
    y_ini = y_ini - 1;
  }
}

void DrawTile(String str, int x, int y) {
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
  }
}

void DrawPiao(int piaoX, int piaoY) {
  tela.fillCircle(coords.x[piaoX], coords.y[piaoY], piaoR, TFT_BLUE);
  tela.drawCircle(coords.x[piaoX], coords.y[piaoY], piaoR, TFT_BLACK);
}

void DelPiao(int piaoX, int piaoY) {
  tela.fillRect(coords.x[piaoX] - 15, coords.y[piaoY] - 15, 30, 30, TFT_WHITE);
  tela.drawRect(coords.x[piaoX] - 15, coords.y[piaoY] - 15, 30, 30, TFT_BLUE);
}

void Labirinto() {
  int aumento = 0;
  modo = 2;
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

  DrawTile("final", 7, 7);
  DrawTile("inicial", 1, 1);
  DrawPiao(piaoX, piaoY);
}
