#ifndef __PROGTEST__
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <string>
#include <memory>
#include <vector>
#include <algorithm>
using namespace std;
#endif /* __PROGTEST__ */

struct rzVin
{
    string rz;
    unsigned int vin;
};

struct history
{
    string prevName;
    string prevSurname;
};

struct car
{
    string rz;
    unsigned int vin;
    string jmeno;
    string prijmeni;
    vector<history> pastOwners;
};


class CCarList
 {
   public:
   CCarList();
    string    RZ         ( void ) const;
    unsigned int   VIN   ( void ) const;
    bool      AtEnd      ( void ) const;
    void      Next       ( void );
    void addCar (rzVin);
   private:
    vector<rzVin> m_CarList;
    int current_pos;
 };

CCarList::CCarList()
{
    current_pos=0;
}


bool CCarList::AtEnd (void) const
{
    if (current_pos==(int)m_CarList.size()) return true;
    else return false;
}

string CCarList::RZ (void) const
{
    return m_CarList[current_pos].rz;
}

unsigned int CCarList::VIN (void) const
{
    return m_CarList[current_pos].vin;
}


void CCarList::Next (void)
{
    current_pos++;
}

void CCarList::addCar (rzVin znacka)
{
    m_CarList.push_back(znacka);
}

class COwnerList
 {
   public:
    COwnerList();
    string         Name         ( void ) const;
    string         Surname      ( void ) const;
    bool           AtEnd        ( void ) const;
    void           Next         ( void );
    void addRecord(history);
    int current;
   private:
    vector<history> m_OwnList;

 };

COwnerList::COwnerList()
{
    current=0;
}

void COwnerList::Next(void)
{
    current--;
}

bool COwnerList::AtEnd(void) const
{
    if (current<1) return true;
    else return false;
}

string COwnerList::Name(void) const
{
    return m_OwnList[current-1].prevName;
}

string COwnerList::Surname(void) const
{
    return m_OwnList[current-1].prevSurname;
}

void COwnerList::addRecord(history hist)
{
    m_OwnList.push_back(hist);
}


class CRegister
 {
   public:
                   CRegister    ( void );
                  ~CRegister    ( void );
    bool           AddCar       ( const string    & rz,
                                  unsigned int      vin,
                                  const string    & name,
                                  const string    & surname );
    bool           DelCar       ( const string    & rz );
    bool           DelCar       ( unsigned int      vin );
    bool           Transfer     ( const string    & rz,
                                  const string    & nName,
                                  const string    & nSurname );
    bool           Transfer     ( unsigned int      vin,
                                  const string    & nName,
                                  const string    & nSurname );
    CCarList       ListCars     ( const string    & name,
                                  const string    & surname ) const;
    int            CountCars    ( const string    & name,
                                  const string    & surname ) const;
    COwnerList     ListOwners   ( const string    & RZ ) const;
    int            CountOwners  ( const string    & RZ ) const;
    COwnerList     ListOwners   ( unsigned int      vin ) const;
    int            CountOwners  ( unsigned int      vin ) const;
   private:
        vector<car> m_Car_RZ; //sorted by RZ
        vector<car> m_Car_Sur; // sorted by Surname
        vector<car> m_Car_Vin;
 };

bool Comparef(const car & a, const car & b) // Compares by surname, then by name
{
    if(a.prijmeni!=b.prijmeni) return a.prijmeni<b.prijmeni;
    else
    {
        return a.jmeno<b.jmeno;
    }
    return true;
}

bool Comparef2(const car & a, const car & b) // Compares by RZ
{
    return a.rz<b.rz;
}

bool Comparef3(const car & a, const car & b)
{
    return a.vin<b.vin;
}

COwnerList CRegister::ListOwners (const string & RZ) const
{
    COwnerList newList;
    car temp;
    history hist;
    temp.rz=RZ;
    if (!std::binary_search (m_Car_RZ.begin(), m_Car_RZ.end(), temp, Comparef2)) return newList;
    auto lowerRZ=std::lower_bound (m_Car_RZ.begin(), m_Car_RZ.end(), temp , Comparef2);
    int index = lowerRZ-m_Car_RZ.begin();
    int vel=m_Car_RZ[index].pastOwners.size();
    for(int i=0;i<vel;i++)
    {
        newList.addRecord(m_Car_RZ.at(index).pastOwners[i]);
    }
    newList.current=m_Car_RZ.at(index).pastOwners.size();
    return newList;
}

