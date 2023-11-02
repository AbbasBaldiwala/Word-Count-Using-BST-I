#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <limits>

using namespace std;

const int LETTER_OFFSET_IN_ASCII = 32, MIN_WORD_LEN = 4, SETW_SIZE = 15;

enum Menu {
    UPDATE_INPUT_FILE = 1, BUILD_AND_PRINT_BST = 2, QUIT = 3
};



class Node {
public:
    string word;
    int count = 0;
    Node* leftLink = nullptr;
    Node* rightLink = nullptr;
};

class BinarySearchTree {
public:
    /*pre:RHS tree must exist
    post: LHS will be a copy of RHS tree*/
    const BinarySearchTree& operator=(const BinarySearchTree& RHS);

    /*pre: root exists
    post: returns if the tree is empty*/
    bool IsEmpty() { return (root == nullptr); }

    /*pre:Inorder function exists
    post: prints the inorder traversal of the tree*/
    void TraverseInorder() { Inorder(root); }

    /*pre:Preorder function exists
    post: prints the preorder traversal of the tree*/
    void TraversePreorder() { Preorder(root); }

    /*pre: Postorder function exists
    post: prints the postorder traversal of the tree*/
    void TraversePostOrder() { Postorder(root); }

    /*pre: Height function exists
    post: returns the height of the tree*/
    int GetHeight() const { return Height(root); }

    /*pre: Destroy function exists
    post: tree is destroyed*/
    void DestroyTree(){ Destroy(root); }

    /*pre: String must be correctly passed as a parameter
    post: returns if the string is in the tree*/
    bool SearchTree(string word);

    /*pre: String must be correctly passed as a parameter
    post: inserts the string if it does not already exist in tree*/
    void InsertWord(string word);

    /*pre: otherTree exists
    post: A copy of otherTree is created*/
    BinarySearchTree(const BinarySearchTree& otherTree);

    /*pre:none
    post: tree object is created*/
    BinarySearchTree() { root = nullptr; }

    /*pre: Destroy function exists
    post: tree is destroyed*/
    ~BinarySearchTree() { Destroy(root); }

protected:
    Node* root = nullptr;

private:
    /*pre: two tree objects exist
    post: first object becomes copy of second*/
    void CopyTree(Node*& copiedTreeRoot, Node* otherTreeRoot);

    /*pre: none
    post: tree is destroyed*/
    void Destroy(Node*& p);

    /*pre: none
    post: prints the inorder traversal of the tree*/
    void Inorder(Node* p) const;

    /*pre: none
    post: prints the preorder traversal of the tree*/
    void Preorder(Node* p) const;

    /*pre: none
    post: prints the postorder traversal of the tree*/
    void Postorder(Node* p) const;

    /*pre: none
    post: returns height of the tree*/
    int Height(Node* p) const;

    /*pre: two integers must be passed in the parameters
    post: returns the larger of the two integers*/
    int Max(int x, int y) const;
};

/*pre: string variable must exist
post: lower case string is returned
*/
string ToLower(string str);

/*pre: string variable must exist
post: string is returned without any punctuation and letters after apostrophes are removed
*/
string ProcessWord(string str);

/*
pre: file name must exist and be properly declared
post: file will not include any puntuation and all words 4 characters or less are removed
*/
void ProcessInputFile(string FileName);

/*
pre: the Binary search tree must exist and the header musht exist
post: all the words will be printed to the screen
*/
void BuildBST(BinarySearchTree& BST);

/*
pre:BST and header string must exist
post: the BST gets filled with words and printed to screen
*/
void BuildAndPrintBST(BinarySearchTree& BST, string header);

/*
pre: none
post: clears cin, clears the keyboard buffer, prints an error message
*/
void ClearInvalidInput(string errMsg);


int main()
{
    int userChoice;
    string FileName = "Test_Data.txt";
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
            ProcessInputFile(FileName);
            break;
        case BUILD_AND_PRINT_BST:
            BuildAndPrintBST(BST, header);
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
        cout << setw(SETW_SIZE) << p->word <<  p->count << "\n";
        Inorder(p->rightLink);
    }
}

void BinarySearchTree::Preorder(Node* p) const {
    if (p != nullptr) {
        cout << setw(SETW_SIZE) << p->word << p->count << "\n";
        Preorder(p->leftLink);
        Preorder(p->rightLink);
    }
}

void BinarySearchTree::Postorder(Node* p) const {
    if (p != nullptr) {
        Postorder(p->leftLink);
        Postorder(p->rightLink);
        cout << setw(SETW_SIZE) << p->word << p->count << "\n";
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

const BinarySearchTree& BinarySearchTree::operator=(const BinarySearchTree& RHS) {
    Node* tempRoot = nullptr;
    try {
        if (this != &RHS) {
            if (RHS.root == nullptr) {
                root = nullptr;
            }
            else {
                CopyTree(tempRoot, RHS.root);
                Destroy(root);
                root = tempRoot;
            }
        }
    }
    catch (std::bad_alloc error) {
        Destroy(tempRoot);
        std::cerr << "Exception caught, Could not allocate memory. Copy Failed, original tree is intact " << error.what() << "\n";
    }
    return *this;
}

BinarySearchTree::BinarySearchTree(const BinarySearchTree& otherTree) {
    try {
        if (otherTree.root == nullptr) {
            root = nullptr;
        }
        else {
            CopyTree(root, otherTree.root);
        }
    }
    catch (std::bad_alloc error) {
        Destroy(root);
        root = nullptr;
        std::cerr << "Exception caught, Could not allocate memory. Copy Failed " << error.what() << "\n";
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
        
        Node* current = nullptr;
		Node* trailCurrent = nullptr;
		Node* newNode;

		newNode = new Node;
		newNode->word = insertWord;
		newNode->count = 1;
		newNode->leftLink = nullptr;
		newNode->rightLink = nullptr;

        bool wordInTree = false;

		if (root == nullptr) {
			root = newNode;
		}
		else
		{
			current = root;
			while (current != nullptr && !wordInTree)
			{
				trailCurrent = current;
                if (current->word == insertWord) {
                    current->count += 1;
                    wordInTree = true;
                }
				else if (current->word > insertWord) {
					current = current->leftLink;
				}
				else {
					current = current->rightLink;
				}
			}
            if (!wordInTree) {
                if (trailCurrent->word > insertWord) {
                    trailCurrent->leftLink = newNode;
                }
                else {
                    trailCurrent->rightLink = newNode;
                }
            }
		}
}


//MAIN FUNCTIONS

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

void ProcessInputFile(string FileName) {
    std::fstream ioFile{ FileName, std::ios::in | std::ios::out };

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

void BuildBST(BinarySearchTree& BST) {
    BST.DestroyTree();
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
        BST.InsertWord(word);
    }
    inFile.close();
}

void BuildAndPrintBST(BinarySearchTree& BST, string header){
    try {
        BuildBST(BST);
        cout << header;
        BST.TraverseInorder();
    }
    catch (const std::bad_alloc error) {
        std::cerr << "Exception caught, Could not allocate memory. Destroying Tree " << error.what() << "\n";
        BST.DestroyTree();
    }
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