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

#ifndef OFTOPOLOGY_H
#define OFTOPOLOGY_H

#include "ofMacros.h"
#include "ofMesh.h"

namespace of
{

#define IDS typename ofTopology<_Traits>::ids


template <class _Traits>
class ofTopology
{
public:

    typedef typename _Traits::ids ids;
    typedef typename _Traits::sVertex sVertex;
    typedef typename _Traits::sCell sCell;

    typedef typename _Traits::sMesh		sMesh;

    ofTopology(sMesh *_mesh = NULL);
    ~ofTopology();

    void setMesh(sMesh *_mesh);

    void flip2D(ids c1, ids c2 OF_DMUTEXVD);

    void addOnEdge(ids idcell, int index, ids idvertex);
    ids collapseEdge(ids idcell1, int index1);

private:

    sMesh *mesh;
};

template <class _Traits>
ofTopology<_Traits>::ofTopology(sMesh *_mesh)
{
    mesh = _mesh;
}

template <class _Traits>
ofTopology<_Traits>::~ofTopology()
{}

template <class _Traits>
void ofTopology<_Traits>::setMesh(sMesh *_mesh)
{
    mesh = _mesh;
}

template <class _Traits>
void ofTopology<_Traits>::flip2D(ids c1, ids c2 OF_DMUTEXV)
{
    sCell 	*temp,
    *cell1 = mesh->getCell(c1),
             *cell2 = mesh->getCell(c2);

    int	e1 = cell1->getMateIndex(c2),
             e2 = cell2->getMateIndex(c1),
                  pos;
    sVertex *v;


    // atualizando os opostos
    cell1->setMateId(e1, cell2->getMateId((e2+2)%3));
    if(cell2->getMateId((e2+2)%3) >= 0)
    {
        temp = mesh->getCell(cell2->getMateId((e2+2)%3));
        temp->setMateId(temp->getMateIndex(c2), c1);
    }

    cell2->setMateId(e2, cell1->getMateId((e1+2)%3));
    if(cell1->getMateId((e1+2)%3) >= 0)
    {
        temp = mesh->getCell(cell1->getMateId((e1+2)%3));
        temp->setMateId(temp->getMateIndex(c1), c2);
    }


    cell1->setMateId((e1+2)%3, c2);
    cell2->setMateId((e2+2)%3, c1);

    // atualizando os vertices

    cell1->setVertexId((e1+1)%3, cell2->getVertexId(e2));
    cell2->setVertexId((e2+1)%3, cell1->getVertexId(e1));

    v = mesh->getVertex(cell1->getVertexId(e1));
    pos = v->inSings(c1);
    if(pos >= 0)
        v->setSingCell(pos, c2);

    v = mesh->getVertex(cell2->getVertexId(e2));
    pos = v->inSings(c2);
    if(pos >= 0)
        v->setSingCell(pos, c1);

    v = mesh->getVertex(cell1->getVertexId((e1+2)%3));
    pos = v->inSings(c2);
    if(pos >= 0)
        v->setSingCell(pos, c1);

    v =  mesh->getVertex(cell2->getVertexId((e2+2)%3));
    pos = v->inSings(c1);
    if(pos >= 0)
        v->setSingCell(pos, c2);
}

template <class _Traits>
void ofTopology<_Traits>::addOnEdge(ids idcell, int index, ids idvertex)
{
    ids coposto;
    sCell *cell = mesh->getCell(idcell);
    coposto = cell->getMateId(index);

    // possui celula oposta
    if(coposto >= 0)
    {
        ids vids[4];

        vids[0] = cell->getVertexId(index);
        vids[1] = cell->getVertexId((index+1)%3);
        vids[2] = mesh->getCell(coposto)->getVertexMateId(idcell);
        vids[3] = cell->getVertexId((index+2)%3);

        ids newcell[3];

        mesh->delCell(idcell);
        mesh->delCell(coposto);

        newcell[0] = idvertex;
        newcell[1] = vids[0];
        newcell[2] = vids[1];
        mesh->addCell(newcell);

        newcell[0] = idvertex;
        newcell[1] = vids[1];
        newcell[2] = vids[2];
        mesh->addCell(newcell);

        newcell[0] = idvertex;
        newcell[1] = vids[2];
        newcell[2] = vids[3];
        mesh->addCell(newcell);

        newcell[0] = idvertex;
        newcell[1] = vids[3];
        newcell[2] = vids[0];
        mesh->addCell(newcell);

    }
    else
    {
        ids vids[3];

        vids[0] = cell->getVertexId(index);
        vids[1] = cell->getVertexId((index+1)%3);
        vids[2] = cell->getVertexId((index+2)%3);

        ids newcell[3];

        mesh->delCell(idcell);

        newcell[0] = idvertex;
        newcell[1] = vids[0];
        newcell[2] = vids[1];
        mesh->addCell(newcell);

        newcell[0] = idvertex;
        newcell[1] = vids[2];
        newcell[2] = vids[0];
        mesh->addCell(newcell);
    }
}

template <class _Traits>
IDS ofTopology<_Traits>::collapseEdge(ids idcell1, int index1)
{
    ids idcell2, idvertex1, idvertex2, idctemp, idcfirst;
    sCell *ctemp;
    sCell *cell1 = mesh->getCell(idcell1);
    idcell2 = cell1->getMateId(index1);
    int sing;
    sVertex *v1, *v2;

    if(idcell2 >= 0)
    {

        sCell *cell2 = mesh->getCell(idcell2);
        ids index2 = cell2->getMateIndex( idcell1 );

        idvertex1 = cell1->getVertexId( (index1+1)%3 );
        idvertex2 = cell1->getVertexId( (index1+2)%3 );

        // Verificando o surgimento de aresta sem celulas
        OF_ASSERT((cell1->getMateId((index1+1)%3)>=0)||(cell1->getMateId((index1+2)%3)>=0));
        OF_ASSERT((cell2->getMateId((index2+1)%3)>=0)||(cell2->getMateId((index2+2)%3)>=0));

        v1 = mesh->getVertex( idvertex1 );
        v2 = mesh->getVertex( idvertex2 );
        sing = v2->getNumberOfSings();

        for(int i = 0; i < sing; i++)
        {
            idctemp = v2->getSingCell(i);
            idcfirst = idctemp;

            do
            {
                ctemp = mesh->getCell(idctemp);

                idctemp = ctemp->getMateId((ctemp->getVertexIndex(idvertex2)+1)%3);
                ctemp->setVertexId(ctemp->getVertexIndex(idvertex2), idvertex1);
            }
            while( (idctemp != idcfirst)&&(idctemp >= 0) );
        }

        idctemp = cell1->getMateId((index1+1)%3);
        if(idctemp >= 0)
        {
            ctemp = mesh->getCell( idctemp );
            ctemp->setMateId(ctemp->getMateIndex(idcell1), cell1->getMateId((index1+2)%3) );
        }

        idctemp = cell1->getMateId((index1+2)%3);
        if(idctemp >= 0)
        {
            ctemp = mesh->getCell( idctemp );
            ctemp->setMateId(ctemp->getMateIndex(idcell1), cell1->getMateId((index1+1)%3) );
        }

        idctemp = cell2->getMateId((index2+2)%3);
        if(idctemp >= 0)
        {
            ctemp = mesh->getCell( idctemp );
            ctemp->setMateId(ctemp->getMateIndex(idcell2), cell2->getMateId((index2+1)%3) );
        }

        idctemp = cell2->getMateId((index2+1)%3);
        if(idctemp >= 0)
        {
            ctemp = mesh->getCell( idctemp );
            ctemp->setMateId(ctemp->getMateIndex(idcell2), cell2->getMateId((index2+2)%3) );
        }

        sVertex *vtemp;

        vtemp = mesh->getVertex( cell1->getVertexId(index1) );
        if ( (sing = vtemp->inSings(idcell1)) >= 0)
            vtemp->setSingCell( sing, cell1->getMateId((index1+1)%3) );

        vtemp = mesh->getVertex( cell2->getVertexId(index2) );
        if ( (sing = vtemp->inSings(idcell2)) >= 0)
            vtemp->setSingCell( sing, cell2->getMateId((index2+1)%3) );



        vtemp = mesh->getVertex( cell1->getVertexId((index1+1)%3) );
        if ((sing = vtemp->inSings(idcell2)) >= 0)
        {
            if (cell2->getMateId((index2+2)%3) >=0)
                vtemp->setSingCell( sing, cell2->getMateId((index2+2)%3) );
            else
                vtemp->delSing( sing );
        }
        else
        {
            if( vtemp->getNumberOfSings() == 1 )
                vtemp->clearSings();
        }

        vtemp = mesh->getVertex( cell1->getVertexId((index1+2)%3) );
        if ((sing = vtemp->inSings(idcell1)) >= 0)
        {
            if (cell1->getMateId((index1+2)%3) >=0)
                vtemp->setSingCell( sing, cell1->getMateId((index1+2)%3) );
            else
                vtemp->delSing( sing );
        }
        else
        {
            if( vtemp->getNumberOfSings() == 1 )
                vtemp->clearSings();
        }

        mesh->freeCell(idcell2);
    }
    else
    {
        idvertex1 = cell1->getVertexId( (index1+1)%3 );
        idvertex2 = cell1->getVertexId( (index1+2)%3 );

        // Verificando o surgimento de aresta sem celulas
        OF_ASSERT((cell1->getMateId((index1+1)%3)>=0)||(cell1->getMateId((index1+2)%3)>=0));

        sVertex* v1 = mesh->getVertex( idvertex1 );
        sVertex* v2 = mesh->getVertex( idvertex2 );
        int sing = v2->getNumberOfSings();

        for(int i = 0; i < sing; i++)
        {
            idctemp = v2->getSingCell(i);

            do
            {
                ctemp = mesh->getCell(idctemp);

                idctemp = ctemp->getMateId((ctemp->getVertexIndex(idvertex2)+1)%3);
                ctemp->setVertexId(ctemp->getVertexIndex(idvertex2), idvertex1);
            }
            while(idctemp >= 0);
        }

        idctemp = cell1->getMateId((index1+1)%3);
        if(idctemp >= 0)
        {
            ctemp = mesh->getCell( idctemp );
            ctemp->setMateId(ctemp->getMateIndex(idcell1), cell1->getMateId((index1+2)%3) );
        }

        idctemp = cell1->getMateId((index1+2)%3);
        if(idctemp >= 0)
        {
            ctemp = mesh->getCell( idctemp );
            ctemp->setMateId(ctemp->getMateIndex(idcell1), cell1->getMateId((index1+1)%3) );
        }

        sVertex *vtemp = mesh->getVertex( cell1->getVertexId(index1) );
        if ( (sing = vtemp->inSings(idcell1)) >= 0)
            vtemp->setSingCell( sing, cell1->getMateId((index1+1)%3) );

        OF_ASSERT(v1->inSings(idcell1) >= 0);
        v1->delSing( v1->inSings(idcell1) );

        if ( (sing = v2->inSings(idcell1)) >= 0)
            v2->setSingCell( sing, cell1->getMateId((index1+2)%3) );
    }

    sing = v2->getNumberOfSings();
    for( int i = 0; i < sing; i++)
        v1->addSing( v2->getSingCell( i ) );

    v2->clearSings();
    mesh->freeVertex(idvertex2);
    mesh->freeCell(idcell1);

    return idvertex1;
}

#undef IDS

}

#endif
