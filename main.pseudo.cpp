#include <iostream>

using namespace std;


int main() {
    int i, j, square;

    for(i=0; i<20; i++) {

        square = 0;
        for(j=0; j<i; j++) {
            square += i;
        }

        cout << square << endl;
    }
}