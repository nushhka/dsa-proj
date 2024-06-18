#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

class Lexicon;

int lex(string s1, string s2)
{
    string s11 = s1;
    string s22 = s2;
    // transform(s11.begin(), s11.end(), s11.begin(), ::tolower);
    // transform(s22.begin(), s22.end(), s22.begin(), ::tolower);

    if (s11 == s22)
        return 0;
    else if (s11 < s22)
        return -1;
    else
        return 1;
}

class IndexEntry; // forward decl

class HybridNode
{
public:
    string key;                     // Word
    string element;                 // Chapter
    HybridNode *parent = NULL;      // Parent node
    HybridNode *left_child = NULL;  // Left child node
    HybridNode *right_child = NULL; // Right child node
    HybridNode *next_node = NULL;   // Next node in the linked list
    string color = "black";         // Color of the node
    vector<pair<string, int>> histo = {};
    HybridNode(string key_val, string element_val) : key(key_val), element(element_val), parent(nullptr), left_child(nullptr), right_child(nullptr), next_node(nullptr) {}
};

class RedBlackTree
{
private:
    HybridNode *root = NULL; // Root node

public:
    RedBlackTree() : root(nullptr) {}

    HybridNode *getRoot()
    {
        return root; // Return the root node
    }

    void setRoot(HybridNode *node)
    {
        root = node; // Set the root node
    }

    HybridNode *getUncle(HybridNode *x)
    {
        HybridNode *gp = x->parent->parent;

        if (lex(gp->key, x->key) == 1)
        {
            return gp->right_child;
        }
        else
        {
            return gp->left_child;
        }
    }
    void rotateLeft(HybridNode *node)
    {

        HybridNode *par = node->parent;
        HybridNode *r = node->right_child;
        HybridNode *rl = r->left_child;
        bool isleft = 1;
        if (par != nullptr)
            isleft = (par->left_child == node);
        if (isleft)
        {
            if (par != nullptr)
                par->left_child = r;
            r->parent = par;
        }
        else
        {
            if (par != nullptr)
                par->right_child = r;
            r->parent = par;
        }
        if (node != NULL && r != nullptr)
            node->right_child = rl;
        if (r != nullptr && r->left_child != NULL)
            r->left_child->parent = node;
        if (r != nullptr)
            r->left_child = node;
        if (node != nullptr)
            node->parent = r;
        if (par == nullptr)
            this->root = r;

        return;
    }

    void rotateRight(HybridNode *node)
    {
        HybridNode *par = node->parent;
        HybridNode *l = node->left_child;
        HybridNode *lr = l->right_child;
        bool isleft = 1;
        if (par != nullptr)
            isleft = (par->left_child == node);

        if (isleft)
        {
            if (par != nullptr)
                par->left_child = l;
            l->parent = par;
        }
        else
        {
            if (par != nullptr)
                par->right_child = l;
            l->parent = par;
        }
        if (node != NULL && l != nullptr)
            node->left_child = lr;
        if (l != nullptr && l->right_child != NULL)
            l->right_child->parent = node;
        if (l != nullptr)
            l->right_child = node;
        if (node != nullptr)
            node->parent = l;
        if (par == nullptr)
            this->root = l;

        return;
    }