COwnerList CRegister::ListOwners (unsigned int vin) const
{
    COwnerList newList;
    car temp;
    history hist;
    temp.vin=vin;
    if (!std::binary_search (m_Car_Vin.begin(), m_Car_Vin.end(), temp, Comparef3)) return newList;
    auto lowerVin=std::lower_bound (m_Car_Vin.begin(), m_Car_Vin.end(), temp , Comparef3);
    int index = lowerVin-m_Car_Vin.begin();
    int vel=m_Car_Vin[index].pastOwners.size();
    for(int i=0;i<vel;i++)
    {
        newList.addRecord(m_Car_Vin.at(index).pastOwners[i]);
    }
    newList.current=m_Car_Vin.at(index).pastOwners.size();
    return newList;
}

CCarList CRegister::ListCars(const string & name, const string & surname) const
{
    CCarList newList2;
    unsigned int i;
    car temp;
    temp.jmeno=name;
    temp.prijmeni=surname;
    rzVin znacka;

    auto lowerSur=std::lower_bound (m_Car_Sur.begin(), m_Car_Sur.end(), temp , Comparef);
    i = lowerSur-m_Car_Sur.begin();
    for(;i<m_Car_Sur.size();i++)
    {
        if(m_Car_Sur[i].jmeno==name && m_Car_Sur[i].prijmeni==surname)
        {
            znacka.rz=m_Car_Sur[i].rz;
            znacka.vin=m_Car_Sur[i].vin;
            newList2.addCar(znacka);
        }
        else break;
    }

    return newList2;
}

CRegister::CRegister() // Constructor
{

}

CRegister::~CRegister() // Destructor
{
}

 bool CRegister::AddCar(const string & rz, unsigned int vin, const string & name, const string & surname) // Adds car to vectors, to it's places
 {
    vector<car>::iterator lowerRZ;
    vector<car>::iterator lowerSur;
    vector<car>::iterator lowerVin;
    int index;
    car temp;
    temp.jmeno=name;
    temp.prijmeni=surname;
    temp.rz=rz;
    temp.vin=vin;
    history hist;
    hist.prevName=name;
    hist.prevSurname=surname;

    if (std::binary_search (m_Car_RZ.begin(), m_Car_RZ.end(), temp, Comparef2)) return false; // Checks for existing rz, returns false if duplicity is about to be inserted
    if (std::binary_search (m_Car_Vin.begin(), m_Car_Vin.end(), temp, Comparef3)) return false;

        lowerRZ=std::lower_bound (m_Car_RZ.begin(), m_Car_RZ.end(), temp, Comparef2);
        index=lowerRZ-m_Car_RZ.begin();
        m_Car_RZ.insert(lowerRZ,temp);
        m_Car_RZ[index].pastOwners.push_back(hist);

        lowerSur=std::lower_bound (m_Car_Sur.begin(), m_Car_Sur.end(), temp , Comparef);
        m_Car_Sur.insert(lowerSur,temp);

        lowerVin=std::lower_bound (m_Car_Vin.begin(), m_Car_Vin.end(), temp , Comparef3);
        index=lowerVin-m_Car_Vin.begin();
        m_Car_Vin.insert(lowerVin,temp);
        m_Car_Vin[index].pastOwners.push_back(hist);

    return true;
 }

bool CRegister::DelCar(const string & rz)
{
    int i;
    int velikost = m_Car_Sur.size();
    vector<car>::iterator lowerRZ;
    vector<car>::iterator lowerSur;
    vector<car>::iterator lowerVin;
    car temp;
    temp.rz = rz;
    if (!std::binary_search (m_Car_RZ.begin(), m_Car_RZ.end(), temp, Comparef2)) return false; // Returns false if car was not found

    lowerRZ=std::lower_bound (m_Car_RZ.begin(), m_Car_RZ.end(), temp, Comparef2);
    temp.prijmeni=m_Car_RZ[lowerRZ-m_Car_RZ.begin()].prijmeni; // abych vedel, kde hledat v druhem poli :D
    temp.jmeno=m_Car_RZ[lowerRZ-m_Car_RZ.begin()].jmeno; // abych vedel, kde hledat v druhem poli :D
    temp.vin=m_Car_RZ[lowerRZ-m_Car_RZ.begin()].vin; // abych vedel, kde hledat v druhem poli :D
    m_Car_RZ.erase(lowerRZ);

    lowerSur=std::lower_bound (m_Car_Sur.begin(), m_Car_Sur.end(), temp , Comparef);
    i = lowerSur-m_Car_Sur.begin();

    for(;i<velikost;i++)
    {
        if (m_Car_Sur[i].rz==rz)
        {
            m_Car_Sur.erase(i+m_Car_Sur.begin());
            break;
        }
    }

    lowerVin=std::lower_bound (m_Car_Vin.begin(), m_Car_Vin.end(), temp, Comparef3);
    m_Car_Vin.erase(lowerVin);

    return true;
}

