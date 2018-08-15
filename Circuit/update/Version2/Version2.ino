#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

float V_Baterias = 0.0;
float V_solar = 0.0;
float C_solar = 0.0;
float max_volt_baterias = 48;
float max_volt_solar = 95;

bool Led_VB = false;
bool Led_VS = false;
bool Led_CS = false;
bool Led_BB = false;
bool k = false;

int RL_Bomba = 8;
int RL_VB = 7;
int RL_VS = 6;


int LD_VB = 5;
int LD_VS = 4;
int LD_CS = 3;
int LD_BB = 2;

int tiempo_espera=3000;
float nivel_esperado=0.7;


const int Sensor_C = A6;
const int DV_VS = A2;
const int DV_VB = A3;
int contador = 0;

void setup()
{
  pinMode(Sensor_C, INPUT);
  pinMode(DV_VS, INPUT);
  pinMode(DV_VB, INPUT);
  pinMode(INT, INPUT);

  pinMode(RL_Bomba, OUTPUT);
  pinMode(RL_VB, OUTPUT);
  pinMode(RL_VS, OUTPUT);

  pinMode(LD_BB, OUTPUT);
  pinMode(LD_CS, OUTPUT);
  pinMode(LD_VB, OUTPUT);
  pinMode(LD_VS, OUTPUT);

  digitalWrite(RL_Bomba, LOW);
  digitalWrite(RL_VB, LOW);
  digitalWrite(RL_VS, LOW);

  digitalWrite(LD_BB, HIGH);
  digitalWrite(LD_CS, HIGH);
  digitalWrite(LD_VB, HIGH);
  digitalWrite(LD_VS, HIGH);

  Serial.begin(9600);
  delay(2000);

  digitalWrite(LD_BB, LOW);
  digitalWrite(LD_CS, LOW);
  digitalWrite(LD_VB, LOW);
  digitalWrite(LD_VS, LOW);
}
void loop()
{
  String con;
  con.concat("C: ");
  con.concat(String(contador));
  int str_con = con.length() + 1;
  char char_con[str_con];
  con.toCharArray(char_con, str_con);

  DisplayLCD("SCANER BATERIAS", char_con, 0);
  Serial.println("-------------------------");
  Serial.print("-MODO LOOP; ");
  Serial.println(contador);
  V_Baterias = Read_V2(RL_VB, DV_VB, 30, 0.004887586, 0.0909);
  Serial.println("-VOLT BATERIAS:"); Serial.print(V_Baterias); Serial.println(" V");
  if (V_Baterias > nivel_esperado * max_volt_baterias)
  {
    //BATERIAS EN BUEN ESTADO, PUES ES MAYOR AL 70 PORCIENTO DE CARGA
    Led_VB = false;
    digitalWrite(LD_VB, LOW);
  }
  else
  {
    // SI NO, SE PROCEDE A APAGAR LA BOMBA SI ESQUE SE ENCUENTRA ENCEDIDA POR EL CICLO ANTERIOR
    Led_VB = true;
    digitalWrite(LD_VB, HIGH);
    Led_BB = false;
    digitalWrite(RL_Bomba, LOW);
    digitalWrite(LD_BB, LOW);
  }
  String display;
  display.concat(String(V_Baterias));
  display.concat(" V ");
  str_con = display.length() + 1;
  char char_array[str_con];
  display.toCharArray(char_array, str_con);
  DisplayLCD("RESUL. BATERIAS", char_array, 0);
  display = "";
  str_con=0;
  delay(tiempo_espera);


  DisplayLCD("SCANER PANELES", char_con, 0);
  V_solar = Read_V2(RL_VS, DV_VS, 30, 0.004887586, 0.0463);
  Serial.println("-VOLT DE PANELES: "); Serial.print(V_solar); Serial.println(" V");
  if (V_solar > nivel_esperado * max_volt_solar)
  {
    Led_VS = false;
    digitalWrite(LD_VS, LOW);
  }
  else
  {
    Led_VS = true;
    digitalWrite(LD_VS, HIGH);
  }

  display.concat(String(V_solar));
  display.concat(" V ");
  str_con = display.length() + 1;
  char_array[str_con];
  display.toCharArray(char_array, str_con);
  DisplayLCD("RESULT. PANELES", char_array, 0);
  display = "";
  str_con=0;
  delay(tiempo_espera);

  DisplayLCD("SCANER CORRIENTE", char_con, 0);
  C_solar = Read_C(Sensor_C, 10, 30);
  Serial.println("-CORRIENTE: "); Serial.print(C_solar); Serial.println(" A");
  if (C_solar > 14.0)
  // MUCHA CORRIENTE DE CARGA SOLAR HACIA EL CARGADOR DE BATERIAS.
  {
    Led_CS = true;
    digitalWrite(LD_CS, HIGH);
  }
  else
  {
    Led_CS = false;
    digitalWrite(LD_CS, LOW);
  }

  display.concat(String(C_solar));
  display.concat(" A ");
  str_con = display.length() + 1;
  char_array[str_con];
  display.toCharArray(char_array, str_con);
  DisplayLCD("RESULT CORRIENTE", char_array, 0);
  display = "";
  str_con=0;
  delay(tiempo_espera);

  display.concat(String(V_Baterias));
  display.concat(" V ");
  display.concat(String(C_solar * V_solar));
  display.concat("W");
  str_con = display.length() + 1;
  char_array[str_con];
  display.toCharArray(char_array, str_con);
  DisplayLCD("RESULTADO LECT", char_array, 0);
  str_con=0;
  display="";
  delay(tiempo_espera);
  Serial.println("CONCLUSIONES:");
  // led_vd: falso cuando las condiciones son buena bateria, true baterias descargadas
  // led_vs: falso cuando esta soleado, verdadero cuando esta nublado
  // led_cs: falso cuando la corriente es menor que 5 A, quiere decir que no se esta cargando la bateria, true si la corriente esta entre
  // 5 y 17 A ya que el cargador si esta cargando la bateria.-
  if (!Led_VB & !Led_VS)
  {
    Encender_bomba(C_solar, RL_Bomba, LD_BB);
  }
  else
  {
    Apagar_bomba(Led_BB, LD_BB, RL_Bomba);
    DisplayLCD("BOMBA APAGADA", "", 0);
    delay(1000);
    if (Led_VB)
    {
      //LA BATERIA SE ENCUENTRA DESCARGADA POR TANTO EL LED ROJO DEBE ESTAR ENCENDIDO.
      Serial.println("NO ENCENDER BOMBA");
      //.....................................
      Serial.println("BATERIAS DESCARGADAS");
      DisplayLCD("BAT DESCARGADAS", "", 0 );
      Serial.print("VOLTAJE BATERIAS= "); Serial.print(35 - V_Baterias); Serial.println(" DE VOLT PARA EL OPTIMO");
      //Serial.println(Estados_Registro(3));
      delay(1000);
      if (Led_VS)
      {
        //Esta nublado
        Serial.println("NIVEL SOLAR BAJO"); DisplayLCD("", "NIVEL SOLAR BAJO", -2 );
        Serial.print("SOLAR: "); Serial.print(57 - V_solar); Serial.println(" UNIDADES DE VOLT PARA EL OPTIMO");
        delay(2000);
        if (Led_CS)
        {
          //LED ROJO ON, LED AMARILLO ON, LED VERDE ON
          //posible presencia del generador
          Serial.println("CARGANDO BATERIAS"); DisplayLCD("NIVEL SOLAR BAJO", "CARGANDO BATERIA", 0);
          int j = 0;
          digitalWrite(LD_VB, LOW);
          digitalWrite(LD_VS, LOW);
          digitalWrite(LD_CS, LOW);
          while (true)
          {
            j += 1;
            digitalWrite(LD_VB, HIGH);
            digitalWrite(LD_VS, LOW);
            digitalWrite(LD_CS, LOW);
            delay(500);
            if (j == 30)
            {
              digitalWrite(LD_VB, LOW);
              digitalWrite(LD_VS, LOW);
              digitalWrite(LD_CS, LOW);
              break;
            }
            digitalWrite(LD_VB, LOW);
            digitalWrite(LD_VS, HIGH);
            digitalWrite(LD_CS, HIGH);
            delay(500);
          }
        }
        else
        {
          //LED ROJO ON, LED AMARILLO ON, LED VERDE OFF
          //no hay presencia de generador
          Serial.println("NIVELES BAJO DE POTENCIA SOLAR"); DisplayLCD("NIVEL SOLAR BAJO", "CARGANDO BATERIA", 0 );
          //Serial.println(Estados_Registro(6));
          //DisplayLCD("ESTADO: ", "", 6);
          delay(30000);
        }
      }
      else
      {
        Serial.println("NIVEL SOLAR ALTO"); 
        DisplayLCD("", "NIVEL SOLAR ALTO", -2 );
        delay(1000);
        if (Led_CS)
        {
          //LED ROJO ON, LED AMARILLO OFF, LED VERDE ON
          //El sistema carga las baterias con max corriente y potencia solar buena
          Serial.println("CARGANDO BATERIAS");
          DisplayLCD("CARGANDO BATERIA", "NIVEL SOLAR ALTO", 0);
          //Serial.println(Estados_Registro(7));
          //DisplayLCD("ESTADO: ", "", 7);
          int j = 0;
          digitalWrite(LD_VB, LOW);
          digitalWrite(LD_VS, LOW);
          digitalWrite(LD_CS, LOW);
          while (true)
          {
            j += 1;
            digitalWrite(LD_VB, LOW);
            digitalWrite(LD_VS, HIGH);
            digitalWrite(LD_CS, HIGH);
            delay(500);
            if (j == 30)
            {
              digitalWrite(LD_VB, LOW);
              digitalWrite(LD_VS, LOW);
              digitalWrite(LD_CS, LOW);
              break;
            }
            digitalWrite(LD_VB, HIGH);
            digitalWrite(LD_VS, LOW);
            digitalWrite(LD_CS, LOW);
            delay(500);
          }
        }
        else
        {
          //LED ROJO ON, LED AMARILLO OFF, LED VERDE OFF
          //El sistema carga las baterias pero con poca potencia tarde o temprano terminara
          Serial.println("NIVEL BAJO EN LA CARGA DE BATERIAS");
          DisplayLCD("BAJO NIVEL DE", "CARGA EN BATERIA", 0);
          delay(30000);
          //Serial.println(Estados_Registro(8));
          //DisplayLCD("ESTADO: ", "", 8);

        }
      }
    }
    else
    {
      Serial.println("BATERIAS CARGADAS");
      DisplayLCD("BATER. CARGADAS", "" , 0 );
      delay(1000);
      if (Led_VS)
      {
        // led rojo off, led verde on
        Serial.println("NIVEL SOLAR BAJO");
        DisplayLCD("", "NIVEL SOLAR BAJO", -2 );
        delay(1000);
      }
    }
  }

  Serial.println("FIN");
  Led_VB = false;
  Led_VS = false;
  Led_CS = false;
  digitalWrite(RL_VB, LOW);
  digitalWrite(RL_VS, LOW);
  contador += 1;
}
float Read_V2(int RL, int AL, int i, float pendiente, float constante)
{
  Serial.println("LEYENDO VOLTAJE");
  int bits = 0;
  float volta_ar = 0.0;
  float volta_re = 0.0;
  float sum = 0.0;
  digitalWrite(RL, HIGH);
  for (int j = 0; j < i; j++)
  {
    delay(50);
    bits = analogRead(AL);
    delay(50);
    volta_ar = pendiente * (bits - 1023.0) + 5.0;
    delay(50);
    Serial.print(bits);
    Serial.print(" ");
    bits = 0;
    volta_re = volta_ar / constante;
    Serial.print(volta_re);
    Serial.println(" V");
    delay(50);
    sum = sum + volta_re;
    delay(50);
  }
  digitalWrite(RL, LOW);
  return sum / i;
}
float Read_C(int AL, int F, int i)
{
  Serial.println("LECTURA CORRIENTE");
  float pendiente = -30 / -511.5; //0.058651
  float Corriente = 0.0;
  float aux3 = 0.0;
  float sum = 0.0;
  int bits = 0;
  for (int j = 0; j < i; j++)
  {
    delay(50);
    bits = analogRead(AL);
    Serial.print(bits);
    Serial.print(" ");
    delay(50);
    Corriente = pendiente * (bits - 511.5);
    if (Corriente < 0) {
      Corriente = Corriente * -1;
    }
    Serial.print(Corriente);
    Serial.println(" A");
    delay(50);
    sum = sum + Corriente;
    delay(50);
    bits = 0;
    Corriente = 0;
  }
  return sum / i;
}
void Encender_bomba(float C_solar, int RL_Bomba, int LD_BB)
{
  DisplayLCD("ENCENDIENDO", "BOMBA...", 0 );
  Serial.println("ENCENDIENDO BOMBA");
  float i = C_solar;
  int x = 0;
  int timex = 0;
  digitalWrite(RL_Bomba, HIGH);
  if (i > 17)
  {
    timex = 60000;
  }
  else
  {
    if (i > 15)
    {
      timex = 120000;
    }
    else
    {
      if (i > 10)
      {
        timex = 180000;
      }
      else {
        timex = 360000;
      }
    }
  }

  Serial.print("tiempo de encendido: "); Serial.println(timex / 1000);
  String display;
  display.concat(String(timex / 1000));
  display.concat(" segundos");
  int str_len = display.length() + 1;
  char char_array[str_len];
  display.toCharArray(char_array, str_len);
  DisplayLCD("BOMBA ENCENDIDA", char_array, 0 );
  Led_BB = true;
  while (Led_BB)
  {
    digitalWrite(LD_BB, HIGH);
    delay(500);
    digitalWrite(LD_BB, LOW);
    delay(500);
    if (x == timex)
    {
      break;
    }
    x = x + 1000;
  }
  digitalWrite(LD_BB, LOW);
}
void Apagar_bomba(int Led_BB, int LD_BB, int RL_Bomba)
{
  Serial.println("SE PROCEDE A APAGAR LA BOMBA");
  DisplayLCD("APAGANDO BOMBA", "", -1 );
  delay(3000);
  Led_BB = false;
  digitalWrite(LD_BB, LOW);
  digitalWrite(RL_Bomba, LOW);
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
