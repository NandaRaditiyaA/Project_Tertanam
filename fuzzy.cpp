#include "fuzzy.h"

float kering(float b, float c, float d, float x)
{
	/*Menghitung derajat keangotaan kurva trapesium
	hilang yang kiri

    Args:
        b (int): nilai tengah
        c (int): nilai tengah 1
        d (int): nilai batas
        x (int): nilai yang dihitung

    Returns:
        float / int : derajat keanggotaan 
	*/

	if (x <= b || x >= d){
        return 0;
	}else if (x >= b && x <= c){
        return 1;
	}else if (x > c && x < d){
        return (d-x)/(d-c);
	}else {
		return 0;
	}
}

float lembab(float a, float b, float c, float x)
{
	/*Menghitung derajat keangotaan kurva segitiga

    Args:
        a (int): nilai awal
        b (int): nilai tengah
        c (int): nilai batas
        x (int): nilai yang dihitung

    Returns:
        float / int : derajat keanggotaan 
	*/

	if (x <= a or x >= c){
        return 0;
	}else if (x == b){
        return 1;
	}else if (x >= a && x < b){
        return (b-x)/(b-a);
	}else if (x > b && x <= c){
        return (c-x)/(c-b);
	}else{
		return 0;   
	}
}

float basah(float a, float b, float c, float x)
{
	/* Menghitung derajat keangotaan kurva trapesium
	hilang yang kanan

	Args:
        a (int): nilai awal
        b (int): nilai tengah
        c (int): nilai tengah 1
        x (int): nilai yang dihitung

    Returns:
        float / int : derajat keanggotaan 
	*/

	if(x <= a || x >= c){
        return 0;
	}else if(x > a && x < b){
        return (x-a)/(b-a);
	}else if(x >= b && x <= c){
        return 1;
	}else{
		return 0;
	}
}

String* fuzzyfikasi(int sens[])
{
	String* result = new String[2];
	String label[] = {"Kering", "Lembab", "Basah"};
	int labelI[2] = {};

	for(int i = 0; i < 2; i++){
		float nKering = kering(0, 10, 25, sens[i]);
		float nLembab = lembab(15, 35, 55, sens[i]);
		float nBasah = basah(45, 60, 75, sens[i]);
		float nk[] = {nKering, nLembab, nBasah};
		
		float max = 0;
		int index = 0;
		for(int j = 0; j < 3; j++){
			if(nk[j] > max){
				max = nk[j];
				index = j;
			}
		}
		labelI[i] = index;
	}
	result[0] = label[labelI[0]];
	result[1] = label[labelI[1]];
	
	return result;
}

String* inferensi(String* sens)
{
	String* result = new String[2];
	int* oc = new int[2];
	String label[] = {"Kering", "Lembab", "Basah"};

	String output[3][3][2] = {
		{{"Banyak", "Cepat"}, {"Banyak", "Cepat"}, {"Banyak", "Cepat"}},
		{{"Banyak", "Cepat"}, {"Cukup", "Sedang"}, {"0", "Mati"}},
		{{"0", "Mati"}, {"0", "Mati"}, {"0", "Mati"}},
	};


	for (int i = 0; i < 2; i++){
		for (int j = 0; j < 3; j++){
			if (sens[i] == label[j]){
				oc[i] = j;
			}
		}
	}

	result[0] = output[oc[0]][oc[1]][0];
	result[1] = output[oc[0]][oc[1]][1];

	return result;
}

int* defuzzyfikasi(String* sens)
{
	int* result = new int[2];
	
	// Output debit air
	if (sens[0] == "Sedikit"){
		result[0] = 50;
	}else if(sens[0] == "Cukup"){
		result[0] = 150;
	}else if(sens[0] == "Banyak"){
		result[0] = 300;
	}else{
		result[0] = 0;
	}

	// Outpout PWM
	if (sens[1] == "Cepat"){
		result[1] = 5;
	}else if(sens[1] == "Sedang"){
		result[1] = 10;
	}else if(sens[1] == "Lama"){
		result[1] = 15;
	}else{
		result[1] = 0;
	}

	return result;
}

int* fuzzy(int* sens)
{
  String* sensFuz = fuzzyfikasi(sens);
//  Serial.print(sensFuz[0]);
//  Serial.print(" ");
//  Serial.println(sensFuz[1]);

  // Pilih nilai output
  String* outFuz = inferensi(sensFuz);
//  Serial.print(outFuz[0]);
//  Serial.print(" ");
//  Serial.println(outFuz[1]);

  // Defuzzyfikasi nilai output
  int* out = defuzzyfikasi(outFuz);
//  Serial.print(out[0]);
//  Serial.print(" ");
//  Serial.println(out[1]);

	int* result = new int[2];
	result[0] = out[0];
	result[1] = out[1];
}
