/*
@author Hasan Masum
@created: 22 feb 2022
*/
#include <bits/stdc++.h>
using namespace std;

class Node
{
public:
    int value;
    Node *left;  // left child
    Node *right; // right child
    int height;
    Node(int value)
    {
        //cout << "new node created for value " << value << endl;

        this->value = value;
        left = nullptr;
        right = nullptr;
        height = 0;
    }
};

class AVLTree
{
private:
    Node *root; // root of the tree
public:
    AVLTree()
    {
        root = nullptr;
    }

    int calculateHeight(Node *parent)
    {
        int l = heightOfLeftTree(parent);
        int r = heightOfRightTree(parent);
        // cout<<parent->value<<" l: "<<l<<" , r: "<<r<<endl;
        return 1 + max(l, r);
    }

    // if a root doesn't exist then its size is -1
    int heightOfLeftTree(Node *parent)
    {
        return parent != nullptr && parent->left != nullptr ? parent->left->height : -1;
    }

    // if a root doesn't exist then its size is -1
    int heightOfRightTree(Node *parent)
    {
        return parent != nullptr && parent->right != nullptr ? parent->right->height : -1;
    }

    // left zig zig
    Node *leftLeftRotation(Node *parent)
    {
        // cout << "performing left left rotation on " << parent->value << endl;

        Node *X = parent;
        Node *Y = parent->left;

        // Y->right will become the left child of current parent X
        X->left = Y->right;
        X->height = calculateHeight(X); // re calculate the parent height

        // Y will become the new parent
        // and X will become the right child of Y
        Y->right = X;
        Y->height = calculateHeight(Y); // re calculate the  height

        return Y; // return the new parent after the rotation
    }

    // left zig zag
    Node *leftRightRotation(Node *parent)
    {
        // cout << "performing left right rotation on " << parent->value << endl;

        Node *X = parent;
        Node *Y = parent->left;        // leftChild
        Node *Z = parent->left->right; // elftRightChild

        // C->right child will be left child of A
        X->left = Z->right;
        X->height = calculateHeight(X); // re calculate height

        // Z->left will be the right child of Y
        Y->right = Z->left;
        Y->height = calculateHeight(Y); // re calculate the height

        // now Z will be the new parent
        // and Y will be the left child of Z
        // and X will be the right child of Z
        Z->left = Y;
        Z->right = X;
        Z->height = calculateHeight(Z);

        return Z; // return new parent after rotation
    }

    // right zig zig
    Node *rightLeftRotation(Node *parent)
    {
        // cout << "performing right left rotation on " << parent->value << endl;

        Node *X = parent;
        Node *Y = parent->right;       // rightChild
        Node *Z = parent->right->left; // rightLeftChild

        // Z->left child will be right child of X
        X->right = Z->left;
        X->height = calculateHeight(X); // re calculate height

        // Z->right will be the left child of Y
        Y->left = Z->right;
        Y->height = calculateHeight(Y); // re calculate the height

        // now Z will be the new parent
        // and Y will be the right child of Z
        // and X will be the left child of Z
        Z->left = X;
        Z->right = Y;
        Z->height = calculateHeight(Z);

        return Z; // return new parent after rotation
    }

    // right zig zag
    Node *rightRightRotation(Node *parent)
    {
        // cout << "performing right right rotation on " << parent->value << endl;

        Node *X = parent;
        Node *Y = parent->right;

        // Y->left will become the right child of current parent X
        X->right = Y->left;
        X->height = calculateHeight(X); // re calculate the parent height

        // Y will become the new parent
        // and X will become the left child of Y
        Y->left = X;
        Y->height = calculateHeight(Y); // re calculate the  height

        return Y; // return the new parent after the rotation
    }

    /**
     * @brief balance the parent if needed otherwise leave it as it it
     * 
     * @param parent 
     * @return Node* 
     */
    Node *balanceTreeIfNeeded(Node *parent, bool deletion = false)
    {
        int heightDiff = heightOfLeftTree(parent) - heightOfRightTree(parent);

        if (heightDiff == 2) // left 
        {
            cout << "Height invariant violated.\nAfter rebalancing: ";

            int leftChildHeightDiff = heightOfLeftTree(parent->left) - heightOfRightTree(parent->left);
            
            if (leftChildHeightDiff == 1)
            {
                return leftLeftRotation(parent);
            }
            else if (leftChildHeightDiff == -1)
            { // needs right and left double rotation
                return leftRightRotation(parent);
            }else if(deletion && leftChildHeightDiff == 0){ // for deletion only
                return leftLeftRotation(parent);
            }
        }
        else if (heightDiff == -2) // right 
        {
            cout << "Height invariant violated.\nAfter rebalancing: ";
            
            int rightChildHeightDiff = heightOfLeftTree(parent->right) - heightOfRightTree(parent->right);

            if (rightChildHeightDiff == 1)
            {
                return rightLeftRotation(parent);
            }
            else if (rightChildHeightDiff == -1)
            { // needs right and left double rotation
                return rightRightRotation(parent);
            }
            else if (deletion && rightChildHeightDiff == 0) // for deletion only
            {
                return rightRightRotation(parent);
            }
        }

        return parent;
    }
    
    void insert(int value)
    {
        // cout << "inserting: " << value << endl;
        root = insert(root, value);
        // cout << value << " inserted successfully." << endl;
    }

