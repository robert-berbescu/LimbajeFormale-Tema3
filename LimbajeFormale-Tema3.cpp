#include <iostream>
#include <unordered_map>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

int main()
{
    //system("bison -d reguli.y --report=state");

    

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
                            undeSarim = line.substr(32, 2);
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



        return 0;
}