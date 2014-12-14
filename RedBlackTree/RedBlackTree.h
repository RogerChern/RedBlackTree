//
//  RedBlackTree.h
//  RedBlackTree
//
//  Created by RogerChen on 14/12/1.
//  Copyright (c) 2014年 RogerChen. All rights reserved.
//

#ifndef RedBlackTree_RedBlackTree_h
#define RedBlackTree_RedBlackTree_h

#include <string>
using namespace std;

class RedBlackTree{
private:
    struct Node{
        Node *left   = nullptr;
        Node *right  = nullptr;
        Node *parent = nullptr;
        int key;
        int value;
        bool color;
        static constexpr bool RED = true;
        static constexpr bool BLACK = false;
        
        Node(int s, int v) :
            key(s),
            value(v)
        {
            
        }
        Node(bool c) :
            color(c)
        {
            
        }
    };
private:
    Node *nil_ = new Node(Node::BLACK);
    Node *root_ = nil_;
private:
    void rotateWithRight(Node *x) {
        Node *y = x -> right;
        //modify x's parent to point to y
        y -> parent = x -> parent;
        if (x -> parent == nil_) {
            root_ = y;
        }
        else if (x -> parent -> left == x) {
            x -> parent -> left = y;
        }
        else {
            x -> parent -> right = y;
        }
        //modify y's left child (if exists)
        x -> right = y -> left;
        if (y -> left != nil_) {
            y -> left -> parent = x;
        }
        y -> left = x;
        x -> parent = y;
    }
    
    void rotateWithLeft(Node *x) {
        Node *y = x -> left;
        y -> parent = x -> parent;
        if (x -> parent == nil_) {
            root_ = y;
        }
        else if (x -> parent ->left == x) {
            x -> parent -> left = y;
        }
        else {
            x -> parent -> right = y;
        }
        x -> left = y -> right;
        if (y -> right != nil_) {
            y -> right -> parent = x;
        }
        y -> right = x;
        x -> parent = y;
    }
    
    void insert(Node *n) {
        //find the position to insert
        Node *px = nil_; //trailing pointer
        Node *x  = root_;
        while (x != nil_) {
            px = x;
            if (n -> key > x -> key) {
                x = x -> right;
            }
            else {
                x = x -> left;
            }
        }
        //insert the node
        n -> parent = px;
        if (px == nil_) {
            root_ = n;
        }
        else if (n -> key > px -> key) {
            px -> right = n;
        }
        else {
            px -> left = n;
        }
        //maintain the property of RedBlackTree
        n -> left = n -> right = nil_;
        n -> color = Node::RED;
        insert_fix_up(n);
    }
    
    void insert_fix_up(Node *n) {
        while (n -> parent -> color == Node::RED) {
            if (n -> parent -> parent -> left == n -> parent) {
                Node *u = n -> parent -> parent ->right;
                if (u -> color == Node::RED) {
                    n -> parent -> color = u -> color = Node::BLACK;
                    n -> parent -> parent -> color = Node::RED;
                    n = n -> parent -> parent;
                }
                else {
                    if (n == n -> parent -> right) {
                        n = n -> parent;//keep the n being the right child
                        rotateWithRight(n);
                    }
                    n -> parent -> color = Node::BLACK;
                    n -> parent -> parent -> color = Node::RED;
                    rotateWithLeft(n -> parent -> parent);
                }
            }
            else {
                Node *u = n -> parent -> parent -> left;
                if (u -> color == Node::RED) {
                    n -> parent -> color = u -> color = Node::BLACK;
                    n -> parent -> parent -> color = Node::RED;
                    n = n -> parent -> parent;
                }
                else {
                    if (n == n -> parent -> left) {
                        n = n -> parent;
                        rotateWithLeft(n);
                    }
                    n -> parent -> color = Node::BLACK;
                    n -> parent -> parent -> color = Node::RED;
                    rotateWithRight(n -> parent -> parent);
                }
            }
        }
        root_ -> color = Node::BLACK;
    }
    
