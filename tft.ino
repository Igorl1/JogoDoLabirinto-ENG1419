#include <Adafruit_ILI9341.h>
//#include <JKSButton.h>
#include <TouchScreen.h>
#include <GFButton.h>

Adafruit_ILI9341 tela = Adafruit_ILI9341(8, 10, 9);
//JKSButton botao_start, botao_niv;
GFButton botao1(A1),botao2(A2),botao5(A5);
TouchScreen touch(25, 26, 27, 9, 300);
int time;
bool CronometroAtivo, IsOnMenu, Ganhou = false;

void ResetCronometro(){
  CronometroAtivo = false;
  time = 60;
}

void setup(void) {
  Serial.begin(9600);
  tela.begin();
  Menu();
  botao1.setPressHandler(Labirinto);
  botao2.setPressHandler(Select);
  botao5.setPressHandler(Menu);
}
 
void loop() {
  //botao_start.process();
  //botao_niv.process();
  botao1.process();
  botao2.process();
  botao5.process();
  Cronometro();
  delay(1000);
}

void Cronometro(){
  if(CronometroAtivo){
    tela.fillRect(86, 245, 30, 100, ILI9341_BLUE);
    tela.setCursor(90, 250);
    tela.setTextColor(ILI9341_WHITE);
    tela.setTextSize(2);
    tela.print(time);
    if (time == 0){
      End();
    }
    time = time - 1;
  }
}

void Menu(){
  ResetCronometro();
  IsOnMenu = true;
  tela.fillScreen(ILI9341_BLACK);
  //botao_start.init(&tela, &touch, 120, 120, 170, 25, ILI9341_WHITE, ILI9341_BLUE, ILI9341_WHITE, "Jogar", 1);
  //botao_niv.init(&tela, &touch, 120, 180, 170, 25, ILI9341_WHITE, ILI9341_BLUE, ILI9341_WHITE, "Nivel", 1);
  //botao_start.setPressHandler(Labirinto);
  //botao_niv.setPressHandler(Select);

  tela.fillRect(100, 120, 170, 25, ILI9341_BLUE);

  tela.fillRect(120, 160, 170, 25, ILI9341_BLUE);

  tela.fillRect(140, 200, 170, 25, ILI9341_BLUE);

  tela.fillRect(18,38,204,32, ILI9341_WHITE);
  
  tela.setCursor(19, 40);
  tela.setTextColor(ILI9341_BLACK);
  tela.setTextSize(2);
  tela.println("Jogo do Labirinto");
  tela.setCursor(93, 60);
  tela.setTextColor(ILI9341_BLACK);
  tela.setTextSize(1);
  tela.println("Turma 3VA");
}

void End(){
  ResetCronometro();
  IsOnMenu = false;
  tela.fillScreen(ILI9341_BLACK);
  tela.setCursor(20, 40);
  tela.setTextColor(ILI9341_WHITE);
  tela.setTextSize(2);
  tela.println("Fim de Jogo.");
  tela.setCursor(20, 60);
  tela.setTextColor(ILI9341_WHITE);
  tela.setTextSize(2);
  if(Ganhou){
    tela.println("Ganhou.");
  }else{
    tela.println("Perdeu.");
  }
}

void Select(GFButton &botao){ 
  if(IsOnMenu){
    tela.fillScreen(ILI9341_BLACK);
    tela.setCursor(10, 40);
    tela.setTextColor(ILI9341_WHITE);
    tela.setTextSize(2);
    tela.println("Selecione o nivel:");
  }
}

void DrawPiao(){
  tela.fillCircle(210, 210, 15, ILI9341_BLUE);
  tela.drawCircle(210, 210, 15, ILI9341_BLACK);
}

void Labirinto(GFButton &botao){ 
  CronometroAtivo = true;
  IsOnMenu = false;
  tela.fillScreen(ILI9341_BLACK);
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

  DrawPiao();

  tela.setCursor(15, 250);
  tela.setTextColor(ILI9341_WHITE);
  tela.setTextSize(2);
  tela.print("Tempo: ");
}
