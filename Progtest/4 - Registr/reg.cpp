#ifndef __PROGTEST__
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <iomanip>
using namespace std;
#endif /* __PROGTEST__ */

struct Car;
struct Owner;

class CRet // Jakoby string
{
    public:
    CRet() {m_ret = NULL; m_len=0;}
    CRet(const char *);
    CRet(const CRet&);
    ~CRet(){delete [] m_ret;}
    int getLen() const {return m_len;}
    char * getRet() const {return m_ret;}
    void setRet(char * newRet) {m_ret=newRet;}
    CRet& operator=(const CRet& a);
    CRet& operator=(const char * a);
    bool operator==(const CRet&) const;
    bool operator<(const CRet&) const;
    bool operator>(const CRet&) const;
    friend ostream& operator<<(ostream & os, CRet retez) {os << retez.getRet(); return os;}

    //private:
    char * m_ret;
    int m_len;
};

bool CRet::operator==(const CRet& druhy) const
{
    if(druhy.getLen()!=this->getLen()) return false;
    for(int i=0; i<druhy.getLen(); i++)
        if(*(this->m_ret+i) != (*(druhy.m_ret+i))) return false;

    return true;
}

bool CRet::operator<(const CRet& druhy) const
{
    int bound;
    if(druhy.getLen()>this->getLen()) bound = druhy.getLen();
    else bound = this->getLen();

    for(int i=0; i<bound; i++)
    {
        if((*(this->m_ret+i)) > (*(druhy.m_ret+i))) return false;
        if((*(this->m_ret+i)) < (*(druhy.m_ret+i))) return true;
    }

    return false;
}

bool CRet::operator>(const CRet& druhy) const
{
    int bound;
    if(druhy.getLen()>this->getLen()) bound = druhy.getLen();
    else bound = this->getLen();

    for(int i=0; i<bound; i++)
    {
        if((*(this->m_ret+i)) > (*(druhy.m_ret+i))) return true;
        if((*(this->m_ret+i)) < (*(druhy.m_ret+i))) return false;
    }

    return false;
}

CRet::CRet(const char * retezec) // Konstruktor pro retezec
{
    m_ret = NULL;
    m_len = 0;
    m_len = strlen(retezec);
    m_ret = new char[m_len+1];
    strcpy(m_ret, retezec);
}

CRet::CRet(const CRet & retezec) // Kopirujici kosntruktor
{
    m_ret = NULL;
    m_len = 0;
    m_len = retezec.m_len;
	m_ret = new char[m_len+1];
    strcpy(m_ret, retezec.m_ret);
}

CRet& CRet::operator=(const CRet & retezec) // operator =
{
    delete [] m_ret;
    m_len = retezec.m_len;
	m_ret = new char[m_len+1];
    strcpy(m_ret, retezec.m_ret);
    return *this;
}

CRet& CRet::operator=(const char * retezec) // operator =
{
    delete [] m_ret;
    m_len = strlen(retezec);
	m_ret = new char[m_len+1];
    strcpy(m_ret, retezec);
    return *this;
}

class CPoleUkC // pole ukazatelu na auta
{
    public:
    CPoleUkC() {m_allocated=10; m_arraySize=0; DATA = (struct Car **)malloc(10*sizeof(struct Car*));}
    ~CPoleUkC() {free(DATA);}
    void push(struct Car *);
    int binSearch(CRet) const;
    int lowBound(CRet) const;
    Car * operator[](int ind) const {return DATA[ind];}
    void insertRet(struct Car *);
    int getSize() const {return m_arraySize;}
    CPoleUkC& operator=(CPoleUkC&);
    void delRet(struct Car * zaznam);

    //private:
    struct Car ** DATA;
    int m_allocated;
    int m_arraySize;
};

class CPoleUkO // pole ukazatelu na majitele
{
    public:
    CPoleUkO() {m_allocated=10; m_arraySize=0; DATA = (struct Owner **)malloc(10*sizeof(struct Owner*));}
    ~CPoleUkO() {free(DATA);}
    void push(struct Owner *);
    int getSize() const {return m_arraySize;}
    int binSearch(const Owner *) const;
    Owner * operator[](int ind) const {return DATA[ind];}
    int lowBound(const Owner *) const;
    void insertRet(struct Owner * zaznam);
    CPoleUkO& operator=(CPoleUkO& source);
    void delRet(struct Owner * zaznam);

