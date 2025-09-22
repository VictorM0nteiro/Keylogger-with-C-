#include <iostream>
#include <fstream> //for working with files
#include <string>
using namespace std;

void log(const string &text) {
    ofstream data("data.txt", ios::app);
    if (data.is_open()) {
        data << text;
    }else {
        cout << "failed to open file" << endl;
    }
}

int main() {

}