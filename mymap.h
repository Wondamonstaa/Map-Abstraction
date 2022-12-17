#pragma once
#include <iostream>
#include <sstream>
#include <utility>
#include <vector>
using namespace std;

template<typename keyType, typename valueType>
class mymap {
private:
    struct NODE {
        keyType key;  // used to build BST
        valueType value;  // stored data for the map
        NODE* left;  // links to left child
        NODE* right;  // links to right child
        int nL;  // number of nodes in left subtree
        int nR;  // number of nodes in right subtree
        bool isThreaded;
    };
    NODE* root;  // pointer to root node of the BST
    int size;  // # of key/value pairs in the mymap

    //
    // iterator:
    // This iterator is used so that mymap will work with a foreach loop.
    //
    struct iterator {
    private:
        NODE* curr;  // points to current in-order node for begin/end

    public:
        iterator(NODE* node) {
            curr = node;
        }

        keyType operator *() {
            return curr -> key;
        }

        bool operator ==(const iterator& rhs) {
            return curr == rhs.curr;
        }

        bool operator !=(const iterator& rhs) {
            return curr != rhs.curr;
        }

        bool isDefault() {
            return !curr;
        }


        //Step 1: create operator++() =>  advances curr to the next in-order node.
        iterator operator++() {

            /*
             Inorder Successor of a node in BST is the next node in in-order traversal of the BST.
             Inorder Successor is NULL for the last node in Inorder traversal.
             */

            //Instantiate a new node
            NODE* current = this -> curr;

            //Instantiate a pointer to the previous node
            NODE* previous = new NODE;

            cycle:
            if (current -> right != nullptr) {
                //If the current node has a child to its right => set it
                current = current -> right;
                //Check if the current node has a left child
                if (current -> left != nullptr) {
                    //if it does, then set the current node to the left child
                    current = current -> left;
                }
            }
            else{
                previous = current;
                //See if current has a left child
                if (current -> left != previous) {
                    current = current -> left;
                }
            }
            try{
                if(curr -> isThreaded == true){
                    previous = curr;
                    (this -> curr) = this -> curr -> right;
                }
                else{
                    throw (curr -> isThreaded);
                }
            }
            catch(bool e){
                if(curr -> right != nullptr){
                    (this -> curr) = this -> curr -> right;
                }
                else if(this -> curr -> left != nullptr){
                    while(this -> curr -> left != nullptr){
                        (this -> curr) = this -> curr -> left;
                    }
                }
                else{
                    goto cycle;
                }
            }

            //Return the iterator
            return iterator(curr);
        }
    };


public:

    //Step 2: create the default constructor => DONE
    mymap(){

        //Set size to 0 and make root empty
        this -> size = 0;
        this -> root = nullptr;

    }

    //Step 4: recursive pre order search helper function (@Zybooks + Lab)
    void preOrderMyMap(NODE* target){

        //Sanity check: see if the target is empty
        if(target == nullptr){
            return;
        }
        else{
            //cout << target -> key << " " << target -> value << endl;
            preOrderMyMap(target -> left);
            if(target -> isThreaded == false){
                preOrderMyMap(target -> right);
            }
            else{
                return;
            }
            //Pass the target == other's root + value to the put function
            this -> put(target -> key, target -> value);
        }
    }

    /*Step 3: create the copy constructor => constructs a new mymap which is a copy of the "other" mymap.
     Sets all member variables appropriately.*/
    mymap(const mymap& other){

        //Instantiate a new node
        this -> root = new NODE();
        this -> root -> key = other.root -> key;
        this -> root -> value = other.root -> value;
        size = 1;

        preOrderMyMap(other.root);
    }

