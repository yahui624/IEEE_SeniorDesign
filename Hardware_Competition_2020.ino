#include <Servo.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <SPI.h>
#include <Pixy2.h>

Pixy2 pixy;

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define MIN_PULSE_WIDTH 650
#define MAX_PULSE_WIDTH 2350
#define DEFAULT_PULSE_WIDTH 1500
#define FREQUENCY 50

uint8_t servonum = 0;

const int enA = 9;
const int in1 = 8;
const int in2 = 7;

const int enB = 6;
const int in3 = 4;
const int in4 = 5;

const int enC = 10;
const int in5 = 12;
const int in6 = 13;

const int limR = 2;
const int limL = 3;

int running = 1;
int start = 1;
int aligned = 0;

const char pi[] = "\
31415926535897932384626433832795028841971693993751\
05820974944592307816406286208998628034825342117067\
98214808651328230664709384460955058223172535940812\
84811174502841027019385211055596446229489549303819\
64428810975665933446128475648233786783165271201909\
14564856692346034861045432664821339360726024914127\
37245870066063155881748815209209628292540917153643\
67892590360011330530548820466521384146951941511609\
43305727036575959195309218611738193261179310511854\
80744623799627495673518857527248912279381830119491\
29833673362440656643086021394946395224737190702179\
86094370277053921717629317675238467481846766940513\
20005681271452635608277857713427577896091736371787\
21468440901224953430146549585371050792279689258923\
54201995611212902196086403441815981362977477130996\
05187072113499999983729780499510597317328160963185\
95024459455346908302642522308253344685035261931188\
17101000313783875288658753320838142061717766914730\
35982534904287554687311595628638823537875937519577\
81857780532171226806613001927876611195909216420198\
93809525720106548586327886593615338182796823030195\
20353018529689957736225994138912497217752834791315\
15574857242454150695950829533116861727855889075098\
38175463746493931925506040092770167113900984882401\
28583616035637076601047101819429555961989467678374\
49448255379774726847104047534646208046684259069491\
29331367702898915210475216205696602405803815019351\
12533824300355876402474964732639141992726042699227\
96782354781636009341721641219924586315030286182974\
";

void setup()
{
Serial.begin(9600);

//pixy.init(); 

pwm.begin();
pwm.setPWMFreq(FREQUENCY);

pinMode(enA, OUTPUT);
pinMode(in1, OUTPUT);
pinMode(in2, OUTPUT);

pinMode(enB, OUTPUT);
pinMode(in3, OUTPUT);
pinMode(in4, OUTPUT);

pinMode(enC, OUTPUT);
pinMode(in5, OUTPUT);
pinMode(in6, OUTPUT);

pinMode(limR, INPUT);
pinMode(limL, INPUT);
}

int pulseWidth(int angle)
{
int pulse_wide, analog_value;
pulse_wide = map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
analog_value = int(float(pulse_wide) / 1000000 * FREQUENCY * 4096);
return analog_value;
}

int calc_dist_x (){
  uint16_t type;
  static int i = 0;
  int j;
  int16_t signature;
  int16_t angle;
  uint16_t blocks;  
  int prev_x_val, prev_y_val = 0;
  int curr_x_val, curr_y_val = 0;
  int x_val, y_val = 0;
   
  int desired_button_x = 896;
  int desired_button_y = 0;
  float adjust_dist_x, adjust_dist_y; 
  blocks = pixy.ccc.getBlocks();
  if (blocks)                
  {
    for ( int m=0; m< 5 ; m++) {
      i++;         //<<<<< ----- Delay each 50 Loops every 50 loop go in
      if (i % 5 == 0) //<<<<< ----- Delay each 50 Loops everi 50 loop go in
      {
        Serial.print("Detected :");
        Serial.print(blocks);
        Serial.print("\n");
  
        for (j = 0; j < blocks; j++)
        {
//          Serial.print("Sign =");
          signature = (pixy.ccc.blocks[j].m_signature);
//          Serial.print(signature,OCT);
//          Serial.print("\t");
          
//          Serial.print("Loc X =");
          curr_x_val = pixy.ccc.blocks[j].m_x;
          Serial.print("curr_x_val :");
        Serial.print(curr_x_val);
        Serial.print("\n");
          
//          Serial.print(pixy.ccc.blocks[j].m_x);
//          Serial.print("\t");
          
//          Serial.print("Loc Y =");
          curr_y_val = pixy.ccc.blocks[j].m_y; 
          
//          Serial.print(pixy.ccc.blocks[j].m_y);
//          Serial.print("\t");
          
//          Serial.print("width =");
//          Serial.print(pixy.ccc.blocks[j].m_width);
//          Serial.print("\t");
          
//          Serial.print("Height =");
//          Serial.print(pixy.ccc.blocks[j].m_height);
//          Serial.print("\t");
          
//          Serial.print("angle =");
          angle = (pixy.ccc.blocks[j].m_angle);
//          Serial.print(angle);
//          Serial.print("\t");

        }

//        Serial.print(x_val);
      }
        y_val = prev_y_val + curr_y_val;
        x_val = prev_x_val + curr_x_val;

        prev_y_val = curr_y_val;
        prev_x_val = curr_x_val;
        
      }

        adjust_dist_x = (x_val/5) - desired_button_x;
        adjust_dist_y = (y_val/5) - desired_button_y;

        // X-coordinate 
        if (adjust_dist_x > 0) {
          Serial.print("The button is to the left of the desired location =");
          Serial.print(adjust_dist_x);
          Serial.print("\t");
        }
        else if (adjust_dist_x < 0) {
          Serial.print("The button is to the right of the desired location =");
          Serial.print(adjust_dist_x);
          Serial.print("\t");
        }
        else if (adjust_dist_x == 0) {
          Serial.print("The button is at the desired location! ");
          Serial.print("\t");
        }

        // Y-coordinate 
        if (adjust_dist_y > 0) {
          Serial.print("The button is to the buttom of the desired location =");
          Serial.print(adjust_dist_y);
          Serial.print("\t");
        }
        else if (adjust_dist_y < 0) {
          Serial.print("The button is to the top of the desired location =");
          Serial.print(adjust_dist_y);
          Serial.print("\t");
        }
        else if (adjust_dist_y == 0) {
          Serial.print("The button is at the desired location! ");
          Serial.print("\t");
        }
                
     
      Serial.print("\n");

    }
  return adjust_dist_x;
}

