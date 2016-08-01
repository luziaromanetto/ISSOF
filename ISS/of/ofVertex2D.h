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

#ifndef OFVERTEX2D_H
#define OFVERTEX2D_H

#include "ofMacros.h"
#include "ofSingularVertex.h"

namespace of
{

/** Base class of 2D vertex
 *
 */
template <class _Traits>
class ofVertex2D : public ofSingularVertex<2, _Traits>
{
public:

    typedef typename _Traits::space space;

    /* Constructor */
    ofVertex2D();
    ofVertex2D(space x, space y);

    /* Destructor */
    ~ofVertex2D();
};

template <class _Traits>
ofVertex2D<_Traits>::ofVertex2D()
{}

template <class _Traits>
ofVertex2D<_Traits>::ofVertex2D(space x, space y)
{
    this->setCoord(0, x);
    this->setCoord(1, y);
}


template <class _Traits>
ofVertex2D<_Traits>::~ofVertex2D()
{}

}

#endif
