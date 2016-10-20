#ifndef COBJECT_H
#define COBJECT_H
#include<string>

using namespace std;

class CObject
{
    public:
        /**
         * Default constructor
         */
        CObject() {}

        /**
         * Constructor with parameters
         *
         * @param string containing host of content
         * @param string containing page of content
         */
        CObject(const string & host, const string & page) : m_host(host), m_page(page) {}

        /**
         * Default destructor
         */
        virtual ~CObject() {}

        /**
         * Abstract method, which downloads content
         */
        virtual void download(int coutner) = 0;


    protected:

        /**
         * Method, which creates unified file name
         *
         * Based on m_host and m_page, this method creates
         * unified file name, which is used by downloaded file
         *
         * @return string containing final file name
         */
        string fileName() const;

        /**
         * Method, which creates unified path name
         *
         * Based on m_host and m_page, this method creates
         * unified path name, to which are downloaded files saved
         *
         * @return string containing final path name
         */
        string pathName() const;

        /**
         * Method, which creates directory
         *
         * @param path to create directory in
         */
        void mkDir(const string & path) const;


        string m_host;
        string m_page;
};



#endif //COBJECT_H
