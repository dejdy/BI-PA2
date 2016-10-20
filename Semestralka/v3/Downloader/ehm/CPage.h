#ifndef CPAGE_H
#define CPAGE_H
#include "CObject.h"
#include <iostream>
#include <string>
#include <set>
#include <queue>
using namespace std;

class CPage : public CObject
{
    public:
     /**
      * Default constructor
      */
     CPage () {};

     /**
      * Constructor with parameters
      *
      * Constructor with parameters receives response from GET request
      * It parses it and splits it into header and html itself
      * Header is saved into m_header, html into m_html variable
      */
     CPage (const string & host, const string & page, queue<pair<CObject*,int> > * toDown, set<string> * Downloaded) : CObject(host, page), m_toDown(toDown), m_downloaded(Downloaded) {}

     /**
      * Default destructor
      */
     virtual ~CPage() {};

     /**
      * Virtual method, which downloads content
      *
      * It creates new CDownload object and uses it to download
      * binary content specified by host "m_host" and page "m_page".
      * Downloaded data is split into header and html itself and html is parsed
      * to find another relative links using parseLinks method.
      */
     virtual void download(int counter);

    private:

     /**
      * Simple setter
      */
     void setHtml(const string & html) {m_html = html;}

     /**
      * Simple setter
      */
     void setHeader(const string & header) {m_header = header;}

     /**
      * Checks if url is relative
      *
      * @param URL to check
      */
     bool isRelative(const string & url);

    /**
     * Parsing html to find links
     *
     * Method searches for keywords specified by argument Needle.
     * It finds the keyword and extracts the link, which follows.
     * Header of GET request response is downloaded for each found link,
     * based on data type, proper class inherited from CObject is created
     * and pushed into m_toDownload queue to be downloaded.
     *
     * @param Keyword to look for
     */
     void parseLinks(const string & needle, int depth);

     /**
      * Splits GET response into header and html itself
      *
      * @param String containing GET response
      */
     void parse(const string &);

     string m_header;
     string m_html;

     queue<pair<CObject*, int> > * m_toDown;
     set<string> * m_downloaded;
};

#endif //CPAGE_H