    //private:
    struct Owner ** DATA;
    int m_allocated;
    int m_arraySize;
};

struct Car
{
    CRet RZ;
    CPoleUkO CarOwner;
    Car * old;
    Car(Car & source) {old = &source; RZ = source.RZ;}
    Car(const char * rz) {RZ = rz;}
    Car(){};
    Car * getOld() {return old;}

};

CPoleUkC& CPoleUkC::operator=(CPoleUkC& source)
{
    free(DATA);
    DATA = (Car**)malloc(10*sizeof(Car*));
    this->m_allocated = 10;
    this->m_arraySize = 0;

    for(int i=0; i<source.getSize(); i++)
    {
        this->push(source.DATA[i]);
    }
    return *this;
}

void CPoleUkC::delRet(struct Car * zaznam)
{

    int index = this->binSearch(zaznam->RZ); // na tomto indexu je zaznam
    if(index==-1) return;
//    Car * delPtr = this->DATA[index];
    for(int i=index; i<this->getSize()-1; i++)
        this->DATA[i] = this->DATA[i+1];
    this->m_arraySize--;
}

struct Owner
{
    CRet name;
    CRet surname;
    CPoleUkC auticko;
    Owner * old;
    Owner(){}
    Owner(const char * Name, const char * Surname){name=Name; surname=Surname;}
    Owner(Owner& source){old = &source; name=source.name; surname=source.surname;}
    Owner * getOld() {return old;}
    // vector aur
};

void CPoleUkC::push(struct Car * record)
{
    if(m_arraySize==m_allocated) // v poli doslo misto
    {
        DATA = (struct Car **)realloc(DATA, 2*m_allocated*sizeof(struct Car *));
        m_allocated*=2;
    }
    DATA[m_arraySize]=NULL;
    DATA[m_arraySize] = record;
    m_arraySize++;
}

int CPoleUkC::binSearch(CRet RZ) const // Implementace binarniho vyhledavani
{
int imin = 0;
int imax = this->m_arraySize-1;

  while (imax >= imin)
    {
      int imid = imin + ((imax - imin) / 2);
      if(this->DATA[imid]->RZ == RZ)
        return imid;
      else if (this->DATA[imid]->RZ < RZ)
        imin = imid + 1;
      else
        imax = imid - 1;
    }
  return -1;
}

int CPoleUkC::lowBound(CRet RZ) const // Implementace binarniho vyhledavani
{
if(this->m_arraySize==0) return 0;

int imin = 0;
int imax = this->m_arraySize-1;
int imid = 0;

  while (imax >= imin)
    {
        imid = imin + ((imax - imin) / 2);
        if(this->DATA[imid]->RZ == RZ ) return imid;
        else if(this->DATA[imid]->RZ < RZ) imin = imid+1;
        else imax = imid-1;
    }

if((this->DATA[imid]->RZ > RZ) || (this->DATA[imid]->RZ == RZ)) return imid;
return imid+1;

}

void CPoleUkC::insertRet(struct Car * zaznam)
{
    int index = this->lowBound(zaznam->RZ); // na tento index je nutne umistit tento zaznam
    this->push(NULL);

    for(int i=this->getSize()-1; i>index; i--)
        this->DATA[i] = this->DATA[i-1];

    DATA[index] = zaznam;
}

/* ------------------------------------------------------------------------------------------------------*/

int cmp(const struct Owner * a, const struct Owner * b)
{
    if(a->surname > b->surname) return 1;
    else if(a->surname < b->surname) return -1;
    else
    {
        if(a->name > b->name) return 1;
        else if(a->name < b->name) return -1;
    }
return 0;
}

CPoleUkO& CPoleUkO::operator=(CPoleUkO& source)
{
    free(DATA);
    DATA = (Owner**)malloc(10*sizeof(Owner*));
    this->m_allocated = 10;
    this->m_arraySize = 0;

    for(int i=0; i<source.getSize(); i++)
    {
        this->push(source.DATA[i]);
    }
    return *this;
}

void CPoleUkO::push(struct Owner * record)
{
    if(m_arraySize==m_allocated) // v poli doslo misto
    {
        DATA = (struct Owner **)realloc(DATA, 2*m_allocated*sizeof(struct Owner *));
        m_allocated*=2;
    }
    DATA[m_arraySize]=NULL;
    DATA[m_arraySize] = record;
    m_arraySize++;
}

