#include "DHT.h" //library sensor yang telah diimportkan
#include <Fuzzy.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#define DHTPIN 2     //Pin apa yang digunakan
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);
const int SoilSensor = A0;
const int relayPin = 3;
int pinLed = 5;

// object fuzzy
Fuzzy *fuzzy = new Fuzzy();

void setup(){
  Serial.begin(9600);
  lcd.init(); 
  lcd.backlight();
  lcd.setCursor(0,0);
  dht.begin(); //prosedur memulai pembacaan module sensor
  pinMode(relayPin, OUTPUT);
  pinMode(pinLed, OUTPUT);

  fungsiSetupFuzzy();

}

void loop(){
  float kelembabanTanah;
  int hasilPembacaan = analogRead(SoilSensor);
  kelembabanTanah = (100 - ((hasilPembacaan/1023.00)*100));
  float suhu = dht.readTemperature();

  fuzzy->setInput(1, suhu);
  fuzzy->setInput(2, kelembabanTanah);
  fuzzy->fuzzify();

  // Melakukan defuzzifikasi untuk mendapatkan output
  float out_durasi = fuzzy->defuzzify(1);
  
  relayNyala(out_durasi);
  lcd.println("Suhu: "+ String(suhu));
  delay (3000);
  lcd.clear ();
  lcd.println("Kelmbabn: "+ String(kelembabanTanah) +"%");
  delay (3000);
  lcd.clear ();
  lcd.println("Output : "+ String(out_durasi));
  delay (3000);
  lcd.clear ();
}

