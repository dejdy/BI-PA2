#ifndef __PROGTEST__
#include <cassert>
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <set>
#include <list>
#include <map>
#include <algorithm>
using namespace std;

class CResult
 {
   public:
CResult (const string & name, unsigned int studentID, const string & test, int result)
: m_Name ( name ), m_StudentID ( studentID ),m_Test ( test ), m_Result ( result ) { }
    string         m_Name;
    unsigned int   m_StudentID;
    string         m_Test;
    int            m_Result;
 };
#endif /* __PROGTEST__ */

struct Test
{
    string testName;
    int result;
};

struct hodnoceni
{
    string name;
    string test;
    unsigned int id;
};

vector<hodnoceni> assessOrder;

class CStudent
{
   public:
    CStudent () {}
    CStudent (unsigned int id, string name) {m_id = id; m_name = name;}

   //private:
    unsigned int m_id;
    string m_name;
    vector<string> m_card;
    vector<Test> m_tests;
};


bool cmp1 (const CResult a, const CResult b)
{
  if(a.m_StudentID > b.m_StudentID) return false;
  else return true;
}

bool cmp2 (const CResult a, const CResult b)
{
  if(a.m_Name > b.m_Name) return false;
  if(a.m_Name < b.m_Name) return true;
  if(a.m_Name == b.m_Name)
  {
    for(unsigned int i=0; i<assessOrder.size(); i++)
    {
        if(assessOrder[i].id == a.m_StudentID) return true;
        if(assessOrder[i].id == b.m_StudentID) return false;
    }
  }

  return true;
}

bool cmp3 (const CResult a, const CResult b)
{
  if(a.m_Result > b.m_Result) return true;
  if(a.m_Result < b.m_Result) return false;

  if(a.m_Result == b.m_Result)
  {
    for(unsigned int i=0; i<assessOrder.size(); i++)
    {
        if(assessOrder[i].id == a.m_StudentID) return true;
        if(assessOrder[i].id == b.m_StudentID) return false;
    }
  }

  return true;
}

class CExam
 {
   public:
    static const int SORT_NONE   = 0;
    static const int SORT_ID     = 1;
    static const int SORT_NAME   = 2;
    static const int SORT_RESULT = 3;
    CExam() {}
    ~CExam();
    void print();
    bool           Load        ( istream      & cardMap );
    bool           Register    ( const string & cardID,
                                 const string & test );
    bool           Assess      ( unsigned int   studentID,
                                 const string & test,
                                 int            result );
    list<CResult>  ListTest    ( const string & test,
                                 int            sortBy ) const;
    set<string>    ListMissing ( const string & test ) const;

    map<string, vector<CResult>> m_assessed;
   private:
    map<string, CStudent *> m_studMap;
    map<unsigned int, CStudent *> m_idMap;
    map<string, vector<CStudent *>> m_testMap;
};

CExam::~CExam()
{
    map<unsigned int, CStudent *>::iterator it;
    for(it = m_idMap.begin(); it!= m_idMap.end(); it++)
        delete it->second;
}

void CExam::print()
{
    map<unsigned int, CStudent *>::iterator it;
    for(it = m_idMap.begin(); it!= m_idMap.end(); it++)
        {
        cout << it->second->m_id << " " << it->second->m_name;
        for(unsigned int j=0; j<it->second->m_card.size(); j++)
            cout << " " << it->second->m_card[j];
        cout << endl;
        for(unsigned int k=0; k<it->second->m_tests.size(); k++)
            cout << " " << it->second->m_tests[k].testName << " : " << it->second->m_tests[k].result << endl;

        cout << endl << endl;
        }
}

bool CExam::Load(istream & cardMap)
{
    string buffer, buf, idString;
    istringstream iss;
    char c='0';

    map<string, CStudent *> t_studMap;
    map<unsigned int, CStudent *> t_idMap;
    map<string, vector<CStudent *>> t_testMap;
    vector<CStudent *> delPtr;

    t_testMap = m_testMap;
    t_studMap = m_studMap;
    t_idMap = m_idMap;


    while(getline(cardMap, buffer, '\n'))  // Nacitani a parsovani vstupu
    {
        CStudent * temp = new CStudent();
        delPtr.push_back(temp);
        temp->m_card.clear();
        iss.str(buffer);
        buf = "";
        idString="";

        while(iss >> noskipws >> c)
        {
            if(c==':') break;
            if(!isdigit((int)c))
            {
                for(unsigned int i=0; i<delPtr.size(); i++)
                    delete delPtr[i];
                return false;
            }
            idString.append(1,c);
        }
        temp->m_id = (atoi(idString.c_str()));
        if(t_idMap.find(temp->m_id) != t_idMap.end())
            {
                for(unsigned int i=0; i<delPtr.size(); i++)
                    delete delPtr[i];
                return false;
            }
        buf = "";

        while(iss >> noskipws >> c)
        {
            if(c==':') break;
            if(!isalnum((int)c) && !isspace((int)c))
            {
                for(unsigned int i=0; i<delPtr.size(); i++)
                    delete delPtr[i];
                return false;
            }
            buf.append(1,c);
        }
        temp->m_name = buf;

        if(t_studMap.find(temp->m_name) != t_studMap.end())
            {
                for(unsigned int i=0; i<delPtr.size(); i++)
                    delete delPtr[i];
                return false;
            }

        buf="";
        while(iss >> c)
        {
            if(c==EOF)
            {
                temp->m_card.push_back(buf);
                break;
            }
            if(!isalnum((int)c) && !isspace((int)c) && c!=',')
            {
                for(unsigned int i=0; i<delPtr.size(); i++)
                    delete delPtr[i];
                return false;
            }
            if(c==' ') {continue;}
            if(c==',')
            {
                temp->m_card.push_back(buf);
                buf="";
                continue;
            }
            buf.append(1,c);
        }
        if(buf=="" || buf==" ")
            {
                for(unsigned int i=0; i<delPtr.size(); i++)
                    delete delPtr[i];
                return false;
            }

        if(t_studMap.find(buf) != t_studMap.end())
            {
                for(unsigned int i=0; i<delPtr.size(); i++)
                    delete delPtr[i];
                return false;
            }
        temp->m_card.push_back(buf);

        for(unsigned int i=0; i< temp->m_card.size(); i++)
            t_studMap[temp->m_card[i]] = temp;

        t_idMap[temp->m_id] = temp;
        iss.clear();
    }

    m_testMap = t_testMap;
    m_studMap = t_studMap;
    m_idMap = t_idMap;

    return true;
}

