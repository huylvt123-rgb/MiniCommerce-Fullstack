/*
 * MINI E-COMMERCE CORE ENGINE - BAN VIET HOA CO CHU THICH
 * Module 1: Danh sach lien ket doi quan ly kho
 * Module 2: Stack hoan tac, Queue xu ly don hang (mo phong)
 * Module 3: Cay BST tim theo ID va in tang dan
 * Module 4: Selection Sort, QuickSort va do thoi gian
 * Module 5: Hash Table tim theo ten, lich su Move-To-Front
 *
 * LUU Y: Chuc nang gio hang va thanh toan chi la mo phong,
 * khong phai he thong don hang/thanh toan thuc te.
 * Ten bien va ten ham giu tieng Anh de de doi chieu voi tai lieu C++.
 * Chu thich va giao dien dung tieng Viet khong dau de tuong thich terminal.
 */
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <random>
#include <cmath>

using namespace std;

// ==========================================
// CAU TRUC SAN PHAM: luu ma, ten, gia, ton kho va danh gia
// ==========================================
struct Product {
    int id;
    string name;
    double price;
    int stock;
    double rating;

    void print() const {
        cout << "| ID: " << setw(6) << id 
             << " | Ten: " << setw(20) << left << name 
             << " | Gia: " << setw(8) << fixed << setprecision(2) << price 
             << " | Ton kho: " << setw(5) << stock 
             << " | Danh gia: " << fixed << setprecision(1) << rating << " |\n";
    }
};

// ==========================================
// MODULE 1: DANH SACH LIEN KET DOI - QUAN LY KHO HANG
// ==========================================
struct ListNode {
    Product data;
    ListNode* next;
    ListNode* prev;
    ListNode(const Product& p) : data(p), next(nullptr), prev(nullptr) {}
};

class DynamicInventory {
private:
    ListNode* head;
    ListNode* tail;
    size_t count;

public:
    DynamicInventory() : head(nullptr), tail(nullptr), count(0) {}

    ~DynamicInventory() {
        clear();
    }

    // Duyet tung nut, giai phong bo nho va dat danh sach ve rong.
    void clear() {
        ListNode* curr = head;
        while (curr) {
            ListNode* nxt = curr->next;
            delete curr;
            curr = nxt;
        }
        head = tail = nullptr;
        count = 0;
    }

    // Chen san pham vao dau danh sach: O(1).
    void insertHead(const Product& p) {
        ListNode* node = new ListNode(p);
        if (!head) {
            head = tail = node;
        } else {
            node->next = head;
            head->prev = node;
            head = node;
        }
        count++;
    }

    // Chen san pham vao cuoi danh sach: O(1).
    void insertTail(const Product& p) {
        ListNode* node = new ListNode(p);
        if (!tail) {
            head = tail = node;
        } else {
            tail->next = node;
            node->prev = tail;
            tail = node;
        }
        count++;
    }

    // Tim va xoa theo ma: O(n); cap nhat ca lien ket truoc/sau.
    bool deleteByID(int id) {
        ListNode* curr = head;
        while (curr) {
            if (curr->data.id == id) {
                if (curr->prev) curr->prev->next = curr->next;
                else head = curr->next;

                if (curr->next) curr->next->prev = curr->prev;
                else tail = curr->prev;

                delete curr;
                count--;
                return true;
            }
            curr = curr->next;
        }
        return false;
    }

    ListNode* getHead() const { return head; }
    size_t size() const { return count; }
};

// ==========================================
// MODULE 2: NGAN XEP (LIFO) VA HANG DOI (FIFO)
// ==========================================
class CartStack {
private:
    struct StackNode {
        Product item;
        StackNode* next;
        StackNode(const Product& p) : item(p), next(nullptr) {}
    };
    StackNode* topNode;

public:
    CartStack() : topNode(nullptr) {}
    ~CartStack() {
        while (!isEmpty()) pop();
    }

    bool isEmpty() const { return topNode == nullptr; }

    // PUSH: dua phan tu moi len dinh ngan xep (LIFO).
    void push(const Product& p) {
        StackNode* node = new StackNode(p);
        node->next = topNode;
        topNode = node;
    }

