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

#ifndef OFBOUNDARYCELLITERATORSURFACE_H
#define OFBOUNDARYCELLITERATORSURFACE_H

#include "ofMacros.h"
#include "ofMesh.h"
#include "ofIterator.h"
#include "ofObjectVector.h"
#include "ofList.h"
#include "ofCellsIterator.h"

namespace of
{

#define SCELL typename ofBoundaryCellIteratorSurface<_Traits>::sCell
#define IDS typename ofBoundaryCellIteratorSurface<_Traits>::ids

template <class _Traits>
class ofBoundaryCellIteratorSurface : public ofIterator<_Traits>
{
public:

    typedef typename _Traits::sCell 	sCell;
    typedef typename _Traits::ids 		ids;
    typedef typename _Traits::sMesh		sMesh;

    ofBoundaryCellIteratorSurface(sMesh *_mesh);

    ~ofBoundaryCellIteratorSurface();

    bool initialize(ids init = -1, int face = -1);

    bool finish();

    bool notFinish();

    bool operator++();

    sCell *operator->();
    sCell *operator*();

    ids operator&();

    int getFace();

private:

    struct pair
    {
        ids a, b;
    };

    struct schar
    {
        schar()
        {
            value=0;
        };
        char value;
    };

    ofObjectVector<schar,ids> flags;
    ofList<pair> lista;

    ids iatu;
    sCell *atu;

    int eatu, status;
    // 0 - iterator sem posicao
    // 1 - iterator em posicao normal
    // 2 - iterator no limite inferior
    // 3 - iterator no limite superior

    bool isFlag(schar flag, int index);
    void addFlag(schar &flag, int index);
    void addNeighbor();
};

template <class _Traits>
ofBoundaryCellIteratorSurface<_Traits>::ofBoundaryCellIteratorSurface(sMesh *_mesh)
        : ofIterator<_Traits>(_mesh), flags(_mesh->getBlockSize())
{
    status = 0;
    iatu = eatu = -1;
    atu = NULL;
}

template <class _Traits>
ofBoundaryCellIteratorSurface<_Traits>::~ofBoundaryCellIteratorSurface()
{}

template <class _Traits>
bool ofBoundaryCellIteratorSurface<_Traits>::initialize(ids init, int face)
{
    if (init < 0)
    {
        ofCellsIterator<_Traits> itc(this->mesh);
        bool ficar = true;
        for(itc.initialize(); itc.notFinish() && ficar; ++itc)
            for(int i=0; i<3; i++)
                if( itc->getMateId(i) < 0)
                {
                    init = &itc;
                    face = i;
                    ficar = false;
                    break;
                }
    }

    OF_ASSERT(init >= 0);

    iatu = init;

    atu = this->mesh->getCell(iatu);

    OF_ASSERT(atu->inMesh());

    if(face < 0)
    {
        if(atu->getMateId(0) < 0)
            eatu = 0;
        else if(atu->getMateId(1) < 0)
            eatu = 1;
        else if(atu->getMateId(2) < 0)
            eatu = 2;
    }
    else
        eatu = face;

    OF_ASSERT((eatu>=0)&&(eatu<3));

    addFlag(flags[iatu],eatu);

    addNeighbor();

    status = 1;

    return true;
}

template<class _Traits>
bool ofBoundaryCellIteratorSurface<_Traits>::operator++()
{
    if(status == 1)
    {
        if(lista.empty())
        {
            atu = NULL;
            iatu = -1;
            eatu = -1;
            status = 3;
            return false;
        }
        else
        {
            pair p = lista.pos(0);
            iatu  = p.a;
            eatu  = p.b;
            lista.erase(0);

            atu = this->mesh->getCell(iatu);
            OF_ASSERT( (atu) && (atu->inMesh()) );

            addNeighbor();
        }

        return true;
    }
    else
        return false;
}

template<class _Traits>
void ofBoundaryCellIteratorSurface<_Traits>::addFlag(schar &flag, int index)
{
    flag.value |= (1 << index);
}

template<class _Traits>
bool ofBoundaryCellIteratorSurface<_Traits>::isFlag(schar flag, int index)
{
    return ((flag.value & (1 << index)) != 0);
}

template<class _Traits>
void ofBoundaryCellIteratorSurface<_Traits>::addNeighbor()
{
    ids idx[2] = {(eatu+1)%4, (eatu+2)%4 };

    //verificar
    for(int edge = 0; edge < 2; edge++)
    {
        int iant = iatu;
        int eant = idx[edge];
        sCell *cant = this->mesh->getCell(iant);
        int e1 = cant->getVertexId(idx[(edge+1)%3]);
        int e2 = cant->getVertexId(idx[(edge+2)%3]);

        bool loop = true;
        while(loop)
        {
            int itemp, etemp;
            itemp = cant->getMateId(eant);

            if(itemp >= 0 )
            {
                sCell *ctemp = this->mesh->getCell(itemp);
                for(int i=0; i<4; i++)
                    if( (ctemp->getMateId(i) != iant)&&(ctemp->getVertexId(i)!=e1)&&(ctemp->getVertexId(i)!=e2) )
                    {
                        etemp = i;
                        break;
                    }
                iant = itemp;
                cant = ctemp;
                eant = etemp;
            }
            else
                loop = false;
        }

        if(!isFlag(flags[iant],eant))
        {
            pair p;
            p.a = iant;
            p.b = eant;
            lista.insert(p);
            addFlag(flags[iant], eant);
        }
    }
}

template<class _Traits>
SCELL* ofBoundaryCellIteratorSurface<_Traits>::operator->()
{
    return atu;
}

template<class _Traits>
SCELL* ofBoundaryCellIteratorSurface<_Traits>::operator*()
{
    return atu;
}

template<class _Traits>
IDS ofBoundaryCellIteratorSurface<_Traits>::operator&()
{
    return iatu;
}

template<class _Traits>
bool ofBoundaryCellIteratorSurface<_Traits>::finish()
{
    return ((status == 2)||(status == 3));
}

template<class _Traits>
bool ofBoundaryCellIteratorSurface<_Traits>::notFinish()
{
    return ((status == 0)||(status == 1));
}

template<class _Traits>
int ofBoundaryCellIteratorSurface<_Traits>::getFace()
{
    if(status != 0)
        return eatu;

    return -1;
}

#undef SCELL
    #undef IDS

}

#endif