int CPoleUkO::binSearch(const Owner * majitel) const // Implementace binarniho vyhledavani
{
int imin = 0;
int imax = this->m_arraySize-1;

  while (imax >= imin)
    {
      int imid = imin + ((imax - imin) / 2);
      if(((this->DATA[imid]->surname) == (majitel->surname)) && ((this->DATA[imid]->name) == (majitel->name)))
        return imid;
      else if (cmp(this->DATA[imid], majitel)<0)
        imin = imid + 1;
      else
        imax = imid - 1;
    }
  return -1;
}

int CPoleUkO::lowBound(const Owner * majitel) const // Implementace binarniho vyhledavani
{

if(this->m_arraySize==0) return 0;

int imin = 0;
int imax = this->m_arraySize-1;
int imid = 0;

  while (imax >= imin)
    {
        imid = imin + ((imax - imin) / 2);
        if(cmp(this->DATA[imid], majitel) == 0) return imid;
        else if(cmp(this->DATA[imid], majitel) < 0) imin = imid+1;
        else imax = imid-1;
    }

if(cmp(this->DATA[imid], majitel) >= 0) return imid;
return imid+1;
}

void CPoleUkO::insertRet(struct Owner * zaznam)
{

    int index = this->lowBound(zaznam); // na tento index je nutne umistit tento zaznam
    this->push(NULL);

    for(int i=this->getSize()-1; i>index; i--)
        this->DATA[i] = this->DATA[i-1];

    DATA[index] = zaznam;
}

void CPoleUkO::delRet(struct Owner * zaznam)
{

    int index = this->binSearch(zaznam); // na tomto indexu je zaznam
    if(index==-1) return;

    for(int i=index; i<this->getSize()-1; i++)
        this->DATA[i] = this->DATA[i+1];

    this->m_arraySize--;
}

/* -------------------------------------------------------------------------------------------------------- */

class CCarList
 {
   public:
    // copy cons, op=, dtor ...
    CCarList() {current_pos=0; vel=0; m_carPtrs=NULL;vel=0;}
    ~CCarList() {}
    CCarList(CPoleUkC * src) {m_carPtrs=src; current_pos=0;vel=1;}
    const char   * RZ           ( void ) const;
    bool           AtEnd        ( void ) const;
    void           Next         ( void );
    CCarList& operator=(const CCarList & src) {vel=src.vel; current_pos=src.current_pos; m_carPtrs=src.m_carPtrs; return *this;}
    CCarList(const CCarList & src) {*this = src;}

    CPoleUkC * m_carPtrs;
    int vel;
    private:
    int current_pos;

 };

void CCarList::Next (void)
{
    current_pos++;
}

bool CCarList::AtEnd (void) const
{
    if(vel==0) return true;
    if(m_carPtrs->getSize()==0) return true;
    if (current_pos==m_carPtrs->getSize()) return true;
    else return false;
}

const char * CCarList::RZ (void) const
{
    return m_carPtrs->DATA[current_pos]->RZ.m_ret;
}

/* -------------------------------------------------------------------------------------------------------- */
class COwnerList
 {
   public:
    COwnerList () {current=0;m_ownerPtrs=NULL;vel2=0;}
    ~COwnerList() {}
    COwnerList(CPoleUkO *);
    COwnerList& operator=(const COwnerList & src) {vel2=src.vel2; current=src.current; m_ownerPtrs=src.m_ownerPtrs; return *this;}
    COwnerList(const COwnerList & src) {*this = src;}

    const char * Name         ( void ) const;
    const char * Surname      ( void ) const;
    bool AtEnd        ( void ) const;
    void Next         ( void );

    int vel2;
    private:
    CPoleUkO * m_ownerPtrs;
    int current;

 };

COwnerList::COwnerList(CPoleUkO * src)
{
    m_ownerPtrs=src;
    vel2=1;
    current=m_ownerPtrs->getSize();
}

void COwnerList::Next(void)
{
    current--;
}

bool COwnerList::AtEnd(void) const
{
    if(vel2==0) return true;
    if (current<1) return true;
    else return false;
}

const char * COwnerList::Name(void) const
{
    return m_ownerPtrs->DATA[current-1]->name.m_ret;
}

