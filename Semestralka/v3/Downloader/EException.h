#ifndef EEXCEPTION_H
#define EEXCEPTION_H
#include <iostream>

using namespace std;

/**
 * Abstract class, which represents abstract exception.
 * Two classes inherit from this one - EMissingArgument and ESyntaxError
 * It contains pure method for printing error.
 */

class EException
{
    public:

        /**
         * Default constructor
         */
        EException();

        /**
         * Default destructor
         */
        virtual ~EException();

        /**
         * Pure method for printing error
         */
        virtual void Print() const = 0;

    private:

};


#endif // EEXCEPTION_H
