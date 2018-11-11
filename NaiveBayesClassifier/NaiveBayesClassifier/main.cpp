//
//  main.cpp
//  NaiveBayesClassifier
//
//  Created by 王宇晨 on 2018/11/11.
//  Copyright © 2018 王宇晨. All rights reserved.
//

#include <iostream>
#include <fstream>
#include<map>
using namespace std;

int main()
{
    ifstream training("/Users/wangyuchen/Desktop/training.txt");
    
    if(!training)
    {
        cerr<<"fail to read"<<endl;
    }
    else
    {
        cerr<<"read successfully"<<endl;
        string read;
        if(training.is_open())
        {
            while(getline(training,read,' '))
            {
                map<string,int> word;
            }
            training.close();
            
        }
    }
}

