// Mini E-commerce - 7 Labs, C++17. Console demo; no real payments.
#include <algorithm>
#include <chrono>
#include <cmath>
#include <deque>
#include <iomanip>
#include <iostream>
#include <limits>
#include <list>
#include <memory>
#include <numeric>
#include <queue>
#include <random>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;
struct Product { int id; string name; double price; int stock; double rating; };
void show(const Product& p){cout<<p.id<<" | "<<p.name<<" | "<<fixed<<setprecision(0)<<p.price<<" VND | stock "<<p.stock<<" | rating "<<setprecision(1)<<p.rating<<'\n';}
int inputInt(const string& msg){int x;for(;;){cout<<msg;if(cin>>x)return x;cin.clear();cin.ignore(numeric_limits<streamsize>::max(),'\n');cout<<"Invalid number.\n";}}
string inputText(const string& msg){cout<<msg;string s;getline(cin>>ws,s);return s;}
template<class F> double timed(F f){auto a=chrono::steady_clock::now();f();return chrono::duration<double,milli>(chrono::steady_clock::now()-a).count();}
// Lab 2: actual singly linked list with owned nodes.
class SinglyList {
 struct Node{Product p;unique_ptr<Node> next;explicit Node(Product x):p(move(x)){} };unique_ptr<Node> head;
 public:
 void insertHead(Product p){auto n=make_unique<Node>(move(p));n->next=move(head);head=move(n);}
 void insertTail(Product p){auto* link=&head;while(*link)link=&((*link)->next);*link=make_unique<Node>(move(p));}
 bool erase(int id,Product* out=nullptr){auto* link=&head;while(*link&&(*link)->p.id!=id)link=&((*link)->next);if(!*link)return false;if(out)*out=(*link)->p;*link=move((*link)->next);return true;}
 Product* find(int id){for(auto* n=head.get();n;n=n->next.get())if(n->p.id==id)return &n->p;return nullptr;}
 vector<Product> values()const{vector<Product> v;for(auto* n=head.get();n;n=n->next.get())v.push_back(n->p);return v;}
 void clear(){while(head)head=move(head->next);}
};
// Lab 4: pointer-based BST; insertion, removal, search, all traversals.
class BST {
 struct Node{Product p;unique_ptr<Node> left,right;explicit Node(Product x):p(move(x)){} };unique_ptr<Node> root;
 static void insert(unique_ptr<Node>& n,const Product& p){if(!n)n=make_unique<Node>(p);else if(p.id<n->p.id)insert(n->left,p);else if(p.id>n->p.id)insert(n->right,p);else n->p=p;}
 static bool erase(unique_ptr<Node>& n,int id){if(!n)return false;if(id<n->p.id)return erase(n->left,id);if(id>n->p.id)return erase(n->right,id);if(!n->left){n=move(n->right);return true;}if(!n->right){n=move(n->left);return true;}Node* successor=n->right.get();while(successor->left)successor=successor->left.get();n->p=successor->p;return erase(n->right,successor->p.id);}
 static void walk(const Node* n,int mode,vector<Product>& out){if(!n)return;if(mode==0)out.push_back(n->p);walk(n->left.get(),mode,out);if(mode==1)out.push_back(n->p);walk(n->right.get(),mode,out);if(mode==2)out.push_back(n->p);}
 public:
 void insert(const Product& p){insert(root,p);}bool erase(int id){return erase(root,id);}
 const Product* find(int id)const{auto* n=root.get();while(n){if(id==n->p.id)return &n->p;n=(id<n->p.id?n->left:n->right).get();}return nullptr;}
 vector<Product> traverse(int mode)const{vector<Product> v;walk(root.get(),mode,v);return v;}
};
// Lab 5: five explicit sorting algorithms, configurable comparator.
template<class C>void insertionSort(vector<Product>& a,C less){for(size_t i=1;i<a.size();++i){auto x=a[i];size_t j=i;while(j&&less(x,a[j-1])){a[j]=a[j-1];--j;}a[j]=x;}}
template<class C>void selectionSort(vector<Product>& a,C less){for(size_t i=0;i<a.size();++i){size_t k=i;for(size_t j=i+1;j<a.size();++j)if(less(a[j],a[k]))k=j;swap(a[i],a[k]);}}
template<class C>void bubbleSort(vector<Product>& a,C less){for(size_t end=a.size();end>1;--end){bool changed=false;for(size_t j=1;j<end;++j)if(less(a[j],a[j-1])){swap(a[j],a[j-1]);changed=true;}if(!changed)break;}}
template<class C>void quickSort(vector<Product>& a,int lo,int hi,C less){while(lo<hi){int i=lo,j=hi;Product pivot=a[lo+(hi-lo)/2];while(i<=j){while(less(a[i],pivot))++i;while(less(pivot,a[j]))--j;if(i<=j){swap(a[i],a[j]);++i;--j;}}if(j-lo<hi-i){if(lo<j)quickSort(a,lo,j,less);lo=i;}else{if(i<hi)quickSort(a,i,hi,less);hi=j;}}}
template<class C>void mergeSort(vector<Product>& a,vector<Product>& temp,size_t l,size_t r,C less){if(r-l<2)return;size_t m=(l+r)/2;mergeSort(a,temp,l,m,less);mergeSort(a,temp,m,r,less);size_t i=l,j=m,k=l;while(i<m&&j<r)temp[k++]=less(a[j],a[i])?a[j++]:a[i++];while(i<m)temp[k++]=a[i++];while(j<r)temp[k++]=a[j++];for(size_t x=l;x<r;++x)a[x]=temp[x];}
template<class C>void sortBy(vector<Product>& a,int alg,C less){if(alg==1)insertionSort(a,less);else if(alg==2)selectionSort(a,less);else if(alg==3)bubbleSort(a,less);else if(alg==4){if(!a.empty())quickSort(a,0,(int)a.size()-1,less);}else{vector<Product> tmp(a.size());mergeSort(a,tmp,0,a.size(),less);}}
// Lab 6: hash table with separate chaining via linked lists.
class NameHash {static constexpr size_t CAP=1009;vector<list<Product>> buckets=vector<list<Product>>(CAP);
 size_t hashName(const string& s)const{size_t h=0;for(unsigned char c:s)h=(h*131+c)%CAP;return h;}
 public:void rebuild(const vector<Product>& v){for(auto& b:buckets)b.clear();for(const auto& p:v)buckets[hashName(p.name)].push_back(p);}
 vector<Product> exact(const string& name)const{vector<Product> out;for(const auto& p:buckets[hashName(name)])if(p.name==name)out.push_back(p);return out;}
};
class History {list<string> queries;public:void access(const string& q){queries.remove(q);queries.push_front(q);if(queries.size()>10)queries.pop_back();}void print()const{cout<<"Recent searches (Move-to-Front): ";for(const auto& q:queries)cout<<'['<<q<<"] ";cout<<'\n';}};
struct Order {int id;string customer;vector<Product> items;double total;};
class Shop {
 vector<Product> array;SinglyList inventory,cart;BST tree;NameHash hash;History history;stack<Product> deletedItems;queue<Order> orders;int nextId=1,nextOrder=1;
 void refresh(){tree=BST();auto v=inventory.values();mt19937 rng(42);shuffle(v.begin(),v.end(),rng);for(const auto& p:v)tree.insert(p);hash.rebuild(v);}
 public:
 Shop(){generate(100);}
 void generate(int n){array.clear();inventory.clear();cart.clear();while(!deletedItems.empty())deletedItems.pop();while(!orders.empty())orders.pop();nextId=1;mt19937 rng(42);uniform_int_distribution<int> price(10,5000),stock(1,99),rating(10,50);for(int i=0;i<n;++i){Product p{nextId++,"Product_"+to_string(i+1),double(price(rng))*1000,stock(rng),rating(rng)/10.0};array.push_back(p);inventory.insertTail(p);}refresh();cout<<"Generated "<<n<<" products in static-array demo and linked inventory.\n";}
 void lab1(){cout<<"Lab 1: ADT, array, sequential search, Big-O\n";vector<Product> a(100000);for(int i=0;i<100000;++i)a[i]={i+1,"Item_"+to_string(i+1),1000,10,4};auto find=[&](int id){for(const auto& p:a)if(p.id==id)return p.id;return -1;};int found=0;double best=timed([&]{found=find(1);});double worst=timed([&]{found=find(-1);});cout<<"100000 records; first ID: "<<best<<" ms; missing ID: "<<worst<<" ms\nBest O(1), worst O(n). Polynomial-time problem (P).\n";}
 void manage(){for(;;){cout<<"\nInventory: 1 list 2 add 3 delete 4 head insert 0 back\n";int c=inputInt("Choice: ");if(!c)return;if(c==1){auto v=inventory.values();cout<<"Total "<<v.size()<<"\n";for(size_t i=0;i<min<size_t>(v.size(),15);++i)show(v[i]);}else if(c==2||c==4){string name=inputText("Name: ");int price=inputInt("Price VND: "),stock=inputInt("Stock: ");if(name.empty()||price<0||stock<0){cout<<"Invalid product.\n";continue;}Product p{nextId++,name,double(price),stock,5};if(c==4)inventory.insertHead(p);else inventory.insertTail(p);refresh();cout<<"Added ID "<<p.id<<"\n";}else if(c==3){int id=inputInt("ID: ");cout<<(inventory.erase(id)?"Deleted":"Not found")<<'\n';refresh();}}}
 void cartMenu(){for(;;){cout<<"\nCart: 1 add 2 remove 3 undo 4 show 0 back\n";int c=inputInt("Choice: ");if(!c)return;if(c==1){int id=inputInt("Product ID: ");auto* p=inventory.find(id);if(p&&p->stock>0){cart.insertTail(*p);cout<<"Added to cart.\n";}else cout<<"Not found or out of stock.\n";}else if(c==2){int id=inputInt("Product ID: ");Product p;if(cart.erase(id,&p)){deletedItems.push(p);cout<<"Removed; saved on LIFO stack.\n";}else cout<<"Not in cart.\n";}else if(c==3){if(deletedItems.empty())cout<<"Stack empty.\n";else{cart.insertTail(deletedItems.top());deletedItems.pop();cout<<"Undo complete.\n";}}else if(c==4){double total=0;for(auto& p:cart.values()){show(p);total+=p.price;}cout<<"Total: "<<total<<" VND\n";}}}
 void checkout(){auto items=cart.values();if(items.empty()){cout<<"Cart empty.\n";return;}for(auto& p:items){auto* inv=inventory.find(p.id);if(!inv||inv->stock<=0){cout<<"Insufficient stock for ID "<<p.id<<'\n';return;}}for(auto& p:items)--inventory.find(p.id)->stock;double total=0;for(auto& p:items)total+=p.price;orders.push({nextOrder++,inputText("Customer: "),items,total});cart.clear();refresh();cout<<"Order enqueued. Queue size "<<orders.size()<<'\n';}
 void queueMenu(){for(;;){cout<<"\nOrders: 1 checkout cart 2 enqueue 5 mock orders 3 process next 4 process all 0 back\n";int c=inputInt("Choice: ");if(!c)return;if(c==1)checkout();else if(c==2){for(int i=0;i<5;++i)orders.push({nextOrder++,"Mock_customer_"+to_string(i+1),{},0});cout<<"5 orders enqueued.\n";}else if(c==3||c==4){if(orders.empty())cout<<"Queue empty.\n";while(!orders.empty()){auto o=orders.front();orders.pop();cout<<"FIFO processed #"<<o.id<<" for "<<o.customer<<" total "<<o.total<<'\n';if(c==3)break;}}}}
 void bstMenu(){int id=inputInt("Search Product ID: ");const Product* found=nullptr;double bstTime=timed([&]{found=tree.find(id);});if(found)show(*found);else cout<<"Not found.\n";auto v=inventory.values();const Product* linear=nullptr;double linearTime=timed([&]{for(auto& p:v)if(p.id==id){linear=&p;break;}});cout<<"BST "<<bstTime<<" ms vs linear "<<linearTime<<" ms\n";for(int mode=0;mode<3;++mode){auto a=tree.traverse(mode);cout<<(mode==0?"Preorder":mode==1?"Inorder":"Postorder")<<": ";for(size_t i=0;i<min<size_t>(a.size(),8);++i)cout<<a[i].id<<' ';cout<<'\n';}cout<<"BST average O(log n), worst O(n) if skewed.\n";}
 void sortMenu(){int alg=inputInt("Algorithm 1 Insertion 2 Selection 3 Bubble 4 Quick 5 Merge: ");if(alg<1||alg>5)return;int field=inputInt("Field 1 Price 2 Rating: ");int descending=inputInt("Descending? 1 yes / 0 no: ");auto less=[=](const Product& a,const Product& b){double x=field==2?a.rating:a.price,y=field==2?b.rating:b.price;return x==y?a.id<b.id:(descending?x>y:x<y);};auto v=inventory.values();if(alg<=3&&v.size()>3000){cout<<"Basic O(n^2) sort limited to first 3000 records for responsiveness.\n";v.resize(3000);}double ms=timed([&]{sortBy(v,alg,less);});cout<<"Sorted "<<v.size()<<" records in "<<ms<<" ms\n";for(size_t i=0;i<min<size_t>(v.size(),12);++i)show(v[i]);}
 void searchMenu(){string q=inputText("Keyword / exact name: ");history.access(q);vector<Product> seq;double ms=timed([&]{for(auto& p:inventory.values())if(p.name.find(q)!=string::npos)seq.push_back(p);});cout<<"Sequential substring: "<<seq.size()<<" matches, "<<ms<<" ms\n";for(size_t i=0;i<min<size_t>(seq.size(),5);++i)show(seq[i]);vector<Product> exact;ms=timed([&]{exact=hash.exact(q);});cout<<"Hash exact-name with chaining: "<<exact.size()<<" matches, "<<ms<<" ms\n";int price=inputInt("Exact price for binary search (-1 skip): ");if(price>=0){auto v=inventory.values();sort(v.begin(),v.end(),[](const Product&a,const Product&b){return a.price<b.price;});auto range=equal_range(v.begin(),v.end(),double(price),[](const auto& a,const auto& b){if constexpr(is_same_v<decay_t<decltype(a)>,Product>)return a.price<b;else return a<b.price;});cout<<"Binary price search matches: "<<distance(range.first,range.second)<<'\n';}history.print();}
 void benchmark(){cout<<"Lab 5 benchmark: Bubble vs Quick on the SAME 100000-product dataset.\n";mt19937 rng(123);uniform_int_distribution<int> d(1,1000000);vector<Product> original;original.reserve(100000);for(int i=0;i<100000;++i)original.push_back({i+1,"P",double(d(rng)),1,4});auto less=[](const Product&a,const Product&b){return a.price<b.price;};auto q=original;double quick=timed([&]{sortBy(q,4,less);});cout<<"QuickSort 100000: "<<quick<<" ms\n";cout<<"BubbleSort 100000 is O(n^2) and may take a long time.\n";int ok=inputInt("Run full BubbleSort? 1 yes / 0 no: ");if(ok==1){auto b=original;double bubble=timed([&]{sortBy(b,3,less);});cout<<"BubbleSort 100000: "<<bubble<<" ms; identical sorted order: "<<boolalpha<<equal(b.begin(),b.end(),q.begin(),[](auto&a,auto&b){return a.price==b.price;})<<'\n';}}
 void selfTest(){cout<<"Lab 7 tests:\n";SinglyList l;l.insertHead({2,"B",2,1,4});l.insertTail({3,"C",3,1,4});cout<<"List insert/remove: "<<(l.erase(2)&&l.find(2)==nullptr?"PASS":"FAIL")<<'\n';BST t;t.insert({2,"B",2,1,4});t.insert({1,"A",1,1,4});t.insert({3,"C",3,1,4});auto v=t.traverse(1);cout<<"BST inorder/delete: "<<(v.size()==3&&v[0].id==1&&t.erase(2)&&!t.find(2)?"PASS":"FAIL")<<'\n';NameHash h;h.rebuild(v);cout<<"Hash exact: "<<(!h.exact("A").empty()&&h.exact("missing").empty()?"PASS":"FAIL")<<'\n';stack<int>s;s.push(1);s.push(2);queue<int>q;q.push(1);q.push(2);cout<<"LIFO/FIFO: "<<(s.top()==2&&q.front()==1?"PASS":"FAIL")<<'\n';}
 void run(){for(;;){cout<<"\n==== MINI E-COMMERCE C++ / LAB 1-7 ====\n1 Lab1 array benchmark\n2 Inventory linked list\n3 Cart + undo stack\n4 Checkout FIFO queue\n5 BST search/traversals\n6 Sort/filter algorithms\n7 Search + hash + history\n8 Lab5 100k benchmark\n9 Lab7 integration tests\n10 Generate new inventory\n0 Exit\n";int c=inputInt("Choice: ");switch(c){case 0:return;case 1:lab1();break;case 2:manage();break;case 3:cartMenu();break;case 4:queueMenu();break;case 5:bstMenu();break;case 6:sortMenu();break;case 7:searchMenu();break;case 8:benchmark();break;case 9:selfTest();break;case 10:{int n=inputInt("Product count (1..100000): ");if(n>0&&n<=100000)generate(n);break;}default:cout<<"Unknown choice.\n";}}}
};
int main(){ios::sync_with_stdio(false);cin.tie(nullptr);Shop app;app.run();}
