#pragma once
#include "Regula.h"
#include <string>
#include <vector>

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

	string getElementDerivatY() {
		string s(this->element_derivat);
		vector<char> v(s.begin(), s.end());

		string stringFinal="";

		for (char v1 : v)
		{
			if (v1 == 'E' || v1 == 'T' || v1 == 'F')
			{
				stringFinal += v1;
			}
			else {
				stringFinal += " '";
				stringFinal += v1;
				stringFinal += "' ";
			}
		}
		return stringFinal;
	}

	void setElementStart(char date) {
		this->element_start = date;
	}
	void setElementDerivat(string date) {
		this->element_derivat = date;
	}


};