    //Step 5: create the operator=() => clears "this" mymap and then makes a copy of the "other" mymap => DONE
    mymap& operator=(const mymap& other){

        //FIXME:
        if(this == &other){
            return *this;
        }
        else{
            this -> clear();
            /*this -> root = new NODE();
            this -> root -> key = other.root -> key;
            this -> root -> value = other.root -> value;
            this -> size = 1;
            preOrderMyMap(other.root);*/
            mymap();
        }

        //Clear this
        /*this -> clear();

        //Make a copy of the other mymap
        this = new NODE();
        this -> root -> key = other.root -> key;
        this -> root -> value = other.root -> value;
        this -> size = 1;*/

        return *this;
    }

    //Helper function for the to clear the mymap
    void clearRecursive(NODE* target){


        if(target == nullptr){
            return;
        }
        else{
            clearRecursive(target -> left);
            if(target -> isThreaded == false){
                clearRecursive(target -> right);
                this -> size--;
                delete target;
            }
            else{
                return;
            }
        }
    }

    //Step 6: create the clear() function => frees the memory => DONE
    void clear() {

        //FIXME:
        clearRecursive(this -> root);

        this -> root = nullptr;
    }

    //Step 7: create ~mymap() destructor => frees the memory associated with the mymap => DONE
    ~mymap(){

        /*this -> root = 0;
         this -> left = 0;
         this -> right = 0;
         this -> size = 0;*/

        //Time complexity: O(n)
        this -> clear();
        this -> size = 0;
    }

    //Step 8: insert a new key/value pair into the mymap @Zybooks + Lectures => FIXME
    void insertionAlgorithm(NODE* target, keyType key, valueType value){

        //Case 1: the key is less than the target's key
        if(key < target -> key){
            //Check if the target does not have a left child
            if(target -> left == nullptr){
                target -> left = new NODE();
                target -> left -> right = target;
                target -> left -> isThreaded = true;
                this -> size++;
            }
            else{
                insertionAlgorithm(target -> left, key, value);
            }
        }
        //Case 2: check if the key is greater than the target's key
        else if(key > target -> key){
            if(target -> right == nullptr){
                target -> right = new NODE();
                target -> nR = 0;
                target -> nL = 0;
                target -> isThreaded = true;
                this -> size++;
            }
            else{
                insertionAlgorithm(target -> right, key, value);
            }
        }
        else{
            target -> value = value;
        }
    }

    //Step 9: create the put() function => inserts the key/value into the threaded, self-balancing BST based on the key => FIXME
    void put(keyType key, valueType value){

        /*
        Notes:
        1. All nodes = leaves;
        2. If(key1 == key2){
            update value;
        }
        3. prev -> link;
        4. size++;
        */

        //Step 1: create a new node
        NODE* current = new NODE();
        current -> key = key;
        current -> value = value;
        current -> isThreaded = false;

        insertionAlgorithm(current, key, value);
        //Step 2: implement basic BST search algorithm
        if(this -> root == nullptr){
            this -> root = current;
        }
        else{
            NODE* prev = nullptr;
            NODE* curr = this -> root;
            while(curr != nullptr){
                prev = curr;
                if(current -> key < curr -> key){
                    if(curr -> left == nullptr){
                        curr -> left = current;
                        curr = nullptr;
                    }
                    else{
                        curr = curr -> left;
                    }
                }
                else{
                    if(curr -> right == nullptr){
                        curr -> right = current;
                        curr = nullptr;
                    }
                    else{
                        curr = curr -> right;
                    }
                }
            }
        }

        //Step 3: increment size
        this -> size++;
    }

    //Step 10: create contains() => returns true if the key is in mymap, return false if not => DONE
    bool contains(keyType key){

        //Instantiate the new node
        NODE* current = this -> root;

        //Check if the current node is not empty
        while(current != nullptr){

            //Check if the main condition is satisfied => return true
            if(key == current -> key){
                return true;
            }
            //If key < the current node => must be to the left
            else if(key < current -> key){
                current = current -> left;
            }
            //If key > the current node => must be to the right
            else if(key > current -> key){
                  if(current -> isThreaded == true){
                        return false;
                  }
                  else{
                      current = current -> right;
                  }
            }
            else{
                return false;
            }
        }

        return false;
    }