    /**
     * @brief recursive insert function
     *
     * @param parent
     * @param value
     */
    Node *insert(Node *parent, int value)
    {
        if (parent != nullptr && parent->value == value)
        { // value already exist in the search tree
            return parent;
        }

        if (parent == nullptr)
        {
            parent = new Node(value);
            return parent;
        }

        // if the value is less then the parent
        // then it wil be righ child
        if (value < parent->value)
        {
            parent->left = insert(parent->left, value); // insert to left sub tree
        }
        else
        {
            parent->right = insert(parent->right, value);
        }

        // update the height
        parent->height = calculateHeight(parent);
        //cout << parent->value << " height: " << parent->height << endl;

        return balanceTreeIfNeeded(parent);
    }

    /**
     * complexity: O(h) , where h is height of the tree
     *
     * The function will delete an existing item from the binary search tree. tree
     * is re-structured after deletion In case of deletion of a node where both of
     * its children exist, the next largest node(successor) in the tree is used for
     * replacement. If the item is not in the tree, it does nothing.
     */
    void deleteItem(int value)
    {
        //cout<<"deleteing "<<value<<endl;
        root = findAndDelete(root, value);
    }

    /**
     * recursive helper method of deleteItem method
     * @param root of the sub-tree in recursive calls
     * @param value to be deleted
     * @return Node to re-structure the tree after deletion
     */
    Node *findAndDelete(Node *root, int value)
    {

        // base case for leaf
        if (root == nullptr)
        {
            return nullptr;
        }

        //cout<<"root is now in "<<root->value<<endl;

        if (root->value == value)
        {
            //cout<<"found "<<value<<" for deletion"<<endl;

            // case-1: value-node has one or no children
            // if it is leaf node then simply return a null which is used to disconnect with its parent
            // if it has one child then return it's pointer which is used to connect with the parent.
            if (root->left == nullptr){
                Node* temp = root->right;
                delete root;
                return temp;
            }
            else if (root->right == nullptr){
                Node *temp = root->left;
                delete root;
                return temp;
            }
    
            // case-2: value-node has both left and right children
            // we will search for next max value in right sub-tree
            // we used successor here , but predecessor can also be used.

            // track successorRoot
            Node *successorRoot = root;
            // track successor
            Node *successor = root->right;
            while (successor->left != nullptr)
            {
                successorRoot = successor;
                successor = successor->left;
            }

            // set value-node as successor value
            root->value = successor->value;

            // finally delete the successor by disconnecting it
            // from its parent and garbage collector will delete the successor node
            // successor will be leaf node in this case for sure
            if (successorRoot->left == successor) // check if successor it a left child
                successorRoot->left = nullptr;
            else // check if successor is a right child.
                successorRoot->right = nullptr;

            delete successorRoot;
            
            // return root which will be connected to its parent node
            return balanceTreeIfNeeded(root, true);
        }
        // check if value-node may exist in left subtree
        if (value < root->value)
        {
            root->left = findAndDelete(root->left, value);
        }
        else
        {
            root->right = findAndDelete(root->right, value);
        }

        root->height = calculateHeight(root);

        return balanceTreeIfNeeded(root, true);
    }

    /**
     *
     *
     * This function searches for an item in the binary search tree. If the item,
     * say 52, is found, the function prints “52 has been found”; otherwise the
     * function prints “52 has not been found”.
     * complexity: O(h) , where h is height of the tree
     */
    bool searchItem(int value)
    {
        return searchItem(root, value);
    }

    /**
     * complexity: O(h) , where h is height of the tree
     *
     * recursive helper method to search a item in the tree
     *
     * recursively search a value in the tree.
     *
     * @param root  is the root of sub-tree in recursive calls
     * @param value is the value to be searched
     * @return true if the value is found and false otherwise.
     */
    bool searchItem(Node *root, int value)
    {
        // tree is empty or node is a leaf
        if (root == nullptr)
            return false;

        // check is the value is matched with this node
        if (root->value == value)
            return true;

        // check is serach item is less then current root value
        // if so then search left sub-tree
        if (value < root->value)
            return searchItem(root->left, value);

        // else search in right sub-tree
        return searchItem(root->right, value);
    }

    void print()
    {
        print(root);
        cout << endl;
    }

    void print(Node *parent)
    {
        if (parent == nullptr)
            return;
        cout << parent->value;
        if (parent->left == nullptr && parent->right == nullptr)
            return;
        cout << "(";
        print(parent->left);
        cout << ")(";
        print(parent->right);
        cout << ")";
    }

    void debugPrint()
    {
        debugPrint(root);
        cout << endl;
    }

    void debugPrint(Node *parent)
    {
        if (parent == nullptr)
            return;
        cout << parent->value<<", h: "<<parent->height<<endl;
        if (parent->left == nullptr && parent->right == nullptr)
            return;
        cout << "("<<endl;
        debugPrint(parent->left);
        cout<<"("<<endl;
        cout<<")"<<endl;
        print(parent->right);
        cout << ")"<<endl;
    }
};

int main()
{
    AVLTree tree;

    //freopen("in.txt", "r", stdin);

    char instruction;
    int value;

    while(cin>>instruction>>value){
        switch (instruction)
        {
        case 'I':
            tree.insert(value);
            tree.print();
            break;
        case 'D':
            tree.deleteItem(value);
            tree.print();
            break;
        case 'F':
            tree.searchItem(value) ? cout<<"True"<<endl : cout<<"False"<<endl;
            break;
        }
    }
}