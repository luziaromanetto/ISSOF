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

#ifndef OFMESHOPER_H
#define OFMESHOPER_H

#include "ofMacros.h"
#include "ofMesh.h"

namespace of
{

/**	Base Operation Class for ofMesh
 *
 * 	_Traits must have: ids, sMesh
 */
template<class _Traits>
class ofMeshOper
{
public:

    typedef typename _Traits::ids	ids;

    typedef typename _Traits::sMesh		sMesh;

protected:

    /* Constructor */
    ofMeshOper(sMesh *_mesh);

    /* Destrutor */
    ~ofMeshOper();

    sMesh *mesh;
};

template <class _Traits>
ofMeshOper<_Traits>::ofMeshOper(sMesh *_mesh)
{
    OF_ASSERT(_mesh);
    mesh = _mesh;
}

template <class _Traits>
ofMeshOper<_Traits>::~ofMeshOper()
{}

}

#endif
