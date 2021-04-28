#include <cmath>
#include "doctest.h"
#include "NumberWithUnits.hpp"
#include <cstdlib>
#include <ctime>
#include <string>
#include <fstream>
#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <exception>
#include <stdexcept>
using namespace std;
using namespace ariel;

map<string, map<string, double>> dictionary; // conversion map

/*
* this func create a random units file
*/
void createNewUnits()
{
	string fileName = "unitsS.txt";
	ofstream myfile;
	myfile.open(fileName);
	char first = 'a';
	char second = 'b';
	for(int i=0; i<25; i++)
	{
		string s1, s2;
		s1+= first;
		s2 += second;
		double d2 = ((rand() % 21 )+119) /17;
		myfile << "1 " << first << " = " << d2 << " " << second << "\n";
		map<std:: string, double> temp = dictionary[s1];
        for(auto m : temp)
        {
            std:: string k;
            k = m.first.at(m.first.length()-1);
            double v = m.second;
            if(dictionary[s2][k] == 0)
            {
                dictionary[s2][k] = (1/d2) * v; 
            }
            if(dictionary[k][s2] == 0)
            {
                dictionary[k][s2] = d2 *(1/v);
            }
        }

        temp = dictionary[s2];
        for(auto m : temp)
        {
            std:: string k;
            k = m.first.at(m.first.length()-1);
            double v = m.second;
            if(dictionary[s1][k] == 0)
            {
                dictionary[s1][k] = (d2) * (v); 
            }
            if(dictionary[k][s1] == 0)
            {
                dictionary[k][s1] = (1/d2)*(1/v);
            }
        }
        dictionary[s1][s2] = d2;
        dictionary[s2][s1] = 1/d2;
        first++; second++;
    } 	
}

TEST_CASE("don't initialize units that are not in the units file")
{
	char c;
    try
	{
		c = 'a';
		string s = ""; 
		s += c;
		NumberWithUnits nwu{35, s};
	}
	catch(const std::exception& e)
	{
		CHECK(true);
	}

    createNewUnits();
    try
    {
        c = '&';
		string s = ""; 
		s += c;
		NumberWithUnits nwu{35, s};
	}
	catch(const std::exception& e)
	{
		CHECK(true);
	}
}

TEST_CASE(" operators == != ")
{
	char c1 = ('a' + (rand() % 26));
    string s1;
    s1 += c1;
    double randD1 = rand();
	NumberWithUnits nwu1{randD1, s1};
    NumberWithUnits nwu2{randD1, s1};
    CHECK(nwu1 == nwu2);
    
    NumberWithUnits nwu3{randD1+3, s1};
    CHECK(nwu1 != nwu3);
    
    s1 += "b";
    NumberWithUnits nwu4{randD1, s1};
    try
    {
        bool b = nwu4 != nwu1;
    }
    catch(const std::exception& e)
    {
        CHECK(true);
    }

    NumberWithUnits nwu5{randD1 + 3, s1};
    try
    {
        bool b = nwu5 != nwu1;
    }
    catch(const std::exception& e)
    {
        CHECK(true);
    }
}

TEST_CASE(" operators + - ")
{
	ifstream units_file{"unitsS.txt"};
  	NumberWithUnits:: read_units(units_file);
    
    char c1 = ('a' + (rand() % 26));
    string s1;
    s1 += c1;
    double randD1 = (rand() % 30) / 10;
	NumberWithUnits nwu1{randD1, s1};

    double randD2 = (rand() % 30) / 10;
	NumberWithUnits nwu2{randD2, s1};

    NumberWithUnits nwu3 = nwu1+nwu2;

    NumberWithUnits nwu4{randD1+randD2, s1};

    CHECK(nwu3 == nwu4);
    NumberWithUnits nwu5{randD1-randD2, s1};
    
    char c2 = ('a' + (rand() % 26));
    string s2;
    s2 += c2;
    double randD3 = rand();
	NumberWithUnits nwu6{randD3, s2};
    NumberWithUnits nwu7{randD3, s2};
    nwu3 = nwu1 + nwu6;

    CHECK(nwu6 == nwu7);
    CHECK(nwu1 == NumberWithUnits{randD1, s1});

    nwu3 = nwu1 - nwu6;
    CHECK(nwu6 == nwu7);
    CHECK(nwu1 == NumberWithUnits{randD1, s1});
}

TEST_CASE(" operators += -= ")
{   
    char c1 = ('a' + (rand() % 26));
    string s1;
    s1 += c1;
    double randD1 = ((rand() % 21 ) +119) /13;
	NumberWithUnits nwu1{randD1, s1};

    double randD2 = ((rand() % 21 ) +119) /13;
	NumberWithUnits nwu2{randD2, s1};

    NumberWithUnits nwu3{randD2, s1};


    nwu3 += nwu1;
    NumberWithUnits nwu4{randD1+randD2, s1};

    CHECK(nwu3 == nwu4);
    CHECK(nwu2 != nwu3);

    nwu3 -= nwu1;
    CHECK(nwu3 == nwu2);
    
    char c2 = ('a' + (rand() % 26));
    string s2;
    s2 += c2;
    double randD3 = ((rand() % 21 ) +119) /13;
	NumberWithUnits nwu5{randD3, s2};
    NumberWithUnits nwu6{randD3, s2};

    double wantedAmount = randD1 +randD2*(dictionary[s1][s2]);
    nwu3 += nwu6;
    try
    {
        bool b = nwu3 != nwu1;
    }
    catch(const std::exception& e)
    {
        CHECK(true);
    }
    
    
    CHECK(nwu6 == nwu5);

    wantedAmount = randD1 -randD2*(dictionary[s1][s2]);
    nwu3 -= nwu6;
    CHECK(nwu3 == nwu2);
    CHECK(nwu6 == nwu5);
}

