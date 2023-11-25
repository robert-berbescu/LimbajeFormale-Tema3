#include <iostream>
#include <unordered_map>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

int main()
{
    //system("bison -d reguli.y --report=state");

    unordered_map<string, vector<string>> actiuni;

    ifstream f("reguli.output");

    bool stateNou = false;
    string line;
    int nrState = 0;
    int undeSarim;
    string token;
    string rSauD;


    unordered_map<string, vector<string>> salt;

    while (!f.eof()) { 

        if (stateNou == false) {
            getline(f, line);
            stateNou = false;
        }
        if (line.find("state") != std::string::npos && line.find("go to") == std::string::npos) {
            if (line.length() == 7) {
                nrState = stoi(line.substr(6, 7));
            }
            else {
                nrState = stoi(line.substr(6, 8));
            }
            cout << "state nr: " << nrState << endl;


            while (getline(f, line) && !(line.find("state") != std::string::npos && line.find("go to") == std::string::npos)) {
                
                //Cautam linii care contin "shift" sau "reduce"
                if (line.find("shift") != std::string::npos || line.find("reduce") != std::string::npos) {

                    if (line.find("shift") != std::string::npos) {
                        rSauD = "shift";
					}
					else {
						rSauD = "reduce";
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

                    if (line.length() == 33) {
                        undeSarim = stoi(line.substr(32, 33));
                    }
                    else {
                        undeSarim = stoi(line.substr(32, 34));
                    }

                    //cout<<token<<" "<<rSauD<<" "<< undeSarim << endl;
                }

                //Cautam pentru tabela de salt (gata)
                if (line.find("go to state") != std::string::npos && line.find("'") == std::string::npos && line.find("$") == std::string::npos) {
                    
                    token = line.substr(4, 1);

                    if (line.length() == 20) {
                        undeSarim = stoi(line.substr(19, 1));
                    }
                    else {
                        undeSarim = stoi(line.substr(19, 2));
                    }

                    if (salt.find(token) == salt.end()) {
						salt[token] = vector<string>();
                    }

                    salt[token].push_back(to_string(undeSarim));

				}

            }
            stateNou = true;
        }
        cout << endl;
    }



        return 0;
}