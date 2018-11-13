///
//  main.cpp
//  NaiveBayesClassifier
//
//  Created by 王宇晨 on 2018/11/11.
//  Copyright © 2018 王宇晨. All rights reserved.
//

#include "iostream"
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <math.h>
#include <time.h>
using namespace std;

const int length = 2;



int Classify(map<string, int> *Doc_train, vector<map<string, int>> *DocWordCounter, vector<double> *P_Docs, vector<double> *Num_Word)
{
    double MaxProb = -9999999999;
    double Prob = 0;
    int Label;
    for (int k = 0; k < 15; k++)
    {
        map<string, int>::iterator Titer;
        for (Titer = Doc_train->begin(); Titer != Doc_train->end(); Titer++)
        {
            map<string, int>::iterator iter;
            iter = DocWordCounter->at(k).find(Titer->first);
            if (iter != DocWordCounter->at(k).end())
            {
                Prob += log10((DocWordCounter->at(k)[Titer->first] + 1) / (Num_Word->at(k) + 15));
            }
            else {
                Prob += log10(1 / (Num_Word->at(k) + 15));
            }
        }
        Prob += log10(P_Docs->at(k));
        if (Prob > MaxProb) {
            MaxProb = Prob;
            Label = k;
        }
        Prob = 0;
    }
    return Label + 1;
}

void testeasily(map<string, int> *doc_test, string string_test)
{
    map<string, int>::iterator distribute;
    distribute = doc_test->find(string_test);
    if (distribute == doc_test->end()) {
        doc_test->insert(make_pair(string_test, 1));
    }
    else {
        doc_test->at(string_test)++;
    }
}

void workhard(vector<map<string, int>> *Doc, string str, int t)
{
    map<string, int>::iterator iter;
    iter = Doc->at(t - 1).find(str);
    if (iter == Doc->at(t - 1).end()) {
        Doc->at(t - 1).insert(make_pair(str, 1));
    }
    else {
        Doc->at(t - 1)[str]++;
    }
}


int main()
{
    int test[3000];
    time_t Begin, Middle,End;
    Begin = time(NULL);
    vector<map<string, int>> *DocWordCounter = new vector<map<string, int>>(15);
    vector<int> *DocNumCounter = new vector<int>(15);
    vector<double> *WordNumCounter = new vector<double>(15);
    vector<double> *Prob_Docs = new vector<double>(15);
    vector<double> *Num_Word = new vector<double>(15);
    double Num_Words = 0;
    
    //
    ifstream training("/Users/wangyuchen/Documents/Github/NaiveBayesClassifier/training.txt", ios::in);
    string str;
    string buffer;
    int DocCount = 0;
    while(getline(training, buffer))
    {
        DocCount++;
        int signal = buffer.rfind(' ');
        int order = stoi(buffer.substr(signal + 2));
        DocNumCounter->at(order - 1)++;
        buffer =buffer.substr(0, signal);
        while (int i = buffer.find(' '))
        {
            if (i == -1)
                break;
            str = buffer.substr(0, i);
            buffer = buffer.substr(i + 1);
            if (str.size() < length)continue;
            Num_Words++;
            Num_Word->at(order - 1)++;
            workhard(DocWordCounter, str, order);
            WordNumCounter->at(order - 1)++;
        }
    }
    for (int i = 0; i < 15; i++)
    {
        Prob_Docs->at(i) = Num_Word->at(i) / Num_Words;
    }
    training.close();
    Middle = time(NULL);
    
    ifstream testing("/Users/wangyuchen/Documents/Github/NaiveBayesClassifier/testing.txt", ios::in);
    vector<map<string, int>> *testdoc_counter = new vector<map<string, int>>;
    map<string, int> *test_doc;
    vector<int> *Label_expect = new vector<int>;
    string buffer_test, read_test;
    while (getline(testing, buffer_test)) {
        test_doc = new map<string, int>;
        int signal = buffer_test.rfind(' ');
        int Label_test = stoi(buffer_test.substr(signal + 2));
        Label_expect->push_back(Label_test);
        buffer_test = buffer_test.substr(0, signal);
        while (int i = buffer_test.find(' '))
        {
            if (i == -1)
                break;
            read_test = buffer_test.substr(0, i);
            buffer_test = buffer_test.substr(i + 1);
            if (read_test.size() < length)continue;
            testeasily(test_doc, read_test);
        }
        testdoc_counter->push_back(*test_doc);
        delete test_doc;
    }
    testing.close();
    int correct_test = 0;
    int Total_test = 0;
    int MaxSize_test = testdoc_counter->size();
    for (int i = 0; i < MaxSize_test; i++)
    {
        int Label_test = Classify(&testdoc_counter->at(i), DocWordCounter, Prob_Docs, Num_Word);
        Total_test++;
        if (Label_test == Label_expect->at(i))
            correct_test++;
        test[i] = Label_test;
    }
    double Accuracy_test = (double)correct_test / Total_test;
    
    //
    ifstream train("/Users/wangyuchen/Documents/Github/NaiveBayesClassifier/training.txt", ios::in);
    vector<map<string, int>> *doccount_train = new vector<map<string, int>>;
    map<string, int> *doc_train;
    vector<int> *Labels_train = new vector<int>;
    string buffer_train, read_train;
    while (getline(train, buffer_train))
    {
        doc_train = new map<string, int>;
        int the_last_space = buffer_train.rfind(' ');//最后一个空格的序号
        int TLabel = stoi(buffer_train.substr(the_last_space + 2));
        Labels_train->push_back(TLabel);
        buffer_train = buffer_train.substr(0, the_last_space);
        while (int i = buffer_train.find(' '))
        {
            if (i == -1)
                break;
            read_train = buffer_train.substr(0, i);
            buffer_train = buffer_train.substr(i + 1);
            if (read_train.size() < length)continue;
            testeasily(doc_train, read_train);
        }
        doccount_train->push_back(*doc_train);
        delete doc_train;
    }
    train.close();
    
    int Hitted_train = 0;
    int Total_total = 0;
    int MaxSize_train = doccount_train->size();
    for (int i = 0; i < MaxSize_train; i++)
    {
        int Label = Classify(&doccount_train->at(i), DocWordCounter, Prob_Docs, Num_Word);
        Total_total++;
        if (Label == Labels_train->at(i))
            Hitted_train++;
    }
    double Accuracy = (double)Hitted_train / Total_total;
    //
    for(int i = 0; i<MaxSize_test; i++)
    {
        cout<<test[i]<<endl;
    }
    End = time(NULL);
    cout<<Middle-Begin<<"seconds"<<"(training)"<<endl;
    cout<<End-Middle<<"seconds"<<"(labeling)"<<endl;
    cout << Accuracy <<"(training)"<< endl;
    cout << Accuracy_test<<"(labeling)"<< endl;
    
    
    return 0;
}
