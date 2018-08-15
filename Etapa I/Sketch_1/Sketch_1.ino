/*
NUEVO CODIGO PARA LA LECTURA DE VOLTAJES.
*/
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);


int contador=0;
const int An1=A3;//VOLTAJE DE PANELES
const int An3=A2;//VOLTAJE DE BATERIAS
const int An2=A2;//SENSOR DE CORRIENTE

const int Ac1=5;
const int Ac2=7;
const int Ac3=10;

bool LevelBateria=false;
bool LevelSolar=false;
bool LevelCorriente=false;

bool Bomba=false;

int Max_Solar=100;
int Min_Solar=30;
int Objt_Solar=70;

int Max_Bateria=50;
int Min_Bateria=20;
int Objt_Bateria=30;

int Max_Corriente=12;
int Min_Corriente=1;
int Objt_Corriente=4;

int Time=500;

bool Req_de_maximo(float value, int Obj){
    if (value >= Obj){
        return true;
    }
    return false;
}

float val1=0.0;
float val2=0.0;

void setup(){
    Serial.begin(9600);
    Serial.println("INICIANDO SOLARBOMB");

    pinMode(An1, INPUT);
    pinMode(An3, INPUT);
    pinMode(An2, INPUT);

    pinMode(Ac1, OUTPUT);
    pinMode(Ac2, OUTPUT);
    pinMode(Ac3, OUTPUT);

    digitalWrite(Ac1, LOW);
    digitalWrite(Ac2, LOW);
    digitalWrite(Ac3, LOW);

    delay(500);
    Serial.println("INICIANDO LECTURAS:...");
}
void loop(){
    contador=contador+1;
    Serial.print("LECTURA NUMERO:  ");Serial.println(contador);
    val1=Lect_Activada(An1,0.0760869565217391,Ac1);
    val2=Lect_Activada(An3,0.0760869565217391,Ac2);
    Serial.print("A1: ");
    Serial.println(val1);
    Serial.print("A3: ");
    Serial.println(val2);
    delay(500);
}

float Lect_Activada(int AL, float k, int act){
    delay(10);
    float Sumador=0.0;
    //Serial.print("LECTURA ACTIVADA DE LA ENTRADA: ");Serial.println(AL);
    digitalWrite(act,HIGH);
    for(int i =0;i<50;i++){
        delay(5);
        Sumador=Sumador+LecturaVoltajeII(AL,k);
        delay(5);
    }
    digitalWrite(act,LOW);
    return Sumador/50;
}
float LecturaVoltajeII(int AL, float K){
    return ((0.004887585532746820*analogRead(AL))/K)-2.9;
}

void DisplayLCD(char up[], char down[], int J)
{

  if (J == -1)
  {
    lcd.begin(16, 2);
    lcd.print(up);
    return;
  }
  if (J == -2)
  {
    lcd.setCursor(0, 1);
    lcd.print(down);
    return;
  }
  lcd.begin(16, 2);
  lcd.print(up);
  lcd.setCursor(0, 1);
  lcd.print(down);
}