    HybridNode *insert(string key, string element, int num_ch, vector<string> ch_names)
    {
        // Implement Red-Black Tree insertion
        if (this->getRoot() == nullptr)
        {
            HybridNode *r = new HybridNode(key, element);
            this->root = r;
            r->histo.resize(ch_names.size());
            // cout << r->histo.size() << "\n" ;
            for (int j = 0; j < ch_names.size(); j++)
            {
                r->histo[j] = make_pair(ch_names[j], 0);
                // cout << r->histo[j].first << " " << r->histo[j].second << "\n" ;
            }
            // cout << r->histo[num_ch].second << "\n" ;
            r->histo[num_ch].second = r->histo[num_ch].second + 1;
            // cout << r->histo[num_ch].second << "\n" ;

            return r;
        }
        // going for searching in the rb tree and insert it like normal bst

        HybridNode *curr = this->getRoot();
        HybridNode *par = NULL;
        while (curr != nullptr)
        {
            if (lex(curr->key, key) == 0)
            {
                // update the count of this word in this chapter
                curr->histo[num_ch].second = curr->histo[num_ch].second + 1;
                // cout << curr->histo[num_ch].second << "\n" ;
                return curr;
            }
            else if (lex(curr->key, key) == 1)
            {
                par = curr;
                curr = curr->left_child;
            }
            else if (lex(curr->key, key) == -1)
            {
                par = curr;
                curr = curr->right_child;
            }
        }

        HybridNode *new_node = new HybridNode(key, element);
        new_node->histo.resize(ch_names.size());
        // cout << new_node->histo.size() << "\n" ;
        for (int j = 0; j < ch_names.size(); j++)
        {
            new_node->histo[j] = make_pair(ch_names[j], 0);
            // cout << new_node->histo[j].first << " " << new_node->histo[j].second << "\n" ;
        }
        // cout << new_node->histo[num_ch].second << "\n" ;
        new_node->histo[num_ch].second = new_node->histo[num_ch].second + 1;
        // cout << new_node->histo[num_ch].second << "\n" ;

        new_node->color = "red";
        bool isleft = lex(par->key, key) == 1;
        if (isleft)
        {
            par->left_child = new_node;
            new_node->parent = par;
        }
        else
        {
            par->right_child = new_node;
            new_node->parent = par;
        }

        //  recoloring and restrucring happen here
        HybridNode *x = new_node;
        while (x != nullptr)
        {
            if (x == this->root)
            {
                root->color = "black";
                return x;
            }

            if (x->parent->color == "black")
            {
                return x;
            }

            else if (getUncle(x) == NULL || getUncle(x)->color == "black")
            {
                // do rotations and recoloring before that
                if (lex(x->parent->parent->key, x->key) == 1 && lex(x->parent->key, x->key) == 1)
                {
                    // LL rotation
                    string temp_col = x->parent->parent->color;
                    x->parent->parent->color = x->parent->color;
                    x->parent->color = temp_col;
                    rotateRight(x->parent->parent);
                }
                else if (lex(x->parent->parent->key, x->key) == 1 && lex(x->parent->key, x->key) == -1)
                {
                    // lr rotation
                    rotateLeft(x->parent);
                    string temp_col = x->parent->color;
                    x->parent->color = x->color;
                    x->color = temp_col;
                    rotateRight(x->parent);
                }
                else if (lex(x->parent->parent->key, x->key) == -1 && lex(x->parent->key, x->key) == -1)
                {
                    // rr rotation
                    string temp_col = x->parent->parent->color;
                    x->parent->parent->color = x->parent->color;
                    x->parent->color = temp_col;
                    rotateLeft(x->parent->parent);
                }
                else if (lex(x->parent->parent->key, x->key) == -1 && lex(x->parent->key, x->key) == 1)
                {
                    // rl rotation
                    rotateRight(x->parent);
                    string temp_col = x->parent->color;
                    x->parent->color = x->color;
                    x->color = temp_col;
                    rotateLeft(x->parent);
                }

                return x;
            }
            else if (getUncle(x)->color == "red")
            {
                // do recolring
                x->parent->parent->color = "red";
                x->parent->color = "black";
                getUncle(x)->color = "black";
                x = x->parent->parent;
            }
        }

        return x;
    }

    HybridNode *successor(HybridNode *x)
    {
        HybridNode *temp = x;
        while (temp->left_child != NULL)
        {
            temp = temp->left_child;
        }

        return temp;
    }

    HybridNode *BSTreplace(HybridNode *x)
    {
        if (x->left_child != NULL && x->right_child != NULL)
        {
            return successor(x->right_child);
        }

        // when leaf ...contd

        if (x->left_child == NULL && x->right_child == NULL)
            return NULL;

        // when single child
        if (x->left_child != NULL)
            return x->left_child;
        else
            return x->right_child;
    }

