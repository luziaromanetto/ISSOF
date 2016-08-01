/*
 * Copyright (C) Mario A. S. Liziér 2004 <lizier at gmail dot com>
 * Copyright (C) Luis Gustavo Nonato 2004 <gnonato at icmc dot usp dot br>
 *
 * OF (Opposite Face) is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * OF is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef OFBOOLEANVECTOR_H
#define OFBOOLEANVECTOR_H

#include "ofMacros.h"

namespace of
{

/**	Boolean Vector
 *
 * 	This is a block allocation boolean version of vector with free space management
 */
template <class ids>
class ofBooleanVector
{
public:

    /**	Constructor
     *
     * 	\param block_size: the size of each block (The maximum capacity of vector if : block_size * block_size)
     */
    ofBooleanVector(ids block_size);

    /**	Destructor
     */
    ~ofBooleanVector();

    /** Return the specified element
     *
     * \param index: position of element
     */
    bool operator[](ids index);

    /** Define the specified boolean element
     *
     * \param index: position of element
     * \param value: value of element
    */
    void setValue(ids index, bool value);

private:

    char** vetor;
    int block, blocke;

};

template <class ids>
ofBooleanVector<ids>::ofBooleanVector(ids block_size)
{
    OF_ASSERT(block_size > 0);

    if( block_size%sizeof(char) == 0 )
        block = block_size/sizeof(char);
    else
        block = block_size/sizeof(char) + 1;

    blocke = block_size*sizeof(char);

    vetor = new char*[block];

    OF_ASSERT(vetor);

    for(ids off1=0; off1 < block; off1++)
        vetor[off1] = NULL;
}

template <class ids>
ofBooleanVector<ids>::~ofBooleanVector()
{
    for(ids off1=0; off1 < block; off1++)
        if(vetor[off1])
            delete [] vetor[off1];

    delete [] vetor;
}

template <class ids>
bool ofBooleanVector<ids>::operator [](ids index)
{
    OF_ASSERT(index >= 0);
    OF_ASSERT(index < blocke*blocke);

    ids pos = index / blocke;

    if(vetor[pos] == NULL)
    {
        vetor[pos] = new char[block];
        OF_ASSERT(vetor[pos]);
        for(ids off1=0; off1 < block; off1++)
            vetor[pos][off1] = 0;
    }

    return (vetor[pos][index % blocke] & (1 << (index % sizeof(char)))) != 0;
}

template <class ids>
void ofBooleanVector<ids>::setValue(ids index, bool value)
{
    OF_ASSERT(index >= 0);
    OF_ASSERT(index < blocke*blocke);

    ids pos = index / blocke;

    if(vetor[pos] == NULL)
    {
        vetor[pos] = new char[block];
        OF_ASSERT(vetor[pos]);
        for(ids off1=0; off1 < block; off1++)
            vetor[pos][off1] = 0;
    }

    if(value)
        vetor[pos][index % blocke] |= (1 << (index % sizeof(char)));
    else
        vetor[pos][index % blocke] &= ~(1 << (index % sizeof(char)));
}
}

#endif
