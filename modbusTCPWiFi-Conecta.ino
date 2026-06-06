//IDE Arduino Code (working perfectly)
/*
More Tutorials:
Website http://trialcommand.com
In English: http://en.trialcommand.com
En Español: http://en.trialcommand.com
*/
#include <WiFi.h>
#include <DHT.h>

const char* ssid = "Castelinho";
const char* password = "MilkaPrincesa";
int ModbusTCP_port = 502;

//////// Required for Modbus TCP / IP /// Requerido para Modbus TCP/IP /////////
#define maxInputRegister 20
#define maxHoldingRegister 20

#define MB_FC_NONE 0
#define MB_FC_READ_REGISTERS 3 //implemented
#define MB_FC_WRITE_REGISTER 6 //implemented
#define MB_FC_WRITE_MULTIPLE_REGISTERS 16 //implemented
//
// MODBUS Error Codes
//
#define MB_EC_NONE 0
#define MB_EC_ILLEGAL_FUNCTION 1
#define MB_EC_ILLEGAL_DATA_ADDRESS 2
#define MB_EC_ILLEGAL_DATA_VALUE 3
#define MB_EC_SLAVE_DEVICE_FAILURE 4
//
// MODBUS MBAP offsets
//
#define MB_TCP_TID 0
#define MB_TCP_PID 2
#define MB_TCP_LEN 4
#define MB_TCP_UID 6
#define MB_TCP_FUNC 7
#define MB_TCP_REGISTER_START 8
#define MB_TCP_REGISTER_NUMBER 10

byte ByteArray[260];
unsigned int MBHoldingRegister[maxHoldingRegister];
//
//Definição variáveis do código
//
#define LED_Int 2
//#define LED_VM 36
//#define LED_VD  36
#define LDR_pin 34
#define DHT_pin 21
#define UMID_pin 39
#define VALV_pin 31

#define Analog_Threshold 500

DHT dht11(DHT_pin, DHT11);

//String dadoRecebido = "";

//////////////////////////////////////////////////////////////////////////

WiFiServer MBServer(ModbusTCP_port);
//Serial.println("travou?1");

void setup() {
  Serial.begin(9600);
  analogSetAttenuation(ADC_11db);

  dht11.begin();
Serial.println("travou?2");
  delay(100) ;
  WiFi.begin(ssid, password);
  delay(100) ;
Serial.println("travou?3");
//Adicionar pinos que vão ser usados aqui
  pinMode(LED_Int, OUTPUT);
  pinMode(VALV_pin, OUTPUT);


  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("travou?4");
    delay(500);
    Serial.print(".");
  }
Serial.println("travou?5");
  MBServer.begin();
  Serial.println("travou?6");
  Serial.println("Connected ");
  Serial.print("ESP8266 Slave Modbus TCP/IP ");
  Serial.print(WiFi.localIP());
  Serial.print(":");
  Serial.println(String(ModbusTCP_port));
  Serial.println("Modbus TCP/IP Online");

  Serial.println("travou?7");
}


