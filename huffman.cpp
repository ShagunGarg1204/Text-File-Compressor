#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
#include <fstream> // For file handling
using namespace std;

// A Tree node
struct Node
{
    char ch;
    int freq;
    Node *left, *right;
};

// Function to allocate a new tree node
Node* getNode(char ch, int freq, Node* left, Node* right)
{
    Node* node = new Node();
    node->ch = ch;
    node->freq = freq;
    node->left = left;
    node->right = right;
    return node;
}

// Comparison object to be used to order the heap
struct comp
{
    bool operator()(Node* l, Node* r)
    {
        // highest priority item has lowest frequency
        return l->freq > r->freq;
    }
};

// Traverse the Huffman Tree and store Huffman Codes in a map
void encode(Node* root, string str, unordered_map<char, string> &huffmanCode)
{
    if (root == nullptr)
        return;

    // Found a leaf node
    if (!root->left && !root->right) {
        huffmanCode[root->ch] = str;
    }

    encode(root->left, str + "0", huffmanCode);
    encode(root->right, str + "1", huffmanCode);
}

// Traverse the Huffman Tree and decode the encoded string
void decode(Node* root, int &index, string str)
{
    if (root == nullptr) {
        return;
    }

    // Found a leaf node
    if (!root->left && !root->right)
    {
        cout << root->ch;
        return;
    }

    index++;

    if (str[index] == '0')
        decode(root->left, index, str);
    else
        decode(root->right, index, str);
}

// Builds Huffman Tree and decodes given input text
void buildHuffmanTree(string text)
{
    // Count frequency of appearance of each character
    // and store it in a map
    unordered_map<char, int> freq;
    for (char ch : text) {
        freq[ch]++;
    }

    // Create a priority queue to store live nodes of Huffman tree
    priority_queue<Node*, vector<Node*>, comp> pq;

    // Create a leaf node for each character and add it to the priority queue
    for (auto pair : freq) {
        pq.push(getNode(pair.first, pair.second, nullptr, nullptr));
    }

    // Do till there is more than one node in the queue
    while (pq.size() != 1)
    {
        // Remove the two nodes of highest priority (lowest frequency) from the queue
        Node *left = pq.top(); pq.pop();
        Node *right = pq.top(); pq.pop();

        // Create a new internal node with these two nodes as children
        // and with frequency equal to the sum of the two nodes' frequencies.
        // Add the new node to the priority queue.
        int sum = left->freq + right->freq;
        pq.push(getNode('\0', sum, left, right));
    }

    // Root stores pointer to root of Huffman Tree
    Node* root = pq.top();

    // Traverse the Huffman Tree and store Huffman Codes in a map
    unordered_map<char, string> huffmanCode;
    encode(root, "", huffmanCode);

    // Print Huffman Codes
    cout << "Huffman Codes are:\n";
    for (auto pair : huffmanCode) {
        cout << pair.first << " " << pair.second << '\n';
    }

    // Print original string
    cout << "\nOriginal string was:\n" << text << '\n';

    // Print encoded string
    string encodedString = "";
    for (char ch : text) {
        encodedString += huffmanCode[ch];
    }

    cout << "\nEncoded string is:\n" << encodedString << '\n';

    // Write the encoded string to the output file
    ofstream outFile("output.txt");
    if (outFile.is_open()) {
        outFile << encodedString;
        outFile.close();
        cout << "\nEncoded string has been written to 'output.txt'.\n";
    } else {
        cout << "\nError writing to output file.\n";
    }

    // Decode the encoded string
    cout << "\nDecoded string is:\n";
    int index = -1;
    while (index < (int)encodedString.size() - 2) {
        decode(root, index, encodedString);
    }
}

int main()
{
    // Read the input text from a file
    ifstream inputFile("input.txt");
    string text = "";
    
    if (inputFile.is_open()) {
        string line;
        while (getline(inputFile, line)) {
            if (!text.empty()) text += "\n"; // Preserve line breaks
            text += line;
        }
        inputFile.close();
    } else {
        cout << "Unable to open input file.\n";
        return 1;
    }

    // Build Huffman Tree and encode the text
    buildHuffmanTree(text);

    return 0;
}