bool CRegister::DelCar(unsigned int vin)
{
    int i;
    int velikost = m_Car_Sur.size();
    vector<car>::iterator lowerRZ;
    vector<car>::iterator lowerSur;
    vector<car>::iterator lowerVin;
    car temp;
    temp.vin = vin;
    if (!std::binary_search (m_Car_Vin.begin(), m_Car_Vin.end(), temp, Comparef3)) return false; // Returns false if car was not found

    lowerVin=std::lower_bound (m_Car_Vin.begin(), m_Car_Vin.end(), temp, Comparef3);

    temp.prijmeni=m_Car_Vin[lowerVin-m_Car_Vin.begin()].prijmeni; // abych vedel, kde hledat v druhem poli :D
    temp.jmeno=m_Car_Vin[lowerVin-m_Car_Vin.begin()].jmeno; // abych vedel, kde hledat v druhem poli :D
    temp.rz=m_Car_Vin[lowerVin-m_Car_Vin.begin()].rz; // abych vedel, kde hledat v druhem poli :D
    m_Car_Vin.erase(lowerVin);

    lowerSur=std::lower_bound (m_Car_Sur.begin(), m_Car_Sur.end(), temp , Comparef);
    i = lowerSur-m_Car_Sur.begin();

    for(;i<velikost;i++)
    {
        if (m_Car_Sur[i].vin==vin)
        {
            m_Car_Sur.erase(i+m_Car_Sur.begin());
            break;
        }
    }

    lowerRZ=std::lower_bound (m_Car_RZ.begin(), m_Car_RZ.end(), temp, Comparef2);
    m_Car_RZ.erase(lowerRZ);

    return true;
}

int CRegister::CountCars ( const string & name, const string & surname ) const
{
    int numCars,i;
    int velikost = m_Car_Sur.size();
    numCars=0;
    car temp;
    temp.jmeno=name;
    temp.prijmeni=surname;

    auto lowerSur = std::lower_bound (m_Car_Sur.begin(), m_Car_Sur.end(), temp , Comparef);
    i = lowerSur-m_Car_Sur.begin();

    for(;i<velikost;i++)
    {
        if (m_Car_Sur[i].jmeno==name && m_Car_Sur[i].prijmeni==surname)
        {
            numCars++;
        }
        else return numCars;
    }
    return numCars;
}

bool CRegister::Transfer(const string & rz, const string & nName, const string & nSurname)
{
    int i;
    int velikost = m_Car_Sur.size();
    vector<car>::iterator lowerRZ;
    vector<car>::iterator lowerSur;
    vector<car>::iterator lowerVin;
    car temp;
    history now;
    now.prevName=nName;
    now.prevSurname=nSurname;
    temp.rz = rz;
    if (!std::binary_search (m_Car_RZ.begin(), m_Car_RZ.end(), temp, Comparef2)) return false; // Returns false if car was not found

    lowerRZ=std::lower_bound (m_Car_RZ.begin(), m_Car_RZ.end(), temp, Comparef2);

    temp.prijmeni=m_Car_RZ[lowerRZ-m_Car_RZ.begin()].prijmeni; // abych vedel, kde hledat v druhem poli :D
    temp.jmeno=m_Car_RZ[lowerRZ-m_Car_RZ.begin()].jmeno; // abych vedel, kde hledat v druhem poli :D
    temp.vin=m_Car_RZ[lowerRZ-m_Car_RZ.begin()].vin;

    if(temp.jmeno==nName && temp.prijmeni==nSurname) return false; // Returns false if car is about to be transfered to it's current owner
  // Pridani do historie
    m_Car_RZ[lowerRZ-m_Car_RZ.begin()].pastOwners.push_back(now);

    m_Car_RZ[lowerRZ-m_Car_RZ.begin()].jmeno = nName;
    m_Car_RZ[lowerRZ-m_Car_RZ.begin()].prijmeni = nSurname;

//***//

    lowerVin=std::lower_bound (m_Car_Vin.begin(), m_Car_Vin.end(), temp , Comparef3);
    m_Car_Vin[lowerVin-m_Car_Vin.begin()].pastOwners.push_back(now);

    m_Car_Vin[lowerVin-m_Car_Vin.begin()].jmeno = nName;
    m_Car_Vin[lowerVin-m_Car_Vin.begin()].prijmeni = nSurname;


//***//
    lowerSur=std::lower_bound (m_Car_Sur.begin(), m_Car_Sur.end(), temp , Comparef);
    i = lowerSur-m_Car_Sur.begin();
    for(;i<velikost;i++)                // Odebrani auta
    {
        if (m_Car_Sur[i].rz==rz)
        {
            m_Car_Sur.erase(i+m_Car_Sur.begin());
            break;
        }
    }
//***//

    temp.jmeno=nName;
    temp.prijmeni=nSurname;
    lowerSur=std::lower_bound (m_Car_Sur.begin(), m_Car_Sur.end(), temp , Comparef);
    m_Car_Sur.insert(lowerSur,temp); // Pridani auta


    return true;
}

