#ifndef CIMAGE_H
#define CIMAGE_H
#include "CObject.h"
#include "CDownload.h"
using namespace std;

/**
 * Class which inherits from CObject.
 * It represents binary content - most commonly images
 * but also pdf files, mp3 files...
 * It only contains virtual method for downloading content
 * as we do not want to parse binary content.
 * It uses methods fileName() and mkDir() which are inherited
 * from CObject class.
 */


class CImage : public CObject
{
    public:
        /**
         * Default constructor
         */
        CImage();

        /**
         * Constructor with parameters
         *
         * @param host String containing host of content
         * @param page String containing page of content
         * @param args Pointer to CArguments object containing command line arguments
         */
        CImage (const string & host, const string & page, CArguments * args) : CObject(host, page, args) {}

        /**
         * Default destructor
         */
        virtual ~CImage();

        /**
         * Virtual method, which downloads content
         *
         * It creates new CDownload object and uses it to download
         * binary content specified by host "m_host" and page "m_page".
         * Downloaded data is saved into file.
         *
         * @param depth Depth of downloaded content
         */
        virtual bool download(int depth);
};



#endif //CIMAGE_H

