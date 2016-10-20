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
     CDownload (const string & webpage) : m_webpage(webpage) {};

     /**
      * Default destructor
      */
     ~CDownload() {}

     string getPage(const char * host, const char * page, const string & dest, int mode);

    private:
    /**
     * Function to test for failures
     */
     void fail (int test, const char * format, ...);

     /**
      * Get the html of desired webpage
      *
      * This method generally downloads part of GET request response, but it behaves
      * differently, based on parameter mode.
      * If parameter mode equals:
      *     0 - it gets header of GET request response and returns it as string
      *     1 - it gets full GET request response and returns it as string
      *     2 - it downloads file without header into file described in dest, returns empty string
      *
      * @param  File descriptor of socket
      * @param  Host of desired webpage
      * @param  Page
      * @param  Destination for binary file to be saved
      * @param  Integer specifiing mode of download
      * @return String that contains downloaded url
      */
     string get_html (int s, const char * host, const char * page, const string & dest, int mode);

     string m_header;
     string m_body;
     string m_webpage;
};


#endif //CDOWNLOAD_H
