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

#ifndef OFCELL3D_H
#define OFCELL3D_H

#include "ofMacros.h"
#include "ofCell.h"

namespace of
{

/** Base class of tetrahedron
 *
 */
template <class _Traits>
class ofCell3D : public ofCell<4, _Traits>
{
public:

    typedef typename _Traits	::ids ids;

    /* Constructor */
    ofCell3D();

    /* Destructor */
    virtual ~ofCell3D();
};

template <class _Traits>
ofCell3D<_Traits>::ofCell3D()
{}

template <class _Traits>
ofCell3D<_Traits>::~ofCell3D()
{}

}

#endif
