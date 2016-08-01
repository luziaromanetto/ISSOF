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

#ifndef OFEDGESTARITERATOR3D_H
#define OFEDGESTARITERATOR3D_H

#include "ofMacros.h"
#include "ofMesh.h"
#include "ofSing.h"
#include "ofIterator.h"
#include "ofList.h"

namespace of
{

#define SCELL typename ofEdgeStarIterator3D<_Traits>::sCell
    #define IDS typename ofEdgeStarIterator3D<_Traits>::ids

template <class _Traits>
class ofEdgeStarIterator3D : public ofIterator<_Traits>
{
public:

    typedef typename _Traits::sCell 	sCell;
    typedef typename _Traits::sVertex 	sVertex;
    typedef typename _Traits::ids 		ids;

    typedef ofSing<_Traits>				sSing;
    typedef typename _Traits::sMesh		sMesh;

    ofEdgeStarIterator3D(sMesh *_mesh);

    ~ofEdgeStarIterator3D();

    bool initialize(ids idcell, int index1, int index2);

    bool finish();
    bool notFinish();

    bool operator++();

    sCell *operator->();
    sCell *operator*();

    ids operator&();

private:

    ids iatu, iinit, iv;
    ids vie1, vie2;
    sCell *atu;
    sVertex *v;

    ofList<ids> lista, flags;

    int status, is;
    // 0 - iterator sem posicao
    // 1 - iterator em posicao normal
    // 2 - iterator no limite inferior
    // 3 - iterator no limite superior

    void addNeighbor();
};

template <class _Traits>
ofEdgeStarIterator3D<_Traits>::ofEdgeStarIterator3D(sMesh *_mesh)
        : ofIterator<_Traits>(_mesh)
{
    is = iatu = iinit = iv -1;
    vie1 = vie2 = -1;
    status = 0;
    v = NULL;
}

template <class _Traits>
ofEdgeStarIterator3D<_Traits>::~ofEdgeStarIterator3D()
{}

template <class _Traits>
bool ofEdgeStarIterator3D<_Traits>::initialize(ids idcell, int index1, int index2)
{
    OF_ASSERT(idcell >= 0);
    OF_ASSERT((index1 >= 0)&&(index1 <= sCell::getDimension()));
    OF_ASSERT((index2 >= 0)&&(index2 <= sCell::getDimension()));

    vie1 = this->mesh->getCell(idcell)->getVertexId(index1);
    vie2 = this->mesh->getCell(idcell)->getVertexId(index2);

    iinit = idcell;

    status = 1;

    iatu = -1;

    atu = NULL;

    lista.clear();
    flags.clear();

    return ++(*this);
}

template<class _Traits>
bool ofEdgeStarIterator3D<_Traits>::operator++()
{
    if(status == 1)
    {
        if(iatu < 0)
        {
            atu = this->mesh->getCell(iinit);
            OF_ASSERT( (atu) && (atu->inMesh()) );

            iatu = iinit;

            flags.insert(iatu);

            addNeighbor();
        }
        else
        {
            if(lista.empty())
            {
                atu = NULL;
                iatu = -1;
                status = 3;
                return false;
            }
            else
            {

                iatu  = lista.pos(0);
                lista.erase(0);

                atu = this->mesh->getCell(iatu);
                OF_ASSERT( (atu) && (atu->inMesh()) );

                addNeighbor();
            }
        }

        return true;
    }
    else
        return false;
}

template<class _Traits>
void ofEdgeStarIterator3D<_Traits>::addNeighbor()
{
    ids temp;

    for(int i=0; i<4; i++)
    {
        if((atu->getVertexId(i)!=vie1)&&(atu->getVertexId(i)!=vie2))
        {
        	temp = atu->getMateId(i);

            if((temp >=0)&&(!flags.inList(temp)))
            {
                flags.insert(temp);
                lista.insert(temp);
            }
        }
    }
}

template<class _Traits>
SCELL* ofEdgeStarIterator3D<_Traits>::operator->()
{
    return atu;
}

template<class _Traits>
SCELL* ofEdgeStarIterator3D<_Traits>::operator*()
{
    return atu;
}

template<class _Traits>
IDS ofEdgeStarIterator3D<_Traits>::operator&()
{
    return iatu;
}

template<class _Traits>
bool ofEdgeStarIterator3D<_Traits>::finish()
{
    return ((status == 2)||(status == 3));
}

template<class _Traits>
bool ofEdgeStarIterator3D<_Traits>::notFinish()
{
    return ((status == 0)||(status == 1));
}

#undef SCELL
    #undef IDS

}

#endif
