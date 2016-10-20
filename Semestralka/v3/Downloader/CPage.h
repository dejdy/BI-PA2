#ifndef CPAGE_H
#define CPAGE_H
#include "CObject.h"
#include "CArguments.h"
#include <iostream>
#include <string>
#include <set>
#include <queue>
using namespace std;

/**
 * Class which inherits from CObject.
 * It represents text content - most commonly webpages
 * but also css files for example.
 * It contains virtual method for downloading content
 * and method for parsing HTML to search links.
 * It uses methods fileName() and mkDir() which are inherited
 * from CObject class.
 */

class CPage : public CObject
{
    public:
        /**
         * Default constructor
         */
        CPage ();

        /**
         * Constructor with parameters
         *
         * Constructor with parameters receives response from GET request
         * It parses it and splits it into header and html itself
         * Header is saved into m_header, html into m_html variable
         *
         * @param host Host of webpage
         * @param page Page
         * @param toDown Pointer to queue of found links
         * @param Downloaded Pointer to set of enqueued links
         * @param args to CArguments object containing command line arguments
         */
        CPage (const string & host, const string & page, queue<pair<CObject*,int> > * toDown, set<string> * Downloaded, CArguments * args)
            : CObject(host, page, args), m_toDown(toDown), m_downloaded(Downloaded) {}

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
         *
         * @param depth Depth of dowloaded webpage
         */
        virtual bool download(int depth);

    private:

        /**
         * Simple setter
         *
         * @param html HTML to be set
         */
        void setHtml(const string & html);

        /**
         * Simple setter
         *
         * @param header Header to be set
         */
        void setHeader(const string & header);

        /**
         * Checks if url is relative
         *
         * For simplicity, we assume, that absolute url
         * begins with "www" or "http".
         *
         * @param url URL to check
         * @return True if relative, false if not
         */
        bool isRelative(const string & url) const;

        /**
         * Parsing html to find links
         *
         * Method searches for keywords specified by argument Needle.
         * It finds the keyword and extracts the link, which follows.
         * Header of GET request response is downloaded for each found link,
         * based on data type, proper class inherited from CObject is created
         * and pushed into m_toDownload queue to be downloaded.
         *
         * @param needle Keyword to look for
         * @param depth Depth of downloaded webpage
         */
         void parseLinks(const string & needle, int depth);

        /**
         * Splits GET response into header and html itself
         *
         * @param resp String containing GET response
         */
        void parse(const string & resp);

        /**
         * Check if index pos is inside <> or () tag
         *
         * @param pos Integer representing index to check
         * @return Corresponding boolean value
         */
        bool isInTag(int pos) const;

        /**
         * Method to check if link leads to binary file
         *
         * It downloads header of GET request and determines if the content is binary or not.
         * It also checks if tag is "href" "HREF" "href " or "HREF ", so that
         * pictures inserted using src tag are downloaded.
         *
         * @param tag Tag to check if it is href
         * @param str Link of checked file
         * @return Corresponding boolean value
         */
        bool isLinkToIm(const string & tag, const string & str) const;

        /**
         * Create new instance of CPage or CImage
         *
         * This method decides if content is text or binary
         * and creates instance of corresponding class and adds it
         * to m_toDownload queue.
         *
         * @param str Final link of the content
         * @param depth Depth of content
         */
        void newObj(const string & str, int depth) const;

        /**
         * Change HTML depending on settings
         *
         * This method changes html according to settings
         * set by command line arguments.
         *
         * @param link Link
         * @param linkLen Length of found link
         * @param pos Position in HTML
         * @param linkToIm Boolean marking if it leads to binary content
         * @param depth Depth of current content
         */
        void changeHtml(const string & link, int linkLen, int pos, bool linkToIm, int depth);

        /**
         * Create error HTML page
         */
        void createErr() const;

        /**
         * Change relative link to absolute
         *
         * @param link Link
         * @param linkLen Length of found link
         * @param pos Position in HTML
         */
        void origLink(const string & link, int linkLen, int pos);

        string m_header;    /**< Header of GET request */
        string m_html;  /**< HTML itself */

        queue<pair<CObject*, int> > * m_toDown; /**< Pointer to queue of links to be downloaded */
        set<string> * m_downloaded; /**< Already downloaded links */
};

#endif //CPAGE_H

