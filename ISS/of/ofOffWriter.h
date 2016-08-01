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

#ifndef OFOFFWRITER_H
#define OFOFFWRITER_H

#include <fstream>
#include <iomanip>

#include "ofMacros.h"
#include "ofMesh.h"
#include "ofWriter.h"
#include "ofVerticesIterator.h"
#include "ofCellsIterator.h"

using namespace std;

namespace of
{

//! Salva malhas em arquivo do tipo OFF sem dados
template <class _Traits>
class ofOffWriter : public ofWriter<_Traits>
{
public:
    typedef typename _Traits::space space;
    typedef typename _Traits::ids ids;
    typedef typename _Traits::sVertex sVertex;
    typedef typename _Traits::sCell sCell;

    typedef typename _Traits::sMesh		sMesh;

    ofOffWriter();
    ~ofOffWriter();

    virtual bool write(sMesh* malha, const char* filename);

private:

    bool writer(ofstream &pf, sMesh *malha);
};

template <class _Traits>
ofOffWriter<_Traits>::ofOffWriter()
{
}

template <class _Traits>
ofOffWriter<_Traits>::~ofOffWriter()
{}

template <class _Traits>
bool ofOffWriter<_Traits>::write(sMesh* malha, const char *filename)
{
    OF_ASSERT(filename);
    OF_ASSERT(malha);

    ofstream pf(filename);

    if( pf.is_open())
    {
        if(! writer(pf, malha))
            return false;

        pf.close();
    }
    else
        return false;

    return true;
}

template <class _Traits>
bool ofOffWriter<_Traits>::writer(ofstream &pf, sMesh* malha)
{
    int i = 0, j;

    pf << "OFF" << endl;
    pf << malha->getNumberOfVertices() << " ";
    pf << malha->getNumberOfCells() << " ";
    pf << "0" << endl;


    ofVerticesIterator<_Traits> itv(malha);

    ofVector<ids, ids> nvids(malha->getBlockSize());
    i = 0;
    for(itv.initialize(); itv.notFinish(); ++itv)
    {
        if (sVertex::getDimension() == 2)
            pf << setprecision(15) << itv->getCoord(0) << " " << setprecision(15) << itv->getCoord(1) << " 0.0" << endl;
        else if (sVertex::getDimension() == 3)
            pf << setprecision(15) << itv->getCoord(0) << " " << setprecision(15) << itv->getCoord(1) << " " << setprecision(15) << itv->getCoord(2) << endl;

        nvids[&itv] = i++;
    }

    ofCellsIterator<_Traits> itc(malha);

    for(itc.initialize(); itc.notFinish(); ++itc)
    {
        pf << sCell::getDimension() + 1;

        for(j = 0; j <= sCell::getDimension(); j++)
            pf << " " << nvids[itc->getVertexId(j)];
        pf << endl;
    }

    return true;
}

}

#endif