    // POP: lay phan tu vua duoc them gan nhat.
    bool pop(Product& outProd) {
        if (isEmpty()) return false;
        StackNode* temp = topNode;
        outProd = temp->item;
        topNode = topNode->next;
        delete temp;
        return true;
    }

    void pop() {
        if (isEmpty()) return;
        StackNode* temp = topNode;
        topNode = topNode->next;
        delete temp;
    }
};

class CheckoutQueue {
private:
    struct QueueNode {
        Product item;
        QueueNode* next;
        QueueNode(const Product& p) : item(p), next(nullptr) {}
    };
    QueueNode* frontNode;
    QueueNode* rearNode;

public:
    CheckoutQueue() : frontNode(nullptr), rearNode(nullptr) {}
    ~CheckoutQueue() {
        while (!isEmpty()) dequeue();
    }

    bool isEmpty() const { return frontNode == nullptr; }

    // ENQUEUE: them vao cuoi hang doi (FIFO).
    void enqueue(const Product& p) {
        QueueNode* node = new QueueNode(p);
        if (!rearNode) {
            frontNode = rearNode = node;
        } else {
            rearNode->next = node;
            rearNode = node;
        }
    }

    // DEQUEUE: lay phan tu o dau hang doi.
    bool dequeue(Product& outProd) {
        if (isEmpty()) return false;
        QueueNode* temp = frontNode;
        outProd = temp->item;
        frontNode = frontNode->next;
        if (!frontNode) rearNode = nullptr;
        delete temp;
        return true;
    }

    void dequeue() {
        if (isEmpty()) return;
        QueueNode* temp = frontNode;
        frontNode = frontNode->next;
        if (!frontNode) rearNode = nullptr;
        delete temp;
    }
};

// ==========================================
// MODULE 3: CAY TIM KIEM NHI PHAN (BST) THEO MA SAN PHAM
// ==========================================
struct BSTNode {
    Product data;
    BSTNode* left;
    BSTNode* right;
    BSTNode(const Product& p) : data(p), left(nullptr), right(nullptr) {}
};

class ProductBST {
private:
    BSTNode* root;

    // Ma nho hon di sang trai, lon hon di sang phai.
    BSTNode* insertRec(BSTNode* node, const Product& p) {
        if (!node) return new BSTNode(p);
        if (p.id < node->data.id) node->left = insertRec(node->left, p);
        else if (p.id > node->data.id) node->right = insertRec(node->right, p);
        return node;
    }

    BSTNode* searchRec(BSTNode* node, int id) const {
        if (!node || node->data.id == id) return node;
        if (id < node->data.id) return searchRec(node->left, id);
        return searchRec(node->right, id);
    }

    // Duyet trai - goc - phai de in ID tang dan.
    void inOrderRec(BSTNode* node, int& limit) const {
        if (!node || limit <= 0) return;
        inOrderRec(node->left, limit);
        if (limit > 0) {
            node->data.print();
            limit--;
        }
        inOrderRec(node->right, limit);
    }

    // Duyet hau tu: xoa hai cay con truoc khi xoa nut cha.
    void clearRec(BSTNode* node) {
        if (!node) return;
        clearRec(node->left);
        clearRec(node->right);
        delete node;
    }

public:
    ProductBST() : root(nullptr) {}
    void clear() { clearRec(root); root = nullptr; }
    ~ProductBST() { clear(); }

    void insert(const Product& p) { root = insertRec(root, p); }
    
    Product* search(int id) const {
        BSTNode* res = searchRec(root, id);
        return res ? &(res->data) : nullptr;
    }

    void printInOrder(int limit = 10) const {
        cout << "--- " << limit << " san pham dau tien (ID tang dan) ---\n";
        inOrderRec(root, limit);
    }
};

// ==========================================
// MODULE 5: BANG BAM VA DANH SACH LICH SU TU TO CHUC
// ==========================================
class HashTable {
private:
    static const int TABLE_SIZE = 100003;
    struct HashNode {
        Product data;
        HashNode* next;
        HashNode(const Product& p) : data(p), next(nullptr) {}
    };
    HashNode** table;

