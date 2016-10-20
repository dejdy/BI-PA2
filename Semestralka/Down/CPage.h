#include <iostream>
#include <string>
#include <set>
using namespace std;

class CPage
{
    public:
     CPage () {};
     CPage (const string & html) : m_html(html) {};
     ~CPage() {};
     string getHtml() const {return m_html;}

     /**
     * Simple setter
     *
     * @param string that should be set
     */
     void setHtml(const string & html) {m_html = html;}

     /**
     * Parsing html to find hyperlinks
     *
     * Method searches for keywords specified by argument Needle
     * It finds the keyword and extracts the link, which follows
     * Extracted link is enqueed to m_toDownload
     *
     * @param Keyword to look for
     */
     void parseLinks(const string & needle);

     /**
     * Prints content of queue m_toDownload
     */
     void print();
    private:
     string m_html;
     set<string> m_toDownload;
};
