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

#ifndef OFVERTEX3D_H
#define OFVERTEX3D_H

#include "ofMacros.h"
#include "ofSingularVertex.h"

namespace of
{

/** Base class of 3D vertex
 *
 */
template <class _Traits>
class ofVertex3D : public ofSingularVertex<3, _Traits>
{
public:

    typedef typename _Traits::space space;

    /* Constructor */
    ofVertex3D();
    ofVertex3D(space x, space y, space z);

    /* Destructor */
    ~ofVertex3D();
};

template <class _Traits>
ofVertex3D<_Traits>::ofVertex3D()
{}

template <class _Traits>
ofVertex3D<_Traits>::ofVertex3D(space x, space y, space z)
{
    this->setCoord(0, x);
    this->setCoord(1, y);
    this->setCoord(2, z);
}

template <class _Traits>
ofVertex3D<_Traits>::~ofVertex3D()
{}

}

#endif
