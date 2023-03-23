#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

# define  SCREEN_WIDTH  128  // Ширина дисплея OLED, у пікселях
# define  SCREEN_HEIGHT  64  // Висота дисплея OLED, у пікселях

#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//VARS
int stpes = 100;
bool chartest = true;
int chartime = 0;
bool idling = false;

// constants won't change. They're used here to set pin numbers:
//BTNs
const int buttonPin1 = 2; // Corner STOP
const int buttonPin2 = 3; // Hook DOWN
const int buttonPin3 = 4; // Hook CLOSE
//MOTORs
// Motor A connections HOOK
const int enA = 10;
const int in1 = 9;
const int in2 = 8;
// Motor B connections CORNER
const int enB = 7;
const int in3 = 6;
const int in4 = 5;
//RELAY
const int rp1 = 11;

//SCREEN VARS
const int is = 2;

// variables will change:
//BTNs
int buttonState1 = 0;  // variable for reading the pushbutton status Corner STOP 
int buttonState2 = 0;  // variable for reading the pushbutton status Hook DOWN
int buttonState3 = 0;  // variable for reading the pushbutton status Hook CLOSE

void setup() {
  Serial.begin(9600);

  // initialize the pushbuttons pin as an input:
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin3, INPUT);

  // Set all the motor control pins to outputs
	pinMode(enA, OUTPUT);
	pinMode(enB, OUTPUT);
	pinMode(in1, OUTPUT);
	pinMode(in2, OUTPUT);
	pinMode(in3, OUTPUT);
	pinMode(in4, OUTPUT);
  // Turn off motors - Initial state
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);

  //Screen setup
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

}

void loop() {
  drawLine();
  drawText();
  if (chartest = true) {
    if (chartime < 1) {
      for (int step = 0; step < stpes; ++step) {
        Serial.print("Step ");
        Serial.print(step);
        Serial.print(" of ");
        Serial.println(stpes);
        //Serial.println("Relay ON.");
        //digitalWrite(rp1, HIGH); // замыкаем реле
        oneStepIdlingPLUS();
        //Serial.println("Relay OFF.");
        //digitalWrite(rp1, LOW); // размыкаем реле
        delay(1000);
      }
    }
  }
}

// Открытие крюка на пол оборота. Верхнее положение
void moveAmotor1(){
  // Set motors to maximum speed
	// For PWM maximum possible values are 0 to 255
	analogWrite(enA, 50);
  // Turn on motor A & B
	digitalWrite(in1, HIGH);
	digitalWrite(in2, LOW);
  delay(1000); // Time to move motor
  // Turn off motors
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
}

// Закрытие крюка до нажатия кнопки
void moveAmotor2(){
  int buttonStateForMotorA = digitalRead(buttonPin2);
  analogWrite(enA, 50);
  bool bnt2push = true;
  while (bnt2push = true){
    if (buttonStateForMotorA == HIGH) {
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      delay(3);
    } else {
      // Turn off motors
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
      bnt2push = false;
    }
  }
}

// Закрытие крюка до контакта с шиной
void moveAmotor3(){
  int buttonStateForMotorA = digitalRead(buttonPin3);
  analogWrite(enA, 50);
  bool bnt3push = true;
  while (bnt3push = true){
    if (buttonStateForMotorA == HIGH) {
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      delay(3);
    } else {
      // Turn off motors
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
      bnt3push = false;
    }
  }
}

// Открытие крюка на четверть оборота. Из зарядного положения в верхнее.
void moveAmotor4(){
  analogWrite(enA, 50);
  digitalWrite(in1, HIGH);
	digitalWrite(in2, LOW);
  delay(500); // Time to move motor
  // Turn off motors
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
}

// Подача уголка до нажатия кнопки
void moveBmotor1(){
  int buttonStateForMotorB = digitalRead(buttonPin1);
  analogWrite(enB, 50);
  bool bnt1push = true;
  while (bnt1push = true){
    if (buttonStateForMotorB == HIGH) {
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
      delay(3);
    } else {
      // Turn off motors
      digitalWrite(in3, LOW);
      digitalWrite(in4, LOW);
      bnt1push = false;
    }
  }
}

// Убрать уголок на пол оборота
void moveBmotor2(){
  analogWrite(enB, 50);
  digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
  delay(1000); // Time to move motor
  // Turn off motors
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
}

void oneStepIdlingTest(){
  // read the state of the pushbuttons value:
  //buttonState1 = digitalRead(buttonPin1);
  buttonState2 = digitalRead(buttonPin2);
  //buttonState3 = digitalRead(buttonPin3);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState2 == HIGH) {
    //Пол оборота по часовой.
    Serial.println("Move CW");
    moveAmotor1();
    //Пауза.
    Serial.println("Pause");
    delay(1000);
    //Обороты против часовой пока кнопка снова не будет нажата
    Serial.println("Move CCW1");
    moveAmotor2();
    //Пауза.
    delay(1000);
  } else {
    //Обороты против часовой пока кнопка снова не будет нажата
    Serial.println("Move CCW2");
    moveAmotor2();
    //Пауза.
    delay(1000);
  }
}

