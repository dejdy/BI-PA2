#include <iostream>
#include "CDownload.h"
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

void makeDir(const string & path)
{
    string newDir;
    newDir="mkdir -p Downloaded/";
    newDir.append(path);
    system(newDir.c_str());
}

int main(int argc, char *argv[])
{
    if(argc<2)
    {
        cout << "Missing argument." << endl;
        return 0;
    }

    //pair <string, string> url = parseArgument(argc, argv);
    set<string> toDownload;

    string site = argv[1];

    CDownload * newDown = new CDownload(site, 3);
    newDown->download();
    delete newDown;
/*
    CPage * newPage = newDown->getPage(url.first.c_str(), url.second.c_str());

    newPage->CParseLinks("href", toDownload);
    newPage->CParseLinks("HREF", toDownload);
    newPage->CParseLinks("src", toDownload);
    newPage->CParseLinks("SRC", toDownload);

//cout << newPage->getHtml();


    set<string>::iterator iter = toDownload.begin();
    iter++;
    iter++;
    iter++;
  //  cout << "Hambladampla: " <<(*iter) << endl;


    CPage * newPage2 = newDown->getPage(url.first.c_str(), url.second.append((*iter)).c_str());

cout << newPage2->getHtml();


    delete newDown;
    delete newPage;
    delete newPage2;
*/
    return 0;
}
