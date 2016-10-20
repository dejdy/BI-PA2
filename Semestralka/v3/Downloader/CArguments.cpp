#include <iostream>
#include <string>
#include "CArguments.h"


using namespace std;

const string DEFPATH = "Downloaded/";
const int DEFDEPTH = 2;

CArguments::CArguments()
{

}


CArguments::~CArguments()
{

}


CArguments::CArguments(int argc, char ** argv)
{
    checkSyntax(argc, argv);
    m_argc = argc;
    m_argv = argv;
    parseSite();
    parseTags();
    parsePath();
    parseDepth();
    parseImageOp();
    parseOriginalLinksOp();
    string cmd = "rm -rf ";
    cmd.append(getPath());
    cmd.append(getSite().first);
    system(cmd.c_str());
}


void CArguments::checkSyntax(int argc, char ** argv)
{
    if(argc<2) throw EMissingArgument();

    string arg;
    for(int i=0; i<argc-1; i++)
    {
        arg=argv[i];
        if(arg[0] == '-' && (arg.size()==1 || ( (arg[1]!='d') && (arg[1]!='l') && (arg[1]!='i') && (arg[1]!='o') && (arg[1]!='t') ) ))
            throw ESyntaxError();

    }
}


void CArguments::parseSite()
{
    int i=0;
    string host, page, adress;
    host="";
    page="";
    pair<string, string> url;
    adress = m_argv[m_argc-1];

    if(adress.substr(0,7)=="http://")
        adress=adress.substr(7,string::npos);
    if(adress.substr(0,8)=="https://")
        adress=adress.substr(8,string::npos);

    while(adress[i] && adress[i] != '/')
    {
        host += adress[i];
        i++;
    }

    while(adress[i])
    {
        page += adress[i];
        i++;
    }

    url.first = host;
    url.second = page;

    m_site = url;
}


void CArguments::parseTags()
{
    string arg;
    bool flag=false;
    for(int i=0; i<m_argc-1; i++)
    {
        arg=m_argv[i];
        if(arg=="-t")
        {
            flag=true;
            i++;
        }
        arg=m_argv[i];
        if(arg[0]=='-' && flag)
        {
            return;
        }
        if(flag)
        {
            arg=m_argv[i];
            m_tags.insert(arg);
        }
    }
    if(m_tags.size()==0)
    {
        m_tags.insert("href=");
        m_tags.insert("href =");
        m_tags.insert("HREF=");
        m_tags.insert("HREF =");
        m_tags.insert("src=");
        m_tags.insert("src =");
        m_tags.insert("SRC=");
        m_tags.insert("SRC =");
        m_tags.insert("url(");
    }
}


void CArguments::parsePath()
{
    int pos = findOption("-o");
    if(pos>=m_argc-2) throw ESyntaxError();
    if(pos!=-1) m_path = m_argv[pos+1];
    else m_path = DEFPATH;
}


void CArguments::parseDepth()
{
    int pos = findOption("-d");
    if(pos!=-1) m_depth = atoi(m_argv[pos+1]);
    else m_depth = DEFDEPTH;
    if(m_depth<0) throw ESyntaxError();
}


void CArguments::parseImageOp()
{
    int test = findOption("-i");
    if(test==-1) m_saveImages = true;
    else m_saveImages = false;
}


void CArguments::parseOriginalLinksOp()
{
    int test = findOption("-l");
    if(test==-1) m_originalLinks = false;
    else m_originalLinks = true;
}


int CArguments::findOption(const string & option)
{
    string arg;
    for(int i=0; i<m_argc-1; i++)
    {
        arg=m_argv[i];
        if(arg == option)
        {
            return i;
        }
    }
    return -1;
}


pair<string,string> CArguments::getSite() const
{
    return m_site;
}


set<string> CArguments::getTags() const
{
    return m_tags;
}


string CArguments::getPath() const
{
    return m_path;
}


int CArguments::getDepth() const
{
    return m_depth;
}


int CArguments::getSaveImages() const
{
    return m_saveImages;
}


int CArguments::getOriginalLinks() const
{
    return m_originalLinks;
}
