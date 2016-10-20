#ifndef __PROGTEST__
#include <cstdio>
#include <assert.h>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;
#endif /* __PROGTEST__ */

class Excep{};

bool allOK=true;

struct coor
{
    int s_stupne;
    double s_min;
    double s_vter;
    int d_stupne;
    double d_min;
    double d_vter;
    char sirk,delk;
};

struct radians
{
    double lat;
    double lon;
};

class CCoord
{
    public:
    CCoord ( double, double);
    CCoord (const char *);
    friend ostream& operator<<(ostream&, const CCoord&);
    friend double countDistance(const CCoord&, const CCoord&);
    struct coor Convert(double, double);

    //private:
    coor m_souradnice;
};
 // (50 6'1.361"N, 14 23'33.045"E)
ostream& operator<<(ostream& os, const CCoord& coordin)
{
    char delka=coordin.m_souradnice.delk;
    if(delka=='F') delka='E';
    os << "(" << coordin.m_souradnice.s_stupne << " " << coordin.m_souradnice.s_min << "'" << round(coordin.m_souradnice.s_vter*1000)/1000 << "\"" << coordin.m_souradnice.sirk << ", ";
    os << coordin.m_souradnice.d_stupne << " " << coordin.m_souradnice.d_min << "'" << round(coordin.m_souradnice.d_vter*1000)/1000 << "\"" << delka << ")";
    return os;
}

CCoord::CCoord (double sirka, double delka)
{
    struct coor temp;
    temp=Convert(sirka, delka);
    m_souradnice.s_stupne=temp.s_stupne;
    m_souradnice.s_min=temp.s_min;
    m_souradnice.s_vter=(round(temp.s_vter*1000)/1000);

    m_souradnice.d_stupne=temp.d_stupne;
    m_souradnice.d_min=temp.d_min;
    m_souradnice.d_vter=(round(temp.d_vter*1000)/1000);
    if(delka>0) m_souradnice.sirk='N';
    else m_souradnice.sirk='S';
    if(sirka>0) m_souradnice.delk='F';
    else m_souradnice.delk='W';
}

struct coor CCoord::Convert(double sirka, double delka)
{
    struct coor temp;
    temp.s_stupne=abs((int)sirka);
    temp.s_min=abs((int)(60*(sirka-(int)sirka)));
    temp.s_vter=round(abs((3600*(sirka-(int)sirka-(temp.s_min/60))))*1000)/1000;
    if(round(temp.s_vter)==60) {temp.s_min++; temp.s_vter=0;}
    if(round(temp.s_min)==60) {temp.s_stupne++; temp.s_min=0;}

    temp.d_stupne=abs((int)delka);
    temp.d_min=abs((int)(60*(delka-(int)delka)));
    temp.d_vter=round(abs((3600*(delka-temp.d_stupne-(temp.d_min/60))))*1000)/1000;
    if(round(temp.d_vter)==60) {temp.d_min++; temp.d_vter=0;}
    if(round(temp.d_min)==60) {temp.d_stupne++; temp.d_min=0;}

    return temp;
}

//(50.1003781N, 14.3925125E)   50.084202, 14.423357   (50 5'3.127"N, 14 25'24.085"E)

