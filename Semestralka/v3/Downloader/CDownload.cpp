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

CDownload::CDownload()
{

}


CDownload::~CDownload()
{

}


bool CDownload::fail (int test, const char * format, ...)
{
    if (test)
    {
        va_list args;
        va_start (args, format);
        vfprintf (stderr, format, args);
        cout << endl;
        va_end (args);
        return false;
    }
    else return true;
}


string CDownload::get_html (int s, const char * host, const char * page, const string & dest, int mode)
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

    if(!fail (status == -1 || ! msg, "asprintf failed.\n"))
    {
        free(msg);
        return "Err";
    }

    /* Send the request. */

    status = send (s, msg, strlen (msg), 0);

    /* Check it succeeded. The FreeBSD manual page doesn't mention
       whether "send" sets errno, but
       "http://pubs.opengroup.org/onlinepubs/009695399/functions/send.html"
       claims it does. */

    if(!fail (status == -1, "send failed: %s\n", strerror (errno)))
    {
        free(msg);
        return "Err";
    }

    std::string strPage = "";
    pair<string,string> tmp;
    int body=0;
    ofstream ofile;
    ofile.open(dest, ios::binary | ios::out);
    while (1) {
	/* The number of bytes received. */
        int bytes;
	/* Our receiving buffer. */
        char buf[2];
	/* Get "BSIZE" bytes from "s". */
        bytes = recvfrom (s, buf, 1, 0, 0, 0);
	/* Stop once there is nothing left to print. */
        if (bytes == 0) {
            break;
        }
        if(!fail (bytes == -1, "%s\n", strerror (errno)))
        {
            ofile.close();
            free(msg);
            return "Err";
        }

        if(body==0 && strPage.find("\r\n\r\n")!=string::npos)
        {
            if(mode==0)
            {
                ofile.close();
                free(msg);
                return strPage;
            }
            body++;
        }
        if(body>0 && mode==2)ofile.put(buf[0]);
        buf[bytes] = '\0';
        if(body==0 || mode!=2)strPage.append(buf);
    }
    ofile.close();

    free (msg);
    if(mode==2) return "";
    return strPage;
}


string CDownload::getPage(const char * host, const char * page, const string & dest, int mode)
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
    if(!fail (error, gai_strerror(error))) return "Err";

    s = -1;
    for (res = res0; res; res = res->ai_next) {
        s = socket (res->ai_family, res->ai_socktype, res->ai_protocol);
        if(!fail (s < 0, "socket: %s\n", strerror (errno))) return "Err";
        if (connect(s, res->ai_addr, res->ai_addrlen) < 0) {
            fprintf (stderr, "connect: %s\n", strerror (errno));
            close(s);
            return "Err";
        }
        break;
    }
    string strPage;
    pair<string,string> Page;
    if (s != -1) {
        strPage = get_html (s, host, page, dest, mode);
    }
    freeaddrinfo (res0);
    return strPage;
}
