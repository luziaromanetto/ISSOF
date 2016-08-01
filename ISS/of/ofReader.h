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

#ifndef OFREADER_H
#define OFREADER_H

#include "ofMacros.h"
#include "ofMesh.h"

namespace of
{

//! Modelo dos leitores de arquivos
/*!
Esta classe ???abstrata, devendo servir apenas de molde para novas implementa???es de leitores de arquivos.
 */
template <class _Traits>
class ofReader
{
public:

    typedef typename _Traits::sMesh		sMesh;

    ofReader();
    //! Destrutor
    virtual ~ofReader();

    //! Executa a leitura de um arquivo
    /*!
     Par???etros de entrada:
     malha : endere??? de mem???ia de destino da malha a ser carregada. J???deve estar alocado.
     filename : nome do arquivo da malha.
     */
    virtual bool read(sMesh* malha, const char* filename) = 0;
};

template <class _Traits>
ofReader<_Traits>::ofReader()
{}

template <class _Traits>
ofReader<_Traits>::~ofReader()
{}

}

#endif