    /*Step 11: create get() => returns the value for the given key;
     if the key is not found, the default value, valueType(), is returned (but not added to mymap).*/
    valueType get(keyType key) {

        //Instantiate the new node
        NODE* current = this -> root;
        int counter = 0;

        //Check if the current is empty
        while(current != nullptr){
            //No match => proceed to the next node
            if(key < current -> key){
                counter++;
                current = current -> left;
            }
                //If the match was found => simply return the value
            else if(key == current -> key){
                return current -> value;
            }
            else if(key > current -> key){
                counter++;
                if(current -> isThreaded){
                    current = nullptr;
                }
                else{
                    current = current -> right;
                }
            }
            else{
                if(counter == 0){
                    return valueType();
                }
            }
        }

        return valueType();
    }

    //Helper function for the operator[] => traverses the mymap recursively @Zybooks + Lectures
    void recursiveTraverse(NODE* target, keyType key, NODE* &prev, NODE* &next){

        NODE* current = target;
        NODE* previous = nullptr;

        if(current == nullptr){
            return;
        }
        else{
           while(current != nullptr){
               if(key == current -> key){
                     return;
               }
               else if(key < current -> key){
                   prev = current;
                   current = current -> left;
               }
               else if(key > current -> key){
                     if(current -> isThreaded == true){
                          return;
                     }
                     else{
                         prev = current;
                         current = current -> right;
                     }
               }
               else{
                    return;
               }
           }
        }
    }

    /*Step 12: implement operator[] => returns the value for the given key;
    if the key is not found, the default value, valueType(), is returned,
    and the resulting new key/value pair is inserted into the map => DONE */
    valueType operator[](keyType key){

        /*
         a. Time complexity: O(logn + mlogm):
            1. n = total number of nodes in the threaded, self-balancing BST;
            2. m = the number of nodes in the sub-trees that need to be re-balanced.
         b. Space complexity: O(1)
         */

        //Instantiate two nodes => current + previous
        NODE* current = this -> root;
        NODE* previous = nullptr;
        NODE* next = this -> root -> right;

        recursiveTraverse(current, key, previous, next);
        if(current != nullptr){
            //Check if there is a match
            if(key == current -> key){
                //The match was found => return the value
                return current -> value;
            }
            else{
                //The key was not found => return valueType()
                this -> put(key, valueType());
                return valueType();
            }
        }
        else{
            //The key was not found => return valueType()
            this -> put(key, valueType());
            return valueType();
        }
    }

    //Step 13: implement Size() => returns the # of key/value pairs in the mymap, 0 if empty => DONE
    int Size(){

        /*if(this -> size > 0){
            return this -> size;
        }
        else{
            return 0;
        }*/

        //Time complexity: O1
        return this -> size;
    }

    //Step 14: implement begin() => returns an iterator to the first in order NODE => DONE @Project 2
    iterator begin(){

        NODE* target = this -> root;
        bool choice;

        if(target == nullptr){
            return iterator(nullptr);
        }
        else{
            while(target -> left != nullptr){
                if(target == nullptr){
                    return iterator(target);
                }
                else{
                    target = target -> left;
                }
            }
        }
        return iterator(target);
    }

    //Step 15: create iterator end() => returns an iterator to the last in order NODE => DONE (@Professor)
    iterator end(){
        return iterator(nullptr);
    }

    //Helper function for toString() => in-order traversal using recursion => DONE @Zybooks
    void toStringTraversal(stringstream &output, NODE* current){

        //Sanity check: if current is not empty
        if(current == nullptr) {
            return;
        }
        else{
            toStringTraversal(output, current -> left);
            //Display the value of the current key
            output << "key: " << current -> key << " " << "value: " << current -> value << endl;
            //Check if the current node is threaded or not
            if(current -> isThreaded == false){
                toStringTraversal(output, current -> right);
            }
            else{
                return;
            }
        }
    }

