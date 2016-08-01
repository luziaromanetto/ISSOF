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

#ifndef OFVECTOR_H
#define OFVECTOR_H

#include "ofMacros.h"

namespace of
{

/**	Base Vector
 *
 * 	This is a block allocation version of vector with free space management
 */
template <class T, class ids>
class ofVector
{
public:
 /*************************** CONSTRUTOR / DESTRUTOR **************************/
    /**	Constructor
     *
     * 	\param block_size: the size of each block (The maximum capacity of vector 
     *  if : block_size * block_size)
     */
    ofVector(ids block_size);

    /**	Destructor
     */
    ~ofVector();

/********************************* METODOS ************************************/

    /** Retorna o ponteiro para  um elemento do tipo template de indice dado 
     *
     * \param index: position of element
     */
    T& operator[](ids index);


    /** Retorna uma posi��o livre no vetor 
     */
    ids getFree();

    /**	Define o tamanho de elementos no vetor negligentemente 
     *
     * 	(DONT USE)
     */
    void setSize(ids size);

    /**	Retorna o numero de elementos do vetor 
     */
    ids getSize();

    /**	Retorna a posi��o maxima no vetor
     */
    ids getMaxId();

    /**	Deleta uma posi��o no vetor
     *
     * 	\return true if this position was deleted
     */
    bool free(ids index);

private:

    T** vetor;  /* vetor de vetor de T template da classe*/
    int block;  /* tamanho dos blocos */

    ids  nt,  /* numero de elementos no vetor */
         ntmax, /* maximo id j� usado */
         first, 
         last;
         
    bool hasfree;
};

////////////////////////////////////////////////////////////////////////////////
//////////////////////////// IMPLEMENTA��O /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/********************* CONSTRUTOR / DESTRUTOR ********************************/

template <class T, class ids>
ofVector<T, ids>::ofVector(ids block_size)
{
    OF_ASSERT(block_size > 0); 
    /* checa se o tamanho do bloco passado � maior que zero */
    block = block_size; 

    vetor = new T*[block]; 
    /* aloca um vetor de ponteiros para T tamplate de tamanho block  */

    OF_ASSERT(vetor); /* checa se o vetor foi mesmo alocado */

    for(ids off1=0; off1 < block; off1++)
        vetor[off1] = NULL; /* seta todos os ponteiros do vetor para NULL */

    nt = ntmax = 0; 
    hasfree = false; 
}

template <class T, class ids>
ofVector<T, ids>::~ofVector()
{
    for(ids off1=0; off1 < block; off1++)
        if(vetor[off1])
            delete [] vetor[off1];

    delete [] vetor;
}

/*************************** METODOS ******************************************/

/** Retorna o ponteiro para o elemento do tipo template de indice dado 
     *
     * \param index: position of element
     */
template <class T, class ids>
T& ofVector<T, ids>::operator [](ids index)
{
/* checa se o indice passado est� no intervalo de indices validos [0, block*block] */
    OF_ASSERT(index >= 0); 
    OF_ASSERT(index < block*block); 

    if(vetor[index / block])  /* se o bloco onde tal indice deve estar j� estiver 
        alocado, retorna a posi��o conveniente deste bloco */
        return vetor[index / block][index % block];
    else
    {  /* se n�o tiver nada ele aloca um bloco do tipo template 
        * e retorna o ponteiro para a regi�o de memoria que ser� alterada 
        * externamente */
        vetor[index / block] = new T[block]; /* aloca */
        OF_ASSERT(vetor[index / block]);     /* chaca a aloca��o */
        return vetor[index / block][index % block]; /* retorna o endere�o */
    }
}

template <class T, class ids>
ids ofVector<T, ids>::getFree()
{
    /* esta fun��o serve para saber qual a primeira posi��o disponivel do vetor,
     * se a flag hasfree estiver setando false o metodo retorna ntmax, ou seja,
     * o numero de indices j� usados pelo menos uma vez, se a utiliza��o estiver
     * sendo feita 'linearmente', este numero, caso n�o haja nenhum indice vazio,
     * corresponde ao primeiro indice livre. E incrementa este contador, pois o 
     * indice retornado ser� usado externamente */
     
    ids i; /* variavel para pegar indice livre */

    if (!hasfree) /* se n�o tiver nenhum elemento intermediario livre */
    {
        i = ntmax; /* coloca na variavel o ntmax */
        ntmax++;   /* e encrementa o ntmax */        
    }
    else /* se tiver algum elemento intermediario livre */
    {
      
        i = first;

        if(last == first)
            hasfree = false; /* se ultimo for igual ao primeiro significa que este 
                              * � o unico elemento livre, ent�o com o seu uso n�o
                              * haver� mais elementos intermediarios livres */
        else /* sen�o, ele procura o proximo elemento livre que ser� agora o
              * o primeiro */
#ifdef OF_THREADS
            first = (*this)[i].getPosition(false); /* idem comentario (*) */
#else

            first = (*this)[i].getPosition();
#endif

    }

#ifdef OF_THREADS
    (*this)[i].setInMesh(true, false); /* Todos os elementos da malha herdam me-
                            * metodos da classe ofBase, que tem tais fun��es de
                            * checagem, e variaveis que indic�o se o elemento es
                            * t� ou n�o na malha */
#else

    (*this)[i].setInMesh(true); /* idem */
#endif

    nt++;
    
    return i;
}

template <class T, class ids>
void ofVector<T, ids>::setSize(ids size)
/* n�o sei qual a finalidade desta fun��o, se a utiliza��o for correta! */
{   /* O size passado for maior que o valor que consta como o numero maximo de
     * indices j� usados, ent�o coloca size como o numero maximo. */
    if(ntmax < size)
        ntmax = size;

    nt = size; /* coloca size como o primero indice disponivel */
}

/* Retorn o numero de elementos do vetor, sem elementos excluidos */
template <class T, class ids>
ids ofVector<T, ids>::getSize()
{
    return nt; 
}

/* Retorna o id max j� alocado usado*/
template <class T, class ids>
ids ofVector<T, ids>::getMaxId()
{
    return ntmax;
}

/* libera a posi��o de indice passado */
template <class T, class ids>
bool ofVector<T, ids>::free(ids index)
{
    OF_ASSERT(index >= 0); /* checa se o idice passado � maior que zero */

    T* v = &(*this)[index]; /* pega o ponteiro para o objeto de indice passado */

    OF_ASSERT(v->inMesh()); /*checa se v est� na malha !!!???*/
    
    if(hasfree) /* se j� tem algum elemento intermediario livre */
    {
#ifdef OF_THREADS
        v->setPosition(first, false); 
#else

        v->setPosition(first);
#endif

        first = index; /* first recebe o elemento */
    }
    else /* se n�o tiver ainda nenhum elemento intermediario livre */
    {
#ifdef OF_THREADS
        v->setInMesh(false, false);
#else

        v->setInMesh(false);
#endif

        hasfree = true;
        first = index;
        last = first;
    }
    nt--;

    return true;
}

}

#endif
