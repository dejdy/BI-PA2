#include "CPage.h"



CPage::CPage (const string & html)
{
    size_t pos;
    pos = html.find("\n\n");
    m_header = html.substr(0,pos);
    m_html = html.substr(pos+2,string::npos);
}


void CPage::print(set<string> & toDownload)
{

    set<string>::iterator iter;
    for(iter=toDownload.begin(); iter!=toDownload.end();iter++)
    {
        cout << (*iter) << endl;
    }
}

void CPage::CParseLinks(const string & Needle, set<string> & toDownload)
{
    unsigned long long int pos = 0;
    string needle=Needle;
    needle+="=\"";
    while(1)
    {
        pos = m_html.find(needle, pos);
        if(pos==string::npos) break;
        pos+=needle.size(); /// pos now marks the index of the start of the link
        string link;

        if(pos>=m_html.size()) return;
        while(m_html[pos]!='"')
        {
            link+=m_html[pos];
            pos++;
        }

        if(link!="/" && isRelative(link))
            toDownload.insert(link);
    }

    print(toDownload);
    cout << endl << endl << endl;
}


bool CPage::isRelative(const string & url)
{
    if(url.substr(0,3)=="www") return false;
    if(url.substr(0,4)=="http") return false;
    else return true;
}
