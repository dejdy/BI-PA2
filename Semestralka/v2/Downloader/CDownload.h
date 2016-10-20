#ifndef CDOWNLOAD_H
#define CDOWNLOAD_H

#include <string>
#include <vector>
#include "CPage.h"

class CDownload
{
    public:
    /**
     * Default constructor
     */
     CDownload() {}

    /**
     * Constructor with parameters
     *
     * @param String containing url of webpae to be downloaded
     * @param Depth of recursive downloading
     */
     CDownload (const string & webpage, int depth) : m_webpage(webpage),  m_depth(depth) {};

    /**
     * Destructor
     */
     ~CDownload() {}

     /**
      * Downloads desired website and saves it into files
      */
     void download();

     CPage * getPage(const char * host, const char * page);

    private:
    /**
     * Function to test for failures
     */
     void fail (int test, const char * format, ...);

     /**
      * Get the html of desired webpage
      *
      * Function downloads the html of desired webpage
      *
      * @param  File descriptor of socket
      * @param  Host of desired webpage
      * @param  Actual page
      * @return String that contains downloaded url
      */
     CPage * get_html (int s, const char * host, const char * page);

     string m_webpage;
     int m_depth;
     vector<CPage*> m_pages;

};


#endif //CDOWNLOAD_H
