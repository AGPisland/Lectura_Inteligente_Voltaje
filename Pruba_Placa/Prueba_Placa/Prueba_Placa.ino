/*
8
7
6

a3 6  constante: 
a2 7  constante: 

8 bomba

*/

const int An=A3;

const int Bn=A2;
const int Ad=6;

const int Bd=7;
const int Bomb=8;

void setup(){
    pinMode(An,INPUT);
    pinMode(Bn,INPUT);
    pinMode(Ad, OUTPUT);
    pinMode(Bd, OUTPUT);
    pinMode(Bomb, OUTPUT);

    digitalWrite(Ad,HIGH);
    digitalWrite(Bd,HIGH);
    digitalWrite(Bomb,HIGH);
    Serial.begin(9600);
    Serial.println("Prueba de conexiones secuencia HIGH comienza se imprime contador: ");
    delay(1000);
    digitalWrite(Ad,LOW);
    digitalWrite(Bd,LOW);
    digitalWrite(Bomb,LOW);
    delay(1000);
    
    Serial.println("HIGH: ");
    digitalWrite(Ad,HIGH);
    digitalWrite(Bd,HIGH);
    digitalWrite(Bomb,HIGH);
    
}
int contador=0;
float val1=0.0;
float val2=0.0;


void loop(){
    Prueba();
}
void firstLoop(){
    contador=contador+1;
    Serial.print("LECTURA NUMERO:  ");Serial.println(contador);
    val1=Lect_Activada(An,0.9537166900420760,Ad);
    delay(300);
    val2=Lect_Activada(Bn,0.0909090909090909,Bd);
    Serial.print("A: ");
    Serial.println(val1);
    Serial.print("B: ");
    Serial.println(val2);
    delay(300);
}
void Prueba(){
    float x=analogRead(A3);
    float y=analogRead(A2);
    Serial.print(x);Serial.print(" ");
    float val1=((0.00488759*x-0.52)/0.04628331)+1.96;
    float aux1=0.004887586*x-0.43;
    float aux2=0.004887586*y-0.20;
    float val2=((0.00488759*y-0.20)/0.09090909);
    Serial.print(val1);
    Serial.print(" ");
    Serial.print(aux1);
    Serial.print(" ");
    Serial.print(y);
    Serial.print(" ");
    Serial.print(val2);
    Serial.print(" ");
    Serial.print(aux2);
    Serial.println();
    x=0.0;
    //y=0.0;
    val1=0.0;
    //val2=0.0;
}
void PruebaII(){
  float x=analogRead(An);
  Serial.print(x);Serial.print("   : ");
  float Val=0.0;
  Val=(0.16591988*(x-448.00)+41.50);
  Serial.println(Val);
  
}
void PruebaIII(){
  float y=analogRead(Bn);
  Serial.print(y);Serial.println("   : ");
  float Val=0.0;
  
}
float Lect_Activada(int AL, float k, int act){
    delay(10);
    float Sumador=0.0;
    int Cr=analogRead(AL)-1;
    //Serial.print("LECTURA ACTIVADA DE LA ENTRADA: ");Serial.println(AL);
    digitalWrite(act,HIGH);
    for(int i =0;i<20;i++){
        delay(100);
        Sumador=Sumador+LecturaVoltajeII(AL,k,Cr);
        delay(100);
    }
    digitalWrite(act,LOW);
    return Sumador/50;
}
float LecturaVoltajeII(int AL, float K, int Cr){
    return ((0.004887585532746820*(analogRead(AL)-Cr))/K);
}

/*
Serial.println("LOW: ");
    digitalWrite(Ad,LOW);
    digitalWrite(Bd,LOW);
    digitalWrite(Bomb,LOW);
    delay(5000);
*/
