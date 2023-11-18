//
//░█████╗░██████╗░██████╗░  ██████╗░██████╗░░█████╗░░░░░░██╗███████╗░█████╗░████████╗
//██╔══██╗██╔══██╗██╔══██╗  ██╔══██╗██╔══██╗██╔══██╗░░░░░██║██╔════╝██╔══██╗╚══██╔══╝
//███████║██████╔╝██║░░██║  ██████╔╝██████╔╝██║░░██║░░░░░██║█████╗░░██║░░╚═╝░░░██║░░░
//██╔══██║██╔══██╗██║░░██║  ██╔═══╝░██╔══██╗██║░░██║██╗░░██║██╔══╝░░██║░░██╗░░░██║░░░
//██║░░██║██║░░██║██████╔╝  ██║░░░░░██║░░██║╚█████╔╝╚█████╔╝███████╗╚█████╔╝░░░██║░░░
//╚═╝░░╚═╝╚═╝░░╚═╝╚═════╝░  ╚═╝░░░░░╚═╝░░╚═╝░╚════╝░░╚════╝░╚══════╝░╚════╝░░░░╚═╝░░░
//

String author = "probably-ARD";
String version = "1.2.4";

// sketch settings ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// sonars trig and echo pins (!!! D ports !!!)
#define TRIG_1 3
#define TRIG_2 6
#define TRIG_3 13

#define ECHO_1 2
#define ECHO_2 5
#define ECHO_3 12

// lamps pins (!!! D ports !!!)
#define LAMP_1 7
#define LAMP_2 10
#define LAMP_3 4

// lamps times in ms
#define LIGHT_TIME 15000

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// variables ------------------------------------------------------------------------

int duration, distance;
int old_distance_1, old_distance_2, old_distance_3;
int now_distance_1, now_distance_2, now_distance_3;
long int start_lamp_1, start_lamp_2, start_lamp_3;
bool move_1, move_2, move_3;

// ----------------------------------------------------------------------------------

// funcs ============================================================================

int getDistance(int trig, int echo){
  // zeroing out hc-sr04
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);

  digitalWrite(trig, LOW);
  
  // get hc-sr04 duration
  duration = pulseIn(echo, HIGH);
  distance = duration / 58.8;

  return distance;
}

void lampStartStop(){
  
  // on lamps if move true
  if ((now_distance_1 * 1.5) < old_distance_1){
    move_1 = true;
    Serial.println("move on 1 sonar");
    
    digitalWrite(LAMP_1, HIGH);
    start_lamp_1 = millis();
    
    digitalWrite(LAMP_2, HIGH);
    start_lamp_2 = millis();
    
    Serial.println("turn ON 1, 2 lamp");
  }
  
  if ((now_distance_2 * 1.5) < old_distance_2){
    move_2 = true;
    Serial.println("move on 2 sonar");
    
    digitalWrite(LAMP_1, HIGH);
    start_lamp_1 = millis();
    
    digitalWrite(LAMP_2, HIGH);
    start_lamp_2 = millis();
    
    digitalWrite(LAMP_3, HIGH);
    start_lamp_3 = millis();
    
    Serial.println("turn ON 1, 2, 3 lamp");
  }
  
  if ((now_distance_3 * 1.5) < old_distance_3){
    move_3 = true;
    Serial.println("move on 3 sonar");
    
    digitalWrite(LAMP_2, HIGH);
    start_lamp_2 = millis();
    
    digitalWrite(LAMP_3, HIGH);
    start_lamp_3 = millis();
    
    Serial.println("turn ON 2, 3 lamp");
  }

  // off lamps if time skeep
  if ((move_1 == true) or (move_2 == true) or (move_3 == true)){
    
    if (millis() - start_lamp_1 > LIGHT_TIME){
      move_1 = false;
      digitalWrite(LAMP_1, LOW);
      Serial.println("turn OFF 1 lamp");
    }
    
    if (millis() - start_lamp_2 > LIGHT_TIME){
      move_2 = false;
      digitalWrite(LAMP_2, LOW);
      Serial.println("turn OFF 2 lamp");
    }

    if (millis() - start_lamp_3 > LIGHT_TIME){
      move_3 = false;
      digitalWrite(LAMP_3, LOW);
      Serial.println("turn OFF 3 lamp");
    }
  }
}

// ==================================================================================

void setup() {
  Serial.begin(9600);
  
  // filling start distances
  old_distance_1 = getDistance(TRIG_1, ECHO_1);
  old_distance_2 = getDistance(TRIG_2, ECHO_2);
  old_distance_3 = getDistance(TRIG_3, ECHO_3);
}

void loop() {
  now_distance_1 = getDistance(TRIG_1, ECHO_1);
  now_distance_2 = getDistance(TRIG_2, ECHO_2);
  now_distance_3 = getDistance(TRIG_3, ECHO_3);

  lampStartStop();

  old_distance_1 = now_distance_1;
  old_distance_2 = now_distance_2;
  old_distance_3 = now_distance_3;

  delay(50);
}