bool CRegister::Transfer(unsigned int vin, const string & nName, const string & nSurname)
{
    int i;
    int velikost = m_Car_Sur.size();
    vector<car>::iterator lowerRZ;
    vector<car>::iterator lowerSur;
    vector<car>::iterator lowerVin;
    car temp;
    history now;
    now.prevName=nName;
    now.prevSurname=nSurname;
    temp.vin = vin;
    if (!std::binary_search (m_Car_Vin.begin(), m_Car_Vin.end(), temp, Comparef3)) return false; // Returns false if car was not found

    lowerVin=std::lower_bound (m_Car_Vin.begin(), m_Car_Vin.end(), temp, Comparef3);

    temp.prijmeni=m_Car_Vin[lowerVin-m_Car_Vin.begin()].prijmeni; // abych vedel, kde hledat v druhem poli :D
    temp.jmeno=m_Car_Vin[lowerVin-m_Car_Vin.begin()].jmeno; // abych vedel, kde hledat v druhem poli :D
    temp.rz=m_Car_Vin[lowerVin-m_Car_Vin.begin()].rz;


    if(temp.jmeno==nName && temp.prijmeni==nSurname) return false; // Returns false if car is about to be transfered to it's current owner


    m_Car_Vin[lowerVin-m_Car_Vin.begin()].pastOwners.push_back(now);

    m_Car_Vin[lowerVin-m_Car_Vin.begin()].jmeno = nName;
    m_Car_Vin[lowerVin-m_Car_Vin.begin()].prijmeni = nSurname;

//***//

    lowerRZ=std::lower_bound (m_Car_RZ.begin(), m_Car_RZ.end(), temp , Comparef2);

    m_Car_RZ[lowerRZ-m_Car_RZ.begin()].pastOwners.push_back(now);

    m_Car_RZ[lowerRZ-m_Car_RZ.begin()].jmeno = nName;
    m_Car_RZ[lowerRZ-m_Car_RZ.begin()].prijmeni = nSurname;


//***//
    lowerSur=std::lower_bound (m_Car_Sur.begin(), m_Car_Sur.end(), temp , Comparef);
    i = lowerSur-m_Car_Sur.begin();
    for(;i<velikost;i++)                // Odebrani auta
    {
        if (m_Car_Sur[i].vin==vin)
        {
            m_Car_Sur.erase(i+m_Car_Sur.begin());
            break;
        }
    }
//***//

    temp.jmeno=nName;
    temp.prijmeni=nSurname;
    lowerSur=std::lower_bound (m_Car_Sur.begin(), m_Car_Sur.end(), temp , Comparef);
    m_Car_Sur.insert(lowerSur,temp); // Pridani auta


    return true;
}

int CRegister::CountOwners(const string & RZ ) const
{
    car temp;
    temp.rz=RZ;
    if (!std::binary_search (m_Car_RZ.begin(), m_Car_RZ.end(), temp, Comparef2)) return 0;
    auto lowerRZ = std::lower_bound (m_Car_RZ.begin(), m_Car_RZ.end(), temp , Comparef2);
    int i = lowerRZ-m_Car_RZ.begin();
    return (int)m_Car_RZ[i].pastOwners.size();

}

int CRegister::CountOwners(unsigned int vin) const
{
    car temp;
    temp.vin=vin;
    if (!std::binary_search (m_Car_Vin.begin(), m_Car_Vin.end(), temp, Comparef3)) return 0;
    auto lowerVin = std::lower_bound (m_Car_Vin.begin(), m_Car_Vin.end(), temp , Comparef3);
    int i = lowerVin-m_Car_Vin.begin();
    return (int)m_Car_Vin[i].pastOwners.size();
}



#ifndef __PROGTEST__
int main ( void )
 {




  return 0;
 }
#endif /* __PROGTEST__ */