const char * COwnerList::Surname(void) const
{
    return m_ownerPtrs->DATA[current-1]->surname.m_ret;
}

/* -------------------------------------------------------------------------------------------------------- */

class CRegister
 {
   public:
   CRegister() {RC=1;}
    // copy cons
    bool           AddCar       ( const char      * rz,
                                  const char      * name,
                                  const char      * surname );
    bool           DelCar       ( const char      * rz );
    bool           Transfer     ( const char      * rz,
                                  const char      * nName,
                                  const char      * nSurname );

    int            CountCars    ( const char      * name,
                                  const char      * surname ) const;
    int            CountOwners  ( const char      * RZ ) const;

    CCarList       ListCars     ( const char      * name,
                                  const char      * surname ) const;
    COwnerList     ListOwners   ( const char      * RZ ) const;

    ~CRegister();
    CRegister(CRegister & novy) {*this = novy;};
    CRegister& operator=(CRegister&);
    CRegister& deepCopy(const CRegister& source);

   //private:
    CPoleUkC m_carPointers; // setridene pole pointeru na auta
    CPoleUkO m_ownerPointers; // setridene pole pointeru na ownery
    int RC;
 };

CRegister::~CRegister()
{
    for(int i=0; i<m_carPointers.getSize(); i++)
    {
        delete m_carPointers[i];
    }

    for(int i=0; i<m_ownerPointers.getSize(); i++)
    {
        delete m_ownerPointers[i];
    }

}

CRegister& CRegister::deepCopy(const CRegister& source)
{
for(int i=0; i<m_ownerPointers.getSize(); i++)
    delete m_ownerPointers[i];
free (m_ownerPointers.DATA);
m_ownerPointers.DATA = (Owner**)malloc(10*sizeof(Owner *));
m_ownerPointers.m_arraySize=0;
m_ownerPointers.m_allocated=10;

for(int i=0; i<m_carPointers.getSize(); i++)
    delete m_carPointers[i];
free (m_carPointers.DATA);
m_carPointers.DATA = (Car**)malloc(10*sizeof(Car *));
m_carPointers.m_arraySize=0;
m_carPointers.m_allocated=10;


	for(int i = 0; i < source.m_ownerPointers.getSize(); i++)
    {
        m_ownerPointers.insertRet(new Owner(*(source.m_ownerPointers[i])));
    }

	for(int i = 0; i < source.m_carPointers.getSize(); i++)
        m_carPointers.insertRet(new Car(*(source.m_carPointers[i])));

	Car * prev;
	CPoleUkO pastOwners;
    Owner * prev2;
	CPoleUkC pastCars;

	int index=0;
	for(int i=0; i<m_carPointers.getSize(); i++)
	{
		prev = m_carPointers[i]->getOld();
		pastOwners = prev->CarOwner;

		for(int j=0; j<pastOwners.getSize(); j++)
		{
			for(int k=0; m_ownerPointers.getSize(); k++)
            {
                if((m_ownerPointers[k]->name==pastOwners[j]->name) && (m_ownerPointers[k]->surname==pastOwners[j]->surname)) {index = k; break;}
            }
            m_carPointers[i]->CarOwner.push(m_ownerPointers[index]);
		}
	}

	for(int i=0; i<m_ownerPointers.getSize(); i++)
	{
		prev2 = m_ownerPointers[i]->getOld();
		pastCars = prev2->auticko;

		for(int j=0; j<pastCars.getSize(); j++)
		{
            for(int k=0; k<m_carPointers.getSize(); k++)
            {
                if(m_carPointers[k]->RZ==pastCars[j]->RZ) {index = k; break;}
            }
            m_ownerPointers[i]->auticko.insertRet(m_carPointers[index]);
		}
	}

    return *this;
}

CRegister& CRegister::operator=(CRegister& source)
{
    if(&source == this) return *this;
    *this = source;
    source.RC++;
    //else return deepCopy(source);
}

