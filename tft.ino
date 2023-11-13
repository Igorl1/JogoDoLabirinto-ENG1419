#include <Adafruit_ILI9341.h>
#include <TouchScreen.h>
#include <GFButton.h>
//#include <JKSButton.h>
//#include <MCUFRIEND.h>
//#include <Adafruit_GFX.h>

//JKSButton botao_start, botao_niv, botao_menu, botao_n1, botao_n2, botao_n3;
GFButton botao1(A1), botao2(A2), botao5(A5);
Adafruit_ILI9341 tela = Adafruit_ILI9341(8, 10, 9);
TouchScreen touch(25, 26, 27, 9, 300);
int timemax = 60, cronometro, timefinal, modo, coordsx[7], coordsy[7], piaoX = 1, piaoY = 1;
const int piaoR = 15;
bool cronometroAtivo, ganhou = false;
unsigned long instAnt = 0;

void setup(void) {
  Serial.begin(9600);
  tela.begin();
  Menu();/*
  botao_start.setPressHandler(Labirinto);
  botao_niv.setPressHandler(Select);
  botao_menu.setPressHandler(Menu);
  botao_n1.setPressHandler(N1);
  botao_n2.setPressHandler(N2);
  botao_n1.setPressHandler(N3);*/
  botao1.setPressHandler(Labirinto);
  botao2.setPressHandler(Select);
  botao5.setPressHandler(Menu);
}
 
void loop() {
  botao1.process();
  botao2.process();
  botao5.process();/*
  if(modo == 1){ //Menu
    botao_start.process();
    botao_niv.process();
  } else if(modo == 2){ //Labirinto, End
    botao_menu.process();
  } else{ //Select
    botao_menu.process();
    botao_n1.process();
    botao_n2.process();
    botao_n3.process();
  }*/
  unsigned long instAtual = millis();
  if(instAtual > instAnt + 1000){
    UpdateCrono();
    instAnt = instAtual;
  }
}

void UpdateCrono(){
  if(cronometroAtivo && modo == 2){
    tela.setCursor(15, 250);
    tela.setTextColor(ILI9341_WHITE);
    tela.setTextSize(2);
    tela.print("Tempo: ");
    tela.fillRect(86, 245, 30, 100, ILI9341_BLUE);
    tela.setCursor(90, 250);
    tela.setTextColor(ILI9341_WHITE);
    tela.setTextSize(2);
    tela.print(cronometro);
    if (cronometro == 0){
      End();
    }
    cronometro = cronometro - 1;
    timefinal = cronometro;
  }
}

void ResetCronometro(){
  cronometroAtivo = false;
  cronometro = timemax;
}

void Menu(){
  ResetCronometro();
  modo = 1;
  tela.fillScreen(ILI9341_BLACK);/*
  botao_start.init(&tela, &touch, 120, 120, 170, 25, ILI9341_BLUE, ILI9341_BLUE, ILI9341_BLACK, "Jogar", 1);
  botao_niv.init(&tela, &touch, 120, 180, 170, 25, ILI9341_BLUE, ILI9341_BLUE, ILI9341_BLACK, "Nivel", 1);*/
  tela.fillRect(18,38,204,32, ILI9341_WHITE);
  tela.setCursor(19, 40);
  tela.setTextColor(ILI9341_BLUE);
  tela.setTextSize(2);
  tela.println("Jogo do Labirinto");
  tela.setCursor(93, 60);
  tela.setTextColor(ILI9341_BLUE);
  tela.setTextSize(1);
  tela.println("Turma 3VA");
}

void Select(){
  if(modo == 1){
    modo = 3;
    tela.fillScreen(ILI9341_BLACK);/*
    botao_n1.init(&tela, &touch, 120, 120, 170, 25, ILI9341_BLUE, ILI9341_BLUE, ILI9341_BLACK, "Nivel 1", 1);
    botao_n2.init(&tela, &touch, 120, 150, 170, 25, ILI9341_BLUE, ILI9341_BLUE, ILI9341_BLACK, "Nivel 2", 1);
    botao_n3.init(&tela, &touch, 120, 180, 170, 25, ILI9341_BLUE, ILI9341_BLUE, ILI9341_BLACK, "Nivel 3", 1);
    botao_menu.init(&tela, &touch, 120, 210, 170, 25, ILI9341_BLUE, ILI9341_BLUE, ILI9341_BLACK, "Menu", 1);*/
    tela.fillRect(18,38,204,20, ILI9341_WHITE);
    tela.setCursor(19, 40);
    tela.setTextColor(ILI9341_BLUE);
    tela.setTextSize(2);
    tela.println("Selecione o Nivel");
  }
}

