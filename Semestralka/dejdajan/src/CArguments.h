#ifndef CARGUMENTS_H
#define CARGUMENTS_H
#include <string>
#include <set>
#include "EMissingArgument.h"
#include "ESyntaxError.h"

using namespace std;

/**
 * Class used for parsing command line arguments.
 * It parses the arguments containing some additional
 * settings and customizations.
 */


class CArguments
{
    public:
        /**
         * Default constructor
         */
        CArguments();

        /**
         * Constructor with parameters
         *
         * @param argc number of passed arguments
         * @param argv pointer to array of arguments
         */
        CArguments(int argc, char ** argv);

        /**
         * Default destructor
         */
        ~CArguments();

        /**
         * This method parses command line arguments and searches for website parameter.
         * Website parameter is expected to be as the last argument and it has to be provided.
         * Found site is saved to m_site
         */
        void parseSite();

        /**
         * Method to check for syntax of command lien arguments
         *
         * If syntax is wrogn or too few arguments were passed,
         * it throws exception.
         *
         * @param argc number of passed arguments
         * @param argv pointer to array of arguments
         */
        void checkSyntax(int argc, char ** argv);

        /**
         * This method parses command line arguments and searches for tag parameters.
         * Tag parameters are expected to be preceded by -t sign.
         * Everything after -t which is not another -? sign or website parameter
         * is saved as tag. These tags are looked for during parsing the HTML for another links.
         * If none are provided, default (most common) tags are used.
         * It is not recomended to change default tags, as it may cause some serious problems
         * with parsing html.
         * Found tags are saved to set<string> m_tags
         */
        void parseTags();

        /**
         * This method parses command line arguments and searches for output path parameter.
         * Website parameter is expected to be preceded by -o sign.
         * If parameter is not provided, default "Downloaded/" will be used.
         * Found path is saved to m_path.
         */
        void parsePath();

        /**
         * This method parses command line arguments and searches for depth parameter.
         * Website parameter is expected to be preceded by -d sign.
         * If parameter is not provided, 2 will be used.
         * Final value is saved into m_depth variable.
         */
        void parseDepth();

        /**
         * This method parses command line arguments and searches for -i option.
         * If found, links to binary files (such as images) will lead to the original website,
         * if not, these files will be dowloaded and link will lead to these.
         * Final boolean value is saved into m_saveImages variable.
         */
        void parseImageOp();

        /**
         * This method parses command line arguments and searches for -l option.
         * If found, links to content deeper than m_depth will lead to created
         * HTML page on hard drive, containing information, that page does not exist.
         * If not found, these links will lead to original website.
         * Final boolean value is saved into m_originalLinks variable.
         */
        void parseOriginalLinksOp();

        /**
         * This method iterates over m_argv array and searches
         * given argument option.
         * If found, this method returns index of wanted option,
         * if not, -1 is returned.
         *
         * @param option Keyword to look for
         * @return index of found option or -1 if not found.
         */
        int findOption(const string & option);

        /**
         * Simple getter
         *
         * @return pair<string,string> containing host and page
         */
        pair<string,string> getSite() const;

        /**
         * Simple getter
         *
         * @return set<string> containing found tags
         */
        set<string> getTags() const;

        /**
         * Simple getter
         *
         * @return Found path
         */
        string getPath() const;

        /**
         * Simple getter
         *
         * @return Depth of downloading
         */
        int getDepth() const;

        /**
         * Simple getter
         *
         * @return m_saveImages
         */
        int getSaveImages() const;

        /**
         * Simple getter
         *
         * @return m_originalLinks
         */
        int getOriginalLinks() const;

    private:
        int m_argc; /**< Number or command line arguments */
        char ** m_argv; /**< Pointer to char array containing command line aguments */
        pair<string,string> m_site; /**< Pair containing host and page */
        set<string> m_tags;  /**< Set of parsed tags */
        string m_path;  /**< Parsed path */
        int m_depth;    /**< Parsed depth of downloading */
        bool m_saveImages; /**< Whether to save images */
        bool m_originalLinks; /**< Whether to keep original links for too deep link */

};


#endif //CARGUMENTS_H