    // Chuyen chuoi ten thanh chi so o trong bang bam.
    int hashFunc(const string& key) const {
        unsigned long long hashVal = 0;
        unsigned long long p = 31;
        for (unsigned char c : key) {
            hashVal = (hashVal * p + c) % TABLE_SIZE;
        }
        return hashVal;
    }

public:
    HashTable() {
        table = new HashNode*[TABLE_SIZE]();
    }

    // Giai phong toan bo danh sach lien ket trong moi o bam.
    void clear() {
        for (int i = 0; i < TABLE_SIZE; ++i) {
            HashNode* curr = table[i];
            while (curr) {
                HashNode* nxt = curr->next;
                delete curr;
                curr = nxt;
            }
            table[i] = nullptr;
        }
    }
    ~HashTable() { clear(); delete[] table; }

    // Xu ly dung do bang danh sach lien ket (separate chaining).
    void insert(const Product& p) {
        int idx = hashFunc(p.name);
        HashNode* node = new HashNode(p);
        node->next = table[idx];
        table[idx] = node;
    }

    Product* searchByName(const string& name) const {
        int idx = hashFunc(name);
        HashNode* curr = table[idx];
        while (curr) {
            if (curr->data.name == name) return &(curr->data);
            curr = curr->next;
        }
        return nullptr;
    }
};

// Lich su tim kiem: truy van vua duoc dung se chuyen len dau (Move-To-Front).
class SearchHistoryList {
private:
    struct HistoryNode {
        string query;
        HistoryNode* next;
        HistoryNode(const string& q) : query(q), next(nullptr) {}
    };
    HistoryNode* head;

public:
    SearchHistoryList() : head(nullptr) {}
    ~SearchHistoryList() {
        while (head) {
            HistoryNode* tmp = head;
            head = head->next;
            delete tmp;
        }
    }

    // Neu tu khoa da co: dua len dau; neu chua co: tao nut moi.
    void accessQuery(const string& query) {
        HistoryNode* curr = head;
        HistoryNode* prev = nullptr;

        while (curr) {
            if (curr->query == query) {
                if (prev) {
                    prev->next = curr->next;
                    curr->next = head;
                    head = curr;
                }
                return;
            }
            prev = curr;
            curr = curr->next;
        }

        HistoryNode* node = new HistoryNode(query);
        node->next = head;
        head = node;
    }

    void displayHistory() const {
        cout << "Lich su tim kiem (moi nhat truoc): ";
        HistoryNode* curr = head;
        while (curr) {
            cout << "[" << curr->query << "] -> ";
            curr = curr->next;
        }
        cout << "NULL\n";
    }
};

// ==========================================
// MODULE 4: SO SANH THUAT TOAN SAP XEP THEO GIA
// ==========================================
// Selection Sort: moi luot chon san pham co gia nho nhat; O(n^2).
void selectionSort(vector<Product>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        int minIdx = i;
        for (int j = i + 1; j < n; ++j) {
            if (arr[j].price < arr[minIdx].price) minIdx = j;
        }
        swap(arr[i], arr[minIdx]);
    }
}