CCoord::CCoord(const char * souradnice)
{
    string coor = souradnice;
    replace( coor.begin(), coor.end(), 'E', 'F');
    stringstream stream2(coor);
    stream2.clear();
    char delimiter='n',sir='A',del='A';
    double sirka=0, delka=0;
    struct coor temp;
    m_souradnice.d_vter=0;
    m_souradnice.s_vter=0;
    m_souradnice.d_min=0;
    m_souradnice.s_min=0;
    m_souradnice.d_stupne=0;
    m_souradnice.s_stupne=0;
    m_souradnice.sirk='N';
    m_souradnice.delk='W';
    temp.d_vter=0;
    temp.s_vter=0;
    temp.d_min=0;
    temp.s_min=0;
    temp.d_stupne=0;
    temp.s_stupne=0;
    temp.sirk='N';
    temp.delk='W';

    stream2 >> delimiter; // zavorka - (
    if(delimiter!='(') allOK=false;
    stream2 >> sirka;
    stream2 >> sir;
    if(sir!='S' && sir!='W' && sir!='N' && sir!='F') // druhy format
    {
        stringstream stream(coor);
        stream.clear();
        stream >> delimiter; // zavorka - (
        if(delimiter!='(') allOK=false;
        stream >> m_souradnice.s_stupne;
        if(m_souradnice.s_stupne>90) allOK=false;
        stream >> m_souradnice.s_min;
        if(m_souradnice.s_min>59) allOK=false;
        stream >> delimiter; // '
        if(delimiter!='\'') allOK=false;
        stream >> m_souradnice.s_vter;
        if(round((m_souradnice.s_vter*1000)/1000)>=60) allOK=false;
        stream >> delimiter; // "
        if(delimiter!='\"') allOK=false;
        stream >> m_souradnice.sirk;
        if(m_souradnice.sirk!='N' && m_souradnice.sirk!='S') allOK=false;
        stream >> delimiter;
        if(delimiter!=',') allOK=false;
// (50 5'3.127"N, 14 25'24.085"E)
        stream >> m_souradnice.d_stupne;
        if(m_souradnice.d_stupne>180) allOK=false;
        stream >> m_souradnice.d_min;
        if(m_souradnice.d_min>59) allOK=false;
        stream >> delimiter; // '
        if(delimiter!='\'') allOK=false;
        stream >> m_souradnice.d_vter;
        if(round((m_souradnice.d_vter*1000)/1000)>=60) allOK=false;
        stream >> delimiter; // "
        if(delimiter!='\"') allOK=false;
        stream >> m_souradnice.delk;
        if(m_souradnice.delk!='W' && m_souradnice.delk!='F') allOK=false;
        stream >> delimiter; // zavorka - );
        if(delimiter!=')') allOK=false;
        stream >> delimiter;

        m_souradnice.d_vter = round(m_souradnice.d_vter*1000)/1000;
        m_souradnice.s_vter = round(m_souradnice.s_vter*1000)/1000;

        if(m_souradnice.s_stupne==90 && (m_souradnice.s_min>0 || m_souradnice.s_vter>0)) allOK=false;
        if(m_souradnice.s_stupne<0 || m_souradnice.s_min<0 || m_souradnice.s_vter<0) allOK=false;
        if(m_souradnice.d_stupne<0 || m_souradnice.d_min<0 || m_souradnice.d_vter<0) allOK=false;
        if(m_souradnice.d_stupne==180 && (m_souradnice.d_min>0 || m_souradnice.d_vter>0)) allOK=false;


        if(!stream.eof()) allOK=false;

    }

    else
    {
        stringstream stream(coor);
        stream.clear();
        stream >> delimiter; // zavorka - (
        if(delimiter!='(') allOK=false;
        stream >> sirka;
        if(sirka>90) allOK=false;
        stream >> sir;
        if(sir!='N' && sir!='S') allOK=false;
        m_souradnice.sirk=sir;
        stream >> delimiter;
        if(delimiter!=',') allOK=false;
        stream >> delka;
        if(delka>=180) allOK=false;
        stream >> del;
        if(del!='W' && del!='F') allOK=false;
        m_souradnice.delk=del;
        temp=Convert(sirka, delka);
        m_souradnice.d_min=temp.d_min;
        m_souradnice.d_vter=(round(temp.d_vter*1000)/1000);
        m_souradnice.d_stupne=temp.d_stupne;

    if(round(m_souradnice.d_vter)==60) {temp.d_min++; temp.d_vter=0;}
    if(round(m_souradnice.d_min)==60) {temp.d_stupne++; temp.d_min=0;}

        m_souradnice.s_min=temp.s_min;
        m_souradnice.s_vter=(round(temp.s_vter*1000)/1000);
        m_souradnice.s_stupne=temp.s_stupne;

    if(round(m_souradnice.s_vter)==60) {temp.s_min++; temp.s_vter=0;}
    if(round(m_souradnice.s_min)==60) {temp.s_stupne++; temp.s_min=0;}


    }
}

class CGPS
{
    public:
    CGPS(){}
    ~CGPS(){}
    CGPS& Add(const CCoord& coord);
    CGPS& Add(const char * coords);
    CGPS& operator+=(const char *);
    CGPS& operator+=(const CCoord &coords);
    CCoord& operator[](int);

    CGPS operator()(int,int) const;
    double Distance() const;
    friend ostream& operator<<(ostream&, const CGPS&);
    friend istream& operator>>(istream&, CGPS&);
    struct radians toRad(int ind) const;
    private:
    vector<CCoord> zaznamnik;
};

struct radians CGPS::toRad(int ind) const
{
    struct radians temp;
    temp.lat = (this->zaznamnik[ind].m_souradnice.s_stupne + this->zaznamnik[ind].m_souradnice.s_min/60 + this->zaznamnik[ind].m_souradnice.s_vter/3600);
    temp.lon = (this->zaznamnik[ind].m_souradnice.d_stupne + this->zaznamnik[ind].m_souradnice.d_min/60 + this->zaznamnik[ind].m_souradnice.d_vter/3600);
    temp.lat*=(M_PI/180);
    temp.lon*=(M_PI/180);

    return temp;
}

