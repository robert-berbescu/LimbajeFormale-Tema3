#include <iostream>
#include <unordered_map>
#include <fstream>
#include <string>
#include <sstream>
#include <stack>
#include "Regula.h"
#include <iomanip>
#include <thread>

using namespace std;

vector<Regula> gramatica;
stack<string> stiva;
stack<string> intrare;
int lungimeRandAfisat = 0;

ofstream r("reguli.y");


void procesareSirIntrare(string sirIntrare) {
    istringstream iss(sirIntrare);
    string token;

    stack<string> intrareInvers;
    while (iss >> token) {
        intrareInvers.push(token);
    }
    //Inversare 
    while (!intrareInvers.empty()) {
        intrare.push(intrareInvers.top());
        intrareInvers.pop();
    }
}

void afisareLinie() {
    cout << "----------------------------------------------------------------------------------------" << endl;
}

void afisareStivaSiIntrare() {

    lungimeRandAfisat = 0;

    //Inversare stiva
    stack<string> temp = stiva;
    stack<string> temp2;
    while (!temp.empty()) {
        temp2.push(temp.top());
        temp.pop();
    }

    //Afisare stiva
    while (!temp2.empty()) {
        cout << left << temp2.top() << " ";
        lungimeRandAfisat = lungimeRandAfisat + temp2.top().size() + 1;
        temp2.pop();
    }

    cout << left << setw(35 - lungimeRandAfisat) << " ";
    lungimeRandAfisat = 0;

    //Afisare intrare
    stack<string> temp6 = intrare;
    while (!temp6.empty()) {
        cout << temp6.top() << " ";
        lungimeRandAfisat = lungimeRandAfisat + temp6.top().size() + 1;
        temp6.pop();
    }


}


void generareReguliY(string pas) {

    string elementCurent="";

    if (pas == "inainte") {
        r << "%{\n";
        r << "#include <stdio.h>\n";
        r << "#include <stdlib.h>\n";
        r << "%}\n";
        r << "\n";
        r << "%%\n";
        r << "\n";
    }
    else if (pas == "dupa") {
        r << ";\n\n";
        r << "\n%%\n";
        r << "\n";
        r << "int yyerror(char *s) {\n";
        r << "    fprintf(stderr, \"%s\\n\", s);\n";
        r << "    return 1;\n";
        r << "}\n";
        r << "\n";
        r << "int main() {\n";
        r << "    if (yyparse())\n";
        r << "        fprintf(stderr, \"Successful parsing.\\n\");\n";
        r << "    else\n";
        r << "        fprintf(stderr, \"Error found.\\n\");\n";
        r << "\n";
        r << "    return 0;\n";
        r << "}";

    }
    else if (pas == "reguli") {
        for (Regula regula : gramatica) {
            if (regula.getElementStart() != elementCurent) {
                if (elementCurent != "")
                    r << ";\n\n";
                elementCurent = regula.getElementStart();
                r << elementCurent << ": ";
                r << "   " << regula.getElementDerivatY();
                r << "\n";
            }
            else {
                r << "    | " << regula.getElementDerivatY() << "\n";
            }
        }
    }

}

