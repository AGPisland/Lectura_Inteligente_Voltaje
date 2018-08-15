#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

float Voltaje_Baterias=0.0;
float Voltaje_Solar=0.0;
float Corriente_Carga=0.0;
float Energia_Solar=0.0;

int Rele_Bateria=7; int Led_Bateria_Rojo=5; const int Lectura_Baterias=A3; bool Nivel_Bateria=false;
int Rele_Solar=6; int Led_Solar_Naranjo=4; const int Lectura_Solar=A2; bool Nivel_Solar=false;
const int Sensor_Corriente=A6; int Led_Corriente_Verde=3; bool Nivel_Corriente=false;
string Estados[]={"BULK OPTIMO","BULK BAJO", "ABSORPTION OPTIMO", "ABSORPTION BAJO", "MAINTENANCE OPTIMO", "MAINTENANCE BAJO",}
float Nivel_Esperado = 0.7;
int Tiempo_Espera=3000;
int Tiempo_Setup=1000;
int C=0;


void setup(){
    DISPLAI("CONTROL BOMBA V2", "CARGANDO........",1,0);
    Serial.begin(9600);
    Serial.println("Cargando sistema. Comprobando extensiones::");
    Serial.println("Cargando Reles::");
    pinMode(Rele_Bateria, OUTPUT);
    pinMode(Rele_Solar, OUTPUT);
    digitalWrite(Rele_Bateria, LOW);
    digitalWrite(Rele_Solar, LOW);
    Serial.println("Reles: Carga OK.");
    Serial.println("Comprobando Reles::");
    delay(Tiempo_Setup);
    digitalWrite(Rele_Bateria, HIGH);
    digitalWrite(Rele_Solar, HIGH);
    delay(Tiempo_Setup);
    digitalWrite(Rele_Bateria, LOW);
    digitalWrite(Rele_Solar, LOW);
    Serial.println("Reles: Comprobados OK.");
    Serial.println("Cargando LEDs.");
    pinMode(Led_Bateria_Rojo, OUTPUT);
    pinMode(Led_Solar_Naranjo, OUTPUT);
    pinMode(Led_Corriente_Verde, OUTPUT);
    digitalWrite(Led_Bateria_Rojo, LOW);
    digitalWrite(Led_Solar_Naranjo, LOW);
    digitalWrite(Led_Corriente_Verde, LOW);
    Serial.println("LEDs: Carga OK.");
    Serial.println("Comprobando LEDs::");
    delay(Tiempo_Setup);
    digitalWrite(Led_Bateria_Rojo, HIGH);
    digitalWrite(Led_Solar_Naranjo, HIGH);
    digitalWrite(Led_Corriente_Verde, HIGH);
    delay(Tiempo_Setup);
    digitalWrite(Led_Bateria_Rojo, LOW);
    digitalWrite(Led_Solar_Naranjo, LOW);
    digitalWrite(Led_Corriente_Verde, LOW);
    Serial.println("LEDs: Comprobados OK.");
    Serial.println("Cargando modulos de lectura.");
    pinMode(Lectura_Baterias, INPUT);
    pinMode(Lectura_Solar, INPUT);
    pinMode(Sensor_Corriente, INPUT);
    Serial.println("Lecturas: Comprobados OK.");
    Serial.println("Fin Setup.");
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println();
    DISPLAI("CONTROL BOMBA V2", "CARGANDO.OK.OK..",1,0);
    delay(Tiempo_Setup);
    
}
void loop(){
    DISPLAI("LECTURA DE EL","VOLTAJE BATERIA",1,0);
    Serial.println("LECTURA DE VOLTAJE BATERIAS: ");
    Voltaje_Baterias=LECTURA_VOLTAJES(Rele_Bateria,Lectura_Baterias,50,0.004887586,0.0909,50);
    DISPLAI("VOLTAJE BATERIA","V",5,Voltaje_Baterias);
    delay(Tiempo_Espera);
    DISPLAI("LECTURA DE EL","VOLTAJE PANEL",1,0);
    Serial.println("LECTURA DE VOLTAJE PANEL SOLAR: ");
    Voltaje_Solar=LECTURA_VOLTAJES(Rele_Solar,Lectura_Solar,50,0.004887586,0.0463,50);
    DISPLAI("VOLTAJE PANEL","V",5,Voltaje_Solar);
    delay(Tiempo_Espera);
    DISPLAI("LECTURA DE ", "CORRIENTE", 1,0);
    Serial.println("LECTURA DE LA CORRIENTE");
    Corriente_Carga=LECTURA_CORRIENTE(Sensor_Corriente,50,50);
    DISPLAI("CORRIENTE CARGA", "Am", 5, Corriente_Carga);
    delay(Tiempo_Espera);
    C=+1;


}
float LECTURA_VOLTAJES(int Rele, int AN, int i, float M, float K, int FREQUENCY){
    Serial.print("LECTURA DE VOLAJE DEL RELE: "); Serial.println(Rele);
    float List_V[i+1];
    digitalWrite(Rele, HIGH);
    for (int j = 0; j<i; j++){
        delay(FREQUENCY);
        List_V[j]=analogRead(AN);
        delay(FREQUENCY);
    }
    digitalWrite(Rele, LOW);
    for (int j=0; j<i; j++){
        Serial.print("Analog: "); Serial.print(List_V[j]);
        List_V[j]= M * (List_V[j] - 1023.0) + 5.0;
        Serial.print(" Voltj with Error: "); Serial.print(List_V[j]); Serial.print(" V");
        List_V[j]=List_V[j]/K;
        Serial.print(" Voltj Real: "); Serial.print(List_V[j]); Serial.println(" V");
        List_V[i+1]=List_V[j]+List_V[i+1];
    }
    Serial.println();
    Serial.println();
    Serial.print("VALOR PROMEDIO: "); Serial.print(List_V[i+1]/i); Serial.println(" V");
    Serial.println();
    Serial.println();
    return List_V[i+1]/i;
}
float LECTURA_CORRIENTE(int AN, int FREQUENCY, int i){
    Serial.println("LECTURA DE CORRIENTE ");
    float M=-30/-511.5;
    float VECTOR[i+1];
    for (int j=0; j<i;j++){
        delay(FREQUENCY);
        VECTOR[j]=analogRead(AN);
        delay(FREQUENCY);
    }
    for (int j=0; j<i;j++){
        Serial.print("Analog: "); Serial.print(VECTOR[j]);
        VECTOR[j]=M*(VECTOR[j]-511.5);
        VECTOR[j]=(VECTOR[j]<0)? VECTOR[j]*-1:VECTOR[j];
        Serial.print(" Corriente: "); Serial.print(VECTOR[j]); Serial.println(" AMP");
        VECTOR[i+1]=VECTOR[i+1]+VECTOR[j];
    }
    Serial.println();
    Serial.println();
    Serial.print("VALOR PROMEDIO: "); Serial.print(VECTOR[i+1]/i); Serial.println(" AMP");
    Serial.println();
    Serial.println();
    return VECTOR[i+1]/i;
}
void DISPLAI(char UP[], char DOWN[], int T, int N){
    switch(T){
        case 1:
            //
            lcd.begin(16,2);
            lcd.print(UP);
            lcd.setCursor(0,1);
            lcd.print(DOWN);
            break;
        case 2:
            lcd.begin(16,2);
            lcd.print(UP);
            break;
        case 3:
            lcd.setCursor(1,0);
            lcd.print(DOWN);
            break;
        case 4:{
            String Palabra;
            Palabra.concat(String(N));
            int l;
            l=Palabra.length()+1;
            char Char_C[l];
            Palabra.toCharArray(Char_C, l);
            DISPLAI(UP, Char_C, 1,0);
            break;}
        case 5:{
            String Palabra;
            Palabra.concat("RESULT: ");
            Palabra.concat(String(N));
            Palabra.concat(DOWN);
            int l=Palabra.length()+1;
            char Char_C[l];
            Palabra.toCharArray(Char_C, l);
            DISPLAI(UP, Char_C, 1,0);
            break;}
        return;
    }
}