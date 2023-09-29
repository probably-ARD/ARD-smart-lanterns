#include <NewPing.h>

//
//░█████╗░██████╗░██████╗░  ██████╗░██████╗░░█████╗░░░░░░██╗███████╗░█████╗░████████╗
//██╔══██╗██╔══██╗██╔══██╗  ██╔══██╗██╔══██╗██╔══██╗░░░░░██║██╔════╝██╔══██╗╚══██╔══╝
//███████║██████╔╝██║░░██║  ██████╔╝██████╔╝██║░░██║░░░░░██║█████╗░░██║░░╚═╝░░░██║░░░
//██╔══██║██╔══██╗██║░░██║  ██╔═══╝░██╔══██╗██║░░██║██╗░░██║██╔══╝░░██║░░██╗░░░██║░░░
//██║░░██║██║░░██║██████╔╝  ██║░░░░░██║░░██║╚█████╔╝╚█████╔╝███████╗╚█████╔╝░░░██║░░░
//╚═╝░░╚═╝╚═╝░░╚═╝╚═════╝░  ╚═╝░░░░░╚═╝░░╚═╝░╚════╝░░╚════╝░╚══════╝░╚════╝░░░░╚═╝░░░
//

String author = "probably-ARD";
String version = "1.0.1";

// стартовые настройки =======================================================================

// 1 сонар
#define TRIG_1 12 // D port
#define ECHO_1 11 // D port
#define LAMP_1 10 // D port
#define MAX_DISTANCE_1 200 // cm

// 2 сонар
#define TRIG_2 9 // D port
#define ECHO_2 8 // D port
#define LAMP_2 7 // D port
#define MAX_DISTANCE_2 200 // cm

// 3 сонар
#define TRIG_3 6 // D port
#define ECHO_3 5 // D port
#define LAMP_3 4 // D port
#define MAX_DISTANCE_3 200 // cm

// 1 кнопка
#define BTN_1 1 // A port

// 2 кнопка
#define BTN_2 2 // A port

// 3 кнопка
#define BTN_3 3 // A port

// время горения света в мс
#define LIGHT_TIME 15000

// ===========================================================================================

// переменные --------------------------------------------------------------------------------

// сонары
NewPing sonar_1(TRIG_1, TRIG_1, MAX_DISTANCE_1);
NewPing sonar_2(TRIG_2, TRIG_2, MAX_DISTANCE_2);
NewPing sonar_3(TRIG_3, TRIG_3, MAX_DISTANCE_3);

// длина
unsigned int distance_1;
unsigned int distance_2;
unsigned int distance_3;

// предыдущие значения
int past_distances_1;
int past_distances_2;
int past_distances_3;

// наличие движения нужны чтобы не было постоянного переключения тока
bool move_1 = false;
bool move_2 = false;
bool move_3 = false;

// начало горения лампы
long int start_lamp_1;
long int start_lamp_2;
long int start_lamp_3;

// -------------------------------------------------------------------------------------------

void setup() {
  Serial.begin(9600);
  
  // первоначальное чтение датчиков
  distance_1 = sonar_1.ping_cm();
  distance_2 = sonar_2.ping_cm();
  distance_3 = sonar_3.ping_cm();

  past_distances_1 = distance_1;
  past_distances_2 = distance_2;
  past_distances_3 = distance_3;
}

void loop() {
  delay(200);
  // дистанция с датчиков
  distance_1 = sonar_1.ping_cm();
  distance_2 = sonar_2.ping_cm();
  distance_3 = sonar_3.ping_cm();

  // чтение кнопок, на случай если датчики плохо работают ====================================
  if (analogRead(BTN_1) == 1023){
    distance_1 = (past_distances_1 + 10) * 10;
    Serial.println("ДВИЖЕНИЕ С КНОПКИ 1");
  };
  if (analogRead(BTN_2) == 1023){
    distance_2 = (past_distances_2 + 10) * 10;
    Serial.println("ДВИЖЕНИЕ С КНОПКИ 2");
  };
  if (analogRead(BTN_3) == 1023){
    distance_3 = (past_distances_3 + 10) * 10;
    Serial.println("ДВИЖЕНИЕ С КНОПКИ 3");
  };
  // ==========================================================================================

  // определяем движение и включаем свет ------------------------------------------------------

  if (distance_1 > (past_distances_1 * 2)){
    move_1 = true;
    Serial.println("ДВИЖЕНИЕ НА 1 ДАТЧИКЕ");
    
    digitalWrite(LAMP_1, HIGH);
    start_lamp_1 = millis();
    
    digitalWrite(LAMP_2, HIGH);
    start_lamp_2 = millis();
    
    Serial.println("ВКЛЮЧАЮ 1 И 2 ЛАМПЫ");
  }
  if (distance_2 > (past_distances_2 * 2)){
    move_2 = true;
    Serial.println("ДВИЖЕНИЕ НА 2 ДАТЧИКЕ");
    
    digitalWrite(LAMP_1, HIGH);
    start_lamp_1 = millis();
    
    digitalWrite(LAMP_2, HIGH);
    start_lamp_2 = millis();
    
    digitalWrite(LAMP_3, HIGH);
    start_lamp_3 = millis();
    
    Serial.println("ВКЛЮЧАЮ 1 И 2 И 3 ЛАМПЫ");
  }
  if (distance_3 > (past_distances_3 * 2)){
    move_3 = true;
    Serial.println("ДВИЖЕНИЕ НА 3 ДАТЧИКЕ");
    
    digitalWrite(LAMP_2, HIGH);
    start_lamp_2 = millis();
    
    digitalWrite(LAMP_3, HIGH);
    start_lamp_3 = millis();
    
    Serial.println("ВКЛЮЧАЮ 2 И 3 ЛАМПЫ");
  }

  // ------------------------------------------------------------------------------------------

  // выключаем свет, если горит больше заданного времени ======================================

  if ((move_1 == true) or (move_2 == true) or (move_3 == true)){
    if (millis() - start_lamp_1 > LIGHT_TIME){
      move_1 = false;
      digitalWrite(LAMP_1, LOW);
      Serial.println("ВЫКЛЮЧАЮ 1 ЛАМПУ");
    }
    
    if (millis() - start_lamp_2 > LIGHT_TIME){
      move_2 = false;
      digitalWrite(LAMP_2, LOW);
      Serial.println("ВЫКЛЮЧАЮ 2 ЛАМПУ");
    }

    if (millis() - start_lamp_3 > LIGHT_TIME){
      move_3 = false;
      digitalWrite(LAMP_3, LOW);
      Serial.println("ВЫКЛЮЧАЮ 3 ЛАМПУ");
    }
  }
  // ==========================================================================================

  // переписываем старые данные
  past_distances_1 = distance_1;
  past_distances_2 = distance_2;
  past_distances_3 = distance_3;
}
