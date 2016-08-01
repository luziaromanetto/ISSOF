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

#ifndef	OFKDTREE_H
#define	OFKDTREE_H

#include "ofMacros.h"
#include "ofList.h"

/* Implementa��o da �rvore k-d, ou k-dimensional. 
 * Esta �rvore � uma �rvore bin�ria, mas a cada n�vel a chave de busca � 
 * trocada. */

namespace of
{/* defini��o dos tipos */

template<class sObject>
struct ofKdTreeNode
{
    sObject	ptr;  /* guarda referencia para um vertice e seu identificador */
    ofKdTreeNode *p[2];  /* ponteiro para outros dois de si */
};


template <class sObject, class sObjectCompare>
class ofKdTree
{
public:
 
 /* ----------------- GERADOR / DESTRUTOR ofKdTree --------------------------*/
    ofKdTree(int dim);
    ~ofKdTree();
 /* ----------------- METODOS ofKdTree --------------------------------------*/
    void insert(sObject obj);
    //bool erase(sObject obj);

    sObject nearest(sObject obj); /* mais proximo */
    sObject nearestAndInsert(sObject obj);

    int size(); /* tamanho */

private:

    ofKdTreeNode<sObject> *add
    (sObject obj); /* adicionar */
    ofKdTreeNode<sObject> *nearestSearch(sObject obj); /* mais proximo */

    sObjectCompare comp; /* obejto da classe sObjectCompare que compara objetos
                          * da classe sObject */

