#include "CPage.h"


void CPage::print()
{
    set<string>::iterator iter;
    for(iter=m_toDownload.begin(); iter!=m_toDownload.end();iter++)
    {
        cout << (*iter) << endl;
    }
}

void CPage::parseLinks(const string & Needle)
{
    long long unsigned int pos = 0;
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
        if(link!="/")
            m_toDownload.insert(link);
    }

    print();
}