    HybridNode *sibling(HybridNode *x)
    {
        HybridNode *par = x->parent;
        bool isleft = (par->left_child == x);

        if (isleft)
            return par->right_child;
        else
            return par->left_child;
    }
    bool hasRedChild(HybridNode *x)
    {
        return (x->left_child != NULL && x->left_child->color == "red") || (x->right_child != NULL && x->right_child->color == "red");
    }
    bool isOnLeft(HybridNode *x)
    {
        return (x->parent->left_child == x);
    }

    void fixDoubleBlack(HybridNode *x)
    {
        if (x == this->root)
            // Reached root
            return;

        HybridNode *sib = sibling(x);
        HybridNode *parent = x->parent;
        if (sib == NULL)
        {
            // No sibling, double black pushed up
            fixDoubleBlack(parent);
        }
        else
        {
            if (sib->color == "red")
            {
                // Sibling red
                parent->color = "red";
                sib->color = "black";
                if (sib == parent->left_child)
                {
                    // left case
                    rotateRight(parent);
                }
                else
                {
                    // right case
                    rotateLeft(parent);
                }
                fixDoubleBlack(x);
            }
            else
            {
                // Sibling black
                if (hasRedChild(sib))
                {
                    // at least 1 red children
                    if (sib->left_child != NULL && sib->left_child->color == "red")
                    {
                        if (isOnLeft(sib))
                        {
                            // left left
                            sib->left_child->color = sib->color;
                            sib->color = parent->color;
                            rotateRight(parent);
                        }
                        else
                        {
                            // right left
                            sib->left_child->color = parent->color;
                            rotateRight(sib);
                            rotateLeft(parent);
                        }
                    }
                    else
                    {
                        if (isOnLeft(sib))
                        {
                            // left right
                            sib->right_child->color = parent->color;
                            rotateLeft(sib);
                            rotateRight(parent);
                        }
                        else
                        {
                            // right right
                            sib->right_child->color = sib->color;
                            sib->color = parent->color;
                            rotateLeft(parent);
                        }
                    }
                    parent->color = "black";
                }
                else
                {
                    // 2 black children
                    sib->color = "red";
                    if (parent->color == "black")
                        fixDoubleBlack(parent);
                    else
                        parent->color = "black";
                }
            }
        }
    }

    void swapValues(HybridNode *u, HybridNode *v)
    {
        // swap all the data fields except colr , left , right and parent
        string temp;
        temp = u->key;
        u->key = v->key;
        v->key = temp;

        temp = u->element;
        u->element = v->element;
        v->element = temp;

        vector<pair<string, int>> temp_h = u->histo;
        u->histo = v->histo;
        v->histo = temp_h;
    }
    void assignVal(HybridNode *u, HybridNode *v)
    {
        // all the data except the colr , left , right and parent
        u->element = v->element;
        u->key = v->key;
        u->histo = v->histo;
    }

    void deletehelper(HybridNode *v)
    {
        HybridNode *u = BSTreplace(v); //..contd

        bool uvBlack = ((u == NULL || u->color == "black") && (v->color == "black"));
        HybridNode *parent = v->parent;

        if (u == NULL)
        {
            // u is NULL therefore v is leaf
            if (v == root)
            {
                // v is root, making root null
                root = NULL;
            }
            else
            {
                if (uvBlack)
                {
                    fixDoubleBlack(v); // contd..
                }
                else
                {
                    // u or v is red
                    if (sibling(v) != NULL)
                        sibling(v)->color = "red";
                }

                if (isOnLeft(v))
                {
                    parent->left_child = NULL;
                }
                else
                {
                    parent->right_child = NULL;
                }
            }

            delete v;
            return;
        }

        //..contd
        if (v->left_child == NULL || v->right_child == NULL)
        {
            // v has 1 child
            if (v == this->root)
            {
                // v is root, assign the value of u to v, and delete u
                assignVal(v, u);
                v->left_child = v->right_child = NULL;
                delete u;
            }
            else
            {
                // Detach v from tree and move u up
                if (isOnLeft(v))
                {
                    parent->left_child = u;
                }
                else
                {
                    parent->right_child = u;
                }
                delete v;
                u->parent = parent;
                if (uvBlack)
                {
                    // u and v both black, fix double black at u
                    fixDoubleBlack(u);
                }
                else
                {
                    // u or v red, color u black
                    u->color = "black";
                }
            }
            return;
        }

        swapValues(u, v);
        deletehelper(u);
    }

