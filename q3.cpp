#include<bits/stdc++.h>
using namespace std;

vector<vector<double> > training_data(1000);


int main()
{
    ifstream file("train.txt");
    string line;
    int i=1;
    while(getline(file,line))
    {
        string word="";
        stringstream lines(line);
        while(getline(lines,word,','))
        {
            stringstream word_val(word);
            double val;
            word_val>>val;
            training_data[i].push_back(val);
        }
        i++;
    }
    for(i=1;i<5;i++)
    {
        for(int j=0;j<training_data[i].size();j++)
        {
            cout<<training_data[i][j]<<"    ";
        }
        cout<<endl;
    }
    return 0;
}