bool CExam::Register(const string& card, const string& test)
{
    Test tempTest;
    tempTest.testName=test;
    tempTest.result=-853;
    map<string, CStudent *>::iterator iter;
    iter = m_studMap.find(card);
    if(iter==m_studMap.end()) return false; // Neexistujici ID
    CStudent * temp = iter->second; // Pointer na studenta, ktereho chceme zapsat na test

    for(unsigned int i=0; i<temp->m_tests.size(); i++)
    {
        if(temp->m_tests[i].testName == test) return false; // Student je jiz na test zapsany
    }

    temp->m_tests.push_back(tempTest); // Studentovi pridame test do vectoru pointeru
    m_testMap[test].push_back(temp); // Pridame do vectoru pointeru na studenty

    return true;
}

bool CExam::Assess (unsigned int studentID, const string & test, int result)
{
    map<unsigned int, CStudent *>::iterator iter;
    iter = m_idMap.find(studentID);
    if(iter==m_idMap.end()) return false; // pokud student s timto ID neexistuje
    CStudent * temp = iter->second;
    int index = -853;
    for(unsigned int i=0; i<temp->m_tests.size(); i++)
    {
        if(temp->m_tests[i].testName == test) index = i;
    }
    if(index==-853) return false; // Student nebyl na test zapsan
    if(temp->m_tests[index].result != -853) return false; // Student jiz byl hodnocen
    temp->m_tests[index].result = result; // Prirazeni vysledku

    CResult tempor(temp->m_name, studentID, test, result);
    m_assessed[test].push_back(tempor);
    hodnoceni tempHod;
    tempHod.id=studentID;
    tempHod.name=temp->m_name;
    tempHod.test = test;
    assessOrder.push_back(tempHod);

    return true;
}

list<CResult> CExam::ListTest (const string & test, int sortBy) const
{
    list<CResult> newList;

    if(sortBy==0)
    {
        auto ite = m_assessed.find(test);
        if(ite==m_assessed.end()) return newList;
        for(unsigned int k=0; k<ite->second.size(); k++)
            newList.push_back(ite->second[k]);

        return newList;
    }


    string index = test;
    auto iter = m_testMap.find(test);
    if(iter==m_testMap.end()) return newList;
    int res=-853;
    for(unsigned int i=0; i<iter->second.size(); i++)
    {
        for(unsigned int j=0; j<iter->second[i]->m_tests.size(); j++)
            if(iter->second[i]->m_tests[j].testName==test) {res = iter->second[i]->m_tests[j].result; break;}
        CResult a(iter->second[i]->m_name, iter->second[i]->m_id, test, res);
        if(res!=-853) newList.push_back(a);
    }


    if(sortBy==1)
    {
        newList.sort(cmp1);
        return newList;
    }
    else if(sortBy==2)
    {
        newList.sort(cmp2);
        return newList;
    }
    else if(sortBy==3)
    {
        newList.sort(cmp3);
        return newList;
    }

    return newList;
}

set<string> CExam::ListMissing (const string & test) const
{
    set<string> newSet;
    auto iter = m_testMap.find(test);
    if(iter==m_testMap.end()) return newSet;

    for(unsigned int i=0; i<iter->second.size(); i++)
    {
        for(unsigned int j=0; j<iter->second[i]->m_tests.size(); j++)
            if(iter->second[i]->m_tests[j].result==-853) {newSet.insert(iter->second[i]->m_name);}
    }
    return newSet;
}

#ifndef __PROGTEST__
string toString ( const list<CResult> & l )
 {
   ostringstream oss;
   for ( auto const & x : l )
    oss << x . m_Name << ":" << x . m_StudentID << ":" << x . m_Test << ":" << x . m_Result << endl;
   return oss . str ();
 }
string toString ( const set<string> & s )
 {
   ostringstream oss;
   for ( const auto & x : s )
    oss << x << endl;
   return oss . str ();
 }



int main ( void )
 {

   istringstream iss;
   CExam         m;
   iss . clear ();
   iss . str ( "123456:Smith John:abc\n" "987:West Peter Thomas:s\n" );
   assert ( m . Load ( iss ) );

   assert ( m . Register ( "abc", "PA2 - #1" ) );
   assert ( m . Register ( "s", "PA2 - #1" ) );
   m.Assess(123456, "PA2 - #1", 10);

   set<string> a = m.ListMissing("PA2 - #1");
   cout << a.size();



   return 0;
 }
#endif /* __PROGTEST__ */