int main()
{

    //Citim regulile
    ifstream g("reguli.txt");
    string date;
    while (g >> date) {
        gramatica.push_back(Regula(date[0], date.substr(3)));
    }

    //Generare reguli.y
    generareReguliY("inainte");
    generareReguliY("reguli");
    generareReguliY("dupa");
    r.close();

    system("bison reguli.y --report=state");

    ifstream f("reguli.output");

    bool stateNou = false;
    string line;
    int nrState = 0;
    string undeSarim;
    string token;
    string rSauD;

    unordered_map<string, vector<string>> actiuni;
    unordered_map<string, vector<string>> salt;

    salt["E"] = vector<string>();
    salt["T"] = vector<string>();
    salt["F"] = vector<string>();
    

    while (!f.eof()) { 

        if (stateNou == false) {
            getline(f, line);
            stateNou = false;

        }

        //Cautam terminalele si le salvam
        if (line.find("Terminals") != std::string::npos) {
            getline(f, line); getline(f, line); getline(f, line);
            actiuni["$"] = vector<string>();
            while (line.find("error") == std::string::npos)
            {
                actiuni[line.substr(1, 1)] = vector<string>();
                getline(f, line);
            }
        }

        if (line.find("state") != std::string::npos && line.find("go to") == std::string::npos) {
            if (line.length() == 7) {
                nrState = stoi(line.substr(6, 7));
            }
            else {
                nrState = stoi(line.substr(6, 8));
            }
            //cout << "state nr: " << nrState << endl;


            for (auto& entry : actiuni) {
                entry.second.push_back("#");
            }

            for (auto& entry : salt) {
                entry.second.push_back("#");
            }
            

            while (getline(f, line) && !(line.find("state") != std::string::npos && line.find("go to") == std::string::npos)) {
                
                undeSarim = "#";

                //Cautam linii care contin "shift" sau "reduce" sau "accept"
                if (line.find("shift") != std::string::npos || line.find("reduce") != std::string::npos || line.find("$default  accept") != std::string::npos) {


                    if (line.find("shift") != std::string::npos) {
                        rSauD = "d";
					}
					else if(line.find("reduce") != std::string::npos) {
						rSauD = "r";
                    }
                    else {
                        rSauD = "acc";
                    }

                    if (line.substr(4, 4) == "$end") //in loc de $end punem $
                    {
                        token = "$";
                    }
                    else if (line.substr(4,8)=="$default") {
                        token = "$default";
                    }
                    else {
                        token = line.substr(5, 1);
                    }

                    if (rSauD == "d") {
                        if (line.length() == 33) {
                            undeSarim = line.substr(32, 1);
                        }
                        else {
                            undeSarim = line.substr(33, 2);
                        }
					}
					else if(rSauD=="r") //daca avem reduce
                    {
                        if (line.length() == 37) {
							undeSarim = line.substr(32, 1);
						}
                        else {
							undeSarim = line.substr(32, 2);
						}
                    }


                    //cout << "Linia " << line << " are " << line.length() << endl;

                    if (line.length() == 20) {
                        undeSarim = "";
                    }

                    
                    //cout << token << " " << rSauD << " " << undeSarim << endl;
                   
                    if (token == "$default") {
                        for (auto& entry : actiuni) {
                            entry.second.at(nrState) = (rSauD + undeSarim);
                        }
                    }
                    else if (undeSarim == "accept") {
                        
                        actiuni[token].at(nrState) = "acc";
                    }
                    else {
                        actiuni[token].at(nrState) = (rSauD + undeSarim);
                    }                      
                }

                //Cautam pentru tabela de salt (gata)
                if (line.find("go to state") != std::string::npos && line.find("'") == std::string::npos && line.find("$") == std::string::npos) {
                    
                    token = line.substr(4, 1);

                    if (line.length() == 20) {
                        undeSarim = line.substr(19, 1);
                    }
                    else {
                        undeSarim = line.substr(19, 2);
                    }

                    salt[token].at(nrState) = (undeSarim);

				}

            }
            //cout << endl;
            stateNou = true;
        }
    }


    //Afisare tabela de actiuni
    cout << "Tabela de actiuni: " << endl;
   
    for (auto& entry : actiuni) {
        cout<<entry.first<<" -> ";
        for (int i = 0;i < entry.second.size();i++)
        {
			cout << entry.second.at(i) << " ";
		}
        cout << endl;
    }

    //Afisare tabela de salt
    cout << endl << "Tabela de salt: " << endl;
    for (auto& entry : salt) {
        cout<<entry.first<<" -> ";
        for (int i = 0;i < entry.second.size();i++)
        {
            cout<<entry.second.at(i)<<" ";
        }
        cout << endl;
    }



    // APD - TEMA 2
    cout << endl << endl;
    cout << "APD - TEMA 2" << endl;


    stiva.push("$");
    stiva.push("0");

    string sirIntrare = "a + a * a $";

    string actiune;
    int contor = 0;
    bool gata = false;
    procesareSirIntrare(sirIntrare);

    //Afisare conditii initiale
    cout << "STIVA" << setw(37) << "INTRARE" << setw(23) << "ACTIUNE" << endl;
    afisareLinie();
    afisareStivaSiIntrare();

    while (gata == false) {
        actiune = actiuni.at(intrare.top()).at(stoi(stiva.top()));

        cout << right << setw(25 - lungimeRandAfisat) << actiune << " " << endl;

        if (actiune[0] == 'd') {
            //Luam din sir de inceput si punem in stiva
            stiva.push(intrare.top());
            intrare.pop();
            stiva.push(actiune.substr(1));
        }
        else if (actiune[0] == 'r') {
            string ceCautam = gramatica.at(actiune[1] - '0' - 1).getElementDerivat();
            string sirGol = "";

            //Stergem tot ce nu e bun
            while (sirGol != ceCautam) {
                if (string::npos == stiva.top().find_first_of("0123456789"))
                {
                    sirGol = stiva.top() + sirGol;
                }
                stiva.pop();
            }

            //Pun in stiva elementul inlocuit
            stiva.push(gramatica.at(actiune[1] - '0' - 1).getElementStart());

            //Ne uitam in tabela de salt
            string varf = stiva.top();
            stiva.pop();
            string cifra = stiva.top();
            stiva.push(varf);

            int cifraDinTabelaDeSalt = stoi(salt.at(varf).at(stoi(cifra)));
            stiva.push(to_string(cifraDinTabelaDeSalt));

        }
        else if (actiune == "acc") {
            afisareLinie();
            cout << "Sirul de intrare este corect" << endl;
            gata = true;
        }
        else {
            afisareLinie();
            cout << "Sirul de intrare este incorect" << endl;
            gata = true;
        }

        if (gata == 0) {
            afisareStivaSiIntrare();
        }
    }
    cout << endl;




    return 0;
}