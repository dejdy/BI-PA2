#include <iostream>
#include <queue>
#include "CDownload.h"
#include "CObject.h"
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
    string host, page, adress;
    host="";
    page="";
    pair<string, string> url;
    adress = argv[argc-1];

    if(adress.substr(0,7)=="http://")
        adress=adress.substr(7,string::npos);
    if(adress.substr(0,8)=="https://")
        adress=adress.substr(8,string::npos);

    while(adress[i] && adress[i] != '/')
    {
        host += adress[i];
        i++;
    }

    while(adress[i])
    {
        page += adress[i];
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
    set<string> Downloaded;
    queue<pair<CObject*, int> > toDownload;
    pair<CObject*,int> cur;

    string site = argv[1];

    CPage * p = new CPage(url.first, url.second, &toDownload, &Downloaded);

    int counter=0;
    p->download(counter);

    while(toDownload.size()>0)
    {
        cur = toDownload.front();
        if(cur.second==5) break;
        cout << " ... Num of files in queue: " << toDownload.size() << endl;
        toDownload.pop();
        cur.first->download(cur.second+1);
    }
    cout << " ... Num of files in queue: " << toDownload.size() << endl;
    cout << "... Done!" << endl;
    delete p;
    return 0;
}
