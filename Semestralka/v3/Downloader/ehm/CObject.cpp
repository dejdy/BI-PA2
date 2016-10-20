#include<iostream>
#include<string>
#include "CObject.h"

using namespace std;


string CObject::fileName() const
{
    string path="Downloaded/";
    path.append(m_host);
    path.append(m_page);
    if(m_page=="") path.append("/index");

    if(path[path.size()-1]=='/') path.erase(path.size()-1, 1);
    for(int i=path.size()-1; i>0; i--)
    {
        if(path[i]=='.') break;
        if(path[i]=='/')
        {
            path.append(".html");
            break;
        }
    }
    return path;
}

string CObject::pathName() const
{
    string path = "";
    path = "Downloaded/";
    path.append(m_host);
    path.append("/");
    path.append(m_page);

    for(int i=path.size()-1; i>0; i--)
    {
        if(path[i]=='.')
        {
            while(path[i]!='/')
            {
                path[i]='\0';
                i--;
            }
            break;
        }
        if(path[i]=='/') break;
    }
    return path;
}

void CObject::mkDir(const string & path) const
{
    string newDir;
    newDir="mkdir -p ";
    newDir.append(path);
    system(newDir.c_str());
}

