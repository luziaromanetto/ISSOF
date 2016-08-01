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

#ifndef OFWRLREADER_H
#define OFWRLREADER_H

#include <fstream>

#include "ofMacros.h"
#include "ofMesh.h"
#include "ofReader.h"

using namespace std;

namespace of
{

template <class _Traits>
class ofWrlReader : public ofReader<_Traits>
{
public:
    typedef typename _Traits::space space;
    typedef typename _Traits::ids ids;
    typedef typename _Traits::sVertex sVertex;
    typedef typename _Traits::sCell sCell;

    typedef typename _Traits::sMesh		sMesh;

    ofWrlReader();
    ~ofWrlReader();

    virtual bool read(sMesh* malha, const char* filename);

private:

    bool reader(ifstream &pf, sMesh* malha);
};

template <class _Traits>
ofWrlReader<_Traits>::ofWrlReader()
{}

template <class _Traits>
ofWrlReader<_Traits>::~ofWrlReader()
{}

template <class _Traits>
bool ofWrlReader<_Traits>::read(sMesh* malha, const char *filename)
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
bool ofWrlReader<_Traits>::reader(ifstream &pf, sMesh* malha)
{
    char auxstr[255];
    space coords[3];
    int j, k;
    int auxvtx[3];
    char c;

    OF_ASSERT(auxstr);

    while( (! pf.eof())&&(strcmp(auxstr,"point") != 0) )
        pf >> auxstr;

    if(! pf.eof())
    {
        pf >> c;

        if( c == '[')
        {

            while ( c != ']')
            {
                for(j = 0; j < 3; j++)
                    pf >> coords[j];

                malha->addVertex(coords);

                pf >> c;
            }
        }
        else
            return false;


        while( (! pf.eof())&&(strcmp(auxstr,"coordIndex") != 0) )
            pf >> auxstr;

        pf >> c;

        if( c == '[' )
        {
            while ( c != ']')
            {
                for(j = 0; j < 3; j++)
                    pf >> auxvtx[j];

                malha->addCell(auxvtx);

                pf >> k;
                pf >> c;
            }
        }
        else
            return false;
    }
    else
        return false;



    return true;
}

}

#endif
