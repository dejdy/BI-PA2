#ifndef CPAGE_H
#define CPAGE_H

#include "CObject.h"
#include <iostream>
#include <string>
#include <set>
using namespace std;

class CPage
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
     CPage (const string & html);

     /**
      * Default destructor
      */
     ~CPage() {};

     /**
      * Simple getter
      */
     string getHtml() const {return m_html;}

     /**
      * Simple setter
      */
     void setHtml(const string & html) {m_html = html;}

    /**
     * Parsing html to find links
     *
     * Method searches for keywords specified by argument Needle
     * It finds the keyword and extracts the link, which follows
     * Extracted link is enqueed to m_toDownload
     *
     * @param Keyword to look for
     */
     void CParseLinks(const string & needle, set<string> &);

     /**
      * Checks if url is relative
      *
      * @param URL to check
      */
     bool isRelative(const string & url);

     /**
      * Prints content of queue m_toDownload
      */
     void print(set<string> &);

    private:
     string m_header;
     string m_html;
};

#endif //CPAGE_H
