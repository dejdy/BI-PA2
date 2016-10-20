#ifndef EMISSINGARGUMENT_H
#define EMISSINGARGUMENT_H
#include <iostream>
#include "EException.h"

using namespace std;

/**
 * Class which inherits from EException.
 * It is thrown when too few arguments are passed.
 */


class EMissingArgument : public EException
{
    public:

        /**
         * Default constructor
         */
        EMissingArgument();

        /**
         * Default destructor
         */
        virtual ~EMissingArgument();

        /**
         * Method for printing Missing argument error.
         */
        virtual void Print() const;

    private:

};


#endif // EMISSINGARGUMENT_H