bool CRegister::AddCar (const char * rz, const char * name, const char * surname)
{

    this->RC--;

    CRegister * novy = new CRegister();
    &novy = deepCopy(*this);
    *this=novy;

    if(m_carPointers.binSearch(rz)!=-1) return false; // Pokud uz rz v databazi existuje

    Car * carPtr = new Car(rz);
    Owner * ownerPtr = new Owner (name, surname);

    ownerPtr->auticko.insertRet(carPtr);
    m_carPointers.insertRet(carPtr);

    int indexx=m_ownerPointers.binSearch(ownerPtr);
    if(indexx!=-1) // Pokud uz owner v databazi existuje
    {
        delete ownerPtr;
        carPtr->CarOwner.push((m_ownerPointers[indexx]));
        m_ownerPointers[indexx]->auticko.insertRet(carPtr);

    }
    else // Owner v databazi jeste neexistuje..
    {
        carPtr->CarOwner.push(ownerPtr);
        m_ownerPointers.insertRet(ownerPtr);
    }
    return true;
}

bool CRegister::DelCar(const char * rz )
{
    int index = m_carPointers.binSearch(rz);

    if(index==-1) return false; // Pokud rz v databazi neexistuje
    Owner temp;
    Car tempCar;
    tempCar.RZ=rz;

    temp.name = m_carPointers[index]->CarOwner[m_carPointers[index]->CarOwner.getSize()-1]->name;
    temp.surname = m_carPointers[index]->CarOwner[m_carPointers[index]->CarOwner.getSize()-1]->surname;

    m_ownerPointers[m_ownerPointers.binSearch(&temp)]->auticko.delRet(&tempCar);
    Car * delPtr = m_carPointers[m_carPointers.binSearch(rz)];
    m_carPointers.delRet(&tempCar);
    delete delPtr;

    return true;
}

bool CRegister::Transfer (const char * rz, const char * nName, const char * nSurname)
{
    int index = m_carPointers.binSearch(rz);
    if(index==-1) return false; // pokud automobil neexistuje
    Owner temp;
    Car tempCar;
    tempCar.RZ = rz;
    temp.name = nName;
    temp.surname = nSurname;

    int ownerIndex = m_ownerPointers.binSearch(&temp); // index ( existence ) noveho vlastnika

    if(ownerIndex==-1) // pokud owner neexistuje
    {


        m_carPointers[index]->CarOwner[m_carPointers[index]->CarOwner.getSize()-1]->auticko.delRet(&tempCar);
        Owner * ownerPtr = new Owner();
        ownerPtr->name=nName;
        ownerPtr->surname=nSurname;
        ownerPtr->auticko.insertRet(m_carPointers[index]);
        m_carPointers[index]->CarOwner.push(ownerPtr);
        m_ownerPointers.insertRet(ownerPtr);
    }

    else // pokud owner existuje
    {
        if((m_ownerPointers[ownerIndex]->name==nName) && (m_ownerPointers[ownerIndex]->surname==nSurname))
        {
            if(m_ownerPointers[ownerIndex]->auticko.binSearch(rz)!=-1) return false;
        }

        m_ownerPointers[ownerIndex]->auticko.insertRet(m_carPointers[index]);
        m_carPointers[index]->CarOwner[m_carPointers[index]->CarOwner.getSize()-1]->auticko.delRet(&tempCar);
        m_carPointers[index]->CarOwner.push(m_ownerPointers[ownerIndex]);
    }


    return true;
}

int CRegister::CountOwners(const char * RZ ) const
{
    int index = m_carPointers.binSearch(RZ);
    if(index<0) return 0;
    return m_carPointers[index]->CarOwner.getSize();
}

int CRegister::CountCars(const char * name, const char * surname ) const
{
    Owner temp;
    temp.name=name;
    temp.surname=surname;
    int index = m_ownerPointers.binSearch(&temp);
    if(index<0) return 0;
    return m_ownerPointers[index]->auticko.getSize();
}

CCarList CRegister::ListCars(const char * name, const char * surname) const
{
  CCarList newList2;

  Owner temp;
  temp.name=name;
  temp.surname=surname;
  int index = m_ownerPointers.binSearch(&temp);
  if(index==-1) {CCarList newList; return newList;}
  else
  {
    CCarList newList2( &(m_ownerPointers[index]->auticko));
    return newList2;
  }
}

COwnerList CRegister::ListOwners(const char * RZ) const
{

  int index = m_carPointers.binSearch(RZ);
  if(index<0) { COwnerList newList; return newList;}
  else
  {
      COwnerList newList( &(m_carPointers[index]->CarOwner));
      return newList;
  }

}



#ifndef __PROGTEST__
int main ( void )
 {

 }
#endif /* __PROGTEST__ */