void relayNyala(int durasi){
  if(durasi <= 0){
    digitalWrite(relayPin, HIGH);
    digitalWrite(pinLed, LOW);
  }else{
   digitalWrite(relayPin, LOW);
   digitalWrite(pinLed, HIGH);
   delay(durasi);
   digitalWrite(relayPin, HIGH);
   digitalWrite(pinLed, LOW);
  }
}
void fungsiSetupFuzzy(){
  
  
// Membership Function Suhu
  FuzzyInput *inputSuhu = new FuzzyInput(1);
  FuzzySet *dingin = new FuzzySet(0, 0, 24, 25);
  inputSuhu->addFuzzySet(dingin);
  FuzzySet *normalSuhu = new FuzzySet(24, 25, 28, 29);
  inputSuhu->addFuzzySet(normalSuhu);
  FuzzySet *panas = new FuzzySet(28, 29, 40, 40);
  inputSuhu->addFuzzySet(panas);
  fuzzy->addFuzzyInput(inputSuhu);

  // Membership Function Kelembaban
  FuzzyInput *inputKelembaban = new FuzzyInput(2);
  FuzzySet *kering = new FuzzySet(0, 0, 30, 50);
  inputKelembaban->addFuzzySet(kering);
  FuzzySet *normalLembab = new FuzzySet(30, 50, 50, 70);
  inputKelembaban->addFuzzySet(normalLembab);
  FuzzySet *basah = new FuzzySet(50, 70, 100, 100);
  inputKelembaban->addFuzzySet(basah);
  fuzzy->addFuzzyInput(inputKelembaban);

  // Membership Function Output Durasi Pompa
  FuzzyOutput *outputDurasi = new FuzzyOutput(1);
  FuzzySet *pendek = new FuzzySet(0, 0, 500, 1000);
  outputDurasi->addFuzzySet(pendek);
  FuzzySet *cukup = new FuzzySet(500, 1000, 1000, 1500);
  outputDurasi->addFuzzySet(cukup);
  FuzzySet *lama = new FuzzySet(1000, 1500, 2500, 2500);
  outputDurasi->addFuzzySet(lama);
  fuzzy->addFuzzyOutput(outputDurasi);

  // Fuzzy Rule 1 Untuk Pompa
  FuzzyRuleAntecedent *dingin_kering = new FuzzyRuleAntecedent();
  dingin_kering->joinWithAND(dingin,kering);
  FuzzyRuleConsequent *lama1 = new FuzzyRuleConsequent();
  lama1->addOutput(lama);
  FuzzyRule *fuzzyRule1 = new FuzzyRule(1, dingin_kering, lama1);
  fuzzy->addFuzzyRule(fuzzyRule1);

  // Fuzzy Rule 2 Untuk Pompa
  FuzzyRuleAntecedent *normalSuhu_kering = new FuzzyRuleAntecedent();
  normalSuhu_kering->joinWithAND(normalSuhu,kering);
  FuzzyRuleConsequent *lama2 = new FuzzyRuleConsequent();
  lama2->addOutput(lama);
  FuzzyRule *fuzzyRule2 = new FuzzyRule(2, normalSuhu_kering, lama2);
  fuzzy->addFuzzyRule(fuzzyRule2);

   // Fuzzy Rule 3 Untuk Pompa
  FuzzyRuleAntecedent *panas_kering = new FuzzyRuleAntecedent();
  panas_kering->joinWithAND(panas,kering);
  FuzzyRuleConsequent *lama3 = new FuzzyRuleConsequent();
  lama3->addOutput(lama);
  FuzzyRule *fuzzyRule3 = new FuzzyRule(3, panas_kering, lama3);
  fuzzy->addFuzzyRule(fuzzyRule3);

   // Fuzzy Rule 4 Untuk Pompa
  FuzzyRuleAntecedent *dingin_normalLembab = new FuzzyRuleAntecedent();
  dingin_normalLembab->joinWithAND(dingin,normalLembab);
  FuzzyRuleConsequent *cukup1 = new FuzzyRuleConsequent();
  cukup1->addOutput(cukup);
  FuzzyRule *fuzzyRule4 = new FuzzyRule(4, dingin_normalLembab, cukup1);
  fuzzy->addFuzzyRule(fuzzyRule4);

   // Fuzzy Rule 5 Untuk Pompa
  FuzzyRuleAntecedent *normalSuhu_normalLembab = new FuzzyRuleAntecedent();
  normalSuhu_normalLembab->joinWithAND(normalSuhu,normalLembab);
  FuzzyRuleConsequent *cukup2 = new FuzzyRuleConsequent();
  cukup2->addOutput(cukup);
  FuzzyRule *fuzzyRule5 = new FuzzyRule(5, normalSuhu_normalLembab, cukup2);
  fuzzy->addFuzzyRule(fuzzyRule5);

  // Fuzzy Rule 6 Untuk Pompa
  FuzzyRuleAntecedent *panas_normalLembab = new FuzzyRuleAntecedent();
  panas_normalLembab->joinWithAND(panas,normalLembab);
  FuzzyRuleConsequent *cukup3 = new FuzzyRuleConsequent();
  cukup3->addOutput(cukup);
  FuzzyRule *fuzzyRule6 = new FuzzyRule(6, panas_normalLembab, cukup3);
  fuzzy->addFuzzyRule(fuzzyRule6);

  // Fuzzy Rule 7 Untuk Pompa
  FuzzyRuleAntecedent *dingin_basah = new FuzzyRuleAntecedent();
  dingin_basah->joinWithAND(dingin,basah);
  FuzzyRuleConsequent *pendek1 = new FuzzyRuleConsequent();
  pendek1->addOutput(pendek);
  FuzzyRule *fuzzyRule7 = new FuzzyRule(7, dingin_basah, pendek1);
  fuzzy->addFuzzyRule(fuzzyRule7);

  // Fuzzy Rule 8 Untuk Pompa
  FuzzyRuleAntecedent *normalSuhu_basah = new FuzzyRuleAntecedent();
  normalSuhu_basah->joinWithAND(normalSuhu,basah);
  FuzzyRuleConsequent *pendek2 = new FuzzyRuleConsequent();
  pendek2->addOutput(pendek);
  FuzzyRule *fuzzyRule8 = new FuzzyRule(8, normalSuhu_basah, pendek2);
  fuzzy->addFuzzyRule(fuzzyRule8);

  // Fuzzy Rule 9 Untuk Pompa
  FuzzyRuleAntecedent *panas_basah = new FuzzyRuleAntecedent();
  panas_basah->joinWithAND(panas,basah);
  FuzzyRuleConsequent *pendek3 = new FuzzyRuleConsequent();
  pendek3->addOutput(pendek);
  FuzzyRule *fuzzyRule9 = new FuzzyRule(9, panas_basah, pendek3);
  fuzzy->addFuzzyRule(fuzzyRule9);
  
}
