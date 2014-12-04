//
//  main.cpp
//  RedBlackTree
//
//  Created by RogerChen on 14/12/1.
//  Copyright (c) 2014年 RogerChen. All rights reserved.
//

#include <fstream>
#include "RedBlackTree.h"

int main(int argc, const char * argv[]) {
    fstream fin("/Users/rogerchen/Desktop/numbers.txt");
    string word;
    int count = 0;
    RedBlackTree rbt;
    while (fin >> word) {
        rbt.insert(word, count++);
    }

    return 0;
}
