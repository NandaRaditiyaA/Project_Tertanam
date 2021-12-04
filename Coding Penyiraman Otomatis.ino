//Penyiram tanaman otomatis

const int pinDigital = A0; //inisialisasi pin sensor
const int relay = 6;       //inisialisasi pin relay 
const int ledGreen = 7;    //inisialisasi pin led hijau
const int ledYellow = 8;   //inisialisasi pin led kuning
const int ledRed = 9;      //inisialisasi pin hijau

void setup() {
Serial.begin (9600);          //memanggil serial monitor
pinMode(pinDigital, INPUT);   //menetapkan pin A0 sebagai input
pinMode(relay, OUTPUT);       //menetapkan pin 6 sebagai output
pinMode(ledGreen, OUTPUT);    //menetapkan pin 7 sebagai output
pinMode(ledYellow, OUTPUT);   //menetapkan pin 8 sebagai output
pinMode(ledRed, OUTPUT);      //menetapkan pin 9 sebagai output
}

void loop() {
int dataAnalog = analogRead (pinDigital);  //membaca nilai dari pin sensor
//print hasil ke serial monitor
Serial.print ("A0 : ");
Serial.print (dataAnalog);

//jika nilai A0 kurang dari 500, print kata serta nyalakan led hijau dan matikan led kuning, merah dan juga relay
if ((dataAnalog >= 400)&&(dataAnalog <1023)) { 
  Serial.println(". Media kering"); 
  digitalWrite (ledGreen, LOW);
  digitalWrite (ledYellow, LOW);
  digitalWrite (ledRed, LOW);
  digitalWrite (relay, HIGH);
  }

 //tetapi, jika nilai A0 kurang dari 900, print kata serta nyalakan led kuning dan relay kemudian matikan led hijau dan juga merah
if ((dataAnalog > 100) && (dataAnalog < 400)) {
  Serial.println(". Kelembaban Tanah Masih Cukup"); 
  digitalWrite (ledGreen, LOW);
  digitalWrite (ledYellow, LOW);
  digitalWrite (ledRed, LOW);
  digitalWrite (relay, LOW);
  }

 delay (5000);
}
