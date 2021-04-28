#pragma once
#include <string>
#include <cmath>
#include <map>
#include <iostream>
#include <exception>
#include <stdexcept>
namespace ariel{
    class NumberWithUnits;
}


class ariel::NumberWithUnits {
    public:
        double amount;
        std:: string type;




        static std::map< std::string, std::map<std:: string, double>> dictionary;
        static double eps;

    public:
        NumberWithUnits(double a, const std::string& t);
        static void read_units(std::ifstream& units_file);

        // == !=
        bool operator == (const NumberWithUnits &nwu) const;
		bool operator != (const NumberWithUnits &nwu) const;

        // ++ -- += -+ + - 
        NumberWithUnits operator-() const; 
        NumberWithUnits operator+() const;
        friend NumberWithUnits operator + 	(const NumberWithUnits &nwu1, const NumberWithUnits &nwu2);
		friend NumberWithUnits operator - 	(const NumberWithUnits &nwu1, const NumberWithUnits &nwu2);
        NumberWithUnits& operator += (const NumberWithUnits &nwu);
		NumberWithUnits& operator -= (const NumberWithUnits &nwu);
		NumberWithUnits& operator ++ ();      // prefix  - ++a
        NumberWithUnits  operator ++ (int);   // postfix - a++
        NumberWithUnits& operator -- ();      // prefix  - --a
        NumberWithUnits  operator -- (int);   // postfix - a--

        // * *=
        NumberWithUnits& operator *= 	(const double d);
        friend NumberWithUnits operator * 	(const double d, const NumberWithUnits &nwu);
        friend NumberWithUnits operator * 	(const NumberWithUnits &nwu, const double d);

        // < <= > >=
        friend bool operator > 	(const NumberWithUnits &nwu1, const NumberWithUnits &nwu2);
		friend bool operator < 	(const NumberWithUnits &nwu1, const NumberWithUnits &nwu2);
        friend bool operator >=	(const NumberWithUnits &nwu1, const NumberWithUnits &nwu2);
		friend bool operator <=	(const NumberWithUnits &nwu1, const NumberWithUnits &nwu2);

        // << >>
        friend std::ostream& operator <<	(std::ostream &os, const NumberWithUnits &nwu);
		friend std::istream& operator >>	(std::istream &input, NumberWithUnits &nwu);

        friend NumberWithUnits conversion(const NumberWithUnits& nwu, const std::string& wantedType);
};




class MyException : public std::exception 
{
    const char* _info;
    
    public:
    	MyException(const char* info) : std::exception(),
        	_info (info)
        {
        }
       
};

