#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>
using namespace std;

const int LETTER_OFFSET_IN_ASCII = 32, MIN_WORD_LEN = 4;

string ToLower(string str);

string ProcessString(string str);


int main()
{
    // Note we have to specify both in and out because we're using fstream
    std::fstream iofile{ "Test_Data.txt", std::ios::in | std::ios::out };

    // If we couldn't open iofile, print an error
    if (!iofile)
    {
        // Print an error and exit
        std::cerr << "Uh oh, Test_Data.txt could not be opened!\n";
        return 1;
    }
    string word;
    stringstream fillBlank;
    while (iofile >> word) {
        word = ToLower(word);
        int len = word.length();
        iofile.seekg(-1 * len, std::ios::cur);
        iofile << word;
        iofile.seekg(iofile.tellg(), std::ios::beg);
    }

    iofile.clear();  
    iofile.seekg(0, std::ios::beg);
    char ch;
    while (iofile.get(ch)) {
        if (ch == '\'') {
            iofile.seekg(-1, std::ios::cur);
            iofile << "  ";
            iofile.seekg(iofile.tellg(), std::ios::beg);
        }
        else if (!((ch >= 'a') && (ch <= 'z'))) {
            iofile.seekg(-1, std::ios::cur);
            iofile << " ";
            iofile.seekg(iofile.tellg(), std::ios::beg);
        }
    }
    int len;
    iofile.clear();
    iofile.seekg(0, std::ios::beg);
    while (iofile >> word) {
        len = word.length();
        if (!(len > MIN_WORD_LEN)) {
            fillBlank.str("");
            fillBlank << setw(len) << "";
            iofile.seekg(-1*len, std::ios::cur);
            iofile << fillBlank.str();
            iofile.seekg(iofile.tellg(), std::ios::beg);
        }
    }

    iofile.close();
    return 0;
    }

string ProcessString(string str) {
    string processedString = "";
    size_t strLen = str.length();
    for (int i = 0; i < strLen; i++) {
        char ch = str[i];
        if (((ch >= 'a') && (ch <= 'z'))) {
            processedString += ch;
        }
        else if (ch == '\'') { //skips letter after apostrophe
            i++;
        }
    }
    return processedString;
}

string ToLower(string str) {
    string lowerCaseStr = "";
    for (int i = 0; i < str.length(); i++) {
        char ch = str[i];
        if (((ch >= 'A') && (ch <= 'Z'))) {
            ch += LETTER_OFFSET_IN_ASCII; //converts char to lower case
        }
        lowerCaseStr += ch;
    }
    return lowerCaseStr;
}