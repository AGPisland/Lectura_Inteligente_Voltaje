#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

//CONSTANTE DE SALIDAS Y ENTRADAS DE TRABAJO
const int RELE_BATERIA=7;
const int RELE_PANEL=6;
const int ANALOGO_BATERIA=A3;
const int ANALOGO_PANEL=A2;
const int ANALOGO_CORRIENTE=A4;
const int LEDRB=8;
const int LEDAP=9;
const int LEDAB=10;
const int LEDVC=11;
const int SAMPLES=10;
//CONSTANTES PARA CALCULOS
const float PENDIENTE=0.004585;
const float DIV_A3_7_VOLTAJE_BATERIA=0.046283;
const float DIV_A2_6_VOLTAJE_PANEL=0.076086-0.003940;

//VARIABLES DE MUESTREO
int contador=0;
bool BATERIA_STATUS=false;
bool PANEL_STATUS=false;
bool CORRIENTE_STATUS=false;
bool BOMBA=false;
float V_PANEL=0.000000, V_BATERIAS=0.000000, C_PANEL=0.00000, P_PANEL=0.000000;

//MUESTREO_DE_VALORES

void TOMA_MEDIDAS(){
    int i;
    float I1=0.00000,V1=0.00000,V2=0.00000;
    MODO_DIGITAL(1);
    for (i=0;i<SAMPLES;i++){
        //CORRIENTE SOLAR
        delay(20);
        I1+=analogRead(ANALOGO_CORRIENTE);
        //VOLTAJE PANEL
        delay(20);
        V1+=TR_LINEAL_1(ANALOGO_PANEL)/DIV_A2_6_VOLTAJE_PANEL;
        //VOLTAJE BATERIA
        delay(20);
        V2+=TR_LINEAL_1(ANALOGO_BATERIA)/DIV_A3_7_VOLTAJE_BATERIA;
    }
    MODO_DIGITAL(0);
    C_PANEL=I1/SAMPLES;
    V_PANEL=V1/SAMPLES;
    V_BATERIAS=V2/SAMPLES;
    delay(20);
}

float TR_LINEAL_1(int MUESTRA){
    return PENDIENTE*(analogRead(MUESTRA)-216.000000)+0.990000;  
}

int RESPUESTA_NODO(float val1, float val2, float val3){
    if (val1 >= 40.000000){
        if (val2 >= 70.000000){
            if (val3 <= 10.000000){
                Serial.print("111");
                return 6;
            }
            Serial.print("110");
            return 5;
        }
        if (val3 <= 10.000000){
            Serial.print("101");
            return 4;
        }
        Serial.print("100");
        return 3;
    }
    if (val2 >= 70.000000){
        if (val3 <=10.000000){
            Serial.print("011");
            return 2;
        }
    }
    Serial.print("000");
    return 1;
}

void setup(){
    Serial.begin(9600);
    pinMode(ANALOGO_BATERIA,INPUT);// VOLTAJE BATERIAS
    pinMode(ANALOGO_PANEL, INPUT);// VOLTAJE PANELES RANGO [30---101,43]
    pinMode(ANALOGO_CORRIENTE_SNSOR, INPUT);
    pinMode(RELE_BATERIA, OUTPUT);// RELE DE PANELES
    pinMode(RELE_PANEL, OUTPUT);// RELE DE BATERIAS
    pinMode(LEDRB,OUTPUT);
    pinMode(LEDAP,OUTPUT);
    pinMode(LEDAB,OUTPUT);
    pinMode(LEDVC,OUTPUT);
    digitalWrite(RELE_BATERIA,LOW);// 6 ACTIVA A A2  6 EN PLACA ES CONSTANTE DE 33 KOHMS
    digitalWrite(RELE_PANEL, LOW);// 7 ACTIVA A A3  7 EN PLACA ES CONSTANTE DE 50 KOHMS 
    digitalWrite(LEDRB,LOW);
    digitalWrite(LEDAP,LOW);
    digitalWrite(LEDAB,LOW);
    digitalWrite(LEDVC,LOW);
    delay(100);
    contador=contador+1;
    Serial.println("INICIADO, LED APAGADOS, RELES APAGADOS, LECTURAS CARGADAS, CONSTANTES CARGADAS.");
}

float MUESTREO_DE_VALORES(int c){
    float v=0.000000;
    if (c == ANALOGO_BATERIA){
        //voltaje bateria
        v=TR_LINEAL_1(ANALOGO_BATERIA)/DIV_A3_7_VOLTAJE_BATERIA;
        Serial.print(" BATERIA: ");Serial.print(v);Serial.print(v);
    }
    if (c == ANALOGO_PANEL){
        //voltaje paneles
        v=TR_LINEAL_1(ANALOGO_PANEL)/DIV_A2_6_VOLTAJE_PANEL;
        Serial.print(" PANEL: ");Serial.print(v);Serial.print(v);
    }
    return v;
}

void MODO_DIGITAL(int ON){
    if (ON == 1){
        delay(2);
        digitalWrite(LEDRB,HIGH);
        delay(2);
        digitalWrite(RELE_BATERIA,HIGH);
        delay(2);
        digitalWrite(LEDAP,HIGH);
        delay(2);
        digitalWrite(RELE_PANEL,HIGH);
        delay(2);
        digitalWrite(LEDVC,HIGH);
        delay(2);
    }
    if (ON == 0){
        delay(2);
        digitalWrite(LEDRB,LOW);
        delay(2);
        digitalWrite(RELE_BATERIA,LOW);
        delay(2);
        digitalWrite(LEDAP,LOW);
        delay(2);
        digitalWrite(RELE_PANEL,LOW);
        delay(2);
        digitalWrite(LEDVC,LOW);
        delay(2);
    }
}

void COMPROBACION(){
    float x=0.000000;
    float y=0.000000;
    MODO_DIGITAL(1);
    Serial.print("RECOPILACION DE DATOS");
    for (int i=0; i<100;i++){
        x=x+MUESTREO_DE_VALORES(ANALOGO_BATERIA);
        y=y+MUESTREO_DE_VALORES(ANALOGO_PANEL);
        Serial.println();
    } 
    x=x/100.000000;
    y=y/100.000000;
    MUESTRAS[0]=x;
    MUESTRAS[1]=y;
    delay(100);
    MODO_DIGITAL(0);
}

void loop(){
    delay(1);
    COMPROBACION();
    Serial.println("COMPROBACION DE LECTURA");

}

void looping(){
    delay(1);
    COMPROBACION();
    CASE_STATUS(RESPUESTA_NODO(MUESTRAS[0],MUESTRAS[1],MUESTRAS[2]));
    delay(1);

}

bool ENCENDER_BOMBA(){
    BOMBA=true;
    return true;
}

bool APAGAR_BOMBA(){
    BOMBA=false;
    return true;
}

void CASE_STATUS(int c){
    if (c==6){
        ENCENDER_BOMBA();
    }
    if (c==5 && BOMBA){
        ENCENDER_BOMBA();
    }
    APAGAR_BOMBA();
    delay(1);
}
