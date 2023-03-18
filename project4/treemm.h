#ifndef TREEMULTIMAP_INCLUDED
#define TREEMULTIMAP_INCLUDED
#include <list>
#include <vector>

template <typename KeyType, typename ValueType>
class TreeMultimap {
private:
    //Node struct contains a key (name), a linkedlist, and left and right values
    struct Node {
        KeyType key;
        std::list<ValueType> values;
        Node* left;
        Node* right;
        Node(const KeyType& k, const ValueType& v):key(k), left(nullptr), right(nullptr) {
            values.push_back(v);
        }
    };
    
    Node* root;
    
    //used to clear TreeMultimap
    void clear(Node* n) {
        if (n == nullptr) return;
        clear(n->left);
        clear(n->right);
        delete n;
    }
    
    //find Node with a key
    Node* findNode(const KeyType& key) const {
        Node* curr = root;
        while (curr != nullptr) {
            if (key < curr->key) curr = curr->left;
            else if (key > curr->key) curr = curr->right;
            else return curr;
        }
        return nullptr;
    }
    
    void insertNode(Node*& n, const KeyType& key, const ValueType& value) {
        if (n == nullptr) {
            n = new Node(key, value);
            return;
        }
        else if (key < n->key)
            insertNode(n->left, key, value);
        else if (key > n->key)
            insertNode(n->right, key, value);
        else
            n->values.push_back(value);
    }
    
public:
    //iterator class iterates through a Node's list
    class Iterator {
    public:
        //default constructor
        Iterator() {
               itr = itrList.begin();
        }
        
        //constructor
        Iterator(std::list<ValueType> lst) {
            itrList = lst;
            itr = itrList.begin();
        }
        
        //simple accessor and mover methods
        ValueType& get_value() const {return *itr;}
        bool is_valid() const {return (itr != itrList.end());}
        void advance(){itr++;}

    private:
        typename std::list<ValueType>::iterator itr;
        std::list<ValueType> itrList;
    };
    
    /*
     TreeMultimap constructor and destructor
     */
    TreeMultimap():root(nullptr){}
    ~TreeMultimap() {
        clear(root);
    }
    
    void insert(const KeyType& key, const ValueType& value) {
        insertNode(root, key, value);
    }

    Iterator find(const KeyType& key) const {
        Node* n = root;
        while (n != nullptr) {
            if (n->key == key) return Iterator(n->values); //node found
            else if (n->key > key) n = n->left;
            else n = n->right;
        }
        return Iterator(); //node not found
    }
};
#endif //TREEMULTIMAP_INCLUDED
