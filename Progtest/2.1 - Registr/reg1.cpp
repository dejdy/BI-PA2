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

struct car
{
    string rz;
    string jmeno;
    string prijmeni;
};

class CCarList
 {
   public:
   CCarList();
    string    RZ         ( void ) const;
    bool      AtEnd      ( void ) const;
    void      Next       ( void );
    void addCar (const string & rz);
   private:
    vector<string> m_CarList;
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
    return m_CarList[current_pos];
}

void CCarList::Next (void)
{
    current_pos++;
}

void CCarList::addCar (const string & rz)
{
    m_CarList.push_back(rz);
}

class CRegister
 {
   public:
              CRegister  ( void );
             ~CRegister  ( void );
    bool      AddCar     ( const string & rz,
                           const string & name,
                           const string & surname );
    bool      DelCar     ( const string & rz );
    bool      Transfer   ( const string & rz,
                           const string & nName,
                           const string & nSurname);
    CCarList  ListCars   ( const string & name,
                           const string & surname ) const;
    int       CountCars  ( const string & name,
                           const string & surname ) const;
   private:
        vector<car> m_Car_RZ; //sorted by RZ
        vector<car> m_Car_Sur; // sorted by Surname
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


CCarList CRegister::ListCars(const string & name, const string & surname) const
{
    CCarList newList;
    unsigned int i;
    car temp;
    temp.jmeno=name;
    temp.prijmeni=surname;

    auto lowerSur=std::lower_bound (m_Car_Sur.begin(), m_Car_Sur.end(), temp , Comparef);
    i = lowerSur-m_Car_Sur.begin();
    for(;i<m_Car_Sur.size();i++)
    {
        if(m_Car_Sur[i].jmeno==name && m_Car_Sur[i].prijmeni==surname) newList.addCar(m_Car_Sur[i].rz);
        else break;
    }

    return newList;
}



CRegister::CRegister() // Constructor
{

}

CRegister::~CRegister() // Destructor
{
}

 bool CRegister::AddCar(const string & rz, const string & name, const string & surname) // Adds car to vectors, to it's places
 {
    vector<car>::iterator lowerRZ;
    vector<car>::iterator lowerSur;
    car temp;
    temp.jmeno=name;
    temp.prijmeni=surname;
    temp.rz=rz;

    if (std::binary_search (m_Car_RZ.begin(), m_Car_RZ.end(), temp, Comparef2)) return false; // Checks for existing rz, returns false if duplicity is about to be inserted

        lowerRZ=std::lower_bound (m_Car_RZ.begin(), m_Car_RZ.end(), temp, Comparef2);
        m_Car_RZ.insert(lowerRZ,temp);

        lowerSur=std::lower_bound (m_Car_Sur.begin(), m_Car_Sur.end(), temp , Comparef);
        m_Car_Sur.insert(lowerSur,temp);

    return true;
 }

bool CRegister::DelCar(const string & rz)
{
    int i;
    int velikost = m_Car_Sur.size();
    vector<car>::iterator lowerRZ;
    vector<car>::iterator lowerSur;
    car temp;
    temp.rz = rz;
    if (!std::binary_search (m_Car_RZ.begin(), m_Car_RZ.end(), temp, Comparef2)) return false; // Returns false if car was not found

    lowerRZ=std::lower_bound (m_Car_RZ.begin(), m_Car_RZ.end(), temp, Comparef2);
    temp.prijmeni=m_Car_RZ[lowerRZ-m_Car_RZ.begin()].prijmeni; // abych vedel, kde hledat v druhem poli :D
    temp.jmeno=m_Car_RZ[lowerRZ-m_Car_RZ.begin()].jmeno; // abych vedel, kde hledat v druhem poli :D
    m_Car_RZ.erase(lowerRZ);

    lowerSur=std::lower_bound (m_Car_Sur.begin(), m_Car_Sur.end(), temp , Comparef);
    i = lowerSur-m_Car_Sur.begin();


    for(;i<velikost;i++)
    {
        if (m_Car_Sur[i].rz==rz)
        {
            m_Car_Sur.erase(i+m_Car_Sur.begin());
            return true;
        }
    }
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
    car temp;
    temp.rz = rz;
    if (!std::binary_search (m_Car_RZ.begin(), m_Car_RZ.end(), temp, Comparef2)) return false; // Returns false if car was not found

    lowerRZ=std::lower_bound (m_Car_RZ.begin(), m_Car_RZ.end(), temp, Comparef2);

    temp.prijmeni=m_Car_RZ[lowerRZ-m_Car_RZ.begin()].prijmeni; // abych vedel, kde hledat v druhem poli :D
    temp.jmeno=m_Car_RZ[lowerRZ-m_Car_RZ.begin()].jmeno; // abych vedel, kde hledat v druhem poli :D
    if(temp.jmeno==nName && temp.prijmeni==nSurname) return false; // Returns false if car is about to be transfered to it's current owner

    m_Car_RZ[lowerRZ-m_Car_RZ.begin()].jmeno = nName;
    m_Car_RZ[lowerRZ-m_Car_RZ.begin()].prijmeni = nSurname;

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

#ifndef __PROGTEST__
int main ( void )
 {

  return 0;
 }
#endif /* __PROGTEST__ */

