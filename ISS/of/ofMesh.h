/*
 * Copyright (C) Mario A. S. LiziÃ©r 2004 <lizier at gmail dot com>
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

#ifndef OFMESH_H
#define OFMESH_H

#include "ofMacros.h"
#include "ofVector.h"

namespace of
{

#define IDS typename ofMesh<_Traits>::ids
    #define SVERTEX typename ofMesh<_Traits>::sVertex
    #define SCELL typename ofMesh<_Traits>::sCell


/**	Mesh Class
 *
 * 	This class must be initialized with elements types (vertices and cells) and the operation class.
 * 	Operation class depends of the vertices and cells dimensions.
 *
 * 	_Traits must have: space, ids, sVertex, sCell, sOper
 */
template <class _Traits>
class ofMesh
{
public:
    typedef _Traits                 traits;
    typedef typename _Traits::space space;
    typedef typename _Traits::ids ids;
    typedef typename _Traits::sVertex sVertex;
    typedef typename _Traits::sCell sCell;
    typedef typename _Traits::sOper sOper;
    typedef typename _Traits::sTopology sTopology;
    typedef typename _Traits::sMesh sMesh;

    //friend template<class _Traits> class sTopology;



    /**	Constructor
     *
     * 	\param block_size: size of vector blocks. The maximum size of vector is block_size * block_size
     */
    ofMesh(ids block_size = 5000);

    /* Destructor */
    ~ofMesh();

    /**	Add a vertex in mesh
     *
     * 	\param coords: vector with the vertex coordinates
     *
     * 	\return the vertex id in mesh
     */
    ids addVertex(space *coords OF_DMUTEXVD);

    /**	Delete a vertex in mesh
     *
     * 	\param idvertex: the vertex will be deleted
     *
     * 	\return true if the vertex was deleted
     */
    bool delVertex(ids idvertex OF_DMUTEXVD);

    /**	Return the number of vertices in mesh
     */
    ids getNumberOfVertices(OF_DMUTEXD);

    /** Return the greater vertex id
     */
    ids getVertexMaxId(OF_DMUTEXD);

    /**	Return the vertex address
     *
     * 	\param idvertex: the vertex id
     */
    sVertex* getVertex(ids idvertex OF_DMUTEXVD);

    /**	Return true if the specified vertex id is a vertex in mesh
     */
    bool isVertex(ids idvertex OF_DMUTEXVD);

    /** Add a cell in mesh
     *
     * 	\param idvertices: vector with vertices ids of the cell
     *
     * 	\return the cell id in mesh
     */
    ids addCell(ids *idvertices OF_DMUTEXVD);

    /** Delete a cell in mesh
     *
     * 	\param idcell: the cell is
     *
     * 	\return true if the cell was deleted
     */
    bool delCell(ids idcell OF_DMUTEXVD);

    /** Return the number of cells in mesh
     */
    ids getNumberOfCells(OF_DMUTEXD);

    /** Return the greater cell id in mesh
     */
    ids getCellMaxId(OF_DMUTEXD);

    /**	Return the cell address
     *
     * 	\param idcell: the cell id
     */
    sCell* getCell(ids idcell OF_DMUTEXVD);

    /** Return if the specified cell id is a cell in mesh
     */
    bool isCell(ids idcell OF_DMUTEXVD);

    /**	Define the number of vertices
     *
     * (DONT USE)
     */
    void setNumberOfVertices(ids number OF_DMUTEXVD);

    /**	Define the number of cells
     *
     * (DONT USE)
     */
    void setNumberOfCells(ids number OF_DMUTEXVD);

    /**	Add a vertex in specified position in mesh
     *
     * 	\param idvertex: position of vertex
     * 	\param coords: vector with the vertex coordinates
     *
     * 	\return the vertex id in mesh
     */
    void addVertex(ids idvertex, space *coords OF_DMUTEXVD);

    /** Add a cell in specified position in mesh
     *
     * 	\param idvertex: position of cell
     * 	\param idvertices: vector with vertices ids of the cell
     *
     * 	\return the cell id in mesh
     */
    void addCell(ids idcell, ids *idvertices OF_DMUTEXVD);

    ids addCell(ids *idvertices, ids *idops OF_DMUTEXVD);

    ids getBlockSize();

    void freeCell(ids idcell OF_DMUTEXVD);
    void freeVertex(ids idvertex OF_DMUTEXVD);

private:

    sOper oper;

    ids bsize; 

    ofVector<sVertex, ids> vertices; /* vetor de vertices */
    ofVector<sCell, ids> cells;      /* vetor de celulas */

#ifdef OF_THREADS

    pthread_mutex_t vertices_mutex;
    pthread_mutex_t cells_mutex;
#endif

};

/********************** Contrutor / Destrutor *********************************/

template <class _Traits>
ofMesh<_Traits>::ofMesh(ids block_size):
        oper((sMesh*)this), vertices(block_size), cells(block_size)
{

    bsize = block_size;

#ifdef OF_THREADS

    pthread_mutex_init(&vertices_mutex, 0);
    pthread_mutex_init(&cells_mutex, 0);
#endif

}

template <class _Traits>
ofMesh<_Traits>::~ofMesh()
{
#ifdef OF_THREADS
    pthread_mutex_destroy(&vertices_mutex);
    pthread_mutex_destroy(&cells_mutex);
#endif

}

/***************************** METODOS ****************************************/

/*Retona o tamanho dos blocos dos vetores que armazna a informação da malha */

template <class _Traits>
IDS ofMesh<_Traits>::getBlockSize()
{
    return bsize;
}

// Vertices ***********************************************************************************

