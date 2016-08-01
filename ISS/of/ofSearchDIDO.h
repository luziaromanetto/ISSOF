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

#ifndef	OFSEARCHDIDO_H
#define	OFSEARCHDIDO_H

#include "ofMacros.h"
#include "ofBase.h"

namespace of
{

template <class _Traits>
class ofSearchDIDO
{  /* A classe ofSearchDIDO tem como variaveis uma malha associada a seu objeto
    * e dois metodos dido, e setMesh que serve para associar o objeto a uma malha
    *  que é passada como parametro */
public:

    typedef typename _Traits::sVertex 	sVertex;
    typedef typename _Traits::sCell 	sCell;
    typedef typename _Traits::ids 		ids;
    typedef typename _Traits::space		space;
    typedef typename _Traits::sGeometric sGeometric;
    typedef typename _Traits::sMesh		sMesh;

    ofSearchDIDO(sMesh *_mesh);
    ~ofSearchDIDO();

    int dido(sVertex *_v, space *coords, ids &idcelula, int &lado);

    void setMesh(sMesh *_mesh);

private:

    sMesh *mesh;
    sGeometric geom;
};

template <class _Traits>
ofSearchDIDO<_Traits>::ofSearchDIDO(sMesh *_mesh)
{
    /* construtor, associa ao objeto ofSearchDIDO a malha parassada e tambem a variavel
     * para operações do geometricas */
    mesh = _mesh;
    geom.setMesh(mesh);
}

template <class _Traits>
ofSearchDIDO<_Traits>::~ofSearchDIDO()
{} /* destrutor não faz nada */

template <class _Traits>
void ofSearchDIDO<_Traits>::setMesh(sMesh *_mesh)
{
    OF_ASSERT(_mesh); /* checa a validade da malha passada */
    mesh = _mesh;     /* associa a malha passada ao campo */
}

template<class _Traits>
int ofSearchDIDO<_Traits>::dido(sVertex *_v, space *coords, ids &idcelula, int &lado)
{
    //Retorna 0 caso esteja dentro
    //Retorna 1 caso esteja fora
    //Retorna 2 caso esteja sobre a aresta lado
    //Retorna 3 caso seja um vertice repetido

    OF_ASSERT(mesh);

    sCell* auxc = NULL;  /* ponteiro auxiliar para uma celula recebe inicialmente NULL */
    sVertex *v[3];  /* vetor de 3 ponteiros para vetices */
    ids icell, iauxc = 0; /* identificadores para celulas */
    int sobre, left, retorno = -1;

    OF_ASSERT(_v->getNumberOfSings() == 1); // nao permite vertice singular ou isolado

    iauxc = _v->getSingCell(0); /* */
    auxc = mesh->getCell(iauxc OF_MUTEXV);

    lado = 0;

    while(retorno < 0)
    {
        v[0] = mesh->getVertex(auxc->getVertexId(lado OF_MUTEXFALSEV) OF_MUTEXV);
        v[1] = mesh->getVertex(auxc->getVertexId((lado+1)%3 OF_MUTEXFALSEV) OF_MUTEXV);
        v[2] = mesh->getVertex(auxc->getVertexId((lado+2)%3 OF_MUTEXFALSEV) OF_MUTEXV);
        sobre = -1;

        left = geom.inLeft(v[0], v[1], coords[0], coords[1] OF_MUTEXFALSEV);

        if(left == 0)
        {
            if (auxc->getMateId((lado+2)%3 OF_MUTEXFALSEV) >= 0)
            {
                icell = iauxc;
                iauxc = auxc->getMateId((lado+2)%3 OF_MUTEXFALSEV);
                auxc = mesh->getCell(iauxc OF_MUTEXV);
                lado = (auxc->getMateIndex(icell OF_MUTEXFALSEV) + 2)%3;
            }
            else
            {
                lado = (lado+2)%3;
                idcelula = iauxc;
                retorno = 1;
            }
        }
        else
        {
            if(left == 2)
                sobre = (lado+2)%3;

            left = geom.inLeft(v[1], v[2], coords[0], coords[1] OF_MUTEXFALSEV);
            if(left == 0)
            {
                if (auxc->getMateId(lado OF_MUTEXFALSEV) >= 0)
                {
                    icell = iauxc;
                    iauxc = auxc->getMateId(lado OF_MUTEXFALSEV);
                    auxc = mesh->getCell(iauxc OF_MUTEXV);
                    lado = (auxc->getMateIndex(icell OF_MUTEXFALSEV) + 2)%3;
                }
                else
                {
                    //lado = lado;
                    idcelula = iauxc;
                    retorno = 1;
                }
            }
            else
            {
                if (left == 2)
                {
                    if(sobre >= 0)
                    {
                        lado = (lado+1)%3;
                        idcelula = iauxc;
                        retorno = 3;
                    }
                    else
                        sobre = lado;
                }

                if(retorno < 0)
                {
                    left = geom.inLeft(v[2], v[0], coords[0], coords[1] OF_MUTEXFALSEV);

                    if(left == 0)
                    {
                        if (auxc->getMateId((lado+1)%3 OF_MUTEXFALSEV) >= 0)
                        {
                            icell = iauxc;
                            iauxc = auxc->getMateId((lado+1)%3 OF_MUTEXFALSEV);
                            auxc = mesh->getCell(iauxc OF_MUTEXV);
                            lado = (auxc->getMateIndex(icell OF_MUTEXFALSEV) + 2)%3;
                        }
                        else
                        {
                            lado = (lado+1)%3;
                            idcelula = iauxc;
                            retorno = 1;
                        }
                    }
                    else
                    {
                        if(left == 2)
                        {
                            if(sobre >= 0)
                            {
                                idcelula = iauxc;

                                if(sobre == lado)
                                    lado = (lado+2)%3;

                                retorno = 3;
                            }
                            else
                            {
                                idcelula = iauxc;
                                lado = (lado+1)%3;
                                retorno = 2;
                            }
                        }
                        else
                        {
                            if(sobre >= 0)
                            {
                                idcelula = iauxc;
                                lado = sobre;
                                retorno = 2;
                            }
                            else
                            {
                                idcelula = iauxc;
                                lado = -1;
                                retorno = 0;
                            }
                        }
                    }
                }
            }
        }
    }

    return retorno;
}

}
#endif
