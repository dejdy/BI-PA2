#include <iostream>
#include "ESyntaxError.h"

using namespace std;

ESyntaxError::ESyntaxError()
{

}


ESyntaxError::~ESyntaxError()
{

}


void ESyntaxError::Print() const
{
    cout << "Syntax error." << endl;
}