void loop() {
// Check if a client has connected // Modbus TCP/IP
Serial.println("travou?8");
WiFiClient client = MBServer.available();

if (!client) {
      Serial.println("sem cliente");
      return;
      }
Serial.println("travou?10");
bool flagClientConnected = 0;
uint8_t byteFN = MB_FC_NONE;
int Start;
int WordDataLength;
int ByteDataLength;
int MessageLength;
int tentativa = 0;

// Modbus TCP/IP
while (client.connected()) {
Serial.println("travou?11");
//      Serial.println("depois de teste conecta");

      if(client.available()){
            flagClientConnected = 1;
            int i = 0;
Serial.println("travou?12");
            while(client.available()){
              Serial.println("travou?13");
                  //MBHR para manter a comunicação ativa
                  MBHoldingRegister[9] = random(0,12);
                  ByteArray[i] = client.read();
                  i++;
                  }
Serial.println("travou?13");
            client.flush();

///// code here --- codigo aqui

float LDR_Value = analogRead(LDR_pin);
LDR_Value = LDR_Value*100/4095;
Serial.print("Valor LDR: ");
Serial.println(LDR_Value);

float Umid_ar = dht11.readHumidity();
Serial.print("Valor umidade ar: ");
Serial.println(Umid_ar);

float Temp = dht11.readTemperature();
Serial.print("Valor temperatura: ");
Serial.println(Temp);

float Umid_terra = analogRead(UMID_pin); // read the analog value from sensor
Umid_terra = Umid_terra*100/1023;
Serial.print("Valor umidade terra: ");
Serial.println(Umid_terra);

///////// Holding Register [0] A [2] = 10 Holding Registers Escritura
///////// Holding Register [0] A [2] = 10 Holding Registers Writing

MBHoldingRegister[0] = random(0,12);
MBHoldingRegister[1] = LDR_Value;
MBHoldingRegister[2] = Umid_ar;
MBHoldingRegister[3] = Temp;
MBHoldingRegister[4] = Umid_terra;

/*while (Serial.available() > 0) {
  char caractere = Serial.read(); 
  // O caractere '\n' indica o fim do envio daquele número
  if (caractere == '\n') { 
    int valor = dadoRecebido.substring(0, dadoRecebido.length()-2).toInt();

    int caracterfinal = dadoRecebido.substring(dadoRecebido.length()-3, dadoRecebido.length()-2).toInt();
    switch(caracterfinal){
      case '0':
        MBHoldingRegister[5] = valor;
        //numDado = numDado + 1;
        break;
      case '1':
        MBHoldingRegister[6] = valor;
        //numDado = numDado + 1;
        break;
      case '2':
        MBHoldingRegister[7] = valor;
        //numDado = 0;
        break;
      default:
        break;
    }

    dadoRecebido = ""; // Limpa a variável para o próximo número
  } else {
    dadoRecebido += caractere; // Concatena os caracteres
  }
}*/
//MBHoldingRegister[2] = random(0,12);

///////// Holding Register [10] A [14] = 10 Holding Registers Lectura
///// Holding Register [10] A [14] = 10 Holding Registers Reading

int Temporal[5];

Temporal[0] = MBHoldingRegister[0];
Temporal[1] = MBHoldingRegister[1];
Temporal[2] = MBHoldingRegister[14];
//Temporal[3] = MBHoldingRegister[13];
//Temporal[4] = MBHoldingRegister[14];

/// Enable Output 14
  digitalWrite(LED_Int, MBHoldingRegister[14]);
  digitalWrite(VALV_pin, MBHoldingRegister[10]);

     // Serial.println("enable output");
//// debug
for (int i = 0; i < 5; i++) {
Serial.print("[");
Serial.print(i);
Serial.print("] =");
Serial.print(Temporal[i]);
}

Serial.println("");


//// end code - fin
Serial.println("travou?15");
//// rutine Modbus TCP
byteFN = ByteArray[MB_TCP_FUNC];
Start = word(ByteArray[MB_TCP_REGISTER_START],ByteArray[MB_TCP_REGISTER_START+1]);
WordDataLength = word(ByteArray[MB_TCP_REGISTER_NUMBER],ByteArray[MB_TCP_REGISTER_NUMBER+1]);
}
else if(tentativa == 3400){
  client.stop();
}
else{
  tentativa++;
}

// Handle request

switch(byteFN) {
case MB_FC_NONE:
break;
Serial.println("travou?16");
case MB_FC_READ_REGISTERS: // 03 Read Holding Registers
ByteDataLength = WordDataLength * 2;
ByteArray[5] = ByteDataLength + 3; //Number of bytes after this one.
ByteArray[8] = ByteDataLength; //Number of bytes after this one (or number of bytes of data).
for(int i = 0; i < WordDataLength; i++)
{
ByteArray[ 9 + i * 2] = highByte(MBHoldingRegister[Start + i]);
ByteArray[10 + i * 2] = lowByte(MBHoldingRegister[Start + i]);
}
Serial.println("travou?17");
MessageLength = ByteDataLength + 9;
if(client.connected()){
  client.write((const uint8_t *)ByteArray,MessageLength);

  byteFN = MB_FC_NONE;
}
break;
Serial.println("travou?18");
case MB_FC_WRITE_REGISTER: // 06 Write Holding Register
MBHoldingRegister[Start] = word(ByteArray[MB_TCP_REGISTER_NUMBER],ByteArray[MB_TCP_REGISTER_NUMBER+1]);
ByteArray[5] = 6; //Number of bytes after this one.
MessageLength = 12;
if(client.connected()){
  client.write((const uint8_t *)ByteArray,MessageLength);
  byteFN = MB_FC_NONE;
}
break;
case MB_FC_WRITE_MULTIPLE_REGISTERS: //16 Write Holding Registers
ByteDataLength = WordDataLength * 2;
ByteArray[5] = ByteDataLength + 3; //Number of bytes after this one.
for(int i = 0; i < WordDataLength; i++)
{
MBHoldingRegister[Start + i] = word(ByteArray[ 13 + i * 2],ByteArray[14 + i * 2]);
}
MessageLength = 12;
if(client.connected()){
  client.write((const uint8_t *)ByteArray,MessageLength);
  byteFN = MB_FC_NONE;
}
Serial.println("travou?20");
  break;
}
}
}
