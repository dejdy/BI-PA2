#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdarg.h>
#include <string>
#include <fstream>

#include "CDownload.h"

/* "BSIZE" is the size of the buffer we use to read from the socket. */

#define BSIZE 0x1000

/* Quickie function to test for failures. It is actually better to use
   a macro here, since a function like this results in unnecessary
   function calls to things like "strerror". However, not every
   version of C has variadic macros. */


void CDownload::fail (int test, const char * format, ...)
{
    if (test) {
	va_list args;
	va_start (args, format);
	vfprintf (stderr, format, args);
	va_end (args);
	exit (EXIT_FAILURE);
    }
}

/* Get the web page and print it to standard output. */


CPage * CDownload::get_html (int s, const char * host, const char * page)
{
    /* "msg" is the request message that we will send to the
       server. */

    char * msg;

    /* "format" is the format of the HTTP request we send to the web
       server. */

    const char * format = "GET /%s HTTP/1.0\r\nHost: %s\r\nUser-Agent: fetch.c\r\n\r\n";

    /* This holds return values from functions. */

    int status;

    /* I am using non-standard function "asprintf" for convenience. If
       you don't have "asprintf", use "snprintf" with a fixed-size
       buffer and check its return value against the length of the
       buffer after the call. */

    status = asprintf (& msg, format, page, host);

    /* Check that "asprintf" succeeded. */

    fail (status == -1 || ! msg, "asprintf failed.\n");

    /* Send the request. */

    status = send (s, msg, strlen (msg), 0);

    /* Check it succeeded. The FreeBSD manual page doesn't mention
       whether "send" sets errno, but
       "http://pubs.opengroup.org/onlinepubs/009695399/functions/send.html"
       claims it does. */

    fail (status == -1, "send failed: %s\n", strerror (errno));

    std::string strPage = "";

    while (1) {
	/* The number of bytes received. */
        int bytes;
	/* Our receiving buffer. */
        char buf[BSIZE+10];
	/* Get "BSIZE" bytes from "s". */
        bytes = recvfrom (s, buf, BSIZE, 0, 0, 0);
	/* Stop once there is nothing left to print. */
        if (bytes == 0) {
            break;
        }
        fail (bytes == -1, "%s\n", strerror (errno));
	/* Nul-terminate the string before printing. */
        buf[bytes] = '\0';
	/* Print it to standard output. */


        strPage.append(buf);
    }

    free (msg);
    CPage * Page = new CPage(strPage);
    m_pages.push_back(Page);
    return Page;
}

CPage * CDownload::getPage(const char * host, const char * page)
{
    struct addrinfo hints, *res, *res0;
    int error;
    /* "s" is the file descriptor of the socket. */
    int s;

    memset (&hints, 0, sizeof(hints));
    /* Don't specify what type of internet connection. */
    hints.ai_family = PF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    error = getaddrinfo (host, "http", & hints, & res0);
    fail (error, gai_strerror(error));
    s = -1;
    for (res = res0; res; res = res->ai_next) {
        s = socket (res->ai_family, res->ai_socktype, res->ai_protocol);
        fail (s < 0, "socket: %s\n", strerror (errno));
        if (connect(s, res->ai_addr, res->ai_addrlen) < 0) {
            fprintf (stderr, "connect: %s\n", strerror (errno));
            close(s);
            exit (EXIT_FAILURE);
        }
        break;
    }

    CPage * Page = NULL;
    if (s != -1) {
        Page = get_html (s, host, page);
    }
    freeaddrinfo (res0);
    return Page;
}

void mkDir(string path)
{
    string newDir;
    newDir="mkdir -p Downloaded/";
    newDir.append(path);
    system(newDir.c_str());
}

void CDownload::download()
{
    set<string> toDownload;
    CPage * newPage = getPage(m_webpage.c_str(), "");

    newPage->CParseLinks("href", toDownload);
    newPage->CParseLinks("HREF", toDownload);
    newPage->CParseLinks("src", toDownload);
    newPage->CParseLinks("SRC", toDownload);

    ofstream ofs;
    string path = "Downloaded/";
    path.append(m_webpage);
    path.append("/main.html");
    mkDir(m_webpage);
    ofs.open(path);
    ofs << newPage->getHtml();
    ofs.close();
/*
    for(int i=0; i<5; i++)
    {
        string cur = "/";
        toDownload.erase(toDownload.begin());
        cur.append(*toDownload.begin());
        cout << "Cur: " << cur << endl;
        toDownload.erase(toDownload.begin());
        CPage * newPage = getPage(m_webpage.c_str(), cur.c_str());
        ofstream ofs;
        string path = "Downloaded/";
        path.append(m_webpage);
        path.append(cur);
        path.append("/index.html");
        mkDir(m_webpage.append(cur));
        ofs.open(path);
        ofs << newPage->getHtml();
        ofs.close();
    }

/*
    while(toDownload.size()>0)
    {
        string cur = *toDownload.begin();
        toDownload.erase(toDownload.begin());

    }
*/

    //newPage->print(toDownload);
//cout << newPage->getHtml();


  //  cout << "Hambladampla: " <<(*iter) << endl;


//    CPage * newPage2 = newDown->getPage(url.first.c_str(), url.second.append((*iter)).c_str());

//cout << newPage2->getHtml();

    delete newPage;

}
