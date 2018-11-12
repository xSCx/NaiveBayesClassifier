//
//  main.cpp
//  NaiveBayesClassifier
//
//  Created by 王宇晨 on 2018/11/11.
//  Copyright © 2018 王宇晨. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
using namespace std;


int main()
{
    ifstream training("/Users/wangyuchen/Desktop/training.txt");
    
    vector<map<string,int>> author;
    map<string,int> *temp;
    for(int i=0; i<15; i++){
        temp = new map<string,int>;
        author.push_back(*temp);
    }
    
    if(!training)
    {
        cerr<<"fail to read"<<endl;
    }
    else
    {
        cerr<<"read successfully"<<endl;
        int Label;
        int DocCount[15];
        int WordCount[15];
        for(int k =0; k<15; k++)
        {
            DocCount[k]=0;
            WordCount[k]=0;
        }
        string read;
        string buffer;
        int hhh=0;
        if(training.is_open())
        {
            while(getline(training, read))
            {
                hhh++;
                int Label_singnal = read.rfind(',');
                Label = stoi(read.substr(Label_singnal + 1));
                read = read.substr(0,Label_singnal-1);
                DocCount[Label-1]++;
                map<string, int>::iterator it;
                while(int i = read.find(' '))
                {
                    WordCount[Label-1] += 1;
                    buffer = read.substr(0,i);
                    if(i == -1)
                    {
                        break;
                    }
                    
                    it = author.at(Label-1).find(buffer);
                    if(it == author.at(Label-1).end())
                    {
                        author.at(Label-1).insert(make_pair(buffer, 1));
                        
                    }
                    else
                    {
                        author.at(Label-1)[buffer] += 1;
                    }
                    read = read.substr(i+1);
                }
                cout<<WordCount[0]<<endl;
            }
            training.close();
            
        }
    }
}

