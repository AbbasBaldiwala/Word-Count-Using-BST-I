#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>
using namespace std;

const int LETTER_OFFSET_IN_ASCII = 32, MIN_WORD_LEN = 4, SETW_SIZE = 15;

enum Menu {
    UPDATE_INPUT_FILE = 1, BUILD_AND_PRINT_BST = 2, QUIT = 3
};



class Node {
public:
    string word;
    int count;
    Node* leftLink = nullptr;
    Node* rightLink = nullptr;

};

class BinarySearchTree {
public:
    const BinarySearchTree& operator=(const BinarySearchTree& RHS);

    bool IsEmpty() { return (root == nullptr); }

    void InorderTraversal() { Inorder(root); }

    void PreorderTraversal() { Preorder(root); }

    void PostorderTraversal() { Postorder(root); }

    int GetHeight() const { return Height(root); }

    void DestroyTree();

    bool SearchTree(string word);

    void InsertWord(string word);

    BinarySearchTree(const BinarySearchTree& otherTree);

    BinarySearchTree() { root = nullptr; }

    ~BinarySearchTree() { Destroy(root); }

protected:
    Node* root = nullptr;

private:
    void CopyTree(Node*& copiedTreeRoot, Node* otherTreeRoot);

    void Destroy(Node*& p);

    void Inorder(Node* p) const;

    void Preorder(Node* p) const;

    void Postorder(Node* p) const;

    int Height(Node* p) const;

    int Max(int x, int y) const;
};

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
void ProcessInputFile();

/*
preconditions: the Binary search tree must exist and the header musht exist
postconditions: all the words will be printed to the screen
*/
void BuildAndPrintBST(BinarySearchTree BST, string header);

/*

*/
void ClearInvalidInput(string errMsg); //clears cin, clears the keyboard buffer, prints an error message


int main()
{
    int userChoice;
    BinarySearchTree BST;
    stringstream headerSS;
    headerSS << left << setw(SETW_SIZE) << "WORD" << "COUNT" << "\n\n";
    string header = headerSS.str();
    cout << left;
    
    do {
        cout << "\n\nMENU: \n"
            "1. UPDATE INPUT FILE\n"
            "2. BUILD AND PRINT BST\n"
            "3. QUIT\n\n";
        cin >> userChoice;
        switch (userChoice) {
        case UPDATE_INPUT_FILE:
            ProcessInputFile();
            break;
        case BUILD_AND_PRINT_BST:
            try {
                BuildAndPrintBST(BST, header);
            }
            catch (const std::bad_alloc error) {
                std::cerr << "Exception caught, Could not allocate memory: " << error.what() << "\n";
            }
            break;
        case QUIT:
            cout << "\nQUITTING..." << endl;
            break;
        default:
            ClearInvalidInput("INVALID MENU SELECTION");
        }
    } while (userChoice != QUIT);

    return 0;
    }


//BST METHODS
void BinarySearchTree::Inorder(Node* p) const {
    if (p != nullptr) {
        Inorder(p->leftLink);
        cout << left << setw(SETW_SIZE) << p->word << right << p->count << "\n";
        Inorder(p->rightLink);
    }
}

void BinarySearchTree::Preorder(Node* p) const {
    if (p != nullptr) {
        cout << left << setw(SETW_SIZE) << p->word << right << p->count << "\n";
        Preorder(p->leftLink);
        Preorder(p->rightLink);
    }
}

void BinarySearchTree::Postorder(Node* p) const {
    if (p != nullptr) {
        Postorder(p->leftLink);
        Postorder(p->rightLink);
        cout << left << setw(SETW_SIZE) << p->word << right << p->count << "\n";
    }
}

int BinarySearchTree::Height(Node* p) const {
    if (p == nullptr) {
        return 0;
    }
    else {
        return 1 + Max(Height(p->leftLink), Height(p->rightLink));
    }
}

int BinarySearchTree::Max(int x, int y) const{
    if (x > y) {
        return x;
    }
    else {
        return y;
    }
}

void BinarySearchTree::CopyTree(Node*& copiedTreeRoot, Node* otherTreeRoot) {
    if (otherTreeRoot == nullptr) {
        copiedTreeRoot = nullptr;
    }
    else {
        copiedTreeRoot = new Node;
        copiedTreeRoot->word = otherTreeRoot->word;
        copiedTreeRoot->count = otherTreeRoot->count;
        CopyTree(copiedTreeRoot->leftLink, otherTreeRoot->leftLink);
        CopyTree(copiedTreeRoot->rightLink, otherTreeRoot->rightLink);
    }
}

void BinarySearchTree::Destroy(Node*& p) {
    if (p != nullptr) {
        Destroy(p->leftLink);
        Destroy(p->rightLink);
        delete p;
        p = nullptr;
    }
}

void BinarySearchTree::DestroyTree() {
    Destroy(root);
}

const BinarySearchTree& BinarySearchTree::operator=(const BinarySearchTree& RHS) {
    if (this != &RHS) {
        if (RHS.root == nullptr) {
            root = nullptr;
        }
        else {
            Destroy(root);
            CopyTree(root, RHS.root);
        }
    }
    return *this;
}

