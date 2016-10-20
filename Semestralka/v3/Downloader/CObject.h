#ifndef COBJECT_H
#define COBJECT_H
#include <string>
#include "CArguments.h"

using namespace std;

/**
 * Abstract class, which represents object, that can be downloaded.
 * Two classes inherit from this one - CPage and CImage
 * It contains pure method for downloading content
 * And some methods, that are use for handling with file names.
 */

class CObject
{
    public:
        /**
         * Default constructor
         */
        CObject();

        /**
         * Constructor with parameters
         *
         * @param host String containing host of content
         * @param page String containing page of content
         * @param args Pointer to CArguments object containing command line arguments
         */
        CObject(const string & host, const string & page, CArguments * args);

        /**
         * Default destructor
         */
        virtual ~CObject();

        /**
         * Abstract method, which downloads content
         *
         * It is implemented in child classes
         *
         * @param depth Depth of downloaded page
         */
        virtual bool download(int depth) = 0;


    protected:

        /**
         * Method, which creates unified file name
         *
         * Based on m_host and m_page, this method creates
         * unified file name, which is used by downloaded file
         *
         * @return Sring containing final file name
         */
        string fileName() const;

        /**
         * Method, which creates unified path name
         *
         * Based on m_host and m_page, this method creates
         * unified path name, to which are downloaded files saved
         *
         * @return String containing final path name
         */
        string pathName() const;

        /**
         * Method, which creates directory
         *
         * @param path Path to create directory in
         */
        void mkDir(const string & path) const;


        string m_host;  /**< host */
        string m_page;  /**< page */
        CArguments * m_args;    /**< pointer to CArguments object containing command line arguments */
};



#endif //COBJECT_H
