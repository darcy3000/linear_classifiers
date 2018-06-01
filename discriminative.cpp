#include<bits/stdc++.h>
#include<math.h>
using namespace std;

vector<vector<double> > training_data(1000),testing_data(500);
vector<double> w(5);
double alpha =50;//learning rate
double tn=0,tp=0,fn=0,fp=0;
double sigmoid(double a)
{
    return (1.0/(1.0+exp(-1.0*a)));
}

double pred(vector<double> &a,vector<double> &b)
{
    double ans=0;
    for(int i=0;i<a.size();i++)
    {
        ans+=a[i]*b[i];
    }
    return ans;
}

double gradient_descent()
{
    //Initialising weight vectors
    w[0]=0.013425;
    w[1]=0.034261;
    w[2]=-0.056129;
    w[3]=0.098456;
    w[4]=-0.923451;
    vector<double> w_temp(10);//Temporary vector to store w
    for(long int i=1;i<=1000;i++)
    {

        for(long int j=1;j<=960;j++)
        {
            for(int k=0;k<5;k++)
            {
                w_temp[k]=w[k]-alpha *(sigmoid(pred(w,training_data[j]))-training_data[j][5]) *training_data[j][k];
            }
            for(int k=0;k<5;k++)
            {
                w[k]=w_temp[k];
            }
        }
    }

    double error=0;
    for(int i=1;i<=960;i++)
    {
        error+=abs(sigmoid(pred(w,training_data[i]))-training_data[i][5]);
    }
    //cout<<error<<endl;
}

double results()
{
    for(int i=1;i<=412;i++)
    {
        //cout<<sigmoid(pred(w,testing_data[i]))<<endl;
        if(testing_data[i][5]==1)
        {
            if(sigmoid(pred(w,testing_data[i]))>=0.5)
            {
                tp++;
            }
            else
            {
                tn++;
            }
        }
        else
        {
            if(sigmoid(pred(w,testing_data[i]))>=0.5)
            {
                fp++;
            }
            else
            {
                fn++;
            }
        }
    }
    cout<<tp<<"     "<<fp<<"    "<<tn<<"        "<<fn<<endl;
    cout<<"Accuracy is "<<(tp+fn)/(tp+fn+tn+fp)<<endl;
    cout<<"Precision is"<<(tp/(tp+fp))<<endl;
    cout<<"Recall is "<<tp/(tp+tn)<<endl;
    cout<<"Weight vector is \n";
    for(int i=0;i<w.size();i++)
    {
        cout<<w[i]<<"   ";
    }
    cout<<"\n";
}


int main()
{
    ifstream file("train.txt");
    string line;
    int i=1;

    while(getline(file,line))
    {
        string word="";
        stringstream lines(line);
        training_data[i].push_back(1);
        while(getline(lines,word,','))
        {
            stringstream word_val(word);
            double val;
            word_val>>val;
            training_data[i].push_back(val);
        }
        i++;
    }
    ifstream file2("test.txt");
    i=1;
    while(getline(file2,line))
    {
        string word2="";
        stringstream lines(line);
        testing_data[i].push_back(1);
        while(getline(lines,word2,','))
        {
            stringstream wordval(word2);
            double val ;
            wordval>>val;
            testing_data[i].push_back(val);
        }
        i++;
    }
    gradient_descent();
    results();
    /*for(i=1;i<500;i++)
    {
        for(int j=0;j<training_data[i].size();j++)
        {
            cout<<training_data[i][j]<<"    ";
        }
        cout<<endl;
    }*/

    return 0;
}
