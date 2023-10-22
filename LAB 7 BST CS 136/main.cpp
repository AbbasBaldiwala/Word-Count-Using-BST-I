#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>
using namespace std;

const int LETTER_OFFSET_IN_ASCII = 32, MIN_WORD_LEN = 4;

/*preconditions: string variable must exist
postconditions: lower case string is returned
*/
string ToLower(string str);

/*precondiitons: string variable must exist
postconditons: string is returned without any punctuation and letters after apostrophes are removed
*/
string ProcessWord(string str);

/*
preconditons: file must be opened without errors
postconditions: file will not include any puntuation and all words 4 characters or less are removed
*/
void ProcessInputFile(fstream& ioFile);


int main()
{
    std::fstream ioFile{ "Test_Data.txt", std::ios::in | std::ios::out };

    
    if (!ioFile)
    {
        std::cerr << "Test_Data.txt could not be opened\n";
        return 1;
    }

    ProcessInputFile(ioFile);

    ioFile.close();
    return 0;
    }

string ProcessWord(string str) {
    string processedWord = "";
    size_t strLen = str.length();
    for (int i = 0; i < strLen; i++) {
        char ch = str[i];
        if (((ch >= 'a') && (ch <= 'z'))) {
            processedWord += ch;
        }
        else if (ch == '\'') { //skips everything after apostrophe
            i = strLen;
        }
    }
    if (!(processedWord.length() > MIN_WORD_LEN)) {
        processedWord = "";
    }
    return processedWord;
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

void ProcessInputFile(fstream& ioFile) {
    string word;
    while (ioFile >> word) {
        stringstream fillSpace("");
        int originalLen = word.length();

        word = ToLower(word);
        word = ProcessWord(word);

        fillSpace << setw(originalLen) << word;

        ioFile.seekg(-1 * originalLen, std::ios::cur);
        ioFile << fillSpace.str();
        ioFile.seekg(ioFile.tellg(), std::ios::beg);
    }
}