void End(){
  char str[1];
  int timedecorrido = timemax - timefinal, minutos = timedecorrido / 60, segundos = timedecorrido % 60;

  ResetCronometro();
  modo = 2;
  tela.fillScreen(ILI9341_BLACK);
  //botao_menu.init(&tela, &touch, 120, 180, 170, 25, ILI9341_BLUE, ILI9341_BLUE, ILI9341_BLACK, "Menu", 1);
  tela.fillRect(0, 75, 165, 25, ILI9341_BLUE);
  tela.setTextColor(ILI9341_WHITE);
  tela.setTextSize(3);
  tela.setCursor(20, 40);
  tela.println(ganhou ? "Vitoria" : "Derrota");
  tela.setTextSize(2);
  tela.setCursor(20, 80);
  tela.print("Tempo: ");
  sprintf(str, "%d:%.2d", minutos,segundos);
  tela.print(str);
}
/*
void N1(){
  timemax = 90;
}

void N2(){
  timemax = 60;
}

void N3(){
  timemax = 30;
}
*/
void DrawPiao(int piaoX, int piaoY){
  int coordx = 0, coordy = 0, x_ini = 7, y_ini = 7;
  for (int i = 0; i < 7; ++i) {
    coordx += 30;
    coordy += 30;
    coordsx[x_ini] = coordx;
    coordsy[y_ini] = coordy;
    x_ini = x_ini - 1;
    y_ini = y_ini - 1; 
  }
  tela.fillCircle(coordsx[piaoX], coordsy[piaoY], piaoR, ILI9341_BLUE);
  tela.drawCircle(coordsx[piaoX], coordsy[piaoY], piaoR, ILI9341_BLACK);
}

void Labirinto(GFButton &botao){ 
  if(modo == 1){
  modo = 2;
  cronometroAtivo = true;
  tela.fillScreen(ILI9341_BLACK);

  //botao_menu.init(&tela, &touch, 160, 200, 100, 25, ILI9341_BLUE, ILI9341_BLUE, ILI9341_BLACK, "Menu", 1);

  tela.fillRect(15, 15, 210, 210, ILI9341_WHITE);

  tela.drawRect(15, 15, 30, 30, ILI9341_BLUE);
  tela.drawRect(45, 15, 30, 30, ILI9341_BLUE);
  tela.drawRect(75, 15, 30, 30, ILI9341_BLUE);
  tela.drawRect(105, 15, 30, 30, ILI9341_BLUE);
  tela.drawRect(135, 15, 30, 30, ILI9341_BLUE);
  tela.drawRect(165, 15, 30, 30, ILI9341_BLUE);
  tela.drawRect(195, 15, 30, 30, ILI9341_BLUE);

  tela.drawRect(15, 45, 30, 30, ILI9341_BLUE);
  tela.drawRect(45, 45, 30, 30, ILI9341_BLUE);
  tela.drawRect(75, 45, 30, 30, ILI9341_BLUE);
  tela.drawRect(105, 45, 30, 30, ILI9341_BLUE);
  tela.drawRect(135, 45, 30, 30, ILI9341_BLUE);
  tela.drawRect(165, 45, 30, 30, ILI9341_BLUE);
  tela.drawRect(195, 45, 30, 30, ILI9341_BLUE);

  tela.drawRect(15, 75, 30, 30, ILI9341_BLUE);
  tela.drawRect(45, 75, 30, 30, ILI9341_BLUE);
  tela.drawRect(75, 75, 30, 30, ILI9341_BLUE);
  tela.drawRect(105, 75, 30, 30, ILI9341_BLUE);
  tela.drawRect(135, 75, 30, 30, ILI9341_BLUE);
  tela.drawRect(165, 75, 30, 30, ILI9341_BLUE);
  tela.drawRect(195, 75, 30, 30, ILI9341_BLUE);

  tela.drawRect(15, 105, 30, 30, ILI9341_BLUE);
  tela.drawRect(45, 105, 30, 30, ILI9341_BLUE);
  tela.drawRect(75, 105, 30, 30, ILI9341_BLUE);
  tela.drawRect(105, 105, 30, 30, ILI9341_BLUE);
  tela.drawRect(135, 105, 30, 30, ILI9341_BLUE);
  tela.drawRect(165, 105, 30, 30, ILI9341_BLUE);
  tela.drawRect(195, 105, 30, 30, ILI9341_BLUE);

  tela.drawRect(15, 135, 30, 30, ILI9341_BLUE);
  tela.drawRect(45, 135, 30, 30, ILI9341_BLUE);
  tela.drawRect(75, 135, 30, 30, ILI9341_BLUE);
  tela.drawRect(105, 135, 30, 30, ILI9341_BLUE);
  tela.drawRect(135, 135, 30, 30, ILI9341_BLUE);
  tela.drawRect(165, 135, 30, 30, ILI9341_BLUE);
  tela.drawRect(195, 135, 30, 30, ILI9341_BLUE);

  tela.drawRect(15, 165, 30, 30, ILI9341_BLUE);
  tela.drawRect(45, 165, 30, 30, ILI9341_BLUE);
  tela.drawRect(75, 165, 30, 30, ILI9341_BLUE);
  tela.drawRect(105, 165, 30, 30, ILI9341_BLUE);
  tela.drawRect(135, 165, 30, 30, ILI9341_BLUE);
  tela.drawRect(165, 165, 30, 30, ILI9341_BLUE);
  tela.drawRect(195, 165, 30, 30, ILI9341_BLUE);

  tela.drawRect(15, 195, 30, 30, ILI9341_BLUE);
  tela.drawRect(45, 195, 30, 30, ILI9341_BLUE);
  tela.drawRect(75, 195, 30, 30, ILI9341_BLUE);
  tela.drawRect(105, 195, 30, 30, ILI9341_BLUE);
  tela.drawRect(135, 195, 30, 30, ILI9341_BLUE);
  tela.drawRect(165, 195, 30, 30, ILI9341_BLUE);
  tela.drawRect(195, 195, 30, 30, ILI9341_BLUE);

  DrawPiao(piaoX,piaoY);
  }
}
