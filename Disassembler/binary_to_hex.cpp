#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>

using namespace std;

int main()
{
    string in;
    string content[1000];
    getline(cin, in);
    int i = 0;
    while (in != ".")
    {
        content[i] = in;
        i++;
        getline(cin, in);
    }
    i--;
    for (int k = 0; k < i; k++)
    {
        string input = content[k];
        string hex;
        stringstream ss;
        ss << std::hex << stoi(input, nullptr, 2);
        ss >> hex;
        transform(hex.begin(), hex.end(), hex.begin(), ::tolower);
        cout << setw(8) << setfill('0') << hex << endl;
    }
}