TEST_CASE(" operators + - unary ")
{
    char c1 = ('a' + (rand() % 26));
    string s1;
    s1 += c1;
    double randD1 = rand();
	NumberWithUnits nwu1{randD1, s1};
	NumberWithUnits nwu2{randD1, s1};
    +nwu1;
    CHECK(nwu1 == nwu2);
	NumberWithUnits nwu3{-randD1, s1};
    CHECK(-nwu1 == nwu3);
}

TEST_CASE(" operators ++ -- pre and post ")
{
    char c1 = ('a' + (rand() % 26));
    string s1;
    s1 += c1;
    double randD = (rand() % 80 + 115) / 23;
	NumberWithUnits nwu1{randD, s1};
	NumberWithUnits nwu2{randD+1, s1};
	NumberWithUnits nwu3{randD-1, s1};

    CHECK(++ nwu3 == nwu1);
    CHECK(--nwu2 == nwu1);
    CHECK(nwu2++ == nwu1);
    CHECK(nwu3 -- == nwu1);
}

TEST_CASE(" operators * *= ")
{
    char c1 = ('a' + (rand() % 26));
    string s1;
    s1 += c1;
    double randD = (rand() % 80 + 115) / 23;
    double d = rand()/3;
	NumberWithUnits nwu1{randD, s1};
    NumberWithUnits nwu11{randD, s1};
	NumberWithUnits nwu2{randD * d, s1};
    
	NumberWithUnits nwu3= nwu1 * d;
    CHECK(nwu1 == nwu11);
    NumberWithUnits nwu4= d * nwu1;
    CHECK(nwu1 == nwu11);
    CHECK(nwu3 == nwu2);
    CHECK(nwu2 == nwu4);

    nwu1 *=d;
    CHECK(nwu2 == nwu1);
}

TEST_CASE(" operators < > ")
{
    char c1 = ('a' + (rand() % 26));
    string s1;
    s1 += c1;
    double randD = (rand() % 80 + 115) / 23;
    double d = (rand() + 91) /3;
	NumberWithUnits nwu1{randD, s1};
    NumberWithUnits nwu2{randD - d, s1};
    NumberWithUnits nwu3{randD + d, s1};

    CHECK(nwu1 > nwu2);
    CHECK(nwu1 < nwu3);

    char c2 = ('a' + ((rand() + 7)% 26));
    string s2;
    s2 += c2;
    NumberWithUnits nwu4{randD, s2};
    if(dictionary[s1][s2] < 1)
    {
        CHECK(nwu1 < nwu4);
    }
    else if(dictionary[s1][s2] > 1)
    {
        CHECK(nwu1 > nwu4);
    }

}

TEST_CASE(" operators <= >= ")
{
    char c1 = ('a' + (rand() % 26));
    string s1;
    s1 += c1;

    char c2 = ('a' + ((rand() + 7)% 26));
    string s2;
    s2 += c2;
    double randD = (rand() % 80 + 115) / 23;
    double d = (rand() + 91) /3;
    NumberWithUnits nwu1{randD, s1};
    NumberWithUnits nwu11{randD, s1};
    NumberWithUnits nwu2{randD - d, s1};
    NumberWithUnits nwu3{randD + d, s1};

    CHECK(nwu1 >= nwu2);
    CHECK(nwu1 <= nwu3);
    CHECK(nwu1 <= nwu11);
    NumberWithUnits nwu4{randD, s2};
    if(dictionary[s1][s2] < 1)
    {
        CHECK(nwu1 <= nwu4);
    }
    else{
        CHECK(nwu1 >= nwu4);
    }
}

TEST_CASE(" operators << >> ")
{
    string sComp;
     char c1 = ('a' + (rand() % 26));
    string s1;
    s1 += c1;
    double randD = (rand() % 80 + 115) / 23;
    NumberWithUnits nwu1{randD, s1};
    sComp += (to_string(randD) +"["+s1+"]");
    
    std::stringstream out;
    out << nwu1;
    double d1;
    out >> d1;
    CHECK(randD == d1);
    CHECK(out.str() ==(to_string((int)d1) + "["+s1+"]"));
    CHECK(randD - (int)(randD) ==0 );


    istringstream iss(sComp);
    char c;
    string t;
    while((!iss.eof()) && ( c == ' ')) {
        iss >> c;
    }
    double a;
    iss>> a;

    while((!iss.eof()) && ( c == ' ' || c=='[' )) {
        iss >> c;
    }
    iss >>c;
    while((iss >> c ||  !iss.eof()) && c != ' ' && c!=']' ) {
        if(iss.fail()) {
            iss.clear();
            string dummy;
            iss >> dummy;
            continue;
        }
        t+=c;
    }
    if(c != ' ' && c != ']') 
    {
        t+=c;
    }
    NumberWithUnits nwu2 = {a, t};
    CHECK(nwu1 == nwu2);
}