    bool deleteNode(string key)
    {
        // deleting by value
        if (this->root == NULL)
            return 0;

        HybridNode *v = this->search(key), *u;

        if (v == nullptr)
            return 0;

        deletehelper(v);

        return 1;
    }

    vector<HybridNode *> traverseUp(HybridNode *node)
    {
        // Traverse up the tree from the given node to the root
        HybridNode *curr = node;
        vector<HybridNode *> ans;

        while (curr != nullptr)
        {
            ans.push_back(curr);
            curr = curr->parent;
        }
        return ans;
        // Return the vector of nodes in the path
    }

    vector<HybridNode *> traverseDown(HybridNode *node, string bit_sequence)
    {
        // Traverse down the tree based on the bit sequence
        HybridNode *curr = node;
        vector<HybridNode *> ans;
        ans.push_back(node);
        for (auto i : bit_sequence)
        {
            if (i == '1')
            {
                ans.push_back(curr->left_child);
                curr = curr->left_child;
            }
            else if (i == '0')
            {
                ans.push_back(curr->right_child);
                curr = curr->right_child;
            }
        }
        return ans;
        // Return the vector of nodes in the path
    }

    void preOrderWithDepth(HybridNode *node, int depth, vector<HybridNode *> &ans)
    {
        if (node == nullptr || depth < 0)
            return;
        ans.push_back(node);
        if (depth > 0)
        {
            preOrderWithDepth(node->left_child, depth - 1, ans);
            preOrderWithDepth(node->right_child, depth - 1, ans);
        }
    }
    vector<HybridNode *> preOrderTraversal(HybridNode *node, int depth)
    {
        vector<HybridNode *> ans;
        preOrderWithDepth(node, depth, ans);
        return ans;
    }

    HybridNode *search(string key)
    {
        // Implement Red-Black Tree search
        HybridNode *curr = this->getRoot();
        while (curr != nullptr)
        {
            if (lex(curr->key, key) == 0)
            {
                return curr;
            }
            else if (lex(curr->key, key) == 1)
            {
                curr = curr->left_child;
            }
            else if (lex(curr->key, key) == -1)
            {
                curr = curr->right_child;
            }
        }
        return curr;
    }

    int blackheight(HybridNode *node)
    {
        // Implement blackheight
        HybridNode *curr = node;
        int bh = 0;
        while (curr != nullptr)
        {
            if (curr->color == "black")
                bh++;

            curr = curr->left_child;
        }
        return bh;
    }
};

class IndexEntry
{
private:
    string word;
    vector<pair<string, int>> chapter_word_counts; // List of (chapter, word_count) tuples

public:
    IndexEntry(string word_val) : word(word_val) {}

    void setWord(string word_val)
    { // Set the word
        word = word_val;
    }

    string getWord()
    { // Get the word
        return word;
    }

    void setChapterWordCounts(vector<pair<string, int>> chapter_word_counts_val)
    { // Set the list of (chapter, word_count) tuples
        chapter_word_counts = chapter_word_counts_val;
    }

    vector<pair<string, int>> getChapterWordCounts()
    { // Get the list of (chapter, word_count) tuples
        return chapter_word_counts;
    }
};

class Lexicon
{
private:
    RedBlackTree red_black_tree; // Red-Black Tree

public:
    Lexicon()
    {
    }

    vector<string> tokenizer(string para)
    {
        vector<string> tokens;

        for (int i = 0; i < para.size(); i++)
        {
            string token = "";
            while (i < para.size() && para[i] != ' ')
            {
                token += para[i++];
            }
            tokens.push_back(token);
        }
        return tokens;
    }