    ofKdTreeNode<sObject> *root; /* raiz da arvore k-d */
    int n,  /* numero de elementos da arvore */
        ndim;   /* dimen��o da arvore */     
};

template <class sObject, class sObjectCompare>
ofKdTree<sObject, sObjectCompare>::ofKdTree(int dim)
{
   /* constru��o da arvore k-d, aponta a raiz para NULL, registra a dimen��o 
    * passada e o numero de elementos para zero */
    
    OF_ASSERT(dim);

    root = NULL;
    ndim = dim;
    n = 0;
}

template <class sObject, class sObjectCompare>
ofKdTree<sObject, sObjectCompare>::~ofKdTree()
{
    /* destrutor da arvore k-d */
    ofList< ofKdTreeNode<sObject> * > lista;

    ofKdTreeNode<sObject> *node = root;
    /* lista recebe raiz */
    while(node)
    {   /* lista recebe os dois filhos do vertice que node aponta */
        if(node->p[0])
            lista.insert(node->p[0]);
        if(node->p[1])
            lista.insert(node->p[1]);
        
        /* deleta o nodo */
        delete node;

        lista.erase(0); /* o primeiro elemento da lista */

        if(!lista.empty()) /* se a lista n�o estiver vazia aponta node para o primeiro n� da lista */
            node = lista.first();
        else
            node = NULL;  /* sen�o, aponta node para null, o que enterromper� o looping */
    }
}

template <class sObject, class sObjectCompare>
void ofKdTree<sObject, sObjectCompare>::insert(sObject obj)
{   
    /* inserir elemento na kdtree, apenas usa a fun��o add implementada abaixo */
    add
        (obj);
}


template <class sObject, class sObjectCompare>
sObject ofKdTree<sObject, sObjectCompare>::nearest(sObject obj)
{
    /* Busca atraves da fun��o nearestSearch o elemento mais proximo do obj 
     * passado, e o retorna */
     
    ofKdTreeNode<sObject> *node = nearestSearch(obj);
    /* Busca o elemento mais proximo */

    //	OF_ASSERT(node);
    if (node) /* se o retorno for positivo o retorna */
        return node->ptr; 
    else   /* sen�o retorna NULL */
        return NULL;
        
    /* OBS:
     * porque o teste para ver se o retorno � valido, pois se a fun��o n�o encon-
     * trar um elemento valido, ela j� retornar� NULL, que � a resposta desejavel 
     */     
}

template <class sObject, class sObjectCompare>
sObject ofKdTree<sObject, sObjectCompare>::nearestAndInsert(sObject obj)
{
    /* Se n�o existir nenhum vertice de mesmas coordenadas que obj, insere um 
     * nodo na arvore k-d, e retorna o nodo mais proximo a vertice */
     
    ofKdTreeNode<sObject> *node = nearestSearch(obj);
     
    if(!comp.equal(node->ptr, obj))
        add
            (obj);

    OF_ASSERT(node); /* teste de concordancia da execu��o */

    return node->ptr;
}

template <class sObject, class sObjectCompare>
ofKdTreeNode<sObject>*ofKdTree<sObject, sObjectCompare>::add
    (sObject obj)
{
    /* adiciona obejeto passado � arvore k-d */
    ofKdTreeNode<sObject> *leaf = root, /* folha */
                          *ant = NULL;  /* anterior */
    int ld = 0, /* idicador que qual o sentido a ser tomado no decorrer da busca
                 * pela posi��o onde o elemento deve ser inserido */
        dim = 0; /* dimen��o onde refente ao nivel da arvore */

    while(leaf)
    {   
        /* Busca pela posi��o onde deve ser inserido o obj, enquanto n�o estiver
         * numa folha nula continua a busca */        
        if(comp.greater(leaf->ptr, obj, dim))
        {
            /* se a coordenada de indice dim da folha for maior que obj */
            ant = leaf;  /* o indicador do nodo anterior recebe o nodo folha */
            ld = 0;      /* o indicador da dire��o a ser seguida recebe 0, que
                          * � a dire��o onde se encontrar todos os elementos de
                          * coordenada dim maiores que a folha que � onde o objeto
                          * deve ser inserido */
        }
        else
        {
            /* se a coordebada de indice dim da folha for menor-igual a obj */
            ant = leaf; /* o indicador do nodo anterior recebe o nodo folha */
            ld = 1;      /* o indicador da dire��o a ser seguida recebe 1, que
                          * � a dire��o onde se encontrar todos os elementos de
                          * coordenada dim menor-igual que a folha que � onde o 
                          * objeto deve ser inserido */
        }

        leaf = ant->p[ld];  /* a folha recebe o filho da dire��o que foi 
                             * selecionada acima pelas compara��es */
                             
        if(++dim == ndim) /* altera a dimen��o que se trabalha no nivel da arvore
                           * voltando para a dimen��o 0 quando chega na dimen��o 
                           * do espa�o da arvore */
            dim = 0;
    }
    /* aqui j� teremos a posi��o onde o novo nodo deve ser inserido */

    leaf = new ofKdTreeNode<sObject>; /* gera um novo nodo da arvore */

    leaf->ptr = obj;  /* atribue obj para o ponteiro do sObject do novo nodo da 
                       * arvore */
    leaf->p[0]  = NULL; /* atribui NULL para os filhos do novo nodo */
    leaf->p[1] = NULL;

    if(ant)  /* se ant n�o for nulo ent�o atribui o novo nodo como fila do nodo
              * anterior selecionado */
        ant->p[ld] = leaf;
        
    else     /* se ant for nulo significa que a arvore � vazia, ent�o o novo nodo 
              * deve ser colocado na raiz */
    {
        root = leaf; 
        ant = root;
    }

    n++; /* incrementa o contador de nodos da arvore */

    return ant;  /* retorna o elemento pai de obj na arvore */
}

template <class sObject, class sObjectCompare>
ofKdTreeNode<sObject>* ofKdTree<sObject, sObjectCompare>::nearestSearch(sObject obj)
{   
    /* busca pelo elemento mais proximo a obj presente na arvore */
    ofKdTreeNode<sObject> *leaf = root, /* folha */
                          *ant = NULL;  /* anterior */
    int ld,      /* idicador que qual o sentido a ser tomado no decorrer da busca */
        dim = 0; /* dimen��o onde refente ao nivel da arvore */     
        
    while(leaf)
    {   
        /* enquanto a folha for n�o nulla continua a busca */
        
        if(comp.equal(leaf->ptr, obj))
        {   /* compara se a folha tem mesmas coordenadas que obj, se sim atribui
             * a anti a folha e para o looping */
            ant = leaf;
            break;
        }
        else
        {   /* se as coordenadas n�o forem iguais ent�o checa a dire��o a ser 
             * tomada e continua a busca */
             
            if(comp.greater(leaf->ptr, obj, dim))
            {
                /* se a coordenada da dimens�o refente ao nivel da arvore k-d � 
                 * maior que a mesma de obj, toma a dire��o do primeiro filho */
                ant = leaf;
                ld = 0;
            }
            else
            {  /* sen�o toma a dire��o do segundo filho */
                ant = leaf;
                ld = 1;
            }
            
            /* a folha recebe o filho escolhido */
            leaf = ant->p[ld];

            if(++dim == ndim) /* troca-se a dimens�o a ser tratada seguindo o 
                               * nivel da arvore */
                dim = 0;
        }
    }

    if(ant)   /* se ant for n�o nulo o devolve */
        return ant;
    else      /* sen�o devolve null */
        return NULL;
}

template <class sObject, class sObjectCompare>
int ofKdTree<sObject, sObjectCompare>::size()
{
    /* retorna o numero de elementos da arvore */
    return n;
}

}
#endif
