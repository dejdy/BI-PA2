#ifndef CDOWNLOAD_H
#define CDOWNLOAD_H

#include <string>
#include <vector>
#include "CPage.h"

/**
 * Class representing download object.
 * It is capable of downloading file described by its host and page url
 * and either saves it to binary file or return downloaded HTML.
 * This class contains code, which is not mine and was copied from:
 * http://www.lemoda.net/c/fetch-web-page/index.html
 * This copied code and all the rights belong to author of the code from the website.
 */


class CDownload
{
    public:
        /**
         * Default constructor
         */
        CDownload();

        /**
         * Default destructor
         */
        ~CDownload();

        /**
         * Get page
         *
         * This method calls get_html, which generally downloads the page.
         * More description at get_html method
         * @param host Host of desired webpage
         * @param page Page
         * @param dest Destination for binary file to be saved
         * @param mode Integer specifiing mode of download
         * @return String that contains downloaded HTML or "Err" if something went wrong
         */
        string getPage(const char * host, const char * page, const string & dest, int mode);

    private:

        /**
         * Quickie function to test for failures. It is actually better to use
         * a macro here, since a function like this results in unnecessary
         * function calls to things like "strerror". However, not every
         * version of C has variadic macros.
         *
         * @param test Status of sendind the request
         * @param format Format of error
         * @return 1 if everything is OK, 0 if something went wrong
         */
        bool fail (int test, const char * format, ...);

        /**
         * Get HTML of desired webpage
         *
         * This method generally downloads part of GET request response, but it behaves
         * differently, based on parameter mode.
         * If parameter mode equals:
         *     0 - it gets header of GET request response and returns it as string
         *     1 - it gets full GET request response and returns it as string
         *     2 - it downloads file without header into file described in dest, returns empty string
         *
         * @param s File descriptor of socket
         * @param host Host of desired webpage
         * @param page Page
         * @param dest Destination for binary file to be saved
         * @param mode Integer specifiing mode of download
         * @return String that contains downloaded HTML or "Err" if something went wrong
         */
        string get_html (int s, const char * host, const char * page, const string & dest, int mode);

};


#endif //CDOWNLOAD_H