void push0()
{
  pwm.setPWM(0, 0, pulseWidth(72));
  delay(500);
  pwm.setPWM(0, 0, pulseWidth(89));
  delay(500);
}

void push1()
{
  pwm.setPWM(0, 0, pulseWidth(115));
  delay(500);
  pwm.setPWM(0, 0, pulseWidth(89));
  delay(500);
}

void push2()
{
  pwm.setPWM(1, 0, pulseWidth(72));
  delay(500);
  pwm.setPWM(1, 0, pulseWidth(89));
  delay(500);
}

void push3()
{
  pwm.setPWM(1, 0, pulseWidth(115));
  delay(500);
  pwm.setPWM(1, 0, pulseWidth(89));
  delay(500);
}

void push4()
{
  pwm.setPWM(2, 0, pulseWidth(65));
  delay(500);
  pwm.setPWM(2, 0, pulseWidth(89));
  delay(500);
}

void push5()
{
  pwm.setPWM(2, 0, pulseWidth(116));
  delay(500);
  pwm.setPWM(2, 0, pulseWidth(89));
  delay(500);
}

void push6()
{
  pwm.setPWM(3, 0, pulseWidth(62));
  delay(500);
  pwm.setPWM(3, 0, pulseWidth(89));
  delay(500);
}

void push7()
{
  pwm.setPWM(3, 0, pulseWidth(108));
  delay(500);
  pwm.setPWM(3, 0, pulseWidth(89));
  delay(500);
}

void push8()
{
  pwm.setPWM(4, 0, pulseWidth(65));
  delay(500);
  pwm.setPWM(4, 0, pulseWidth(89));
  delay(500);
}

void push9()
{
  pwm.setPWM(4, 0, pulseWidth(100));
  delay(500);
  pwm.setPWM(4, 0, pulseWidth(89));
  delay(500);
}


void loop()
{
  // Aligns all the servos to neutral position at the start of operation
  if (running) {
    pwm.setPWM(0, 0, pulseWidth(89));
    pwm.setPWM(1, 0, pulseWidth(89));
    pwm.setPWM(2, 0, pulseWidth(89));
    pwm.setPWM(3, 0, pulseWidth(89));
    pwm.setPWM(4, 0, pulseWidth(91));
    running = 0;
    delay(2000);
  }

   Serial.print("start = ");
   Serial.print(start);
   Serial.print("\n");
// Drives forward motors to the button board until the limit switches are met
  if(!(start==2)) {
    if((!digitalRead(limR)) && (!digitalRead(limL))) {
     start = 0;
    }
  }
  
  if(start == 1) {
   // Serial.print(digitalRead(limR));
    //Serial.print(digitalRead(limL));
    analogWrite(enA,30);
    digitalWrite(in1,LOW);
    digitalWrite(in2,HIGH);
  
    analogWrite(enB,40);
    digitalWrite(in3,HIGH);
    digitalWrite(in4,LOW);
  }
  
  // Stops forward motors when both limit switches are met
  else if(start == 0) {
  analogWrite(enA,0);
  analogWrite(enB,0);
  digitalWrite(in1,HIGH);
  digitalWrite(in2,HIGH);
  digitalWrite(in3,HIGH);
  digitalWrite(in4,HIGH);
  delay(500);

  start = 2;
  pwm.setPWM(5, 0, pulseWidth(120));
  delay(3000);
  pwm.setPWM(5, 0, pulseWidth(100));
  }

if(start == 2) {
  for (int i = 0; i < 100; i++) {
    if( pi[i] == '0') {
      push0();
    }
    else if ( pi[i] == '1') {
      push1();
    }
    else if ( pi[i] == '2') {
      push2();
    }
    else if ( pi[i] == '3') {
      push3();
    }
    else if ( pi[i] == '4') {
      push4();
    }
    else if ( pi[i] == '5') {
      push5();
    }
    else if ( pi[i] == '6') {
      push6();
    }
    else if ( pi[i] == '7') {
      push7();
    }
    else if ( pi[i] == '8') {
      push8();
    }
    else if ( pi[i] == '9') {
      push9();
    }

    Serial.print("pi = ");
    Serial.print(pi[i]);
    Serial.print("\n");
  }
}
}
