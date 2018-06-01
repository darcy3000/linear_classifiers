#include<iostream>
#include<algorithm>
#include<fstream>
#include<bits/stdc++.h>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

vector<vector<double> > data;
vector<vector<double> > data_test;
MatrixXd class_1_mean(4,1);
MatrixXd class_0_mean(4,1);
double count_1=0;
double count_0=0;
MatrixXd Sw(4,4);
MatrixXd Sw_inverse(4,4);
vector<double> results_1d;
vector<double> sorted_results_1d;
MatrixXd W_t(4,1);
double threshold;
double tp=0,tn=0,fp=0,fn=0;

void find_mean()
{
    MatrixXd sum_1(4,1);
    MatrixXd sum_0(4,1);
    for(int i=0;i<data.size();i++)
    {
        if(data[i][4]==1)
        {
            count_1++;
            MatrixXd temp(4,1);
            for(int j=0;j<4;j++)
            {
                temp(j,0) = data[i][j];
            }
            sum_1 = sum_1+temp;
        }
        else
        {
            count_0++;
            MatrixXd temp(4,1);
            for(int j=0;j<4;j++)
            {
                temp(j,0) = data[i][j];
            }
            sum_0 = sum_0+temp;
        }
    }

    class_1_mean = sum_1 / count_1;
    class_0_mean = sum_0 / count_0;
    
}

void find_sw()
{
    for(int i=0;i<data.size();i++)
    {
        if(data[i][4]==1)
        {
            MatrixXd temp(4,1);
            for(int j=0;j<4;j++)
            {
                temp(j,0) = data[i][j];
            }
            Sw = Sw + (temp - class_1_mean)*((temp - class_1_mean).transpose());
        }
        else
        {
            MatrixXd temp(4,1);
            for(int j=0;j<4;j++)
            {
                temp(j,0) = data[i][j];
            }
            Sw = Sw + (temp - class_0_mean)*((temp - class_0_mean).transpose());
        }
    }
}

void find_sw_inverse()
{
    Sw_inverse = Sw.inverse();
    //cout<<Sw_inverse<<endl;
}

void finds_result_1d()
{
    W_t = Sw_inverse * (class_1_mean - class_0_mean);
    for(int i=0;i<data.size();i++)
    {
        MatrixXd temp(4,1);
        for(int j=0;j<4;j++)
        {
            temp(j,0) = data[i][j];
        }
        double ans=0;
        for(int k=0;k<4;k++)
        {
            ans += W_t(k,0)*temp(k,0);
        }
            
        results_1d.push_back(ans);
        //cout<<W_t * temp<<endl;
    }
}

double find_entropy(double fi)
{
    //cout<<fi<<endl;
    double count_pos=0,count_neg=0;
    for(int i=0;i<results_1d.size()-1;i++)
    {
        if((results_1d[i] -fi>0 && data[i][4]==1) || (results_1d[i]-fi<0 && data[i][4]==0))
            count_pos++;
        else
            count_neg++;
    }
    //cout<<count_pos/(count_pos+count_neg)<<endl;
    //cout<<"count_pos "<<count_pos<<endl;
    //cout<<"count_neg "<<count_neg<<endl;
    double p1 = count_pos/(count_pos+count_neg);
    double p0 = count_neg/(count_neg+count_pos);
    //cout<<"entropy "<<-log(p1)*p1 + -log(p0)*p0<<endl;
    double k = -log(p1)*p1 + -log(p0)*p0;
    return k;
}

void find_y0()
{
    double fi;
    double entropy=5;
    for(int i=0;i<sorted_results_1d.size()-1;i++)
    {
        fi = (sorted_results_1d[i]+ sorted_results_1d[i+1])/2;
        double en = find_entropy(fi);
        //cout<<"entropy "<<en<<endl;
        if(en<entropy)
        {
            entropy = en;
            //cout<<"entropy "<<entropy<<endl;
            threshold = fi;
            //cout<<"threshold "<<threshold<<endl;
        }
    }
    //cout<<entropy<<endl;

}

void check_test()
{
    double accuracy=0,count_neg=0,count_pos=0;
    for(int i = 0;i<data_test.size();i++)
    {
        double ans = 0;
        for(int j=0;j<4;j++)
        {
            ans += data_test[i][j]*W_t(j,0);
        }
        if(ans-threshold>0 && data_test[i][4]==1) 
        {
            tp++;
        }
        else if(ans-threshold<0 && data_test[i][4]==0)
        {
            fn++;
        }
        else if(ans-threshold<0 && data_test[i][4]==1)
        {
            tn++;
        }
        else
            fp++;

    }
    count_pos = tp+fn;
    count_neg = fp+tn;
    cout<<"Accuracy is:"<<count_pos/(count_pos+count_neg)<<endl;
    cout<<"Precision is:"<<tp/(tp+fp)<<endl;
    cout<<"Recall is"<<tp/(tp+tn)<<endl;
    cout<<tp<<"     "<<tn<<"        "<<fp<<"        "<<fn<<endl;
}


int main()
{
    ifstream fi("train.txt");
    if(!fi.is_open())
        cout<<"error file not opened"<<endl;
    
    while(fi.good())
    {
        string a,b,c,d,ans;
        getline(fi,a,',');
        if(a == "")
            break;
        getline(fi,b,',');
        getline(fi,c,',');
        getline(fi,d,',');
        getline(fi,ans,'\n');
        vector <double> temp;
        //temp.push_back(1);
        temp.push_back(stod(a));
        temp.push_back(stod(b));
        temp.push_back(stod(c));
        temp.push_back(stod(d));
        //temp.push_back(stod(e));
        temp.push_back(stod(ans));
        data.push_back(temp);
        temp.empty();
    }
    fi.close();
    ifstream test("test.txt");
    if(!test.is_open())
        cout<<"error file not opened"<<endl;
    while(test.good())
    {
        string a,b,c,d,ans;
        getline(test,a,',');
        if(a == "")
            break;
        getline(test,b,',');
        getline(test,c,',');
        getline(test,d,',');
        getline(test,ans,'\n');
        vector <double> temp;
        //temp.push_back(1);
        temp.push_back(stod(a));
        temp.push_back(stod(b));
        temp.push_back(stod(c));
        temp.push_back(stod(d));
        //temp.push_back(stod(e));
        temp.push_back(stod(ans));
        data_test.push_back(temp);
        temp.empty();
    }
    test.close();
    find_mean();
    find_sw();
    find_sw_inverse();
    finds_result_1d();
    sorted_results_1d = results_1d;
    sort(sorted_results_1d.begin(),sorted_results_1d.end());
    find_y0();
    check_test();
    //cout<<data_test.size()<<endl;
    return 0;
}
