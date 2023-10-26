#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>
using namespace std;

const int LETTER_OFFSET_IN_ASCII = 32, MIN_WORD_LEN = 4, SETW_SIZE = 15;

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

    int GetNodeCount() const { return NodeCount(root); }

    int GetLeavesCount() const { return LeavesCount(root); }

    void DestroyTree();

    bool SearchTree(string word);

    void InsertWord(string word);

    void DeleteNode(string word);

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

    int NodeCount(Node* p) const;

    int LeavesCount(Node* p) const;

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
void ProcessInputFile(fstream& ioFile, BinarySearchTree& BST);


int main()
{
    cout << left;
    std::fstream ioFile{ "Test_Data.txt", std::ios::in | std::ios::out };

    
    if (!ioFile)
    {
        std::cerr << "Test_Data.txt could not be opened\n";
        return 1;
    }

    stringstream header;
    header << left << setw(SETW_SIZE) << "WORD" << "COUNT" << "\n";
    cout << header.str();

    BinarySearchTree BST;
    
    ProcessInputFile(ioFile, BST);

    cout << "Inorder Traversal: \n";
    BST.InorderTraversal();

    cout << "Preorder Traversal: \n";
    BST.PreorderTraversal();

    cout << "Postorder Traversal: \n";
    BST.PostorderTraversal();

    ioFile.close();
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
            if (current->word == insertWord)
            {   
                //ask if should return
                return;
            }
            else if (current->word > insertWord)
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

void ProcessInputFile(fstream& ioFile, BinarySearchTree& BST) {
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
        if (!BST.SearchTree(word) && word != "") {
            BST.InsertWord(word);
        }
    }
}