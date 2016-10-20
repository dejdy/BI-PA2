#ifndef ESYNTAXERROR_H
#define ESYNTAXERROR_H
#include <iostream>
#include "EException.h"

using namespace std;

/**
 * Class which inherits from EException.
 * It is thrown when syntax error is detected.
 */

class ESyntaxError : public EException
{
    public:

        /**
         * Default constructor
         */
        ESyntaxError();

        /**
         * Default destructor
         */
        virtual ~ESyntaxError();

        /**
         * Method for printing Syntax error
         */
        virtual void Print() const;

    private:

};


#endif // ESYNTAXERROR_H
