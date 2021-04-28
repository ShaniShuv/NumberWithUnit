#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <cmath>
#include <array>
#include "NumberWithUnits.hpp"
using namespace ariel;
using namespace std;
double const BASE = 10;
namespace ariel 
{

map< std::string, map<std:: string, double>> NumberWithUnits:: dictionary; // conversion map
double NumberWithUnits::eps = pow(BASE, -4);
    // constructor
    NumberWithUnits:: NumberWithUnits(double a, const std::string& t): amount(a), type(t)
    {
        if (dictionary.find(t) == dictionary.end() || t.length() == 0)
        {
            throw std::invalid_argument{t + " doesn't exist in the units file"};
        }
    }

    void NumberWithUnits::read_units(ifstream& units_file)
    {
        if (!units_file) 
        {
            cerr << "File units.txt didn't open" << endl;
            throw std::invalid_argument{" "};
        }
        while(!units_file.eof())
        {
            double d1 = 0;
            double d2 = 0;
            string s1; string s2; string equalS;
            (units_file >> d1 >> s1 >> equalS >> d2 >> s2);
            
            // init all the units s1 can be convert to, with s2
            map<std:: string, double> temp = dictionary[s1];
            for(const auto& m : temp)
            {
                std:: string k;
                k = m.first; //.at(m.first.length()-1);
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

            // init all the units s2 can be convert to, with s1 
            temp = dictionary[s2];
            for(const auto& m : temp)
            {
                std:: string k;
                k = m.first; //.at(m.first.length()-1);
                double v = m.second;
                
                // if it's already in the dictionary don't change it
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
        }  
    } // end of read_units

//----------------------------------------------------------------------------------
//            ==            !=
//----------------------------------------------------------------------------------
    bool NumberWithUnits::operator== (const NumberWithUnits &nwu) const
    {
        
        if(this->type == nwu.type)
        {
            return(abs(amount - nwu.amount) < eps);
        }
        NumberWithUnits rightType = conversion(nwu, type);
        return(abs(amount - rightType.amount) < eps);
    }

	bool NumberWithUnits::operator != (const NumberWithUnits &nwu) const
    {
        return(!(*this == nwu));
    }

//-------------------------------------------------------------------------------------
//            +            -            ++            --            +=            -=
//-------------------------------------------------------------------------------------

    NumberWithUnits  NumberWithUnits::operator-() const
    {
        return NumberWithUnits(-amount, type);
    }
 
    NumberWithUnits NumberWithUnits:: operator+() const
    {
        return NumberWithUnits(amount, type);
    }

    NumberWithUnits& NumberWithUnits::operator += (const NumberWithUnits &nwu)
    {
        if(type != nwu.type)
        {
            NumberWithUnits rightType = conversion(nwu, type);
            amount += rightType.amount;
            return *this;
        }
        amount += nwu.amount;
        return *this;
    }

    NumberWithUnits& NumberWithUnits::operator -= (const NumberWithUnits &nwu)
    {
        if(type != nwu.type)
        {
            NumberWithUnits rightType = conversion(nwu, type);
            amount -= rightType.amount;
            return *this;
        }
        amount -= nwu.amount;
        return *this;
    }

    NumberWithUnits& NumberWithUnits::operator ++ ()
    {
        amount++;
        return *this;
    }
 
    NumberWithUnits NumberWithUnits::operator ++ (int)
    {
        string t = type;
        double a = amount;
        amount ++;
        return NumberWithUnits(a, t);
    }

    NumberWithUnits& NumberWithUnits::operator -- ()
    {
        amount--;
        return *this;
    }

    NumberWithUnits NumberWithUnits::operator -- (int)
    {
        string t = type;
        double a = amount;
        amount --;
        return NumberWithUnits(a, t);
    }

    NumberWithUnits operator + 	(const NumberWithUnits &nwu1, const NumberWithUnits &nwu2)
    {
        if(nwu1.type != nwu2.type)
        {
            NumberWithUnits rightType = conversion(nwu2, nwu1.type);
            return NumberWithUnits(nwu1.amount+rightType.amount, nwu1.type);
        }
        return NumberWithUnits(nwu1.amount+nwu2.amount, nwu1.type);
    }

    NumberWithUnits operator - 	(const NumberWithUnits &nwu1, const NumberWithUnits &nwu2)
    {
    if(nwu1.type != nwu2.type)
        {
            NumberWithUnits rightType = conversion(nwu2, nwu1.type);
            return NumberWithUnits(nwu1.amount-rightType.amount, nwu1.type);
        }
        return NumberWithUnits(nwu1.amount-nwu2.amount, nwu1.type);
    }

//-------------------------------------------------------------------------------------
//            *            *=
//-------------------------------------------------------------------------------------

    NumberWithUnits& NumberWithUnits::operator *= 	(const double d)
    {    
        amount *= d;
        return *this;
    }

    NumberWithUnits operator * 	(const double d, const NumberWithUnits &nwu)
    {
        return NumberWithUnits(nwu.amount * d, nwu.type);
    }

    NumberWithUnits operator * 	(const NumberWithUnits &nwu, const double d)
    {
        return NumberWithUnits(nwu.amount * d, nwu.type);
    }

//-------------------------------------------------------------------------------------
//            <            <=            >            >= 
//-------------------------------------------------------------------------------------

    bool operator > (const NumberWithUnits &nwu1, const NumberWithUnits &nwu2)
    {
        NumberWithUnits rightType = conversion(nwu2, nwu1.type);
        // cout << nwu1.type << " " << nwu1.amount << " " << rightType.type << " " << rightType.amount << endl;
        // bool b = (nwu1.amount > rightType.amount);
        // cout << b << endl;
        // if(nwu1.amount < rightType.amount || nwu1.amount == rightType.amount)
        // {
        //     return false;
        // }
        // return true;
        return((nwu1.amount - rightType.amount) > NumberWithUnits:: eps);
    }

    bool operator < (const NumberWithUnits &nwu1, const NumberWithUnits &nwu2)
    {
        NumberWithUnits rightType = conversion(nwu2, nwu1.type);
        return((rightType.amount - nwu1.amount) > NumberWithUnits:: eps);
    }

    bool operator >= (const NumberWithUnits &nwu1, const NumberWithUnits &nwu2)
    {
        // NumberWithUnits rightType = conversion(nwu2, nwu1.type);

        // return((nwu1 > rightType) || (nwu1 == rightType));
        // cout << nwu1.type << " " << nwu1.amount << " " << nwu2.type << " " << nwu2.amount << endl;
        // bool b = (nwu1.amount > nwu2.amount);
        // cout << b << endl;
         return((nwu1 > nwu2) || (nwu1 == nwu2));
    }

    bool operator <= (const NumberWithUnits &nwu1, const NumberWithUnits &nwu2)
    {
        // NumberWithUnits rightType = conversion(nwu2, nwu1.type);
        // return(nwu1.amount <= rightType.amount);
         return((nwu1 < nwu2) || (nwu1 == nwu2));
    }

//-------------------------------------------------------------------------------------
//            <<            >>
//-------------------------------------------------------------------------------------

    istream& operator>> (istream& input, NumberWithUnits& nwu)
    {
    char c = ' ';
    // while(c == ' ') {
    //     if ((input.eof()))
    //     {
    //         throw std::invalid_argument{"1 cama od"};
    //     }
    //     input >> c;
    //     cout << c<< " 1*** " << endl;
    // }
    double d = 0;
    input>> d;

    input >> c;
    // cout << c<< " 2*** " << endl;
    while(c == ' ') {
        if ((input.eof()))
        {
            throw std::invalid_argument{"2 cama od"};
        }
        input >> c;
        // cout << c<< " 3*** " << endl;
    }
    //  input >> c;
    if(c != '[')
    {
        // cout << d << " " << endl;
        throw std::invalid_argument{"7 cama od"};
    }
     input >> c;
    //  cout << c<< " 4*** " << endl;
    while(c == ' ') {
        if ((input.eof()))
        {
            throw std::invalid_argument{"3 cama od"};
        }
        input >> c;
        // cout << c<< " 5*** " << endl;
    }
    string s;
    // input >> s;
    // input >> c;
    // cout << s<< " 6*** " << endl;

    //  while(c == ' ') {
    //     if ((input.eof()))
    //     {
    //         throw std::invalid_argument{"3 cama od"};
    //     }
    //     input >> c;
    //     cout << c<< " 5*** " << endl;
    // }

    // input >> c;
     while(c != ' ' && c != ']') {
        s+=c;
        if ((input.eof()))
        {
            throw std::invalid_argument{"3 cama od"};
        }
        input >> c;
        // s+=c;
        // cout << c<< " 5*** " << endl;
    }

    // cout << s << " s " << endl;
   
        // cout << nwu.amount << " " << nwu.type << endl;
        if (ariel:: NumberWithUnits:: dictionary.find(s) == ariel:: NumberWithUnits::dictionary.end() || s.length() == 0 )
        {
            throw std::invalid_argument{s + " doesn't exist in the units file"};
        }

    while(c == ' ') {
        if ((input.eof()))
        {
            throw std::invalid_argument{"4 cama od"};
        }
        input >> c;
        // cout << c<< " 7*** " << endl;
    }
    //  input >> c;
    if(c != ']')
    {
        // cout << c<< " 8*** " << endl;
        throw std::invalid_argument{"5 cama od"};
    }

    nwu.amount= d;
    nwu.type = s;
       
        return input;
    }

    ostream& operator <<	(std::ostream &os, const NumberWithUnits &nwu)
    {
        return os << (double)(nwu.amount) << "[" << nwu.type << "]";
    }

    NumberWithUnits conversion(const NumberWithUnits& nwu ,const std::string& wantedType)
    {
        try
        {
            if(nwu.type!= wantedType)
            {
                double d = NumberWithUnits:: dictionary[nwu.type][wantedType];
                if (d == 0)
                {
                    throw std::invalid_argument{"Units"};
                }
                double currA = nwu.amount * d;
                return NumberWithUnits(currA, wantedType);
            }
            return nwu;
        }
        catch(const std::exception& e)
        { 
            throw std::invalid_argument{"Units do not match - [" + nwu.type + "] cannot be converted to [" + wantedType + "]"};
        }
    }
}

