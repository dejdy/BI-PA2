#include<fstream>
#include<string>
#include "CPage.h"
#include "CImage.h"

#include "CDownload.h"



bool isText(const string & url)
{
    if(url.find("Content-Type: text")!=string::npos) return true;
    else return false;
}


void CPage::parseLinks(const string & Needle, int depth)
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
                string str = m_page;
                if(str[str.size()-1]!='/') str.append("/");
                if(link[0]=='/') link.erase(0,1);
                str.append(link);

        if(link!="/" && isRelative(link) && (m_downloaded->find(str)==m_downloaded->end()))
        {
            CDownload * testDown = new CDownload();
            if(isText(testDown->getPage(m_host.c_str(), str.c_str(), "", 0)))
            {
                CPage * newObj = new CPage(m_host, str, m_toDown, m_downloaded);
                pair<CObject*, int> temp;
                temp.first=newObj;
                temp.second=depth;
                m_downloaded->insert(str);
                m_toDown->push(temp);
            }
            else
            {
                CImage * newObj =  new CImage(m_host, str);
                pair<CObject*, int> temp;
                temp.first=newObj;
                temp.second=depth;
                m_downloaded->insert(str);
                m_toDown->push(temp);
            }

            delete testDown;
        }
    }
}


bool CPage::isRelative(const string & url)
{
    if(url.substr(0,3)=="www") return false;
    if(url.substr(0,4)=="http") return false;
    else return true;
}


void CPage::parse(const string & response)
{
    size_t posit = response.find("\r\n\r\n");
    setHeader(response.substr(0, posit));
    setHtml(response.substr(posit+4, string::npos));
}



void CPage::download(int depth)
{
    mkDir( pathName() );
    cout << "Downloading: " << m_host << m_page;
    CDownload * newDown = new CDownload(m_html);
    parse(newDown->getPage(m_host.c_str(),m_page.c_str(), fileName(), 1));

    ofstream ofile;
    ofile.open(fileName());
    ofile << m_html;
    ofile.close();

    parseLinks("HREF", depth);
    parseLinks("HREF ", depth);
    parseLinks("href", depth);
    parseLinks("href ", depth);
    parseLinks("src", depth);
    parseLinks("src ", depth);
    parseLinks("SRC", depth);
    parseLinks("SRC ", depth);


    delete newDown;
}
