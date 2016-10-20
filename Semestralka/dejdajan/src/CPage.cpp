#include<fstream>
#include<string>
#include "CPage.h"
#include "CImage.h"

#include "CDownload.h"

const string ERR = "error.html";
const string ERRCONTENT = "This page does not exist.";

CPage::CPage()
{

}


void CPage::setHtml(const string & html)
{
    m_html = html;
}


void CPage::setHeader(const string & header)
{
    m_header = header;
}


bool isText(const string & head)
{
    if(head.find("Content-Type: text")!=string::npos) return true;
    else return false;
}


bool CPage::isInTag(int pos) const
{
    while(1)
    {
        pos--;
        if(m_html[pos]=='<' || m_html[pos]=='(') return true;
        if(m_html[pos]=='>' || m_html[pos]==')') return false;
        if(pos==0) return false;
    }
}


void CPage::newObj(const string & str, int depth) const
{
            CDownload * testDown = new CDownload();
            // If link leads to text data, we create CPage, if it's not text data, we create CImage
            if(isText(testDown->getPage(m_host.c_str(), str.c_str(), "", 0)))
            {
                CPage * newObj = new CPage(m_host, str, m_toDown, m_downloaded, m_args);
                pair<CObject*, int> temp;
                temp.first=newObj;
                temp.second=depth;
                m_downloaded->insert(str);
                m_toDown->push(temp);
            }
            else
            {
                CImage * newObj =  new CImage(m_host, str, m_args);
                pair<CObject*, int> temp;
                temp.first=newObj;
                temp.second=depth;
                m_downloaded->insert(str);
                m_toDown->push(temp);
            }
            delete testDown;
}


bool CPage::isLinkToIm(const string & tag, const string & str) const
{
    CDownload * testDown = new CDownload();
    bool test = isText(testDown->getPage(m_host.c_str(), str.c_str(), "", 0));
    if((tag=="href=" || tag=="href =" || tag=="HREF=" || tag=="HREF =") && !test)
    {
        delete testDown;
        return true;
    }

    delete testDown;
    return false;
}


void CPage::createErr() const
{
    string path = pathName();
    size_t norm = path.find("//");
    if(norm!=string::npos) path.erase(norm, 1);
    path.append(ERR);

    ofstream ofile;
    ofile.open(path);
    ofile << ERRCONTENT;
    ofile.close();
}


void CPage::origLink(const string & link, int linkLen, int pos)
{
    string newStr = "http://";
    newStr.append(m_host);
    if(m_page[0]!='/') newStr.append("/");
    newStr.append(m_page);
    if(newStr[newStr.size()-1]!='/') newStr.append("/");
    newStr.append(link);
    m_html.replace(pos-linkLen, linkLen, newStr);
}


void CPage::changeHtml(const string & link, int linkLen, int pos, bool linkToIm, int depth)
{
        if(depth>=m_args->getDepth() && isRelative(link) && m_args->getOriginalLinks() && link!="/")
        {
            createErr();
            m_html.replace(pos-linkLen, linkLen, ERR);
            return;
        }

        if(depth>=m_args->getDepth() && isRelative(link) && !m_args->getOriginalLinks())
        {
            origLink(link, linkLen, pos);
            return;
        }

        // if link ends with /, we replace it with index.html in the original html
        // so that the link will lead us to correct saved file
        if(link[link.size()-1]=='/' && isRelative(link) && isInTag(pos) && (m_args->getSaveImages() || !linkToIm))
        {
            string newStr;
            newStr = link;
            newStr.append("index.html");
            m_html.replace(pos-linkLen, linkLen, newStr);
        }

        if(!m_args->getSaveImages() && linkToIm) origLink(link, linkLen, pos);
}


void CPage::parseLinks(const string & Needle, int depth)
{
    unsigned long long int pos = 0;
    string needle=Needle;
    while(1)
    {
        int linkLen = 0;
        pos = m_html.find(needle, pos);
        if(pos==string::npos) break;
        pos+=needle.size();
        if(m_html[pos]=='"' || m_html[pos]=='\'' || m_html[pos]=='(') pos++;
        // pos now marks the index of the start of the link

        string link;
        if(pos>=m_html.size()) return;

        // move forward in html until the end of the link
        while(m_html[pos]!='"' && m_html[pos]!='>' && m_html[pos]!='\'' && m_html[pos]!=')' && m_html[pos]!=' ')
        {
            // linkLen marks the length of found link
            linkLen++;
            link+=m_html[pos];
            pos++;
        }

        if(link[0]=='/') link.erase(0,1);
        string str="";
        if(link.substr(0,2)=="..")
        {
            int flag=0;
            link.erase(0,2);
            str.append(m_page);
            int i = str.size()-1;
            if(str[i]=='/') str.erase(i, 1);
            while(flag<2 && i>0)
            {
                if(str[i]=='/') flag++;
                i--;
            }
            str.erase(i, string::npos);
        }

            if(m_page.find(".")== string::npos) str = m_page;
            if(str[str.size()-1]!='/') str.append("/");
            if(link[0]=='/') link.erase(0,1);
            str.append(link);


        bool linkToIm = isLinkToIm(Needle, str);

        changeHtml(link, linkLen, pos, linkToIm, depth);

        // If the conditions are met (link is not just /, it is relative, we haven't found it yet and it is inside of <> or () tag)
        // we call newObj(string, int), which creates new instance of the correct class inherited from CObject.
        if(link!="/" && isRelative(link) && (m_downloaded->find(str)==m_downloaded->end()) && isInTag(pos) && depth<m_args->getDepth() && (m_args->getSaveImages() || !linkToIm))
            newObj(str, depth);
    }
}


bool CPage::isRelative(const string & url) const
{
    if(url[0]=='#') return false;
    if(url.substr(0,3)=="www") return false;
    if(url.substr(0,4)=="http") return false;
    if(url.substr(0,6)=="mailto") return false;
    else return true;
}


void CPage::parse(const string & response)
{
    size_t posit = response.find("\r\n\r\n");
    setHeader(response.substr(0, posit));
    setHtml(response.substr(posit+4, string::npos));
}


bool CPage::download(int depth)
{
    string resp;
    mkDir( pathName() );
    CDownload * newDown = new CDownload();
    resp = newDown->getPage(m_host.c_str(),m_page.c_str(), fileName(), 1);
    if(resp=="Err")
    {
        delete newDown;
        return false;
    }
    parse(resp);
    cout << "Downloading: " << m_host << m_page;

    set<string> tags = m_args->getTags();
    for(auto it=tags.begin(); it!=tags.end(); it++)
        parseLinks((*it), depth);

    ofstream ofile;
    ofile.open(fileName());
    ofile << m_html;
    ofile.close();

    delete newDown;
    return true;
}
