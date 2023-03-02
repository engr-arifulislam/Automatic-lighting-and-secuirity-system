#include <IRremote.h>

// PINS

const int BUZZER = 7;
const int BULB1 = A1;
const int BULB2 = A2;
const int BULB3 = A3;
const int BULB4 = A4;
const int REMOTE = 5;
const int PIR1 = 8;
const int PIR2 = 6;
const int PIR3 = 3;
const int PIR4 = 4;
const int ldr = A5;


// PIR
int currentPirState = 0; 

// Remote
IRrecv irrecv(REMOTE);
decode_results results;
unsigned long key_value = 0;


// states
int changeAction = 0;
const int mode_select = 1;
const int waiting_for_motion = 2;
const int waiting_for_remote_input = 3;
const int buzzing_lighting_according_to_freq = 4;
const int only_buzzing = 5;
const int only_lighting = 6;
const int end_action = 7;

int value=0;

void setup() {
  Serial.begin(9600);
  
  pinMode(BUZZER, OUTPUT);
  pinMode(BULB1, OUTPUT);
  pinMode(BULB2, OUTPUT);
  pinMode(BULB3, OUTPUT);
  pinMode(BULB4, OUTPUT);
  pinMode(REMOTE, OUTPUT);
  pinMode(PIR1, INPUT);
  pinMode(PIR2, INPUT);
  pinMode(PIR3, INPUT);
  pinMode(PIR4, INPUT);

  irrecv.enableIRIn();
  irrecv.blink13(true);

  digitalWrite(BULB1, LOW);
  digitalWrite(BULB2, LOW);
  digitalWrite(BULB3, LOW);
  digitalWrite(BULB4, LOW);
  digitalWrite(BUZZER, LOW);

  Serial.println('Device Started!');
}

void loop() {
   mode_selection();

}


void mode_selection(){
        if (irrecv.decode(&results)){
 
        if (results.value == 0XFFFFFFFF)
          results.value = key_value;

        switch(results.value){
          case 0xFFA25D:
            Serial.println("1");
            security_remote_input(BULB1,PIR1);
          break;
          case 0xFF629D:
            Serial.println("2");
            security_remote_input(BULB2,PIR1);
          break;
          case 0xFFE21D:
            Serial.println("3");
            security_remote_input(BULB3,PIR1);
          break;
          case 0xFF22DD:
            Serial.println("4");
            security_remote_input(BULB4,PIR1);
          break;
          case 0xFF02FD:
            Serial.println("5");
            digitalWrite(BUZZER, HIGH);
          break ;  
          case 0xFFC23D:
            Serial.println("6");
            digitalWrite(BUZZER, LOW);
          break ; 
          case 0xFFE01F:
            Serial.println("7");
            digitalWrite(BULB1, HIGH);
          break ;  
          case 0xFFA857:
            Serial.println("8");
            digitalWrite(BULB1, LOW);
          break ;  
          case 0xFF906F:
            Serial.println("9");
            digitalWrite(BULB1,LOW);
            //digitalWrite(BULB2,LOW);
            //digitalWrite(BULB3,LOW);
            //digitalWrite(BULB4,LOW);
            ldrFunctioning();
          break ;  

        }
        key_value = results.value;
        irrecv.resume(); 
      }
  }


