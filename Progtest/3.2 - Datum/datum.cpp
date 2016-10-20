#ifndef __PROGTEST__
#include <cstdio>
#include <ctime>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
using namespace std;
#endif /* __PROGTEST__ */

class InvalidDateException {};

//=================================================================================================
// date_format manipulator - a dummy implementation. Keep this code unless you implement your
// own working manipulator.
ios_base & dummy_date_format_manipulator ( ios_base & x )
 {
   return x;
 }

ios_base & ( * ( date_format ( const char * fmt ) ) ) ( ios_base & x )
 {
   return dummy_date_format_manipulator;
 }
//=================================================================================================

class CDate
 {
   public:
   CDate();
   CDate(int, int, int);
   CDate operator+(int) const;
   CDate operator-(int) const;
   CDate & operator++ ();
   CDate operator++ (int);
   CDate & operator-- ();
   CDate operator-- (int);
   bool operator==(CDate) const;
   bool operator!=(CDate) const;
   bool operator<(CDate) const;
   bool operator<=(CDate) const;
   bool operator>(CDate) const;
   bool operator>=(CDate) const;
   int operator-(CDate) const;
   friend std::ostream & operator<< (std::ostream & os, const CDate &dat);
   friend std::istream & operator>> (std::istream & is, CDate &dat);

    private:
    tm m_time;
 };

std::ostream &operator<< (std::ostream & os, const CDate &dat)
{
    char buff[50];
    strftime(buff, 50, "%Y-%m-%d", &(dat.m_time));
    return os << buff;
}

bool isValid(struct tm datum)
{
    if(datum.tm_mday==31 && (datum.tm_mon==1 || datum.tm_mon==3 || datum.tm_mon==5 || datum.tm_mon==8 || datum.tm_mon==10)) return false;
    if(datum.tm_mday==29 && datum.tm_mon==1 && datum.tm_year%4!=0) return false;
    if(datum.tm_mon==1 && datum.tm_mday>29) return false;
    if(datum.tm_mday>31 || datum.tm_mon>11 || datum.tm_mday<1 || datum.tm_mon<0) return false;

    return true;
}

std::istream &operator>> (std::istream & is, CDate &dat)
{
    char buff[20];
    struct tm datum;
    is >> buff;
    if(!strptime(buff,"%Y-%m-%d",&datum))
    {
        is.setstate (ios::failbit);
        return is;
    }
    if(!isValid(datum))
    {
        is.setstate (ios::failbit);
        return is;
    }
    dat.m_time.tm_year=datum.tm_year;
    dat.m_time.tm_mon=datum.tm_mon;
    dat.m_time.tm_mday=datum.tm_mday;

    return is;
}


CDate::CDate()
{
    m_time={};
    m_time.tm_year=1900;
    m_time.tm_mon=0;
    m_time.tm_mday=1;
    m_time.tm_hour=0;
    m_time.tm_min=0;
    m_time.tm_sec=0;
}

CDate::CDate(int y, int m, int d)
{
    if(d>31 || m>12 || d<1 || m<1) throw InvalidDateException();
    if(d==31 && (m==2 || m==4 || m==6 || m==9 || m==11)) throw InvalidDateException();
    if(d==29 && m==2 && y%4!=0) throw InvalidDateException();
    if(m==2 && d>29) throw InvalidDateException();

    m_time.tm_year = y-1900;
    m_time.tm_mon = m-1;
    m_time.tm_mday = d;
    m_time.tm_hour=0;
    m_time.tm_min=0;
    m_time.tm_sec=0;
}

bool CDate::operator==(CDate dat) const
{
    if((this->m_time.tm_mday==dat.m_time.tm_mday) && (this->m_time.tm_mon==dat.m_time.tm_mon) && (this->m_time.tm_year==dat.m_time.tm_year)) return true;
    else return false;
}

bool CDate::operator!=(CDate dat) const
{
    if((this->m_time.tm_mday!=dat.m_time.tm_mday) || (this->m_time.tm_mon!=dat.m_time.tm_mon) || (this->m_time.tm_year!=dat.m_time.tm_year)) return true;
    else return false;
}

bool CDate::operator<(CDate dat) const
{
    tm cas1={};
    tm cas2={};
    time_t timeSec1,timeSec2;
    cas1.tm_year=this->m_time.tm_year;
    cas1.tm_mon=this->m_time.tm_mon;
    cas1.tm_mday=this->m_time.tm_mday;
    cas2.tm_year=dat.m_time.tm_year;
    cas2.tm_mon=dat.m_time.tm_mon;
    cas2.tm_mday=dat.m_time.tm_mday;
    timeSec1=mktime(&cas1);
    timeSec2=mktime(&cas2);
    if (timeSec1<timeSec2) return true;
    else return false;
}

bool CDate::operator<=(CDate dat) const
{
    tm cas1={};
    tm cas2={};
    time_t timeSec1,timeSec2;
    cas1.tm_year=this->m_time.tm_year;
    cas1.tm_mon=this->m_time.tm_mon;
    cas1.tm_mday=this->m_time.tm_mday;
    cas2.tm_year=dat.m_time.tm_year;
    cas2.tm_mon=dat.m_time.tm_mon;
    cas2.tm_mday=dat.m_time.tm_mday;
    timeSec1=mktime(&cas1);
    timeSec2=mktime(&cas2);
    if (timeSec1<=timeSec2) return true;
    else return false;
}

