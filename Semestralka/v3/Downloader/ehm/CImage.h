#ifndef CIMAGE_H
#define CIMAGE_H
#include "CObject.h"
#include "CDownload.h"
using namespace std;

class CImage : public CObject
{
    public:
        /**
         * Default constructor
         */
        CImage() {}

        /**
         * Constructor with parameters
         *
         * @param string containing host of content
         * @param string containing page of content
         */
        CImage (const string & host, const string & page) : CObject(host, page) {}

        /**
         * Default destructor
         */
        virtual ~CImage() {}

        /**
         * Virtual method, which downloads content
         *
         * It creates new CDownload object and uses it to download
         * binary content specified by host "m_host" and page "m_page".
         * Downloaded data is saved into file.
         */
        virtual void download(int counter);
};



#endif //CIMAGE_H

