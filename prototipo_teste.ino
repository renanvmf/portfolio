#include "DHT.h"
#include <SoftwareSerial.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <SdFat.h>
#include "Wire.h"


#define DS1307_ADDRESS 0x68


const uint8_t SD_CS_PIN = 10;
SdFat SD;
SdFile file;
long datasensor = 0;


#define DHTPIN 5
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

SoftwareSerial mySerial(7, 8);


#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress sensor1;




void SubmitHttpRequest(float tempS, float tempA, float umid, String data)
{



  //adicionando dados à string:
  String enviando = "AT+HTTPPARA=\"URL\",\"apphomoext.cnpms.embrapa.br/geophotos/arquivoImages/arduino/arquivo.php?idEstacao=1&vlTempS=";
  enviando.concat(tempS);
  enviando += "&vlTempAr=";
  enviando.concat(tempA);
  enviando += "&vlUmAr=";
  enviando.concat(umid);
  enviando += "&data=";
  enviando.concat(data);
  enviando += "\"";

  //enviando
  mySerial.println("AT+CSQ");
  delay(100);
  mySerial.println("AT+CGATT?");
  delay(100);
  mySerial.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");//setting the SAPBR, the connection type is using gprs
  delay(1000);
  mySerial.println("AT+SAPBR=3,1,\"zap.vivo.com.br\",\"CMNET\"");//setting the APN, the second need you fill in your local apn server
  delay(4000);
  mySerial.println("AT+SAPBR=1,1");//setting the SAPBR, for detail you can refer to the AT command mamual
  delay(2000);
  mySerial.println("AT+HTTPINIT"); //init the HTTP request
  delay(2000);
  mySerial.println(enviando);// setting the httppara, the second parameter is the website you want to access
  delay(1000);
  mySerial.println("AT+HTTPACTION=0");//submit the request
  delay(10000);//the delay is very important, the delay time is base on the return from the website, if the return datas are very large, the time required longer.
  //while(!mySerial.available());
  mySerial.println("AT+HTTPREAD");// read the data from the website you access
  delay(300);
  mySerial.println("");
  delay(2000);


}



byte bcdToDec(byte val)  {
  // Convert binary coded decimal to normal decimal numbers
  return ( (val / 16 * 10) + (val % 16) );
}

void powerShield()
{
  pinMode(9, OUTPUT);
  digitalWrite(9, LOW);
  delay(1000);
  digitalWrite(9, HIGH);
  delay(2000);
  digitalWrite(9, LOW);
  delay(6000);
}

void setup() {
  Serial.begin(19200);
  

  Wire.begin();
  mySerial.begin(19200);               // the GPRS baud rate
  delay(500);
  //** Temperatura-Solo Sensor
  sensors.begin();


  //** Sensor umidade e temperatura

  dht.begin();

  //SD card
  SD.begin(SD_CS_PIN);


  powerShield();
}



void loop() {

  long diaAno = 1;

  //RTC date

  // Reset the register pointer
  Wire.beginTransmission(DS1307_ADDRESS);

  byte zero = 0x00;
  Wire.write(zero);
  Wire.endTransmission();

  Wire.requestFrom(DS1307_ADDRESS, 7);

  int second = bcdToDec(Wire.read());
  int minute = bcdToDec(Wire.read());
  int hour = bcdToDec(Wire.read() & 0b111111); //24 hour time
  int weekDay = bcdToDec(Wire.read()); //0-6 -> sunday - Saturday
  int monthDay = bcdToDec(Wire.read());
  int month = bcdToDec(Wire.read());
  int year = bcdToDec(Wire.read());

  String data;
  data.concat(year);
  data.concat(month);
  data.concat(monthDay);
  data.concat(hour);
  data.concat(minute);
  data.concat(second);



  // Lendo informações sensor de temperatura do solo
  sensors.requestTemperatures();
  float tempS = sensors.getTempC(sensor1);

  // Lendo informações sensor de umidade e temperatura do ar
  float umid = dht.readHumidity();
  float tempA = dht.readTemperature();

  //gravando no cartao sd

  if (datasensor != diaAno)
  {
    file.open("logdata2807.TXT", O_RDWR | O_CREAT | O_AT_END);
    file.println("Umidade ; Temperatura do ar ; Temperatura do solo");
    file.print(umid);
    file.print(";");
    file.print(tempA);
    file.print(";");
    file.print(tempS);
    file.print(";");
    file.println(data);
    datasensor = diaAno;
  }
  else {
    file.open("logdata2807.TXT", O_RDWR | O_CREAT | O_AT_END);
    file.print(umid);
    file.print(";");
    file.print(tempA);
    file.print(";");
    file.print(tempS);
    file.print(";");
    file.println(data);

    file.close();
  }
  //print no serial monitor

  Serial.println("Temperatura solo:\t Temperatura ar:\t Umid ar:\t data e horario:");
  Serial.print(tempS);
  Serial.print("\t");
  Serial.print(tempA);
  Serial.print("\t");
  Serial.print(umid);
  Serial.print("\t");
  Serial.print(data);
  Serial.println();


  //enviando para php.

 // SubmitHttpRequest(tempS, tempA, umid, data);
  

  delay(5000);

}




