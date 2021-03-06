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

#ifndef OFVERTICESITERATOR_H
#define OFVERTICESITERATOR_H

#include "ofMacros.h"
#include "ofMesh.h"
#include "ofIterator.h"

namespace of
{

#define SVERTEX typename ofVerticesIterator<_Traits>::sVertex
    #define IDS typename ofVerticesIterator<_Traits>::ids

template <class _Traits>
class ofVerticesIterator : public ofIterator<_Traits>
{
public:

    typedef typename _Traits::sVertex 	sVertex;
    typedef typename _Traits::ids 		ids;

    typedef typename _Traits::sMesh		sMesh;

    ofVerticesIterator(sMesh *_mesh);

    ~ofVerticesIterator();

    bool initialize(ids init = 0);

    bool finish();
    bool notFinish();

    bool operator++();
    bool operator--();

    sVertex *operator->();
    sVertex *operator*();

    ids operator&();

private:

    ids iatu, iinit;
    sVertex *atu;

    int status;
    // 0 - iterator sem posicao
    // 1 - iterator em posicao normal
    // 2 - iterator no limite inferior
    // 3 - iterator no limite superior
};

template <class _Traits>
ofVerticesIterator<_Traits>::ofVerticesIterator(sMesh *_mesh)
        : ofIterator<_Traits>(_mesh)
{
    iatu = iinit = -1;
    status = 0;
}

template <class _Traits>
ofVerticesIterator<_Traits>::~ofVerticesIterator()
{}

template <class _Traits>
bool ofVerticesIterator<_Traits>::initialize(ids init)
{
    OF_ASSERT(init >= 0);

    iinit = init;

    status = 1;

    iatu = -1;

    atu = NULL;

    return ++(*this);
}

template<class _Traits>
bool ofVerticesIterator<_Traits>::operator++()
{

    if(status == 1)
    {
        if( this->mesh->getNumberOfVertices() > 0 )
        {
            if(iatu < 0)
            {
                if(iinit < 0)
                    iinit = 0;
                else
                    if( iinit > this->mesh->getVertexMaxId() )
                    {
                        status = 3;
                        return false;
                    };

                iatu = iinit;

            }
            else
                iatu++;

            atu = this->mesh->getVertex(iatu);

            while( (!atu->inMesh()) && (iatu + 1 <= this->mesh->getVertexMaxId()) )
            {
                iatu++;
                atu = this->mesh->getVertex(iatu);
            }

            if ( !atu->inMesh() )
            {
                status = 3;
                return false;
            }

            OF_ASSERT( atu );

            return true;
        }
        else
        {
            status = 3;
            return false;
        }
    }
    else
        return false;
}

template<class _Traits>
bool ofVerticesIterator<_Traits>::operator--()
{

    if(status == 1)
    {
        if( this->mesh->getNumberOfVertices() > 0 )
        {
            if(iatu < 0)
            {
                if(iinit < 0)
                    iinit = this->mesh->getVertexMaxId();
                else
                    if( iinit > this->mesh->getVertexMaxId() )
                    {
                        // status = 3;
                        return false;
                    };

                iatu = iinit;

            }
            else
            {
                if( iatu == 0 )
                {
                    status = 2;
                    return false;
                }
                else
                    iatu--;
            }

            atu = this->mesh->getVertex(iatu);

            while( (!atu->inMesh()) && (iatu - 1 >= 0) )
            {
                iatu--;
                atu = this->mesh->getVertex(iatu);
            }

            if ( !atu->inMesh() )
            {
                status = 2;
                return false;
            }

            OF_ASSERT( atu );

            return true;
        }
        else
        {
            status = 2;
            return false;
        }
    }
    else
        return false;
}

template<class _Traits>
SVERTEX* ofVerticesIterator<_Traits>::operator->()
{
    return atu;
}

template<class _Traits>
SVERTEX* ofVerticesIterator<_Traits>::operator*()
{
    return atu;
}

template<class _Traits>
IDS ofVerticesIterator<_Traits>::operator&()
{
    return iatu;
}

template<class _Traits>
bool ofVerticesIterator<_Traits>::finish()
{
    return ((status == 2)||(status == 3));
}

template<class _Traits>
bool ofVerticesIterator<_Traits>::notFinish()
{
    return ((status == 0)||(status == 1));
}

#undef SVERTEX
    #undef IDS

}

#endif