BinarySearchTree::BinarySearchTree(const BinarySearchTree& otherTree) {
    if (otherTree.root == nullptr) {
        root = nullptr;
    }
    else {
        Destroy(root);
        CopyTree(root, otherTree.root);
    }
} 

bool BinarySearchTree::SearchTree(string searchWord) {
    bool found = false;
    Node* current = root;

    if (root == nullptr) {
        found = false;
    }
    else {
        current = root;

        while (current != nullptr && !found) {
            if (current->word == searchWord) {
                current->count += 1;
                found = true;
            }
            else if (current->word > searchWord) {
                current = current->leftLink;
            }
            else {
                current = current->rightLink;
            }
        }
    }
    return found;
}

void BinarySearchTree::InsertWord(string insertWord) {
    Node* current;
    Node* trailCurrent = nullptr;
    Node* newNode;

    newNode = new Node;
    newNode->word = insertWord;
    newNode->count = 1;
    newNode->leftLink = nullptr;
    newNode->rightLink = nullptr;
   
    if (root == nullptr)
        root = newNode;
    else
    {
        current = root;
        while (current != nullptr)
        {
            trailCurrent = current;
            if (current->word > insertWord)
                current = current->leftLink;
            else
                current = current->rightLink;
        }
        if (trailCurrent->word > insertWord)
            trailCurrent->leftLink = newNode;
        else
            trailCurrent->rightLink = newNode;
    }
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

void ProcessInputFile() {
    std::fstream ioFile{ "Test_Data.txt", std::ios::in | std::ios::out };

    if (!ioFile) {
        cout << "Input file not found. Exiting the program." << endl;
        system("pause");
        exit(EXIT_FAILURE);
    }
    if (ioFile.peek() == EOF) {
        cout << "The input file is empty. Quitting the program." << endl;
        ioFile.close();
        system("pause");
        exit(EXIT_FAILURE);
    }
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
    cout << "INPUT FILE UPDATED\n";
    ioFile.close();
}

void BuildAndPrintBST(BinarySearchTree BST, string header) {
    
    ifstream inFile("Test_Data.txt"); 
    if (!inFile) {
        cout << "Input file not found. Exiting the program." << endl;
        system("pause");
        exit(EXIT_FAILURE);
    }
    if (inFile.peek() == EOF) {
        cout << "The input file is empty. Quitting the program." << endl;
        inFile.close();
        system("pause");
        exit(EXIT_FAILURE);
    }

    string word;
    while (inFile >> word) {
        if (!BST.SearchTree(word) && word != "") {
            BST.InsertWord(word);
        }
    }
    cout << header;
    BST.InorderTraversal();
    BST.DestroyTree();
    inFile.close();
}

void ClearInvalidInput(string errMsg) {
    cout << "\n" << errMsg << "\n";
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}


//TEST 1
/*

MENU:
1. UPDATE INPUT FILE
2. BUILD AND PRINT BST
3. QUIT

1
INPUT FILE UPDATED


MENU:
1. UPDATE INPUT FILE
2. BUILD AND PRINT BST
3. QUIT

2
WORD           COUNT

about          1
accurately     2
actually       1
addition       1
agenda         1
agree          2
almost         1
always         1
anything       1
basically      1
because        4
become         1
believe        1
close          1
congress       6
democrat       1
democrats      2
different      2
divided        1
division       1
either         3
everyone       1
falcon         1
fewer          1
focused        1
forced         1
fully          1
going          1
hinders        1
ideas          2
impact         1
increase       1
instead        1
major          1
members        2
might          1
other          2
parties        2
partisan       1
partisanship   1
party          2
passed         1
passing        1
people         4
population     1
power          1
problems       1
professor      1
progress       1
rather         1
reality        1
represent      4
representing   1
republican     1
republicans    2
should         1
showing        1
sides          2
siding         1
solving        1
split          1
states         2
stopping       1
supposed       2
their          3
umbrella       1
under          1
united         1
voted          2
voters         1


MENU:
1. UPDATE INPUT FILE
2. BUILD AND PRINT BST
3. QUIT

3

QUITTING...
*/

//TEST 2
/*

MENU:
1. UPDATE INPUT FILE
2. BUILD AND PRINT BST
3. QUIT

dsfa

INVALID MENU SELECTION


MENU:
1. UPDATE INPUT FILE
2. BUILD AND PRINT BST
3. QUIT

5

INVALID MENU SELECTION


MENU:
1. UPDATE INPUT FILE
2. BUILD AND PRINT BST
3. QUIT

3

QUITTING...

*/

//TEST 3 (empty input file)
/*
MENU:
1. UPDATE INPUT FILE
2. BUILD AND PRINT BST
3. QUIT

1
The input file is empty. Quitting the program.
Press any key to continue . . .
*/

//TEST 4 (no input file found)
/*

MENU:
1. UPDATE INPUT FILE
2. BUILD AND PRINT BST
3. QUIT

1
Input file not found. Exiting the program.
Press any key to continue . . .
*/