template <class _Traits>
IDS ofMesh<_Traits>::addVertex(space *coords OF_DMUTEXV)
{
    ids idvertex; /* variavel para armazenar identificador do vertice */

    OF_ASSERT(coords); /* checa se as coordenadas passadas são validas */

    OF_LOCK(vertices_mutex); 

    idvertex = vertices.getFree(); /* pega a primeira posição que estiver livre na of */

    OF_UNLOCK(vertices_mutex);

    addVertex(idvertex, coords OF_MUTEXV);

    return idvertex;
}

template <class _Traits>
void ofMesh<_Traits>::addVertex(ids idvertex, space *coords OF_DMUTEXV)
{
    sVertex* v;

    OF_ASSERT(coords);

    v = &vertices[idvertex];
    v->setCoords(coords OF_MUTEXV);

}

template <class _Traits>
bool ofMesh<_Traits>::delVertex(ids idvertex OF_DMUTEXV)
{
    OF_ASSERT(idvertex >= 0);

    bool temp;

    if(vertices[idvertex].getNumberOfSings(OF_MUTEX) == 0)
    {
        OF_LOCK(vertices_mutex);

        temp = vertices.free(idvertex);

        OF_UNLOCK(vertices_mutex);
    }
    else
        temp = false;

    return temp;
}

template <class _Traits>
IDS ofMesh<_Traits>::getNumberOfVertices(OF_DMUTEX)
{
    OF_LOCK(vertices_mutex);

    ids temp = vertices.getSize();

    OF_UNLOCK(vertices_mutex);

    return temp;
}

template <class _Traits>
void ofMesh<_Traits>::setNumberOfVertices(ids number OF_DMUTEXV)
{
    OF_LOCK(vertices_mutex);

    vertices.setSize(number);

    OF_UNLOCK(vertices_mutex);
}

template <class _Traits>
IDS ofMesh<_Traits>::getVertexMaxId(OF_DMUTEX)
{
    OF_LOCK(vertices_mutex);

    ids temp = vertices.getMaxId();

    OF_UNLOCK(vertices_mutex);

    return temp;
}

template <class _Traits>
SVERTEX* ofMesh<_Traits>::getVertex(ids idvertex OF_DMUTEXV)
{
    OF_ASSERT(idvertex >= 0);
    return &vertices[idvertex];
}

template <class _Traits>
bool ofMesh<_Traits>::isVertex(ids idvertex OF_DMUTEXV)
{
    return vertices[idvertex].inMesh(OF_MUTEX);
}



// Cells ***********************************************************************************

template <class _Traits>
IDS ofMesh<_Traits>::addCell(ids *idvertices OF_DMUTEXV)
{
    OF_ASSERT(idvertices);

    OF_LOCK(cells_mutex);

    int idcell = cells.getFree();

    OF_UNLOCK(cells_mutex);

    oper.addCell(idcell, idvertices OF_MUTEXV);

    return idcell;
}

template <class _Traits>
void ofMesh<_Traits>::addCell(ids idcell, ids *idvertices OF_DMUTEXV)
{
    OF_ASSERT(idvertices);

    oper.addCell(idcell, idvertices OF_MUTEXV);
}

template <class _Traits>
bool ofMesh<_Traits>::delCell(ids idcell OF_DMUTEXV)
{

    OF_ASSERT( (idcell >= 0)&&(idcell <= getCellMaxId(OF_MUTEX)) );

    oper.delCell(idcell OF_MUTEXV);

    OF_LOCK(cells_mutex);

    cells.free(idcell);

    OF_UNLOCK(cells_mutex);

    return true;
}

template <class _Traits>
IDS ofMesh<_Traits>::getNumberOfCells(OF_DMUTEX)
{
    OF_LOCK(cells_mutex);

    ids temp = cells.getSize();

    OF_UNLOCK(cells_mutex);

    return temp;
}

template <class _Traits>
void ofMesh<_Traits>::setNumberOfCells(ids number OF_DMUTEXV)
{
    OF_LOCK(cells_mutex);

    cells.setSize(number);

    OF_UNLOCK(cells_mutex);
}

template <class _Traits>
IDS ofMesh<_Traits>::getCellMaxId(OF_DMUTEX)
{
    OF_LOCK(vertices_mutex);

    ids temp = cells.getMaxId();

    OF_UNLOCK(vertices_mutex);

    return temp;
}

template <class _Traits>
SCELL* ofMesh<_Traits>::getCell(ids idcell OF_DMUTEXV)
{
    OF_ASSERT(idcell >= 0);
    return &cells[idcell];
}

template <class _Traits>
bool ofMesh<_Traits>::isCell(ids idcell OF_DMUTEXV)
{
    return cells[idcell].inMesh(OF_MUTEX);
}

template <class _Traits>
IDS ofMesh<_Traits>::addCell(ids *idvertices, ids *idops OF_DMUTEXV)
{
    ids idcell;

    OF_ASSERT(idvertices);
    OF_ASSERT(idops);

    OF_LOCK(cells_mutex);

    idcell = cells.getFree();

    OF_UNLOCK(cells_mutex);

    oper.addCell(idcell, idvertices, idops OF_MUTEXV);

    return idcell;
}

template <class _Traits>
void ofMesh<_Traits>::freeCell(ids idcell OF_DMUTEXVD)
{
    OF_LOCK(cells_mutex);

    cells.free(idcell);

    OF_UNLOCK(cells_mutex);
}

template <class _Traits>
void ofMesh<_Traits>::freeVertex(ids idvertex OF_DMUTEXVD)
{
    OF_LOCK(vertices_mutex);

    vertices.free(idvertex);

    OF_UNLOCK(vertices_mutex);
}

#undef IDS
        #undef SVERTEX
        #undef SCELL

}

#endif