// Chia mang quanh pivot (gia san pham cuoi).
int partitionQuick(vector<Product>& arr, int low, int high) {
    double pivot = arr[high].price;
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        if (arr[j].price < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

// QuickSort: chia de tri, trung binh O(n log n).
void quickSort(vector<Product>& arr, int low, int high) {
    if (low < high) {
        int pi = partitionQuick(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// ==========================================
// CHUONG TRINH CHINH - GIAO DIEN DONG LENH
// ==========================================
// Tai tao chi muc BST va Hash Table tu kho hang de du lieu dong bo.
// Chen vao BST theo thu tu tron ngau nhien de tranh cay lech khi ID tang dan.
void rebuildIndexes(DynamicInventory& inv, ProductBST& bst, HashTable& ht) {
    bst.clear();
    ht.clear();
    vector<Product> items;
    for (ListNode* cur = inv.getHead(); cur; cur = cur->next) {
        items.push_back(cur->data);
        ht.insert(cur->data);
    }
    mt19937 rng(random_device{}());
    shuffle(items.begin(), items.end(), rng);
    for (const Product& p : items) bst.insert(p);
}

// Kiem tra ID co ton tai trong kho hay chua.
bool existsID(const DynamicInventory& inv, int id) {
    for (ListNode* cur = inv.getHead(); cur; cur = cur->next)
        if (cur->data.id == id) return true;
    return false;
}

// Tao du lieu gia lap; xoa kho cu de tranh lap ID khi nap nhieu lan.
void generateMockData(DynamicInventory& inv, ProductBST& bst, HashTable& ht, int count) {
    cout << "\nDang tao " << count << " san pham mau...\n";
    inv.clear();
    mt19937 rng(random_device{}());
    uniform_int_distribution<int> gia(1000, 99999), ton(1, 500), sao(10, 50);
    for (int i = 1; i <= count; ++i) {
        Product p{100000 + i, "SanPham_" + to_string(100000 + i),
                  gia(rng) / 100.0, ton(rng), sao(rng) / 10.0};
        inv.insertTail(p);
    }
    rebuildIndexes(inv, bst, ht);
    cout << "Da nap thanh cong " << count << " san pham!\n";
}

// Nhap so nguyen; xoa trang thai loi neu nguoi dung nhap chu.
int inputInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) return value;
        cout << "Gia tri khong hop le, vui long nhap so nguyen.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// Nhap so thuc co kiem tra.
double inputDouble(const string& prompt) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value && isfinite(value)) return value;
        cout << "Gia tri khong hop le, vui long nhap lai.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

int main() {
    DynamicInventory inventory;
    CartStack cartUndo;
    CheckoutQueue checkoutQueue;
    ProductBST bstIndex;
    HashTable searchHash;
    SearchHistoryList historyList;

    int choice = 0;
    while (true) {
        cout << "\n===================================================\n";
        cout << "    HE THONG THUONG MAI DIEN TU MINI (C++)        \n";
        cout << "===================================================\n";
        cout << "1. Nap du lieu mau (100.000 san pham)\n";
        cout << "2. Them san pham moi\n";
        cout << "3. Xoa san pham theo ID (Kho hang)\n";
        cout << "4. Mo phong gio hang va hoan tac (Stack LIFO)\n";
        cout << "5. Mo phong xu ly don hang (Queue FIFO)\n";
        cout << "6. Tim theo ten (Bang bam va lich su MTF)\n";
        cout << "7. So sanh Selection Sort va QuickSort\n";
        cout << "8. Hien thi 10 san pham theo ID (BST)\n";
        cout << "0. Thoat\n";
        cout << "Chon chuc nang: ";
        choice = inputInt("");

        if (choice == 0) {
            cout << "Dang thoat; bo nho duoc giai phong boi cac ham huy.\n";
            break;
        }

        switch (choice) {
        case 1: {
            generateMockData(inventory, bstIndex, searchHash, 100000);
            break;
        }
        case 2: {
            Product p;
            p.id = inputInt("Nhap ma san pham: ");
            if (existsID(inventory, p.id)) {
                cout << "Ma san pham da ton tai!\n";
                break;
            }
            cout << "Nhap ten san pham: ";
            cin >> ws;
            getline(cin, p.name);
            if (p.name.empty()) { cout << "Ten khong duoc de trong!\n"; break; }
            p.price = inputDouble("Nhap gia: ");
            p.stock = inputInt("Nhap so luong ton: ");
            p.rating = inputDouble("Nhap danh gia (1-5): ");
            if (p.id <= 0 || p.price < 0 || p.stock < 0 || p.rating < 1 || p.rating > 5) {
                cout << "Du lieu khong hop le!\n";
                break;
            }
            inventory.insertTail(p);
            bstIndex.insert(p);
            searchHash.insert(p);
            cout << "Them san pham thanh cong!\n";
            break;
        }
        case 3: {
            int id;
            id = inputInt("Nhap ID can xoa: ");
            if (inventory.deleteByID(id)) {
                rebuildIndexes(inventory, bstIndex, searchHash);
                cout << "Da xoa san pham va cap nhat chi muc!\n";
            }
            else cout << "Khong tim thay ID!\n";
            break;
        }
        case 4: {
            cout << "\n--- CART & UNDO MODULE ---\n";
            cout << "1. Xoa san pham khoi Gio hang (Dua vao Stack Undo)\n";
            cout << "2. Nhan Undo (Hoan tac phuc hoi san pham)\n";
            int c = inputInt("Chon thao tac: ");
            if (c == 1) {
                Product p = {101, "Dien thoai mau", 999.0, 10, 4.8};
                cartUndo.push(p);
                cout << "Da luu san pham mau vao ngan xep hoan tac.\n";
            } else if (c == 2) {
                Product restored;
                if (cartUndo.pop(restored)) {
                    cout << "Hoan tac thanh cong (mo phong): " << restored.name << endl;
                } else {
                    cout << "Ngan xep hoan tac dang rong!\n";
                }
            }
            break;
        }
        case 5: {
            cout << "\n--- FLASH SALE CHECKOUT QUEUE ---\n";
            Product p1 = {201, "May tinh xach tay", 1999.0, 5, 4.9};
            Product p2 = {202, "Tai nghe khong day", 249.0, 20, 4.7};
            checkoutQueue.enqueue(p1);
            checkoutQueue.enqueue(p2);
            cout << "Da dua 2 don hang mau vao hang doi.\n";

            Product outP;
            cout << "Dang xu ly thanh toan theo thu tu FIFO...\n";
            while (checkoutQueue.dequeue(outP)) {
                cout << " -> Da xu ly don hang mau: " << outP.name << " (gia: " << outP.price << ")\n";
            }
            break;
        }
        case 6: {
            string queryName;
            cout << "Nhap ten san pham tim kiem: "; cin >> ws; getline(cin, queryName);
            historyList.accessQuery(queryName);

            auto start = chrono::high_resolution_clock::now();
            Product* found = searchHash.searchByName(queryName);
            auto end = chrono::high_resolution_clock::now();
            chrono::duration<double, milli> elapsed = end - start;

            if (found) {
                cout << "Tim thay san pham trong bang bam (" << elapsed.count() << " ms):\n";
                found->print();
            } else {
                cout << "Khong tim thay san pham trong bang bam!\n";
            }
            historyList.displayHistory();
            break;
        }
        case 7: {
            cout << "\n--- BENCHMARK SORTING ALGORITHMS (N = 10.000) ---\n";
            vector<Product> data1, data2;
            ListNode* curr = inventory.getHead();
            int limit = 10000; 
            while (curr && limit--) {
                data1.push_back(curr->data);
                curr = curr->next;
            }
            data2 = data1;

            if (data1.empty()) {
                cout << "Kho hang trong! Hay nap du lieu hoac them san pham truoc.\n";
                break;
            }

            cout << "Dang chay Selection Sort tren " << data1.size() << " items...\n";
            auto t1 = chrono::high_resolution_clock::now();
            selectionSort(data1);
            auto t2 = chrono::high_resolution_clock::now();
            chrono::duration<double, milli> d1 = t2 - t1;
            cout << "Thoi gian Selection Sort: " << d1.count() << " ms\n";

            cout << "Dang chay QuickSort tren " << data2.size() << " items...\n";
            auto t3 = chrono::high_resolution_clock::now();
            quickSort(data2, 0, data2.size() - 1);
            auto t4 = chrono::high_resolution_clock::now();
            chrono::duration<double, milli> d2 = t4 - t3;
            cout << "Thoi gian QuickSort: " << d2.count() << " ms\n";

            if (d2.count() > 0)
                cout << "Ty le thoi gian Selection/Quick: " << d1.count() / d2.count() << " lan.\n";
            break;
        }
        case 8: {
            bstIndex.printInOrder(10);
            break;
        }
        default:
            cout << "Lua chon khong hop le!\n";
        }
    }
    return 0;
}