bool CDate::operator>(CDate dat) const
{
    tm cas1={};
    tm cas2={};
    time_t timeSec1,timeSec2;
    cas1.tm_year=this->m_time.tm_year;
    cas1.tm_mon=this->m_time.tm_mon;
    cas1.tm_mday=this->m_time.tm_mday;
    cas2.tm_year=dat.m_time.tm_year;
    cas2.tm_mon=dat.m_time.tm_mon;
    cas2.tm_mday=dat.m_time.tm_mday;
    timeSec1=mktime(&cas1);
    timeSec2=mktime(&cas2);
    if (timeSec1>timeSec2) return true;
    else return false;
}

bool CDate::operator>=(CDate dat) const
{
    tm cas1={};
    tm cas2={};
    time_t timeSec1,timeSec2;
    cas1.tm_year=this->m_time.tm_year;
    cas1.tm_mon=this->m_time.tm_mon;
    cas1.tm_mday=this->m_time.tm_mday;
    cas2.tm_year=dat.m_time.tm_year;
    cas2.tm_mon=dat.m_time.tm_mon;
    cas2.tm_mday=dat.m_time.tm_mday;
    timeSec1=mktime(&cas1);
    timeSec2=mktime(&cas2);
    if (timeSec1>=timeSec2) return true;
    else return false;
}

CDate CDate::operator+(int cislo) const
{
    CDate datum;
    time_t timeSec;
    struct tm *prevod;
    datum.m_time.tm_year=this->m_time.tm_year;
    datum.m_time.tm_mon=this->m_time.tm_mon;
    datum.m_time.tm_mday=this->m_time.tm_mday;

    timeSec=mktime(&datum.m_time);
    timeSec+=(cislo*86400);
    timeSec+=86400;

    prevod=gmtime(&timeSec);
    datum.m_time.tm_year = prevod->tm_year;
    datum.m_time.tm_mon = prevod->tm_mon;
    datum.m_time.tm_mday = prevod->tm_mday;
    return datum;
}

CDate CDate::operator-(int cislo) const
{
    CDate datum;
    time_t timeSec;
    struct tm *prevod;
    datum.m_time.tm_year=this->m_time.tm_year;
    datum.m_time.tm_mon=this->m_time.tm_mon;
    datum.m_time.tm_mday=this->m_time.tm_mday;

    timeSec=mktime(&datum.m_time);
    timeSec-=(cislo*86400);
    timeSec+=86400;

    prevod=gmtime(&timeSec);
    datum.m_time.tm_year = prevod->tm_year;
    datum.m_time.tm_mon = prevod->tm_mon;
    datum.m_time.tm_mday = prevod->tm_mday;
    return datum;
}

int CDate::operator-(CDate dat) const
{
    tm cas1={};
    tm cas2={};
    time_t timeSec1,timeSec2,res;
    cas1.tm_year=this->m_time.tm_year;
    cas1.tm_mon=this->m_time.tm_mon;
    cas1.tm_mday=this->m_time.tm_mday;
    cas2.tm_year=dat.m_time.tm_year;
    cas2.tm_mon=dat.m_time.tm_mon;
    cas2.tm_mday=dat.m_time.tm_mday;

    timeSec1=mktime(&cas1);
    timeSec2=mktime(&cas2);
    res=abs(timeSec1-timeSec2);
    return (int)(res/86400);
}

CDate & CDate::operator++()
{
    tm cas = {};
    time_t timeSec;
    struct tm *prevod;
    cas.tm_year=this->m_time.tm_year;
    cas.tm_mon=this->m_time.tm_mon;
    cas.tm_mday=this->m_time.tm_mday;

    timeSec=mktime(&cas);
    timeSec+=(2*86400);

    prevod=gmtime(&timeSec);
    this->m_time.tm_year = prevod->tm_year;
    this->m_time.tm_mon = prevod->tm_mon;
    this->m_time.tm_mday = prevod->tm_mday;

    return *this;
}

CDate CDate::operator++(int)
{
    CDate datum;
    time_t timeSec;
    struct tm *prevod;
    datum.m_time.tm_year=this->m_time.tm_year;
    datum.m_time.tm_mon=this->m_time.tm_mon;
    datum.m_time.tm_mday=this->m_time.tm_mday;

    timeSec=mktime(&datum.m_time);
    timeSec+=(2*86400);

    prevod=gmtime(&timeSec);
    this->m_time.tm_year = prevod->tm_year;
    this->m_time.tm_mon = prevod->tm_mon;
    this->m_time.tm_mday = prevod->tm_mday;
    return datum;
}

CDate CDate::operator--(int)
{
    CDate datum;
    time_t timeSec;
    struct tm *prevod;
    datum.m_time.tm_year=this->m_time.tm_year;
    datum.m_time.tm_mon=this->m_time.tm_mon;
    datum.m_time.tm_mday=this->m_time.tm_mday;

    timeSec=mktime(&datum.m_time);

    prevod=gmtime(&timeSec);
    this->m_time.tm_year = prevod->tm_year;
    this->m_time.tm_mon = prevod->tm_mon;
    this->m_time.tm_mday = prevod->tm_mday;
    return datum;
}

CDate & CDate::operator--()
{
    tm cas = {};
    time_t timeSec;
    struct tm *prevod;
    cas.tm_year=this->m_time.tm_year;
    cas.tm_mon=this->m_time.tm_mon;
    cas.tm_mday=this->m_time.tm_mday;

    timeSec=mktime(&cas);

    prevod=gmtime(&timeSec);
    this->m_time.tm_year = prevod->tm_year;
    this->m_time.tm_mon = prevod->tm_mon;
    this->m_time.tm_mday = prevod->tm_mday;

    return *this;
}




#ifndef __PROGTEST__
 int main(void)
 {
    return 0;
 }
#endif /* __PROGTEST__ */
