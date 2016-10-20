#include<iostream>
#include "CImage.h"

void CImage::download(int counter)
{
    mkDir( pathName() );
    cout << "Downloading: " << m_host << m_page;
    CDownload * newDown = new CDownload();
    newDown->getPage(m_host.c_str(),m_page.c_str(), fileName(), 2);
    delete newDown;
}
