//
// Created by GalSade on 23/05/2016.
//
#include <iostream>
#include <fstream>
#include <string>
#include <cstddef>
using namespace std;
int main() {
    string line;
    ifstream myfile ("all_threads");
    if (myfile.is_open()) {
        int primeNum=1;
        int justNum=1;
        int numNums=0;
        int numPrimes=0;
        if (myfile)
        {
            while (getline( myfile, line ))
            {
                if (line.length() != 0) {
             //       cout << "LINE: "<<line<<endl;
                    std::size_t found = line.find("prime");
                    if (found == std::string::npos) {

                        numNums++;
                        justNum = std::stoi(line);
               //         cout <<"NUM: "<<justNum <<" PRIME: "<<primeNum<<endl;
                        if (justNum % primeNum != 0) {
                            cout << "Aieee, killed the idle prime candidate :( " <<endl;
                            cout<< "The evil prime "<< primeNum << " deleted " << justNum
                            << "which doesn't divide it."<<endl;
                            cout << "BADDDDDDDDDDDDDDD. You be BAD. No beach for you :(" <<endl;
                            goto out;
                        }
                    }
                    else {
                        primeNum = std::stoi(line.substr(line.find(' ')));
                        numPrimes++;
                    }
                }
            }
            myfile.close();
            cout << "Finished. You be GOOD. You had "<<numPrimes <<" prime candidates and "<< numNums<< " numbers deleted."<<endl;
        }
        else cout << "SOME SORT OF FILE PROBLEM OCCURED :(";
    }
    else cout << "OH-NO! UNABLE TO OPEN FILE!!" <<endl;
    out:
    return 0;
}



