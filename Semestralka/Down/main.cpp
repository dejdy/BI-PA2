
#include <iostream>
#include "GetPage.cpp"
#include "CPage.h"
using namespace std;

/**
 * <Parsing arguments from command line>
 *
 * <This function parses argumen given in the command line>
 * <It splits given url into host and page>
 *
 * @param  Number of given parameters
 * @param  Actual arguments
 * @return Returns pair <host, page>
 */
pair<string, string> parseArgument(int argc, char * argv[])
{
    int i=0;
    string host, page;
    pair<string, string> url;

    while(argv[1][i] && argv[1][i] != '/')
    {
        host += argv[1][i];
        i++;
    }

    while(argv[1][i])
    {
        page += argv[1][i];
        i++;
    }
    url.first = host;
    url.second = page;

    return url;
}

int main(int argc, char *argv[])
{
    if(argc<2)
    {
        cout << "Missing argument." << endl;
        return 0;
    }

    pair <string, string> url = parseArgument(argc, argv);

    CPage * newPage = new CPage(printPage(url.first.c_str(), url.second.c_str()));

    //cout << newPage->getHtml();
    newPage->parseLinks("href");
    newPage->parseLinks("HREF");
    newPage->parseLinks("src");
    newPage->parseLinks("SRC");

    delete newPage;
    return 0;
}
