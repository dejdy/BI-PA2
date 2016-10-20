#include <iostream>
#include <queue>
#include "CDownload.h"
#include "CObject.h"
#include "CPage.h"
#include "CArguments.h"
#include "EMissingArgument.h"
using namespace std;

void delFiles(CArguments * args)
{
    string cmd = "rm -rf ";
    cmd.append(args->getPath());
    if(cmd[cmd.size()-1]!='/' && args->getSite().first[0]!='/') cmd.append("/");
    cmd.append(args->getSite().first);
    system(cmd.c_str());
}

void delAll(queue<pair<CObject*,int> > & q, CArguments * args, CPage * p)
{
    pair<CObject*,int> cur;
    while(q.size()>0)
    {
        cur = q.front();
        q.pop();
        delete cur.first;
    }
    delete args;
    delete p;
}

int main(int argc, char *argv[])
{
    set<string> Downloaded;
    Downloaded.insert("/");
    queue<pair<CObject*, int> > toDownload;
    pair<CObject*,int> cur;
    CArguments * args;
    try
    {
        args = new CArguments(argc, argv);
    }
    catch (const EException & e)
    {
        e.Print();
        return -1;
    }

    cout << "------ DOWNLOADER ------" << endl;
    cout << " You chose to download: " << args->getSite().first << endl << endl;

    CPage * p = new CPage(args->getSite().first, args->getSite().second, &toDownload, &Downloaded, args);

    if(!p->download(0))
    {
        delFiles(args);
        delAll(toDownload, args, p);
        return -1;
    }

    while(toDownload.size()>0)
    {
        cur = toDownload.front();
        if(cur.second>=args->getDepth()) break;
        cout << " ... Num of files in queue: " << toDownload.size() << endl;
        toDownload.pop();
        if(!cur.first->download(cur.second+1))
        {
            delFiles(args);
            delete cur.first;
            delAll(toDownload, args, p);
            return -1;
        }
        delete cur.first;
    }
    cout << " ... Num of files in queue: " << toDownload.size() << endl;
    cout << "... Done!" << endl;

    delAll(toDownload, args, p);
    return 0;
}