    void remove(Node *n) {
        Node *x; //use to trace the Node breaks rule 5
        bool y_original_color = n -> color;
        if (n -> right == nil_) {
            x = n -> left;
            transplant(n, n -> left);
            delete n;
        }
        else if (n -> left == nil_) {
            x = n -> right;
            transplant(n, n -> right);
            delete n;
        }
        else {
            Node *y = treeMin(n -> right);
            y_original_color = y -> color;
            x = y -> right;
            if (y == n -> right) {
                x -> parent = y; //whether condition stands, we have to set x's parent for the use in fix_up
            }
            else {
                transplant(y, x);
                y -> right = n -> right;
                y -> right -> parent = y;
            }
            y -> left = n -> left;
            y -> left -> parent = y;
            y -> color = n -> color;
            transplant(n, y);
            delete n;
        }
        if (y_original_color == Node::BLACK) {
            remove_fix_up(x);
        }
    }
    
    void remove_fix_up(Node *n) {
        while (n -> color != Node::RED && n != root_) {
            if (n == n -> parent -> left) {
                Node *b = n -> parent -> right;//b means brother
                if (b -> color == Node::RED) {
                    swap(b -> color, b -> parent -> color);
                    rotateWithRight(b -> parent);
                    b = n -> parent -> right;
                }
                if (b -> left -> color == Node::BLACK && b -> right -> color == Node::BLACK) {
                    b -> color = Node::RED;
                    n = n -> parent;
                }
                else {
                    if (b -> left -> color == Node::RED) {
                        swap(b -> left -> color, b -> color);
                        rotateWithLeft(b);
                        b = n -> parent -> right;
                    }
                    b -> color = b -> parent -> color;
                    b -> parent -> color = Node::BLACK;
                    b -> right -> color = Node::BLACK;
                    rotateWithRight(b -> parent);
                    n = root_;
                }
            }
            else {
                Node *b = n -> parent -> left;
                if (b -> color == Node::RED) {
                    swap(b -> color, b -> parent -> color);
                    rotateWithLeft(b -> parent);
                    b = n -> parent -> left;
                }
                if (b -> left -> color == Node::BLACK && b -> right -> color == Node::BLACK) {
                    b -> color = Node::RED;
                    n = n -> parent;
                }
                else {
                    if (b -> right -> color == Node::RED) {
                        swap(b -> right -> color, b -> color);
                        rotateWithRight(b);
                        b = n -> parent -> left;
                    }
                    b -> color = b -> parent -> color;
                    b -> parent -> color = Node::BLACK;
                    b -> left -> color = Node::BLACK;
                    rotateWithLeft(b -> parent);
                    n = root_;
                }
            }
        }
        n -> color = Node::BLACK;
    }
    
    //transplant has nothing to do with both children. It only modifies the parent pointer and parent's pointer of both nodes;
    void transplant(Node *elder, Node *newer) {
        if (elder -> parent == nil_) {
            root_ = newer;
        }
        else {
            if (elder == elder -> parent -> left) {
                elder -> parent -> left = newer;
            }
            else {
                elder -> parent -> right = newer;
            }
        }
        newer -> parent = elder -> parent;
    }
    
    Node *treeMin(Node *t) {
        while (t -> left != nil_) {
            t = t -> left;
        }
        return t;
    }
    
    Node *treeMax(Node *t) {
        while (t -> right != nil_) {
            t = t -> right;
        }
        return t;
    }
    
    Node *search(Node *t, int key) {
        while (t != nil_ && t -> key != key) {
            if (key > t -> key) {
                t = t -> right;
            }
            else {
                t = t -> left;
            }
        }
        return t;
    }
    
public:
    void insert(int key, int value) {
        Node *n = new Node(key, value);
        insert(n);
    }

    void remove(int key) {
        Node *n = search(root_, key);
        remove(n);
    }
    
    void cleanup(Node *n) {
        if (n != nil_) {
            cleanup(n -> left);
            cleanup(n -> right);
            delete n;
        }
    }
    
    ~RedBlackTree() {
        cleanup(root_);
        delete nil_;
    }
};

#endif
