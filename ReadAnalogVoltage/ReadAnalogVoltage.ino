/*
  ReadAnalogVoltage
  Reads an analog input on pin 0, converts it to voltage, and prints the result to the serial monitor.
  Graphical representation is available using serial plotter (Tools > Serial Plotter menu)
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.
*/

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  int sensorValue = analogRead(A2);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage = 0.004584882*(sensorValue-216.00000)+0.99000;
  // print out the value you read:
  Serial.print(sensorValue);
  Serial.print(" ");
  Serial.print(voltage);
  Serial.print(" ");
  Serial.print(voltage/0.04628331);
  Serial.print(" ");
  
  sensorValue = analogRead(A3);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  voltage = 0.004584882*(sensorValue-216.00000)+0.99000;
  // print out the value you read:
  Serial.print(sensorValue);
  Serial.print(" ");
  Serial.print(voltage);
  Serial.print(" ");
  Serial.println((voltage/(0.076086957-0.00394)));
  
}
