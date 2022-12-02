/*
Name: Benjamin Liu
Class: CS3010
Assignmen: Project 4
Due Date: 12/04/2022
Note: Due to how numbers are represented on machines, some numbers may not be calculated or displayed accurately.
      The divided difference table can also easily be misalgined depending on how many digits are in each number,
      but the numbers should still be displayed in their proper orders regardless.
      Numbers are rounded to 3 decimal points.
      For the file inputs, if simply giving the file name is not enough to find it, you may need to specify the whole file path.
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
void simplifyPoly(vector<vector<double>>); //This specifically simplifies the polynomial from Newtons since its easier compared to Lagrange

int main()
{
    vector<vector<double>> data = getData();
    newtonPoly(data);
    cout << endl;
    lagrange(data);
    cout << endl;
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
    while (getline(dataFile, rowData)) //Gets data row by row until eof
    {
        istringstream ss(rowData); //Puts the rows of number into a string stream so that we can pull each individual numbers that are seperated by spaces
        vector<double> temp;
        while (ss >> hold) //Takes the individual numbers
        {
            temp.push_back(stod(hold));//Convert from string to double and push into the vector
        }
        data.push_back(temp);
    }

    return data; //data should have all the x values on row 0 and the f(x) values on row 1.
}

void lagrange(vector<vector<double>> x)
{
    vector<double> d;
    for (int i = 0; i < x.at(0).size(); i++) //Calculating the denominator at each term
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
    //Displaying the polynomial
    cout << "Lagrange interpolated polynomial:\n";
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
            cout << a.at(j).at(i) << "     ";
        }
        cout << endl;
    }

    //Creating the polynomial
    vector<vector<double>> poly; //Column 0 of poly stores the first values of the f primes from the table, and the x is stored in column 1
    for (int i = 0; i < a.size(); i++)
    {
        vector<double> temp;
        temp.push_back(a.at(i).at(0));
        if (i != 0)
        {
            temp.push_back(-1 * x.at(0).at(i-1));
        }
        poly.push_back(temp);
    }
    //Displaying the polynomial
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
        for (int j = 1; j <= i; j++) //Because the polynomial is built off the previous term, we start at row 1 of poly and build our way down to row i.
        {
            cout << "(x";
            if (poly.at(j).at(1) < 0)
            {
                cout << " - ";
            }
            else if (poly.at(j).at(1) >= 0)
            {
                cout << " + ";
            }
            cout << abs(poly.at(j).at(1)) << ")";
        }
    }
    //Simplifying Newton's polynomial
    simplifyPoly(poly);
}

/*Assumes a Newton polynomial is being passed to it. The algorithm is based on the fact that in each term of a Newton polynomial, it builds off
from the previous term, i.e. (x-1) + (x-1)(x-2), where the (x-1) is considered the first term and (x-1)(x-2) is the second term. Because of this we
can simply look back at what the previous term was and do the necessary calculations to figure out the new term*/
void simplifyPoly(vector<vector<double>> poly)
{
    int n = poly.size();
    vector<vector<double>> terms; //Will store each polynomial at each term row by row, the highest degree of that polynomial will match the row number
    terms.resize(n);
    terms.at(0).push_back(1); //The highest degree of the row will always be 1
    terms.at(1).push_back(poly.at(1).at(1)); 
    terms.at(1).push_back(1); //After this push, row 1 should mimic what (a - x) looks like

    for (int i = 2; i < n; i++) //Calculating the rest of the rows, where these terms will build off from the term one row above them
    {
        terms.at(i).push_back(terms.at(i - 1).at(0) * poly.at(i).at(1));
        for (int j = 1; j < i; j++)
        {
            double val = (poly.at(i).at(1) * terms.at(i - 1).at(j)) + terms.at(i - 1).at(j - 1);
            terms.at(i).push_back(val);
        }
        terms.at(i).push_back(1);
    }

    vector<double> simpPoly; //After figuring out what the polynomials are multiply them with the coefficients stored in the columns of poly
    for (int j = 0; j < terms.size(); j++)
    {
        double val = 0;
        for (int i = j; i < terms.size(); i++)
        {
            val += terms.at(i).at(j) * poly.at(i).at(0);         
        }
        simpPoly.push_back(val);
    }

    //Printing simplified polynomial
    cout << "\n\nSimplified Polynomial:\n";
    cout << simpPoly.at(simpPoly.size() - 1) << "x^" << simpPoly.size() - 1;
    for (int i = simpPoly.size() - 2; i >= 0; i--)
    {
        if (simpPoly.at(i) < 0)
        {
            cout << " - ";
        }
        else if (simpPoly.at(i) >= 0)
        {
            cout << " + ";
        }

        if (i != 0)
        {
            cout << abs(simpPoly.at(i)) << "x^" << i;
        }
        else
        {
            cout << abs(simpPoly.at(i));
        }
        
    }
    cout << endl;
}