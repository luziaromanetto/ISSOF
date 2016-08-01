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

#ifdef OF_THREADS

#ifndef OFTREADER_H
#define OFTREADER_H

#include "ofMacros.h"
#include "ofMesh.h"

namespace of
{

//! Modelo dos leitores de arquivos
/*!
 */
template <class sTraits>
class ofTReader
{
public:
    typedef typename sTraits::ids ids;

    typedef typename _Traits::sMesh		sMesh;

    //! Construtor
    ofTReader(int threads_vertices, int threads_cells);
    //! Destrutor
    virtual ~ofTReader();

    void setNumberOfThreads(int threads_vertices, int threads_cells);

    //! Executa a leitura de um arquivo
    /*!
     Par???metros de entrada:
     malha : endere???o de mem???ria de destino da malha a ser carregada. J??? deve estar alocado.
     filename : nome do arquivo da malha.
     */
    virtual bool read(sMesh* malha, char* xml) = 0;

protected:

    struct ofTReaderData
    {
        char *str;

        ids id;
        ids start;
        ids end;
        sMesh *malha;
        ofXmlParser *config;

    };

    int nthreadsv, nthreadsc;
};

template <class sTraits>
ofTReader<sTraits>::ofTReader(int threads_vertices, int threads_cells)
{
    setNumberOfThreads(threads_vertices, threads_cells);
}

template <class sTraits>
ofTReader<sTraits>::~ofTReader()
{}

template <class sTraits>
void ofTReader<sTraits>::setNumberOfThreads(int threads_vertices, int threads_cells)
{
    OF_ASSERT(threads_vertices > 0);
    OF_ASSERT(threads_cells > 0);

    nthreadsv = threads_vertices;
    nthreadsc = threads_cells;
}

}

#endif
#endif