    void setRedBlackTree(RedBlackTree tree)
    { // Set the Red-Black Tree
        red_black_tree = tree;
    }

    RedBlackTree getRedBlackTree()
    { // Get the Red-Black Tree
        return red_black_tree;
    }

    void printTreeHelper(HybridNode *root, int space)
    {
        if (root != nullptr)
        {
            space += 10;
            printTreeHelper(root->right_child, space);
            std::cout << std::endl;
            for (int i = 10; i < space; i++)
                std::cout << " ";
            std::cout << root->key << " " << root->color << std::endl;
            printTreeHelper(root->left_child, space);
        }
    }

    void pruninghelper(HybridNode *curr, vector<string> &words)
    {
        if (curr == NULL)
            return;

        bool possible = 1;
        vector<pair<string, int>> his = curr->histo;
        for (int i = 0; i < his.size(); i++)
        {
            if (his[i].second >= 1)
                continue;

            possible = 0;
            break;
        }
        if (possible)
            words.push_back(curr->key);

        pruninghelper(curr->left_child, words);
        pruninghelper(curr->right_child, words);
    }
    void pruning()
    {
        // finding all such words which is common
        vector<string> words;
        pruninghelper(red_black_tree.getRoot(), words);
        // cout << words.size() << "\n" ;

        for (auto i : words)
        {
            // cout << i << "\n";
            red_black_tree.deleteNode(i);
        }
    }
    void readChapters(vector<string> chapter_names)
    {
        // Process words from a chapter and update the Red-Black Tree
        int num_ch = 0;
        int tot_ch = chapter_names.size();
        vector<string> ch_names;
        for (auto ch_n : chapter_names)
        {
            string ch_name = "";
            for (int i = 0; i < ch_n.size() - 4; i++)
                ch_name += ch_n[i];

            ch_names.push_back(ch_name);
        }
        for (auto ch : chapter_names)
        {

            fstream fin;
            fin.open(ch, ios::in);
            // read every line into para in that chap;
            string row = "";
            vector<string> words;
            while (getline(fin, row))
            {
                if (row.empty())
                    break;
                vector<string> tokens = tokenizer(row);
                words.insert(words.end(), tokens.begin(), tokens.end());
            }
            // after reading particular file add it to  rb tree
            string ch_name = "";
            for (int i = 0; i < ch.size() - 4; i++)
                ch_name += ch[i];

            // cout << ch_name << " ";

            for (auto i : words)
            {
                transform(i.begin(), i.end(), i.begin(), ::tolower);
                red_black_tree.insert(i, ch_name, num_ch, ch_names);
            }

            // printTreeHelper(red_black_tree.getRoot() , 0);
            fin.close();
            num_ch++;
        }

        pruning();

        // red_black_tree.deleteNode("i");
        // red_black_tree.deleteNode("pro");
        // red_black_tree.deleteNode("and");
        // cout << red_black_tree.deleteNode("his") << "\n";
        // red_black_tree.deleteNode("my");
        // printTreeHelper(red_black_tree.getRoot() , 0);
        //printtreehelper..
        // chapter_names is a vector of chapter names
    }

    void buildIndexhelper(HybridNode *curr, vector<IndexEntry> &ans)
    {
        if (curr == NULL)
            return;

        buildIndexhelper(curr->left_child, ans);

        // process to fill the ans
        string word = curr->key;
        vector<pair<string, int>> his = curr->histo;
        transform(word.begin(), word.end(), word.begin(), ::tolower);
        IndexEntry indx(word);
        indx.setChapterWordCounts(his);
        ans.push_back(indx);

        buildIndexhelper(curr->right_child, ans);
    }

    vector<IndexEntry> buildIndex()
    {
        // Build the index using the remaining words in the Red-Black Tree
        vector<IndexEntry> ans;

        // do in order and fill the ans vec
        buildIndexhelper(red_black_tree.getRoot(), ans);

        // do code here
        return ans;
    }
};
