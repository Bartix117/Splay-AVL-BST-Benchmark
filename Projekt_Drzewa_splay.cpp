#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <chrono>
#include <random>
#include <stack>

using namespace std;
struct AVLNode
{
    int key;
    int height;
    AVLNode *left;
    AVLNode *right;

    AVLNode(int value)
    {
        key = value;
        left=nullptr;
        right =nullptr;
    }
};
int height(AVLNode* n)
{
    if (n == nullptr) return 0;
    return n->height;
}
int getBalance(AVLNode* n)
{
    if(n==nullptr) return 0;
    return height(n->left)-height(n->right);
}

AVLNode* rightRotate(AVLNode* r)
{
    AVLNode* x = r->left;
    AVLNode* y = x->right;
    x->right = r;
    r->left = y;
    r->height = max(height(r->left), height(r->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    return x;
}

AVLNode* leftRotate(AVLNode* r)
{
    AVLNode* x = r->right;
    AVLNode* y = x->left;
    x->left = r;
    r->right = y;
    r->height = max(height(r->left), height(r->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    return x;
}

AVLNode* insertAVL(AVLNode* node, int key)
{
    if (!node) return new AVLNode(key);
    if (key < node->key) node->left = insertAVL(node->left, key);
    else if (key > node->key) node->right = insertAVL(node->right, key);
    else return node;

    node->height = 1 + max(height(node->left), height(node->right));
    int balance = getBalance(node);

    // Lewo-Lewo
    if (balance> 1 && key < node->left->key) return rightRotate(node);
    // Prawo-Prawo
    if (balance< -1 && key > node->right->key) return leftRotate(node);
    // Lewo-prawo
    if (balance > 1&& key > node->left->key)
    {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    // Prawo-Lewo
    if (balance < -1 && key < node->right->key)
    {
        node->right =rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}

AVLNode* searchAVL(AVLNode* root, int key)
{
    while (root)
    {
        if (key ==root->key) return root; //znaleziono element
        if(key < root->key)
        {
            root = root->left; //idzie w lewo
        }
        else
        {
            root = root->right; // idzie w prawo
        }
    }
    return nullptr; //gdy nie ma elementu
}
struct Node
{
    int key;
    Node* left;
    Node* right;
    Node* parent;

    Node(int value)
    {
        key = value;
        left = nullptr;
        right = nullptr;
        parent = nullptr;
    }
};
void rotation(Node *r) //uogólniona funkcja Zig
{
    Node* p= r->parent;
    Node* g= p->parent;
    if(p->left==r) //rotacja w prawo
    {
        p->left = r->right;
        if(r->right)
            r->right->parent = p;
        r->right = p;
    }
    else//rotacja w lewo
    {
        p->right = r->left;
        if(r->left)
            r->left->parent = p;
        r->left = p;
    }
    r->parent=g;
    p->parent = r;
    if(g)
    {
        if (g->left == p) g->left = r;
        else g->right = r;
    }
};
void splay(Node *r,Node *& root)
{
    while (r->parent != nullptr)
    {
        Node* p = r->parent;
        Node* g = p->parent;
        if(g==nullptr)
        {
            rotation(r); //Przypadek Zig, gdy rodzic jest korzeniem
        }
        else
        {
            // jesli nie przypadek Zig, to sprawdzamy czy to Zig-Zig, czy Zig-Zag
            bool rLeft = (p->left == r);
            bool pLeft = (g->left == p);
            if (rLeft == pLeft)
            {
                // Zig-Zig: najpierw rotujemy rodzica
                rotation(p);
                rotation(r);
            }
            else
            {
                // Zig-Zag: dwa razy rotujemy r
                rotation(r);
                rotation(r);
            }
        }
    }
    root=r;
}
Node *Search(Node*& root, int num)
{
    if (root == nullptr) return nullptr;
    Node* current = root;
    Node* last_visited = nullptr;
    //szukanie wezla
    while(current!= nullptr)
    {
        last_visited=current;
        if(num == current->key)
        {
            splay(current,root); //jesli znaleziono - splay do korzenia
            return root;
        }
        if(num<current->key)
            current=current->left;
        else
            current=current->right;
    }
    //nie znaleziono - splay ostatniego wezla pomaga utrzymac strukture drzewa
    if(last_visited)
    {
        splay(last_visited,root);
    }
    return nullptr;
}
Node* SearchBST(Node* root, int num)
{
    Node* current = root;
    while (current != nullptr)
    {
        if (num == current->key) return current; // Znaleziono, ale nie splayujemy!
        if (num < current->key) current = current->left;
        else current = current->right;
    }
    return nullptr;
}
void AddBST(Node*& root, int key)
{
    // 1. Jeśli drzewo jest puste, stwórz korzeń
    if (root == nullptr)
        {
        root = new Node(key);
        return;
    }

    Node* current = root;

    while (true)
        {
        // Ignorowanie duplikatow (jezeli klucz już jest, nic nie robi)
        if (key == current->key)
            {
            return;
        }

        if (key < current->key)
            {
            if (current->left == nullptr)
            {
                // Znaleziono miejsce - wstawianie
                current->left = new Node(key);
                current->left->parent = current;
                break;
            }
            current = current->left;
        }
        else
        {
            if (current->right == nullptr)
            {
                current->right = new Node(key);
                current->right->parent = current;
                break;
            }
            current = current->right;
        }
    }
}
void Add(Node*& root, int num)
{
    //gdy drzewo jest puste
    if (root == nullptr)
    {
        root = new Node(num);
        return;
    }
    // Szukamy elementu (to przy okazji przesunie najblizszy wezel do korzenia)
    Search(root, num);
    // Jeœli Search znalazl element, to jest on juz w korzeniu i nic nie robimy
    if (root->key == num) return;
    // Jeœli nie znalazl, tworzymy nowy wezel i robimy go nowym korzeniem
    Node* newNode = new Node(num);
    if (num < root->key)
    {
        // Nowy wezel staje sie korzeniem, stary korzen staje sie jego prawym synem
        newNode->right = root;
        newNode->left = root->left;
        if (root->left) root->left->parent = newNode;
        root->left = nullptr;
        root->parent = newNode;
    }
    else
    {
        // Nowy wezel staje siê korzeniem, stary korzen staje sie jego lewym synem
        newNode->left = root;
        newNode->right = root->right;
        if (root->right) root->right->parent = newNode;
        root->right = nullptr;
        root->parent = newNode;
    }

    root = newNode;
}

void Myk(Node *r, int depth = 0)
{
    if (r == nullptr) return;
    Myk(r->right, depth + 1);
    for (int i = 0; i < depth; i++)
        cout << "   ";
    cout << r->key << endl;
    Myk(r->left, depth + 1);

}
int Height(Node* r)
{
    if (r == nullptr) return 0;
    int leftH = Height(r->left);
    int rightH = Height(r->right);
    return 1 + max(leftH, rightH);
}
void PrintInOrder(Node *root)
{
    if (root == nullptr) return;
    PrintInOrder(root->left);
    printf("%d ", root->key);
    PrintInOrder(root->right);
}
int losujZRozkladem(double alfa, int n = 1000000)
{
    // Generator liczb losowych
    static random_device rd;
    static mt19937 gen(rd());

    // 1. Losujemy decyzję: czy liczba < 100 (z prawdopodobieństwem alfa)
    bernoulli_distribution d_decyzja(alfa);

    if (d_decyzja(gen))
    {
        // 2. Losujemy z przedziału {1, ..., 99}
        uniform_int_distribution<int> dist_male(1, 99);
        return dist_male(gen);
    }
    else
    {
        // lub losujemy z przedziału {100, ..., n}
        uniform_int_distribution<int> dist_duze(100, n);
        return dist_duze(gen);
    }
}
int main()
{
    Node* root = nullptr;
    Node* rootBST = nullptr;
    AVLNode* rootAVL = nullptr;
    const int N = 1000000;

    // 1. Budowa drzewa - najpierw musimy mieć dane w drzewie
    vector<int> klucze(N);
    for(int i = 0; i < N; ++i) klucze[i] = i + 1;
    // Mieszamy klucze, żeby drzewo nie było od początku linią
    random_device rd;
    mt19937 g(rd());
    shuffle(klucze.begin(), klucze.end(), g);

    for(int i = 0; i < N; ++i)
    {
        Add(root, klucze[i]);
        AddBST(rootBST, klucze[i]);
        rootAVL = insertAVL(rootAVL, klucze[i]);
    }
    const long long LICZBA_TESTOW = 10000000; // 10^8 dla każdego alfa
    const int MAX_VAL = 2000000;

    // Tablica wartości alfa do przetestowania
    double wartosci_alfa[] = {0.01, 0.1, 0.25, 0.5, 0.75, 0.9, 0.99};

    cout << "Alfa, Czas calkowity, Sr. czas operacji (ns)" << endl;
    cout << "--------------------------------------------------------" << endl;

    for (double alfa : wartosci_alfa)
    {
        auto start = chrono::high_resolution_clock::now();

        for (long long i = 0; i < LICZBA_TESTOW; ++i)
        {
            int wylosowana;
            wylosowana = losujZRozkladem(alfa, MAX_VAL);
            Search(root, wylosowana);
        }
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsed = end - start;

        double sr_czas_ns = (elapsed.count() / LICZBA_TESTOW) * 1e9;

        cout <<"SPLAY"<<endl<<"Alfa: "<<alfa <<endl<<"Czas calkowity: "<<elapsed.count()<<" s"<<endl<<"Sr. czas operacji: "<< sr_czas_ns << " ns" << endl<<"------------"<<endl;
        auto startBST = chrono::high_resolution_clock::now();
        for (long long i = 0; i < LICZBA_TESTOW; ++i)
        {
            int klucz = losujZRozkladem(alfa, MAX_VAL);
            SearchBST(rootBST, klucz);
        }
        auto endBST = chrono::high_resolution_clock::now();
        elapsed = endBST - startBST;
        sr_czas_ns = (elapsed.count() / LICZBA_TESTOW) * 1e9;
        cout <<"BST"<<endl<<"Alfa: "<<alfa <<endl<<"Czas calkowity: "<<elapsed.count()<<" s"<<endl<<"Sr. czas operacji: "<< sr_czas_ns << " ns" << endl<<"------------"<<endl;
        auto startAVL = chrono::high_resolution_clock::now();
         for (long long i = 0; i < LICZBA_TESTOW; ++i)
        {
            searchAVL(rootAVL, losujZRozkladem(alfa, MAX_VAL));
        }
        auto endAVL = chrono::high_resolution_clock::now();
        elapsed = endAVL -startAVL;
        sr_czas_ns = (elapsed.count() / LICZBA_TESTOW) * 1e9;
        cout <<"AVL"<<endl<<"Alfa: "<<alfa <<endl<<"Czas calkowity: "<<elapsed.count()<<" s"<<endl<<"Sr. czas operacji: "<< sr_czas_ns << " ns" << endl<<"------------"<<endl;
    }


    return 0;

};