void security_remote_input(const int BULB,const int PIR){
  changeAction = waiting_for_motion;
  Serial.println("Hello");
  for(;;){
    currentPirState = digitalRead(PIR);
    delay(25);
    Serial.println(currentPirState);
    switch(changeAction){
      case waiting_for_motion:
        if (irrecv.decode(&results)){
          if (results.value == 0XFFFFFFFF)
          results.value = key_value;
          if (results.value == 0xFF38C7){
            Serial.println("Ok");
            return;
          }
        key_value = results.value;
        irrecv.resume();
        }
        else{
          if (currentPirState == HIGH){
            digitalWrite(BUZZER, HIGH);
            digitalWrite(BULB, HIGH);
            changeAction = waiting_for_remote_input;
            delay(200);
            }
        }
      break;
  
      case waiting_for_remote_input:
        //Serial.println('Motion detected!');
        //Serial.println(currentPirState);
  
          if (irrecv.decode(&results)){
   
          if (results.value == 0XFFFFFFFF)
            results.value = key_value;
  
          switch(results.value){
            case 0xFFA25D:
              Serial.println("1");
            break;
            case 0xFF629D:
              Serial.println("2");
            break;
            case 0xFFE21D:
              Serial.println("3");
            break;
            case 0xFF22DD:
              Serial.println("4");
            break;
            case 0xFF02FD:
            Serial.println("5");
            break ;  
            case 0xFFC23D:
              Serial.println("6");
            break ;               
            case 0xFFE01F:
              Serial.println("7");
            break ;  
            case 0xFFA857:
              Serial.println("8");
            break ;  
            case 0xFF906F:
              Serial.println("9");
            break ;  
            case 0xFF6897:
              Serial.println("*");
              digitalWrite(BULB, LOW);
              changeAction = only_buzzing;
            break ;  
            case 0xFF9867:
              Serial.println("0");
            break ;
            case 0xFFB04F:
              Serial.println("#");
              digitalWrite(BUZZER, LOW);
              changeAction = only_lighting;
            break ;
            case 0xFF18E7:
              Serial.println("Up");
              //delay(200);
              digitalWrite(BUZZER, LOW);
              digitalWrite(BULB, LOW);
              changeAction = buzzing_lighting_according_to_freq;
              
            break ;
            case 0xFF10EF:
              Serial.println("Left");
              digitalWrite(BUZZER, LOW);
              digitalWrite(BULB, HIGH);
            break ;
            case 0xFF38C7:
              Serial.println("Ok");
              changeAction = end_action;
            break ;
            case 0xFF5AA5:
              Serial.println("Right");
              digitalWrite(BUZZER, HIGH);
              digitalWrite(BULB, LOW);
            break ;
            case 0xFF4AB5:
              Serial.println("Down");
              digitalWrite(BUZZER, LOW);
              digitalWrite(BULB, LOW);
              changeAction = waiting_for_motion;
              //buzzingNlightingAccordingToFrequency(BULB,BUZZER);
            break ;
          }
          key_value = results.value;
          irrecv.resume(); 
      }
      break;
      case buzzing_lighting_according_to_freq:        
        delay(300);
        //Serial.println("hello");
        if (irrecv.decode(&results)){
          if (results.value == 0XFFFFFFFF)
            results.value = key_value;
            if (results.value == 0xFF10EF){
              Serial.println("Left");
              digitalWrite(BUZZER, LOW);
              digitalWrite(BULB, LOW);
              changeAction = waiting_for_remote_input; 
            }
            key_value = results.value;
            irrecv.resume();
        }
        else{
          digitalWrite(BUZZER, LOW);
          digitalWrite(BULB, LOW);
          delay(500);
          digitalWrite(BUZZER, HIGH);
          digitalWrite(BULB, HIGH);
          delay(500);
          }
     break;
     case only_buzzing:        
        delay(300);
        //Serial.println("hello");
        if (irrecv.decode(&results)){
          if (results.value == 0XFFFFFFFF)
            results.value = key_value;
            if (results.value == 0xFF10EF){
              Serial.println("Left");
              digitalWrite(BUZZER, LOW);
              changeAction = waiting_for_remote_input; 
            }
            key_value = results.value;
            irrecv.resume();
        }
        else{
          digitalWrite(BUZZER, LOW);
          delay(500);
          digitalWrite(BUZZER, HIGH);
          delay(500);
          }
     break;
     case only_lighting:        
        delay(300);
        //Serial.println("hello");
        if (irrecv.decode(&results)){
          if (results.value == 0XFFFFFFFF)
            results.value = key_value;
            if (results.value == 0xFF10EF){
              Serial.println("Left");
              digitalWrite(BULB, LOW);
              changeAction = waiting_for_remote_input; 
            }
            key_value = results.value;
            irrecv.resume();
        }
        else{
          digitalWrite(BULB, LOW);
          delay(500);
          digitalWrite(BULB, HIGH);
          delay(500);
          }
     break;     
     case end_action:
        digitalWrite(BUZZER, LOW);
        digitalWrite(BULB, LOW);
        return;
        //goto exit_loop;
     break;
    }
  }
  //exit_loop:;
}


void ldrFunctioning(){
  for(;;){

  if (irrecv.decode(&results)){
    if (results.value == 0XFFFFFFFF)
    results.value = key_value;
    if (results.value == 0xFF10EF){
      Serial.println("Left");
      digitalWrite(BULB1,LOW);
      //digitalWrite(BULB2,LOW);
      //digitalWrite(BULB3,LOW);
      //digitalWrite(BULB4,LOW);
      return;
    }
    key_value = results.value;
    irrecv.resume();
  }
  else{
    value=analogRead(ldr);//Reads the Value of LDR(light).
    Serial.println("LDR value is :");//Prints the value of LDR to Serial Monitor.
    Serial.println(value);
    if(value>500)
      {
      digitalWrite(BULB1,HIGH);//Makes the LED glow in Dark.
      //digitalWrite(BULB2,HIGH);
      //digitalWrite(BULB3,HIGH);
      //digitalWrite(BULB4,HIGH);
    }
    else
    {
      digitalWrite(BULB1,LOW);
      //digitalWrite(BULB2,LOW);
      //digitalWrite(BULB3,LOW);
      //digitalWrite(BULB4,LOW);
    }
    
  }
  } 
}
