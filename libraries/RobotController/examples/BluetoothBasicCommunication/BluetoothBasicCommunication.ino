#include <RobotController.h>

void setup()
{
  Serial.begin(9600);
  RobotBegin();
  setBTName("BT");  // Configurem el nom del Bluetooth
  setBTPassword("1234");  // Configurem la contrasenya del Bluetooth, és necessari que sigui de 4 numeros.
}

void loop()
{
  while (SerialBT.available()) {  // Rebem dades del robot amb Bluetooth
    Serial.write(SerialBT.read());  // Mostrem les dades al monitor sèrie.
  }
  
  while (Serial.available()) {      // Comprovem si hi han dades per enviar al Bluetooth.
    SerialBT.write(Serial.read());  // Enviem dades al Robot amb Bluetooth
  }
}
