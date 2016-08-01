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

#ifndef OFVTKREADER_H
#define OFVTKREADER_H

#include <fstream>

#include "ofMacros.h"
#include "ofMesh.h"
#include "ofReader.h"

using namespace std;

namespace of
{

template <class _Traits>
class ofVtkReader : public ofReader<_Traits>
{
public:
    typedef typename _Traits::space space;
    typedef typename _Traits::ids ids;
    typedef typename _Traits::sVertex sVertex;
    typedef typename _Traits::sCell sCell;

    typedef typename _Traits::sMesh		sMesh;

    ofVtkReader();
    ~ofVtkReader();

    virtual bool read(sMesh* malha, const char* filename);

private:

    bool reader(ifstream &pf, sMesh* malha);
};

template <class _Traits>
ofVtkReader<_Traits>::ofVtkReader()
{}

template <class _Traits>
ofVtkReader<_Traits>::~ofVtkReader()
{}

template <class _Traits>
bool ofVtkReader<_Traits>::read(sMesh* malha, const char *filename)
{
    OF_ASSERT(filename);
    OF_ASSERT(malha);

    ifstream pf(filename);

    if( pf.is_open())
    {
        if(! reader(pf, malha))
            return false;

        pf.close();
    }
    else
        return false;

    return true;
}

template <class _Traits>
bool ofVtkReader<_Traits>::reader(ifstream &pf, sMesh* malha)
{
    char auxstr[255];
    space coords[3];
    int i, nv, j, nc, k, idnv;
    int auxvtx[sCell::getDimension() + 1];

    OF_ASSERT(auxstr);

    while( (! pf.eof())&&(strcmp(auxstr, "POINTS") != 0) )
        pf >> auxstr;

    if(! pf.eof())
    {
        pf >> nv;
        pf >> auxstr;

        OF_ASSERT(nv > 0);

        for (i=0; i<nv; i++)
        {
            for(j = 0; j < 3; j++)
                pf >> coords[j];

            malha->addVertex(coords);
        }
    }
    else
        return false;


    while( (! pf.eof())&&(strcmp(auxstr, "CELLS") != 0) )
        pf >> auxstr;

    if( !pf.eof() )
    {
        pf >> nc;
        pf >> auxstr;

        OF_ASSERT(nc >= 0);

        for (i=0; i<nc; i++)
        {
            pf >> idnv;

            OF_ASSERT(idnv == sCell::getDimension() + 1);

            for (j=0; j<sCell::getDimension() + 1 ; j++)
            {
                pf >> k;
                OF_ASSERT((k >= 0)&&(k < nv));
                auxvtx[j] = k;
            }

            malha->addCell(auxvtx);
        }
    }
    else
        return false;

    return true;
}

}

#endif
