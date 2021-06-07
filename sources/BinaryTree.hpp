#pragma one
#include <memory>
#include <stack>
#include <iostream>

namespace ariel
{
    enum iterator_type {preorder, inorder, postorder};
    template <typename T> class BinaryTree {
    
        /*-----------------------------------
           ***** INNER CLASS NODE *****
        ------------------------------------*/       
        class Node { 
            friend class BinaryTree; // So we can access the variables in the class
            T _key;
            Node *_left;
            Node *_right;
            Node *_parent;

        public:
            T get_key(){return _key;}
            Node(T val) : _key(val), _right(nullptr), _left(nullptr), _parent(nullptr){};
            bool operator==(Node other){
                bool A = this->_parent == other->_parent;
                bool B = this->_right == other->_right;
                bool C = this->_left == other->_left;
                bool D = this->_key == other->_key;
                return (A && B && C && D);
            }
        };
        
        /*-----------------------------------
           ***** INNER CLASS ITERATOR *****
        ------------------------------------*/
        class Iterator{

        Node* _curr;
        iterator_type _order;
        std::stack<Node*> stack;

        public:
            
            Iterator(const iterator_type order_by, Node* node=nullptr) : _curr(node) ,_order(order_by){
                init(node);
                if (!stack.empty()){
                    _curr = stack.top();
                    stack.pop();
                }
            }

            Node *get_curr(){return _curr;}
            Iterator operator++(int){
                Iterator temp = *this;
                if (stack.empty()){
                    _curr = nullptr;
                }
                else{
                    _curr = stack.top();
                    stack.pop();
                }
                return temp;
            }
            Iterator &operator++(){
                if (stack.empty()) {_curr = nullptr;}
                else{
                    _curr = stack.top(); 
                    stack.pop();
                }
                return *this;
            }
            T &operator*() const {return _curr->_key;}
            T *operator->() const{ return &(_curr->_key);}
            bool operator==(const Iterator &other) const{return _curr == other._curr;}
            bool operator!=(const Iterator &other) const{return !(*this == other);}

            void init(Node *node){ //Insert the vertices of the tree into the stack in the desired order
                if (node == nullptr) {return;}
                if(_order == preorder){
                    init(node->_right);
                    init(node->_left);
                    stack.push(node);
                }
                else if (_order == inorder){
                    init(node->_right);
                    stack.push(node);
                    init(node->_left);
                }
                else {
                    stack.push(node);
                    init(node->_right);
                    init(node->_left);
                }
            }

        };
    
    /*-----------------------------------------------------------
        ********************* TREE *********************
    -------------------------------------------------------------*/

    Node *root;

    /*-----------------------------------
        ***** PRIVATE FUNCTIONS *****
    ------------------------------------*/

    /* private function to find node in the tree */
    Node* find(Node* n ,T v) { 
        if (n == nullptr) {return nullptr;}
        for (auto it = begin_inorder(); it != end_inorder(); ++it){
            if (*it == v){return it.get_curr();}
        }
        return nullptr;
    }

    /* private function for copy constractur */
    void copy(Node *n, const Node *other){ 
        if (other->_left != nullptr){
            n->_left = new Node(other->_left->_key);
            copy(n->_left, other->_left);
        }
        if (other->_right != nullptr){
            n->_right = new Node(other->_right->_key);
            copy(n->_right, other->_right);
        }
    }


    public:
        BinaryTree<T>() : root(nullptr){};
        
        BinaryTree<T>(const BinaryTree<T> &other){ 
            if (other.root != nullptr){
                this->root = new Node(other.root->_key);
                copy(root, other.root);
            }
        }
        BinaryTree(BinaryTree<T> &&other) noexcept {
            this->root = other.root;
            other.root = nullptr;
        }

        BinaryTree<T> &operator=(const BinaryTree<T> &other)
        {
            if (this == &other){return *this;}
            if (root != nullptr){delete root;}
            if (other.root != nullptr){
                root = new Node{other.root->_key};
                copy(root, other.root);
            }
            return *this;
        }

        
        BinaryTree<T> &operator=(BinaryTree<T> &&other) noexcept {
            if (root){delete root;}
            root = other.root;
            other.root = nullptr;
            return *this;
        }

       
        ~BinaryTree<T>()
        {
            if (root){
                for (auto it = (*this).begin_postorder(); it != (*this).end_postorder(); ++it){
                    delete it.get_curr();
                }
            }
        }

        
        BinaryTree& add_root(const T& r);
        BinaryTree& add_right(const T& p, const T& c);
        BinaryTree& add_left(const T& p, const T& c);

        Iterator begin(){return Iterator{inorder,root};}
        Iterator end(){return Iterator{inorder,nullptr};}
        Iterator begin_preorder(){return Iterator{preorder,root};}
        Iterator end_preorder(){return Iterator{preorder,nullptr};}
        Iterator begin_inorder(){return Iterator{inorder, root};}
        Iterator end_inorder(){return Iterator{inorder, nullptr};}
        Iterator begin_postorder(){return Iterator{postorder, root};}
        Iterator end_postorder(){return Iterator{postorder, nullptr};}

        friend std::ostream &operator<<(std::ostream &os, const BinaryTree<T> &t){
            os << t.root->_key;
            return os;
        }
       
    };

    /*---------------------------------------------------------------
        ********************* IMPLEMENTATIONS *****************
    -----------------------------------------------------------------*/

    template<typename T> BinaryTree<T>& BinaryTree<T>::add_root(const T& r) {
        if (root != nullptr){root->_key = r;}
        else {this->root = new Node(r);}
        return *this;
    }

    template<typename T> BinaryTree<T>& BinaryTree<T>::add_right(const T& p, const T& c) {
        Node* found = find(this->root, p);
        if (found == nullptr){throw std::invalid_argument("Parent not found\n");} // parent doesn't exist
        if (found->_right == nullptr){ //The parent does not have a right child
            found->_right = new Node(c);
            found->_right->_parent = found; // Updating the child's parent
        }
        else {found->_right->_key = c;} //He has child
        return *this;
    }

    template<typename T> BinaryTree<T>& BinaryTree<T>::add_left(const T& p, const T& c) {
        Node *found = find(this->root, p);
        if (found == nullptr){throw std::invalid_argument("Parent not found\n");} // parent doesn't exist
        if (found->_left == nullptr){//The parent does not have a left child
            found->_left = new Node(c);
            found->_left->_parent = found; // // Updating the child's parent
        }
        else{found->_left->_key = c;} // If he has a child
        return *this;
    }
}