    //Step 16: create toString() => returns a string of the entire mymap, in order => DONE @Previous project by me
    string toString() {

        /*
         Input:
         8/80, 15/150, 20/200:

         Output:
         "key: 8 value: 80
          key: 15 value: 150
          key: 20 value: 200

         Time complexity: O(n)
         */

        //Create a stringstream object for creating a string from numerical values
        stringstream output;

        //Instantiate a new node
        NODE* current = this -> root;

        //Call the helper function
        toStringTraversal(output, current);

        //Using str(), I assign data for the stringstream buffer
        string result = output.str();

        //Return the result
        return output.str();
    }

    //Helper function to check if the current node is threaded or not => DONE @Lecture
    void threadChecker(NODE* current, vector<pair<keyType, valueType> > &holder ){

            //Check if the current node is not threaded
            if(current -> isThreaded == false){
                preOrderVectorTraversal(holder, current -> right);
            }
            else{
                return;
            }
            /*while(curr != nullptr){
                if(curr -> isThreaded == true){
                    //cout << "The Node is Threaded!" << endl;
                }
                else{
                    //cout << "The Node is Not Threaded!" << endl;
                }
                curr = curr -> right;
            }*/
    }

    //Helper function to begin recursive pre-order traversal of the BST for the vector @Zybooks => DONE
    vector<pair<keyType, valueType> > preOrderVectorTraversal(vector<pair<keyType, valueType> > &holder, NODE* current){

        //Check if the current node is empty
        if(current == nullptr){
            return holder;
        }
        else{
            //Traverse the left sub-tree
            preOrderVectorTraversal(holder, current -> left);
            //Update the vector
            holder.push_back(make_pair(current -> key, current -> value));
            //Check if the current node is not threaded
            threadChecker(current, holder);
            return holder;
        }

        return holder;
    }

    //Step N: create toVector() => Returns a vector of the entire map, in order => DONE
    vector<pair<keyType, valueType> > toVector(){

        /*
         Input:
         8/80, 15/150, 20/200

         Output:
         {{8, 80}, {15, 150}, {20, 200}}

         Time complexity: O(n)
         */

        //Instantiate a new node
        NODE* current = this -> root;

        //Create a container to store the entire map inside the vector => USE PAIR TYPE (TA)
        vector<pair<keyType, valueType> > holder;

        //Fill the vector with the new contents and return the result
        holder = preOrderVectorTraversal(holder, current);

        return holder;
    }

    //Helper function to begin recursive pre-order traversal of the BST @Lecture by Professor Koehler => DONE
    void preOrderTraversal(NODE* current, stringstream& output){

        //Check if the current node is empty
        if(current == nullptr){
            return;
        }

        output << "key: " << current -> key << ", " << "nL: " << current -> nL
               << ", " << "nR: " << current -> nR << endl;

        //Begin the recursive call in accordance with the pre-order algorithm (@ZyBooks)
        preOrderTraversal(current -> right, output);

        if(current -> isThreaded == false){
            preOrderTraversal(current -> right, output);
        }
        else{
            return;
        }
    }

    //Step 17: returns a string of mymap that verifies that the tree is properly balanced => DONE
    string checkBalance(){

        /*
         Keys:
         1, 2, 3

         Pre-order:
         "key: 2, nL: 1, nR: 1
          key: 1, nL: 0, nR: 0
          key: 3, nL: 0, nR: 0";

         Time complexity: O(n)

         THREADED => if false - traverse again to the right!
         */

        //Create a stringstream object for creating a string from numerical values
        stringstream output;

        //Instantiate a new node
        NODE* current = this -> root;
        //NODE* current = new NODE();
        //current = this -> root;

        //Begin the recursive call in accordance with the pre-order algorithm (@ZyBooks)
        preOrderTraversal(current, output);

        //Using str(), convert stringstream buffer to string
        string result = output.str();

        return result;
    }
};
