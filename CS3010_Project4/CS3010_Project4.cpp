/*
Name: Benjamin Liu
Class: CS3010
Assignmen: Project 4
Due Date: 12/04/2022
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <vector>
using namespace std;

//Function Prototypes here
vector<vector<double>> getData();
void lagrange(vector<vector<double>>);
void newtonPoly(vector<vector<double>>);
void printVect(vector<vector<double>>);

int main()
{
    vector<vector<double>> data = getData();
    newtonPoly(data);
    cout << endl;
    lagrange(data);
}

vector<vector<double>> getData()
{
    vector<vector<double>> data;
    string filename = "";
    cout << "Enter file name: ";
    cin >> filename;

    ifstream dataFile;
    dataFile.open(filename);
    if (!dataFile)
    {
        cout << "Could not open file, ending program.\n";
        exit(1);
    }
    
    string rowData, hold = "";
    while (getline(dataFile, rowData))
    {
        istringstream ss(rowData);
        vector<double> temp;
        while (ss >> hold)
        {
            temp.push_back(stod(hold));
        }
        data.push_back(temp);
    }

    return data;
}

void lagrange(vector<vector<double>> x)
{
    vector<double> d;
    for (int i = 0; i < x.at(0).size(); i++)
    {
        double val = 1;
        for (int j = 0; j < x.at(0).size(); j++)
        {
            if (i != j)
            {
                val *= x.at(0).at(i) - x.at(0).at(j);
            }
        }
        d.push_back(val);
    }
    cout << "Lagrange polynomial:\n";
    for (int i = 0; i < x.at(0).size(); i++)
    {
        double c = x.at(1).at(i) / d.at(i);
        if (i != 0)
        {
            if (c < 0)
            {
                cout << " - ";
            }
            else if (c >= 0)
            {
                cout << " + ";
            }
        }
        cout << abs(c);
        for (int j = 0; j < x.at(0).size(); j++)
        {
            if (i == j)
            {
                continue;
            }
            cout << "(x";
            if (x.at(0).at(j) < 0)
            {
                cout << " + ";
            }
            else if (x.at(0).at(j) >= 0)
            {
                cout << " - ";
            }
            cout << x.at(0).at(j) << ")";
        }
    }
}

void newtonPoly(vector<vector<double>> x)
{
    //The divided difference calculations
    int n = x.at(1).size();
    vector<vector<double>> a;
    a.resize(n);
    for (int i = 0; i < x.at(1).size(); i++)
    {
        a.at(0).push_back(x.at(1).at(i));
    }

    for (int i = 1; i < n; i++)
    {
        a.at(i).resize(n - i);
        for (int j = 0; j < n - i; j++)
        {
            a.at(i).at(j) = (a.at(i - 1).at(j + 1) - a.at(i - 1).at(j)) / (x.at(0).at(i + j) - x.at(0).at(j));
        }
    }

    //Printing divided difference table
    cout << "x        ";
    for (int i = 0; i < n; i++)
    {
        cout << "f[";
        for (int j = 0; j < i; j++)
        {
            cout << "'";
        }
        cout << "]     ";
    }
    cout << "\n---------------------------------------------------------------\n";

    for (int i = 0; i < n; i++)
    {
        cout << fixed << showpoint << setprecision(3) << x.at(0).at(i) << "     ";
        for (int j = 0; j < n - i; j++)
        {
            cout << a.at(i).at(j) << "     ";
        }
        cout << endl;
    }

    //Creating the polynomial
    vector<vector<double>> poly;
    for (int i = 0; i < a.size(); i++)
    {
        vector<double> temp;
        temp.push_back(a.at(i).at(0));
        if (i != 0)
        {
            temp.push_back(x.at(0).at(i-1));
        }
        poly.push_back(temp);
    }
    //Displaying the polynomical
    cout << "\nNetwon's interpolated polynomial:\n";
    cout << poly.at(0).at(0);
    for (int i = 1; i < poly.size(); i++)
    {
        if (poly.at(i).at(0) < 0)
        {
            cout << " - ";
        }
        else if (poly.at(i).at(0) >= 0)
        {
            cout << " + ";
        }
        cout << abs(poly.at(i).at(0));
        for (int j = 1; j <= i; j++)
        {
            cout << "(x";
            if (poly.at(j).at(1) < 0)
            {
                cout << " + ";
            }
            else if (poly.at(j).at(1) >= 0)
            {
                cout << " - ";
            }
            cout << abs(poly.at(j).at(1)) << ")";
        }
    }
}

void printVect(vector<vector<double>> v)
{
    for (int i = 0; i < v.size(); i++)
    {
        for (int j = 0; j < v.at(i).size(); j++)
        {
            cout << v.at(i).at(j) << " ";
        }
        cout << endl;
    }
}