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


    /** Retorna uma posição livre no vetor 
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

    /**	Retorna a posição maxima no vetor
     */
    ids getMaxId();

    /**	Deleta uma posição no vetor
     *
     * 	\return true if this position was deleted
     */
    bool free(ids index);

private:

    T** vetor;  /* vetor de vetor de T template da classe*/
    int block;  /* tamanho dos blocos */

    ids  nt,  /* numero de elementos no vetor */
         ntmax, /* maximo id já usado */
         first, 
         last;
         
    bool hasfree;
};

////////////////////////////////////////////////////////////////////////////////
//////////////////////////// IMPLEMENTAÇÃO /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/********************* CONSTRUTOR / DESTRUTOR ********************************/

template <class T, class ids>
ofVector<T, ids>::ofVector(ids block_size)
{
    OF_ASSERT(block_size > 0); 
    /* checa se o tamanho do bloco passado é maior que zero */
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
/* checa se o indice passado está no intervalo de indices validos [0, block*block] */
    OF_ASSERT(index >= 0); 
    OF_ASSERT(index < block*block); 

    if(vetor[index / block])  /* se o bloco onde tal indice deve estar já estiver 
        alocado, retorna a posição conveniente deste bloco */
        return vetor[index / block][index % block];
    else
    {  /* se não tiver nada ele aloca um bloco do tipo template 
        * e retorna o ponteiro para a região de memoria que será alterada 
        * externamente */
        vetor[index / block] = new T[block]; /* aloca */
        OF_ASSERT(vetor[index / block]);     /* chaca a alocação */
        return vetor[index / block][index % block]; /* retorna o endereço */
    }
}

template <class T, class ids>
ids ofVector<T, ids>::getFree()
{
    /* esta função serve para saber qual a primeira posição disponivel do vetor,
     * se a flag hasfree estiver setando false o metodo retorna ntmax, ou seja,
     * o numero de indices já usados pelo menos uma vez, se a utilização estiver
     * sendo feita 'linearmente', este numero, caso não haja nenhum indice vazio,
     * corresponde ao primeiro indice livre. E incrementa este contador, pois o 
     * indice retornado será usado externamente */
     
    ids i; /* variavel para pegar indice livre */

    if (!hasfree) /* se não tiver nenhum elemento intermediario livre */
    {
        i = ntmax; /* coloca na variavel o ntmax */
        ntmax++;   /* e encrementa o ntmax */        
    }
    else /* se tiver algum elemento intermediario livre */
    {
      
        i = first;

        if(last == first)
            hasfree = false; /* se ultimo for igual ao primeiro significa que este 
                              * é o unico elemento livre, então com o seu uso não
                              * haverá mais elementos intermediarios livres */
        else /* senão, ele procura o proximo elemento livre que será agora o
              * o primeiro */
#ifdef OF_THREADS
            first = (*this)[i].getPosition(false); /* idem comentario (*) */
#else

            first = (*this)[i].getPosition();
#endif

    }

#ifdef OF_THREADS
    (*this)[i].setInMesh(true, false); /* Todos os elementos da malha herdam me-
                            * metodos da classe ofBase, que tem tais funções de
                            * checagem, e variaveis que indicão se o elemento es
                            * tá ou não na malha */
#else

    (*this)[i].setInMesh(true); /* idem */
#endif

    nt++;
    
    return i;
}

template <class T, class ids>
void ofVector<T, ids>::setSize(ids size)
/* não sei qual a finalidade desta função, se a utilização for correta! */
{   /* O size passado for maior que o valor que consta como o numero maximo de
     * indices já usados, então coloca size como o numero maximo. */
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

/* Retorna o id max já alocado usado*/
template <class T, class ids>
ids ofVector<T, ids>::getMaxId()
{
    return ntmax;
}

/* libera a posição de indice passado */
template <class T, class ids>
bool ofVector<T, ids>::free(ids index)
{
    OF_ASSERT(index >= 0); /* checa se o idice passado é maior que zero */

    T* v = &(*this)[index]; /* pega o ponteiro para o objeto de indice passado */

    OF_ASSERT(v->inMesh()); /*checa se v está na malha !!!???*/
    
    if(hasfree) /* se já tem algum elemento intermediario livre */
    {
#ifdef OF_THREADS
        v->setPosition(first, false); 
#else

        v->setPosition(first);
#endif

        first = index; /* first recebe o elemento */
    }
    else /* se não tiver ainda nenhum elemento intermediario livre */
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
