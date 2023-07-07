#include <Wire.h>                             // библиотека для протокола IIC
#include <LiquidCrystal_I2C.h>        // подключаем библиотеку LCD IIC
LiquidCrystal_I2C LCD(0x3F, 16, 2); // присваиваем имя lcd для дисплея

int level = 1;       // переменная для отсчета уровня
int pause = 400; // переменная для задержки
byte p = 0;          // переменная для времени прыжка

int xPin = A0; // Пин для оси X джойстика
int yPin = A1; // Пин для оси Y джойстика
int buttonPin = 2; // Пин для кнопки джойстика

// создаем массивы дракончика, дерева, камня и птицы
byte dracon[8] = {
  0b01110,
  0b11011,
  0b11111,
  0b11100,
  0b11111,
  0b01100,
  0b10011,
  0b11000
};
byte derevo[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b11011,
  0b11011,
  0b11011,
  0b01100,
  0b01100
};
byte kamen[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b01110,
  0b11111
};
byte ptica[8] = {
  0b00100,
  0b00101,
  0b01111,
  0b11111,
  0b10100,
  0b00100,
  0b00000,
  0b00000
};
byte dracon1[8] = {
  0b01110,
  0b11011,
  0b11111,
  0b11111,
  0b11100,
  0b01100,
  0b11010,
  0b00011
};

void setup() {
  //pinMode (A1, INPUT);     // подключаем кнопку
  //analogWrite(A1, LOW);
  LCD.init();                        // инициализация LCD дисплея
  LCD.backlight();              // включение подсветки дисплея

  // создаем символы дракончика, дерева, камня и птицы
  LCD.createChar(0, dracon);
  LCD.createChar(1, derevo);
  LCD.createChar(2, kamen);
  LCD.createChar(3, ptica);
  LCD.createChar(4, dracon1);
  
  // начинаем игру: выводим надпись GO!
  LCD.setCursor(7, 0);
  LCD.print("GO!");
  delay(400);
  LCD.clear();
}

void loop() {
  // первоначальное положение дракончика и препятствия
  byte d = 1;
  byte x = 15;
  byte y = 1;
  byte a = 0;
  // выбираем препятствие, которое появится, рандомно
  byte i = random (1, 4);
  if (i == 3) y = 0;
  else y = 1;

  while (x > 0) {
    // очищаем экран и выводим номер уровня
    LCD.clear();
    LCD.setCursor(0, 0);
    LCD.print(level);

    
    //int xAxisValue = analogRead(xPin);
    int yAxisValue = analogRead(yPin);

    // считываем данные с кнопки и учитываем количество циклов в прыжке
    // если дакончик находится в прыжке долго - возвращаем его вниз
    //Serial.print("Y = ");
    //Serial.println(analogRead(yPin));
    if (yAxisValue < 10) d = 0;
    else d = 1;
    if (p > 3) d = 1;

    // выводим дракончика в нужной строке
    LCD.setCursor(3, d);
    if (a == 0) a = 4;
    else a = 0;
    LCD.print(char(a));
    // выводим препятствие
    LCD.setCursor(x, y);
    tone(10, 50);
    LCD.print(char(i));
    noTone(10);

    // если дракончик наткнулся на препятствие выводим надпись GAME OVER!
    if (x == 3 && y == d) {
      LCD.clear();
      delay(200);
      LCD.setCursor(3, 0);
      LCD.print("GAME OVER!");
      delay(600);
      LCD.clear();
      delay(400);
      LCD.setCursor(3, 0);
      LCD.print("GAME OVER!");
      delay(600);
      LCD.clear();
      LCD.setCursor(3, 1);
      LCD.print("LEVEL: ");
      LCD.print(level);
      delay(400);
      LCD.setCursor(3, 0);
      LCD.print("GAME OVER!");
      delay(3000);
      LCD.clear();

      // начинаем игру заново, обнулив уровень игры
      LCD.setCursor(7, 0);
      LCD.print("GO!");
      LCD.clear();

      level = 0;
      pause = 400;
      p = 0;
      y = 1;
      x = 0;
      break;
    }

    // если дракончик не столкнулся, то меняем положение препятствия
    // начинаем считать сколько циклов дракончик находится в прыжке
    delay(pause);
    x = x - 1;
    p = p + 1;
    if (p > 4) p = 0;
  }

  // переходим на следующий уровень и сокращаем время задержки
  level = level + 1;
  pause = pause - 20;
  if (pause < 0) pause = 0;
}
