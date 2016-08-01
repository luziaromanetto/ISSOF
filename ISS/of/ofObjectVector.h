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

#ifndef OFOBJECTVECTOR_H
#define OFOBJECTVECTOR_H

#include "ofMacros.h"

namespace of
{

/**	Object Vector
 *
 * 	This is a block allocation boolean version of vector with free space management
 */
template <class T, class ids>
class ofObjectVector
{
public:

    /**	Constructor
     *
     * 	\param block_size: the size of each block (The maximum capacity of vector if : block_size * block_size)
     */
    ofObjectVector(ids block_size);

    /**	Destructor
     */
    ~ofObjectVector();

    /** Return the specified element
     *
     * \param index: position of element
     */
    T& operator[](ids index);


private:

    T** vetor;
    int block, blocke;

};

template <class T, class ids>
ofObjectVector<T,ids>::ofObjectVector(ids block_size)
{
    OF_ASSERT(block_size > 0);

    block = block_size;

    vetor = new T*[block];

    OF_ASSERT(vetor);

    for(ids off1=0; off1 < block; off1++)
        vetor[off1] = NULL;
}

template <class T, class ids>
ofObjectVector<T,ids>::~ofObjectVector()
{
    for(ids off1=0; off1 < block; off1++)
        if(vetor[off1])
            delete [] vetor[off1];

    delete [] vetor;
}

template <class T, class ids>
T& ofObjectVector<T,ids>::operator [](ids index)
{
    OF_ASSERT(index >= 0);
    OF_ASSERT(index < block*block);

    if(vetor[index / block] == NULL)
    {
        vetor[index / block] = new T[block];
        OF_ASSERT(vetor[index / block]);
    }

    return vetor[index / block][index % block];
}
}

#endif