double CGPS::Distance() const
{
    double soucet=0;
    double dlon, dlat,a,c,d;

    for(unsigned int i=0; i<this->zaznamnik.size()-1; i++)
    {
        struct radians temp1, temp2;
        temp1 = this->toRad(i);
        temp2 = this->toRad(i+1);
        if(this->zaznamnik[i].m_souradnice.sirk=='S') temp1.lat*=-1;
        if(this->zaznamnik[i].m_souradnice.delk=='W') temp1.lon*=-1;
        if(this->zaznamnik[i+1].m_souradnice.sirk=='S') temp2.lat*=-1;
        if(this->zaznamnik[i+1].m_souradnice.delk=='W') temp2.lon*=-1;
        dlon = temp2.lon - temp1.lon;
        dlat = temp2.lat - temp1.lat;
        a = (sin(dlat/2)) * (sin(dlat/2)) + cos(temp1.lat) * cos(temp2.lat) * (sin(dlon/2)) * (sin(dlon/2));
        c = 2 * atan2(sqrt(a), sqrt(1-a));
        d = 6371000 * c;
        soucet+=d;
    }

    return soucet;
}

CGPS CGPS::operator()(int a, int b) const
{
    CGPS newinst;
    if(a<0 || a>(int)this->zaznamnik.size()-1 || b<a || b>(int)this->zaznamnik.size()-1 || b<0) throw Excep();
    for(int i=a; i<=b; i++)
        newinst.zaznamnik.push_back(this->zaznamnik[i]);

    return newinst;
}

ostream& operator<<(ostream& os, const CGPS& coordin)
{
    os << "[";
    for(unsigned int i=0; i<coordin.zaznamnik.size(); i++)
    {
        os << coordin.zaznamnik[i];
        if(i<coordin.zaznamnik.size()-1) os << " > ";
    }
    os << "]";
    os << endl;
    return os;
}

CGPS& CGPS::Add(const CCoord& coord)
{
    zaznamnik.push_back(coord);
    return *this;
}

CGPS& CGPS::Add(const char * coords)
{
    CCoord tempor(coords);
    zaznamnik.push_back(tempor);
    return *this;
}

CGPS& CGPS::operator+=(const CCoord& coords)
{
    zaznamnik.push_back(coords);
    return *this;
}

CGPS& CGPS::operator+=(const char * coords)
{
    CCoord tempor(coords);
    zaznamnik.push_back(tempor);
    return *this;
}

CCoord& CGPS::operator[](int index)
{
    if(index<0) throw Excep();
    if(index>(int)zaznamnik.size()-1) throw Excep();
    return this->zaznamnik[index];
}

istream& operator>>(istream& is, CGPS& coor)
{
    // [(50 6\'16.5\"N, 14 23\'20.25\" E) > (51.5N, 0.0E) > (33.9S, 151.2E) > (37.42190N, 122.08405W) > (0.00N, 50.000W)]
    allOK=true;
    string sour;
    vector<string> coordlist;
    vector<CCoord> backup;

    for(unsigned int i=0; i<coor.zaznamnik.size(); i++) // zaloha
        backup.push_back(coor.zaznamnik[i]);

    coor.zaznamnik.clear();

    while(getline(is, sour, '>'))
    {
       coordlist.push_back(sour);
    }

is.clear();

for (unsigned int i=0; i< coordlist.size(); i++)
    {
        if(i==0)
        {
            if(coordlist[0][0]=='[') coordlist[i].erase(0,1);
            else
            {
                coor.zaznamnik.clear();
                is.setstate(ios::failbit);
                for (unsigned int i=0; i<backup.size(); i++) coor.zaznamnik.push_back(backup[i]);
                return is;
            }
        }

        if(i==coordlist.size()-1) // kdyz jsem na konci
        {
            if(coordlist[i][coordlist[i].length()-1]==']') coordlist[i].erase(coordlist[i].length()-1,coordlist[i].length());
            else
            {
                coor.zaznamnik.clear();
                is.setstate(ios::failbit);
                for (unsigned int i=0; i<backup.size(); i++) coor.zaznamnik.push_back(backup[i]);
                return is;
            }
        }
        coor.Add(coordlist[i].c_str());
    }


            if(allOK==false)
            {
                is.setstate(ios::failbit);
                coor.zaznamnik.clear();
                for (unsigned int i=0; i<backup.size(); i++) coor.zaznamnik.push_back(backup[i]);
                return is;
            }

    return is;
}


#ifndef __PROGTEST__
int main()
{


double d;
CGPS x0;
cout << x0;
 // []

x0.Add("(13 4'8.123\"N, 54 3'23.87\"E)");
x0.Add("(13 4'8.123\"S, 54 3'23.87\"W)");
cout << x0;
cout << endl << fixed << x0.Distance();

	return 0;
}
#endif /* __PROGTEST__ */