void oneStepIdlingPLUS(){
  // read the state of the pushbuttons value:
  buttonState1 = digitalRead(buttonPin1);
  buttonState2 = digitalRead(buttonPin2);
  buttonState3 = digitalRead(buttonPin3);

  //  Условия
  // Если крюк закрыт
  if (buttonState2 == HIGH){  
    // Контакта нет   
    if (buttonState3 == LOW){   
      // Уголк отведен назад 
      if (buttonState1 == LOW){  
        moveAmotor1();  // Повернуть крюк на пол оброта
        delay(1000);    // Пауза
        moveBmotor1();  // Подать кголок до нажатия кнопки
        delay(1000);// Пауза
        moveAmotor3();// Повернуть крюк до контакта с шиной
      }
    }
  }

  // Если контакт замкнут
  if (buttonState3 == HIGH){
    // Уголок в переднем положении
    if (buttonState1 == HIGH){
      // Крюк не закрыт. В зарядном положениии
      if (buttonState2 == LOW){  
          moveAmotor4();// Повернуть крюк на четверть оборота
          delay(1000);// Пауза
          moveBmotor2();// Убрать уголок на пол оборота
          delay(1000);// Пауза
          moveAmotor2();// Повернуть крюк до закрытия
          delay(1000);// Пауза
      }
    }
  }
}

void drawLine(){
  display.clearDisplay();
  
  // Horisontal line
  display.drawLine(0, 0, display.width(), 0, SSD1306_WHITE);
  display.drawLine(0, 15, display.width(), 15, SSD1306_WHITE);
  display.drawLine(0, 23, display.width(), 23, SSD1306_WHITE);
  display.drawLine(0, 31, display.width(), 31, SSD1306_WHITE);
  display.drawLine(0, 39, display.width(), 39, SSD1306_WHITE);
  display.drawLine(0, 47, display.width(), 47, SSD1306_WHITE);
  display.drawLine(0, 55, display.width(), 55, SSD1306_WHITE);
  display.drawLine(0, display.height(), display.width(), display.height(), SSD1306_WHITE);

  // Vertical line
  display.drawLine(0, 0, 0, display.height(), SSD1306_WHITE);
  display.drawLine(40, 15, 40, display.height(), SSD1306_WHITE);
  display.drawLine(54, 55, 54, display.height(), SSD1306_WHITE);
  display.drawLine(64, 15, 64, display.height(), SSD1306_WHITE);
  display.drawLine(104, 15, 104, display.height(), SSD1306_WHITE);
  display.drawLine(118, 15, 118, display.height(), SSD1306_WHITE);
  display.drawLine(128, 15, 128, display.height(), SSD1306_WHITE);

  display.display();
}

void drawText(){

  // Temporary variables
  char tv_test_mode = "TEST MODE: ";
  int tv_step = 17;
  bool tv_btn1 = false;
  bool tv_btn2 = false;
  bool tv_btn3 = false;
  int tv_time1 = 10000;
  int tv_time2 = 500;
  float tv_temper = 30.2;
  float tv_volt1 = 11.4;
  float tv_volt2 = 14.3;
  float tv_cur1 = 10.4;
  float tv_cur2 = 11.3;

  display.clearDisplay();
  // Text size
  display.setTextSize(2);
  // Text color
  display.setTextColor(WHITE);

  // Test mode part
  display.setCursor(2,2);
  display.println("TEST MODE: ");
  display.setCursor(2,30);
  display.println(tv_test_mode);

  // Steps Inscription
  display.setCursor(17,2);
  display.println("STEPS:");
  display.setCursor(17,42);
  display.println(tv_step);
  display.setCursor(17,46);
  display.println("/");
  display.setCursor(17,48);
  display.println(stpes);

  //Btn1 status
  display.setCursor(25,2);
  display.println("BTN1:");
  display.setCursor(25,42);
  display.println(tv_btn1);

  //Btn2 status
  display.setCursor(33,2);
  display.println("BTN2:");
  display.setCursor(33,42);
  display.println(tv_btn2);

  //Btn3 status
  display.setCursor(41,2);
  display.println("BTN3:");
  display.setCursor(41,42);
  display.println(tv_btn2);

  // Time since the start of the test
  display.setCursor(49,2);
  display.println("TIME1:");
  display.setCursor(49,42);
  display.println(tv_time1);

  // Charging test time
  display.setCursor(57,2);
  display.println("TIME2:");
  display.setCursor(57,42);
  display.println(tv_time2);
  display.setCursor(57,56);
  display.println("S");

  // Temperature val
  display.setCursor(17,66);
  display.println("TEMPER.:");
  display.setCursor(17,106);
  display.println(tv_temper);

  //Voltage 1 val
  display.setCursor(25,66);
  display.println("VOLTAGE1:");
  display.setCursor(25,106);
  display.println(tv_volt1);

  //Voltage 2 val
  display.setCursor(33,66);
  display.println("VOLTAGE2:");
  display.setCursor(33,106);
  display.println(tv_volt2);

  //Current 1 val
  display.setCursor(41,66);
  display.println("CURRENT1:");
  display.setCursor(41,106);
  display.println(tv_cur1);

  //Current 2 val
  display.setCursor(49,66);
  display.println("CURRENT2:");
  display.setCursor(49,106);
  display.println(tv_cur2);

  // Notation
  display.setCursor(17,120);
  display.println("С");
  display.setCursor(17,120);
  display.println("V");
  display.setCursor(17,120);
  display.println("V");
  display.setCursor(17,120);
  display.println("A");
  display.setCursor(17,120);
  display.println("A");

  display.display();
}