#include<iostream>
#include "CImage.h"

CImage::CImage()
{

}


CImage::~CImage()
{

}


bool CImage::download(int depth)
{
    mkDir( pathName() );
    cout << "Downloading: " << m_host << m_page;
    CDownload * newDown = new CDownload();
    if(newDown->getPage(m_host.c_str(),m_page.c_str(), fileName(), 2)=="Err")
    {
        delete newDown;
        return false;
    }
    delete newDown;
    return true;
}

