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

#ifndef OFITERATOR_H
#define OFITERATOR_H

#include "ofMacros.h"
#include "ofMesh.h"

namespace of
{

//! Iterator da estrutura of
/*!

 */
template<class _Traits>
class ofIterator
{
public:

    typedef typename _Traits::sMesh		sMesh;

    void setMesh( sMesh *_mesh);

protected:

    //! Construtor
    ofIterator(sMesh *_mesh);

    //! Destrutor
    ~ofIterator();

    sMesh *mesh;
};

template <class _Traits>
ofIterator<_Traits>::ofIterator(sMesh *_mesh)
{
    OF_ASSERT(_mesh);
    mesh = _mesh;
}

template <class _Traits>
ofIterator<_Traits>::~ofIterator()
{}

template <class _Traits>
void ofIterator<_Traits>::setMesh(sMesh *_mesh)
{
    OF_ASSERT(_mesh);
    mesh = _mesh;
}

}

#endif
