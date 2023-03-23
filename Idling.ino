//VARS
int stpes = 100;
bool chartest = false;
int chartime = 0;
bool idling = true;

// constants won't change. They're used here to set pin numbers:
//BTNs
//const int buttonPin1 = 2; // Corner STOP
const int buttonPin2 = 3; // Hook DOWN
//const int buttonPin3 = 4; // Hook CLOSE
//MOTORs
// Motor A connections HOOK
const int enA = 10;
const int in1 = 9;
const int in2 = 8;
// Motor B connections CORNER
//const int enB = 7;
//const int in3 = 6;
//const int in4 = 5;

// variables will change:
//BTNs
//int buttonState1 = 0;  // variable for reading the pushbutton status Corner STOP 
int buttonState2 = 0;  // variable for reading the pushbutton status Hook DOWN
//int buttonState3 = 0;  // variable for reading the pushbutton status Hook CLOSE

void setup() {
  Serial.begin(9600);

  // initialize the pushbuttons pin as an input:
  //pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  //pinMode(buttonPin3, INPUT);

  // Set all the motor control pins to outputs
	pinMode(enA, OUTPUT);
	//pinMode(enB, OUTPUT);
	pinMode(in1, OUTPUT);
	pinMode(in2, OUTPUT);
	//pinMode(in3, OUTPUT);
	//pinMode(in4, OUTPUT);
  // Turn off motors - Initial state
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
	//digitalWrite(in3, LOW);
	//digitalWrite(in4, LOW);

}

void loop() {
  if (idling = true){
    Serial.println("Start idling test!");
    for (int step = 0; step < stpes; ++step){
      Serial.print("Step ");
      Serial.print(step);
      Serial.print(" of ");
      Serial.println(stpes);
      oneStepIdlingTest();
      delay(1000);
    }
  } else {
    Serial.println("Idling test FLASE!");
  }
}

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

void moveAmotor2() {
  int buttonStateForMotor = digitalRead(buttonPin2);
  analogWrite(enA, 50);
  bool bnt2push = true;
  while (bnt2push = true){
    if (buttonStateForMotor == HIGH) {
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
    } else {
      // Turn off motors
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
      bnt2push = false;
    }
  }
  

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
