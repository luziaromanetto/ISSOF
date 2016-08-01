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

#ifndef OFWRITER_H
#define OFWRITER_H

#include "ofMacros.h"
#include "ofMesh.h"

namespace of
{

//! Modelo dos escritores de arquivos
/*!
Esta classe ???abstrata, devendo servir apenas de molde para novas implementa???es de escritores de arquivos.
 */
template <class _Traits>
class ofWriter
{
public:

    typedef typename _Traits::sMesh		sMesh;

    //! Construtor
    ofWriter();

    //! Destrutor
    virtual ~ofWriter();

    //! Executa a escrita de um arquivo (salva uma malha)
    /*!
     Par???etros de entrada:
     malha : endere??? de mem???ia da malha a ser salva.
     filename : nome do arquivo da malha. (destino)
     */
    virtual bool write(sMesh* malha, const char* filename) = 0;
};

template <class _Traits>
ofWriter<_Traits>::ofWriter()
{}

template <class _Traits>
ofWriter<_Traits>::~ofWriter()
{}

}

#endif
