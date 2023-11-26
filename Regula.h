#pragma once
#include "Regula.h"
#include <string>

using namespace std;

class Regula
{
	string element_start;
	string element_derivat;

public:

	Regula(char start, string derivat)
	{
		this->element_start = start;
		this->element_derivat = derivat;
	}


	string getElementStart() {
		return this->element_start;
	}
	string getElementDerivat() {
		return this->element_derivat;
	}

	void setElementStart(char date) {
		this->element_start = date;
	}
	void setElementDerivat(string date) {
		this->element_derivat = date;
	}


};
