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

#ifndef OFDELAUNAY2D_H
#define OFDELAUNAY2D_H

#include "ofMacros.h"
#include "ofBoundaryCellCIterator2D.h"   
#include "ofMesh.h"
#include "ofList.h"
#include "ofSearchDIDO.h"

namespace of
{

#define SPACE typename ofDelaunay2D<_Traits>::space
    #define IDS typename ofDelaunay2D<_Traits>::ids
    #define SMESH typename ofDelaunay2D<_Traits>::sMesh

template <class _Traits>
class ofDelaunay2D
{
      
/** declaração da classe ofDelaunay2D que controe a triangulação Delaunay de uma
 ** malha */
 
public:
    /* redeclaração de tipos para uso interno da classe */
    typedef typename _Traits::space 	space;
    typedef typename _Traits::ids 		ids;
    typedef typename _Traits::sVertex 	sVertex;
    typedef typename _Traits::sCell 	sCell;
    typedef typename _Traits::sGeometric sGeometric;
    typedef typename _Traits::sTopology sTopology;

    typedef typename _Traits::sMesh		sMesh;  
    
    /* ---------------- GERADOR / DESTRUTOR ofDelaunay2D ---------------------*/

    ofDelaunay2D(int block_size = 1000, bool kdtree = true);  // Gerador 
    ofDelaunay2D(sMesh *_mesh, bool kdtree = true);           // Gerador sobre carregado
    ~ofDelaunay2D();                                          // Destrutor

    /* ---------------------- VARIAVEIS ofDelaunay2D -------------------------*/
    
    ids addVertex(space *coords, ids inicell = -1 OF_DMUTEXVD);
        /* adiciona vertice com coordenadas dadas na variavel *coords, e 
         * identificador dado ou, se inicell=-1, gerado. Devolve o identificador
         * do vertice gerado. */

    ids searchVertex(space *coords);
        /* procura vertice com coordenadas em *coord na malha associada ao objeto
         * do tipo ofDelaunay2D. Devolve o identificador do vertice. */

    ids searchCellIdCoordsIn(space *coords, ids inicell);
        /* procura celula com contem as coordenadas dadas em *coords, começando 
         * a busca em inicell */        

    sMesh* getMesh(OF_DMUTEXD);
    
    //////////////////////////////////////////////////////////////////////////
    /// MUDEI ESTA FUNÇÃO DE PRIVADA PARA PUBLICA POIS TIVE PROBLEMAS ////////
    //////////////////////////////////////////////////////////////////////////
    
    void insertIn(ids idcelula, ids idvertice OF_DMUTEXV); 

protected:
    sMesh* mesh;   /* armazena o ponteiro para a malha que estará associada ao
                    * objeto chamado. */
    /* --------------------- METODOS ofDelaunay2D ----------------------------*/
    
    void add
        (ids idvertice, ids inicell OF_DMUTEXV);
                   /* adiciona ??????????????????????????????????????????? */
                   
    /**************************************************************************/
                   
    class sObject
    { /* declaração da classe sObject que armazena um ponteiro para um vertice 
       * e seu identicicador */
    public:
           
     /*----------------- GERADOR sObject -------------------------------------*/
        sObject(sVertex *_v, ids _id)  
        {
            v = _v;
            id = _id;
        };
        
      /*----------------- VARIAVEIS sObject ----------------------------------*/  
        sVertex *v;  /* ponteiro para um vertice */
        ids id;      /* identicifador */
        
      /*------------------------- sObject ------------------------------------*/
   
    };

    /**************************************************************************/
    class sObjectCompare
    { /* declaração da classe sObjectCompare que para comparar obejtos da classe
       * sObject */ 
    public:
        /* --------------- METODOS sObjectCompare ------------ ---------------*/
        bool equal(sObject *v1, sObject *v2)
        {   /* recebe dois objetos sObject e retorna a resposta da sentença que 
             * compara as coordenadas dos vertices associados aos obejtos 
             * passados */
            return ( v1->v->getCoord(0) == v2->v->getCoord(0) )&&( v1->v->getCoord(1) == v2->v->getCoord(1) );
        };

        bool greater(sObject *v1, sObject *v2, int dim) // maior
        {   /* compara a coordenada de indece dado por dim dos dois objetos 
             * passados, comparando se a coordenada de v1 é maior que a de v2, 
             * se sim retorna true, se dim de v1 menor - igual a dim v2 retorna
             * false */
            OF_ASSERT((dim == 0)||(dim == 1));
            return v1->v->getCoord(dim) > v2->v->getCoord(dim);
        };
        /*--------------------------------------------------------------------*/
    };
    /**************************************************************************/
    
    /* ---------------------- VARIAVEIS ofDelaunay2D -------------------------*/
   
    ofKdTree<sObject*, sObjectCompare> *kd;  
    /* arvore k-d, estrutura de dados para armazena pontos n dimensionais, para
     * fazer busca eficiente, baseada em arvores binarias de busca. */

private:
        
    bool free;
        
    sGeometric geom; /* objeto para fazer operações geometricas */
    sTopology  top; 
    ofSearchDIDO<_Traits> search;

    //	ofKdTree<sObject*,sObjectCompare> *kd;

    //	int searchInMesh(ids ivertex, ids &icelula, int &lado OF_DMUTEXV);

    void legalizeEdge(ids celula, int edge OF_DMUTEXV);

    //	void searchInCircle(ids c, int e, ids nvertex, ofList<ids> *pontos, ofList<ids> *w);


   
    
    void insertOut(ids idcelula, ids idvertice, int edge OF_DMUTEXV);
    
    void insertOn(ids idcelula, ids idvertice, int edge OF_DMUTEXV);

#ifdef OF_THREADS

    void lockCells(ids id1, ids id2, sCell *c1, sCell *c2);
#endif

};

/**--------------------------------------------------------------------------**/

/** ------------------ IMPLEMENTAÇÃO ofDelaunay2D ---------------------------**/

/* Este é o gerador da ofDelaunay2D para quando a malha não é passada, e sim o
 * tamanho do bloco de vertices e se vai ser gerada uma arvore k-d, então o me-
 * todo gera uma malha associando à variavel ofDelaunay2D e um arvore k-d se
 * a flag estiver indicando */
template <class _Traits>
ofDelaunay2D<_Traits>::ofDelaunay2D(int block_size, bool kdtree)
        :search(NULL)  /* variavel do tipo ofDelaunay2D que tem malha associada NULL*/
{
    free = true; /* sinaliza que a malha está vazia */
    mesh = new sMesh(block_size); /* gera uma nova malha do tamanho passado */
    geom.setMesh(mesh);           /* associa a variavel para operações geometricas à malha gerada */
    top.setMesh(mesh);            /* associa a variavel para operações topologicas à malha gerada */

    if(kdtree) /* se a flag kdtree for verdadeira, gera uma arvore k-d em kd */
        kd = new ofKdTree<sObject*, sObjectCompare>(2);
    else      /* senão aponta kd para NULL */
        kd = NULL;

    search.setMesh(mesh); /* associa a variavel search a maha gerada */
}



template <class _Traits>
ofDelaunay2D<_Traits>::ofDelaunay2D(sMesh *_mesh, bool kdtree)
        :search(_mesh) 
{
    OF_ASSERT(_mesh); /* testa a validade da malha passada */

    free = false; /* sinaliza que a malha não esta livre ou vazia */  
    mesh = _mesh; 
    geom.setMesh(mesh); /* associa a variavel para operações geometricas à malha passada */
    top.setMesh(mesh);  /* associa a variavel para operações topologicas à malha passada */

    if(kdtree) /* se a flag que indica se se deve gerar uma arvore k-d for true, então gera a arvore k-d */
        kd = new ofKdTree<sObject*, sObjectCompare>(2);
    else       /* senão aponta kd para null */
        kd = NULL;

    //	if(mesh->getNumberOfVertices() >= 3)
    //	{
    //		ofVerticesIterator<_Traits> it(mesh);
    //		ids newcell[3];
    //
    //		it.initialize();
    //
    //		if(kd) kd->insert(*it);
    //		newcell[0] = &it;
    //		++it;
    //		if(kd) kd->insert(*it);
    //		newcell[1] = &it;
    //		++it;
    //		if(kd) kd->insert(*it);
    //		newcell[2] = &it;
    //
    //		mesh->addCell(newcell);
    //
    //		for(++it; it.notFinish(); ++it)
    //			add(&it, -1);
    //	}
    //	else
    //	{
    //		ofVerticesIterator<_Traits> it(mesh);
    //
    //		for(it.initialize(); it.notFinish(); ++it)
    //			if(kd)
    //				kd->insert(*it);
    //	}
}

 
template <class _Traits>
ofDelaunay2D<_Traits>::~ofDelaunay2D()
{
   /* destrutor da ofDelaunay2D, deleta a arvore k-d e a malha associada */
    if(kd)
        delete kd;

    if(free)
        delete mesh;
}


template <class _Traits>
IDS ofDelaunay2D<_Traits>::searchCellIdCoordsIn(space *coords, ids inicell)
{
    /* busca a celula que contenha as coordenadas de coord, começando a busca igual
     * a inicell */
    int edge = -1, pos;      
    ids idcell, new_inicell;
    sVertex v;  /* cria um vertice */ 
    v.setCoords(coords);  /* associa ao vertice gerado às coordenada passadas */

    OF_ASSERT(kd); /* checa a validade da arvore k-d */

    new_inicell = inicell; /* sinaliza new_inivcell com o mesmo que inicell que foi passado */

    if( inicell == -1 )
    {
        /* enquanto celula in for invalida, continua a busca */
        sObject o(&v, -1); /* cria um sObject com ponteiro para o vertice gerado e identificador invalido */
        sObject *near;  /* a um sObject para ser associado ao vertice proximo */

        near = kd->nearest(&o); /* faz uma busca na arvore k-d para saber qual o 
                                 * vertice mais proximo a o então coloca em near este */

        if(near) /* se near for não nulo pega a celula sigular associada ao vertice mais proximo */
        {
            new_inicell = near->v->getSingCell();
        }
        else    /* se não houver vertice mais proximo então indica que a nova celula 
                 * inicial é a primeira da malha */
            new_inicell = 0; 

        if(new_inicell == -1) /* e entra caso a celula singular associada ao vertice mais proximo
                               * seja invalida, então indica que a celula inicial é
                               * a primeira da malha */
            new_inicell = 0;
    }
    /* coloca em idCell a celula onde o vertice de coordenadas coord está contida */
    pos = search.dido( mesh->getVertex(mesh->getCell(new_inicell)->getVertexId(0)) , coords, idcell, edge OF_MUTEXV);

    return idcell; /* retorna idcell */
}

template <class _Traits>
IDS ofDelaunay2D<_Traits>::searchVertex(space *coords)
{
    /* busca vertice que tenha coordenas iguais a coord, e retorna o identificador
     * do vertice, se encontrado, senão retorna o identificador invalido -1 */
    sVertex v; /* cria um vertice */
    v.setCoords(coords); /* atribui a este vertice as coordenadas passadas como 
                          * parametro para o metodo */
    OF_ASSERT(kd); /* checa a validade da arvore k-d do obejto de chamada */

    sObject o(&v, -1); /* cria um objeto sObject associando a ele o ponteiro para
                        * o vertice e um identificar invalido */
    sObject *near; /* tambem um ponteiro para sObject para conter o vertice mais proximo */

    near = kd->nearest(&o); /* faz uma busca na arvore k-d e coloca na variavel near
                             * o ponteiro retornado */

    if((near)&&(geom.sameVertices(&v, near->v))) /* se near for não nulo, e v e near 
                                                  * forem iguais retorna o identificador
                                                  * do vertice associado a near */
        return near->id;
    else   /* se uma das sentensar for falsa então retorna um identificador invalido */
        return -1;
}

template <class _Traits>
void ofDelaunay2D<_Traits>::add
    (ids idvertice, ids inicell OF_DMUTEXV)
{
    OF_ASSERT(idvertice >=0); /* Checa se o identificador passado é valido */
   
    int edge = -1, pos; 
    if(kd) /* se a avore k-d for não nula */
    {
           /* cria um novo objeto da classe sObject associando este ao vertice de
            * identificador passado e ao identificador */
        sObject *novo = new sObject( mesh->getVertex(idvertice), idvertice);

        if(inicell < 0) 
        {   /* se o identificador para a celula passada for invalido, então 
             * cria  um objeto o armazenando neste o elemento mais proximo ao
             * vertice, colocando o vertice na arvore k-d */
            sObject *o = kd->nearestAndInsert(novo);
            if(o) /* se o vertice mais proximo for não nulo então coloca em inicell
                   * a celula singular do vertice mais proximo */
                inicell = o->v->getSingCell();
        }
        else /* se o identificador para a celula passado for valido, então só adiciona
              * o objeto criado na arvore k-d */
            kd->insert(novo);
    }

    //	pos = searchInMesh(idvertice, inicell, edge OF_MUTEXV);
    /* chama o metodo dido da bibliote DIDO que tem os seguintes posiveis retornos:
             //Retorna 0 caso esteja dentro
             //Retorna 1 caso esteja fora
             //Retorna 2 caso esteja sobre a aresta lado
             //Retorna 3 caso seja um vertice repetido */             
    pos = search.dido(mesh->getVertex(mesh->getCell(inicell)->getVertexId(0)), mesh->getVertex(idvertice)->getCoords(), inicell, edge OF_MUTEXV);
   
   ///////////////////////////////////////////////////////////////////////////////
   ///// O PROBLEMA ESTAVA NA FUNÇÃO ACIMA QUE DEVOLVIA QUE O PONTO ESTAVA FORA //
   // DA TRIANGULAÇÃO ////////////////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////////////
   
   //cout << " ----- pos"<< pos<< endl
    
    
    OF_ASSERT(pos != 3); // vertice repetido, ou seja não deve ser inserido 

    if( pos == 0 ) // vertice dentro da celula, então inseri o vertice dentro da celula 
        insertIn(inicell, idvertice OF_MUTEXV);
    else if( pos == 1 )  // vertice fora, então inseri vertice fora da celula 
        insertOut(inicell, idvertice, edge OF_MUTEXV);
    else if( pos == 2)  // vertice em uma das arestas, e inseri o vertice na aresta
        insertOn(inicell, idvertice, edge OF_MUTEXV);
}

template <class _Traits>
IDS ofDelaunay2D<_Traits>::addVertex(space *coords, ids inicell OF_DMUTEXV)
{
    
    ids idvertice = searchVertex(coords); /* busca um vertice de mesmas coordenadas 
                                           * que coord */
    if( idvertice < 0 )
    /* se a busca anterior retornar um vertice invalido então*/
    {
        idvertice = mesh->addVertex(coords OF_MUTEXV); /* adiciona o vertice a malha, 
                                                        * mas sem associa-lo a uma celula */

        if( mesh->getNumberOfVertices(OF_MUTEX) > 3) /* se o numero de vertices da malha
                                                      * for maior que 3, o que significa que a 
                                                      * triangulação já foi inicializada, então chama a função
                                                      * que inseri o vertice na triangulação de acordo 
                                                      * com sua posição com relação a triangulação
                                                      * já existente*/
            add
                (idvertice, inicell OF_MUTEXV);
        else
        {   /* se o numero de vertices for menor ou igual a 3 */
            /* cria um novo objeto sObject associando a este o vertice criado com
             * coordenadas coord */
            sObject *novo = new sObject(mesh->getVertex(idvertice), idvertice);
            
            if(kd) /* se a arvore k-d for não nula, insere o novo objeto nela*/
                kd->insert(novo);

            if( mesh->getNumberOfVertices(OF_MUTEX) == 3)
            {   /* se o numero de vertices for igual a 3 entao */
                ids newcell[3]; /* cria um vetor de 3 identificadores para celulas */

                ofVerticesIterator<_Traits> it(mesh); /* cria um objeto iterador de vertices */
                it.initialize(); /* inicializa o objeto iterador */

                newcell[0] = &it; /* armazena na primeira posição o identificador do
                                   * primeiro vertice da malha */
                ++it; /* incrementa o iterador */
                newcell[1] = &it; /* armazena na segunda posição o identificador do
                                   * segundo vertice da malha */
                ++it; /* incrementa o iterador */
                newcell[2] = &it; /* armazena na terceira posição o identificador do
                                   * terceira vertice da malha */

                mesh->addCell(newcell OF_MUTEXV); /* com estes tres identificadores
                                                   * gera uma nova celula na triangulação */
            }
            /* OBS: se o numero de vertices não for 3 então será menor que tres que significa 
             * não podemos gerar nenhuma celula na triangulação */
        }
    }/* OBS: se o identificador obtido na busca for maior igual a 0 significa que 
     * já existe um vertice de coordenas iguais a coord, então não deve ser inserido
     * na triangulação */

    return idvertice;
}

template <class _Traits>
SMESH* ofDelaunay2D<_Traits>::getMesh(OF_DMUTEX)
{
    /* retorna um ponteiro para a malha associada ao objeto que realiza a triangulação */
    return mesh;
}

/* --------- FUNÇÃO SIMILAR A dido DA BIBLIOTECA ofSearchDIDO.h ------------- */

//template <class _Traits> int ofDelaunay2D<_Traits>::searchInMesh(ids idvertex, ids &idcelula, int &lado OF_DMUTEXV)
//{
//	//Retorna 0 caso esteja dentro
//	//Retorna 1 caso esteja fora
//	//Retorna 2 caso esteja sobre a aresta lado
//	//Retorna 3 caso seja um vertice repetido
//
//	sCell* auxc = NULL;
//	sVertex *v[3];
//	ids icell, iauxc = 0;
//	int sobre = 0, left, retorno = -1;
//
//	if(idcelula >= 0)
//	{
//		iauxc = idcelula;
//		auxc = mesh->getCell(idcelula OF_MUTEXV);
//	}
//	else
//	{
//		ofCellsIterator<_Traits> itc(mesh);
//		itc.initialize();
//
//		auxc = *itc;
//
//		OF_ASSERT(auxc);
//	}
//
//#ifdef OF_THREADS
//	if(mutex) auxc->lock();
//#endif
//
//	lado = 0;
//	sVertex *vertex = mesh->getVertex(idvertex OF_MUTEXV);
//	space *coords = vertex->getCoords();
//
//	while(retorno < 0)
//	{
//		v[0] = mesh->getVertex(auxc->getVertexId(lado OF_MUTEXFALSEV) OF_MUTEXV);
//		v[1] = mesh->getVertex(auxc->getVertexId((lado+1)%3 OF_MUTEXFALSEV) OF_MUTEXV);
//		v[2] = mesh->getVertex(auxc->getVertexId((lado+2)%3 OF_MUTEXFALSEV) OF_MUTEXV);
//		sobre = -1;
//
//		left = geom.inLeft(v[0],v[1],coords[0], coords[1] OF_MUTEXFALSEV);
//
//		if(left == 0)
//		{
//			if (auxc->getMateId((lado+2)%3 OF_MUTEXFALSEV) >= 0)
//			{
//				icell = iauxc;
//				iauxc = auxc->getMateId((lado+2)%3 OF_MUTEXFALSEV);
//				auxc = mesh->getCell(iauxc OF_MUTEXV);
//				lado = (auxc->getMateIndex(icell OF_MUTEXFALSEV) + 2)%3;
//			}
//			else
//			{
//				lado = (lado+2)%3;
//				idcelula = iauxc;
//				retorno = 1;
//			}
//		}
//		else
//		{
//			if(left == 2)
//				sobre = (lado+2)%3;
//
//			left = geom.inLeft(v[1],v[2],coords[0], coords[1] OF_MUTEXFALSEV);
//			if(left == 0)
//			{
//				if (auxc->getMateId(lado OF_MUTEXFALSEV) >= 0)
//				{
//					icell = iauxc;
//					iauxc = auxc->getMateId(lado OF_MUTEXFALSEV);
//					auxc = mesh->getCell(iauxc OF_MUTEXV);
//					lado = (auxc->getMateIndex(icell OF_MUTEXFALSEV) + 2)%3;
//				}
//				else
//				{
//					//lado = lado;
//					idcelula = iauxc;
//					retorno = 1;
//				}
//			}
//			else
//			{
//				if (left == 2)
//				{
//					if(sobre >= 0)
//					{
//						lado = (lado+1)%3;
//						idcelula = iauxc;
//						retorno = 3;
//					}
//					else
//						sobre = lado;
//				}
//
//				if(retorno < 0)
//				{
//					left = geom.inLeft(v[2],v[0],coords[0], coords[1] OF_MUTEXFALSEV);
//
//					if(left == 0)
//					{
//						if (auxc->getMateId((lado+1)%3 OF_MUTEXFALSEV) >= 0)
//						{
//							icell = iauxc;
//							iauxc = auxc->getMateId((lado+1)%3 OF_MUTEXFALSEV);
//							auxc = mesh->getCell(iauxc OF_MUTEXV);
//							lado = (auxc->getMateIndex(icell OF_MUTEXFALSEV) + 2)%3;
//						}
//						else
//						{
//							lado = (lado+1)%3;
//							idcelula = iauxc;
//							retorno = 1;
//						}
//					}
//					else
//					{
//						if(left == 2)
//						{
//							if(sobre >= 0)
//							{
//								idcelula = iauxc;
//
//								if(sobre == lado)
//									lado = (lado+2)%3;
//
//								retorno = 3;
//							}
//							else
//							{
//								idcelula = iauxc;
//								lado = (lado+1)%3;
//								retorno = 2;
//							}
//						}
//						else
//						{
//							if(sobre >= 0)
//							{
//								idcelula = iauxc;
//								lado = sobre;
//								retorno = 2;
//							}
//							else
//							{
//								idcelula = iauxc;
//								lado = -1;
//								retorno = 0;
//							}
//						}
//					}
//				}
//			}
//		}
//	}
//
//#ifdef OF_THREADS
//	if(mutex) auxc->unlock();
//#endif
//
//	return retorno;
//}


template <class _Traits>
void ofDelaunay2D<_Traits>::legalizeEdge(ids idcelula, int edge OF_DMUTEXV)
{
    /* a aresta é referenciada pelo seu vertices oposto na celula */
    
    sCell *celula = mesh->getCell(idcelula OF_MUTEXV); 
    /* cria um ponteiro para celula e o faz apontar para a celula que contem a 
     * aresta */

    if ( celula->getMateId(edge OF_MUTEXV) >= 0) /* se existir uma outra celula 
    * alem da passada que contenha a aresta então */    
    {
        /* cria um identificador que armazena o identificador da outra celula que 
         * contem a aresta */ 
        ids idoposta = celula->getMateId(edge OF_MUTEXV);
        /* e um ponteiro para celula que armazena um ponteiro para esta outra celula */
        sCell *oposta = mesh->getCell(idoposta OF_MUTEXV); 

#ifdef OF_THREADS

        if(mutex) /* se mutex (que eu não sei o que é ) for não nulo */
            lockCells(idcelula, idoposta, celula, oposta);
#endif

        ids idv = celula->getVertexId(edge OF_MUTEXFALSEV); /* cria um identificador de vertice
         * e coloca o identificador do vertice oposto a aresta */

        /* checa se existe um vertice que seja oposta a 'oposta' em idcelula, ou
         * seja se estas são vizinhas */
        OF_ASSERT( oposta->getVertexMateId(idcelula OF_MUTEXFALSEV) >= 0); // troquei: getMateVertexId
        /* se elas forem visinha vai continuar executando o metodo, portanto apartir
         * daqui assumimos isso*/
        if (idv == 3002) /* x ??????????????????????????????????????????????/*/
            cout << "qq coisa " << endl;

        if(geom.inCircle(mesh->getVertex(celula->getVertexId( (edge+2)%3 ) ),
                         mesh->getVertex(celula->getVertexId( edge )),
                         mesh->getVertex(celula->getVertexId((edge+1)%3)),
                         mesh->getVertex( oposta->getVertexMateId(idcelula OF_MUTEXFALSEV) OF_MUTEXV) OF_MUTEXFALSEV))//troquei: getMateVertexId
        { /* o vertice de 'oposta' oposto a celula estiver contido no circirculo desta */
              
            /* faz checagens nos vertices de 'oposta' */
            OF_ASSERT(oposta->getVertexId(0 OF_MUTEXFALSEV) != oposta->getVertexId(1 OF_MUTEXFALSEV));
            OF_ASSERT(oposta->getVertexId(2 OF_MUTEXFALSEV) != oposta->getVertexId(1 OF_MUTEXFALSEV));
            OF_ASSERT(oposta->getVertexId(0 OF_MUTEXFALSEV) != oposta->getVertexId(2 OF_MUTEXFALSEV));
            /* faz checagens nos vertices de 'celula' */
            OF_ASSERT(celula->getVertexId(0 OF_MUTEXFALSEV) != celula->getVertexId(1 OF_MUTEXFALSEV));
            OF_ASSERT(celula->getVertexId(2 OF_MUTEXFALSEV) != celula->getVertexId(1 OF_MUTEXFALSEV));
            OF_ASSERT(celula->getVertexId(0 OF_MUTEXFALSEV) != celula->getVertexId(2 OF_MUTEXFALSEV));

            //{// TESTES
            //	cout << "W([" << celula->getVertexId((edge+2)%3);
            //	cout << setprecision(9) << "]" << mesh->getVertex(celula->getVertexId((edge+2)%3))->getCoord(0) ;
            //	cout << setprecision(9) << "," << mesh->getVertex(celula->getVertexId((edge+2)%3))->getCoord(1) ;
            //	cout << ") W([" << celula->getVertexId(edge);
            //	cout << setprecision(9) << "]" << mesh->getVertex(celula->getVertexId(edge))->getCoord(0) ;
            //	cout << setprecision(9) << "," << mesh->getVertex(celula->getVertexId(edge))->getCoord(1) ;
            //	cout << ") W([" << celula->getVertexId((edge+1)%3);
            //	cout << setprecision(9) << "]" << mesh->getVertex(celula->getVertexId((edge+1)%3))->getCoord(0) ;
            //	cout << setprecision(9) << "," << mesh->getVertex(celula->getVertexId((edge+1)%3))->getCoord(1) ;
            //	cout << ") o([" << oposta->getVertexMateId(idcelula OF_MUTEXFALSEV);
            //	cout << setprecision(9) << "]" << mesh->getVertex(oposta->getVertexMateId(idcelula OF_MUTEXFALSEV) OF_MUTEXV)->getCoord(0) ;
            //	cout << setprecision(9) << "," << mesh->getVertex(oposta->getVertexMateId(idcelula OF_MUTEXFALSEV) OF_MUTEXV)->getCoord(1) ;
            //	cout << setprecision(9) << ")" << endl;
            //
            //	cout << "cell: " << idcelula << "  oposta: " << idoposta << endl;
            //	cout << "ids_oposta: " << oposta->getVertexId(0 OF_MUTEXFALSEV);
            //	cout << " " <<oposta->getVertexId(1 OF_MUTEXFALSEV);
            //	cout << " " <<oposta->getVertexId(2 OF_MUTEXFALSEV) << endl;
            //}

            /* faz o fliping das celulas */
            top.flip2D(idcelula, idoposta OF_MUTEXFALSEV);

            /* e volta a fazer as checagens da validade dos vertices das celulas */
            OF_ASSERT(oposta->getVertexId(0 OF_MUTEXFALSEV) != oposta->getVertexId(1 OF_MUTEXFALSEV));
            OF_ASSERT(oposta->getVertexId(2 OF_MUTEXFALSEV) != oposta->getVertexId(1 OF_MUTEXFALSEV));
            OF_ASSERT(oposta->getVertexId(0 OF_MUTEXFALSEV) != oposta->getVertexId(2 OF_MUTEXFALSEV));

            OF_ASSERT(celula->getVertexId(0 OF_MUTEXFALSEV) != celula->getVertexId(1 OF_MUTEXFALSEV));
            OF_ASSERT(celula->getVertexId(2 OF_MUTEXFALSEV) != celula->getVertexId(1 OF_MUTEXFALSEV));
            OF_ASSERT(celula->getVertexId(0 OF_MUTEXFALSEV) != celula->getVertexId(2 OF_MUTEXFALSEV));

            //Carlos
            //if (exibir) cout << "QQQQQ" << endl;
            //{
            //	cout << "v([" << celula->getVertexId((edge+2)%3);
            //	cout << setprecision(9) << "]" << mesh->getVertex(celula->getVertexId((edge+2)%3))->getCoord(0) ;
            //	cout << setprecision(9) << "," << mesh->getVertex(celula->getVertexId((edge+2)%3))->getCoord(1) ;
            //	cout << ") v([" << celula->getVertexId(edge);
            //	cout << setprecision(9) << "]" << mesh->getVertex(celula->getVertexId(edge))->getCoord(0) ;
            //	cout << setprecision(9) << "," << mesh->getVertex(celula->getVertexId(edge))->getCoord(1) ;
            //	cout << ") v([" << celula->getVertexId((edge+1)%3);
            //	cout << setprecision(9) << "]" << mesh->getVertex(celula->getVertexId((edge+1)%3))->getCoord(0) ;
            //	cout << setprecision(9) << "," << mesh->getVertex(celula->getVertexId((edge+1)%3))->getCoord(1) ;
            //	cout << ") o([" << oposta->getVertexMateId(idcelula OF_MUTEXFALSEV);
            //	cout << setprecision(9) << "]" << mesh->getVertex(oposta->getVertexMateId(idcelula OF_MUTEXFALSEV) OF_MUTEXV)->getCoord(0) ;
            //	cout << setprecision(9) << "," << mesh->getVertex(oposta->getVertexMateId(idcelula OF_MUTEXFALSEV) OF_MUTEXV)->getCoord(1) ;
            //	cout << setprecision(9) << ")" << endl;
            //}

#ifdef OF_THREADS

            if(mutex) /* se mutex não for nulo, faz alguma coisa qu eeu não sei o que é */
                lockCells(idcelula, idoposta, celula, oposta);
#endif
            /* chama recursivamente o metodo legalizeEdge, para as celulas flipadas*/
            legalizeEdge(idcelula, celula->getVertexIndex(idv OF_MUTEXV) OF_MUTEXV);
            legalizeEdge(idoposta, oposta->getVertexIndex(idv OF_MUTEXV) OF_MUTEXV);
        }
#ifdef OF_THREADS
        else if(mutex) /* se mutex não for nulo, faz alguma coisa qu eeu não sei o que é */
            lockCells(idcelula, idoposta, celula, oposta);
#endif

    }
}

#ifdef OF_THREADS
template <class _Traits>
void ofDelaunay2D<_Traits>::lockCells(ids id1, ids id2, sCell *c1, sCell *c2)
{   /* este metodo utiliza o metodo lock que não consegui identificar o que este
     * faz */
    if(id1 < id2)
    {
        c1->lock()
        ;
        c2->lock()
        ;
    }
    else
    {
        c2->lock()
        ;
        c1->lock()
        ;
    }
}
#endif

//template <class _Traits> void ofDelaunay2D<_Traits>::searchInCircle(ids c, int e, ids nvertex, ofList<ids> *pontos, ofList<ids> *w)
//{
//	sCell *cell = mesh->getCell(c), *tempc;
//	ids cant, cini  = c;
//	sVertex *v = mesh->getVertex(nvertex);
//
//	int eini = e;
//	do
//	{
//
//		if (cell->getMateId(e) >= 0)
//		{
//			tempc = mesh->getCell(cell->getMateId(e));
//
//			if (geom.inCircle(tempc, v)>0)
//			{
//				if(!w->inList(c))
//					w->insert(c);
//
//				cant = c;
//				c = cell->getMateId(e);
//				cell = mesh->getCell(c);
//				e = (cell->getMateIndex(cant) + 1)%3;
//			}
//			else
//			{
//				if(!w->inList(c))
//					w->insert(c);
//				if(!pontos->inList(cell->getVertexId((e+1)%3)))
//					pontos->insert(cell->getVertexId((e+1)%3));
//				e = (e+1)%3;
//			}
//		}
//		else
//		{
//			if(!w->inList(c))
//				w->insert(c);
//			if(!pontos->inList(cell->getVertexId((e+1)%3)))
//				pontos->insert(cell->getVertexId((e+1)%3));
//			e = (e+1)%3;
//		}
//
//
//	}while(((c != cini)||(e != eini))||(pontos->size()==0));
//
//	if(!w->inList(c))
//		w->insert(c);
//}

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////


/* DAQUI PRA FRENTE SÃO SÓ FUNÇÕES DE INSERÇÃO DE VERTICES NA TRIANGULAÇÃO */
template <class _Traits>
void ofDelaunay2D<_Traits>::insertIn(ids idcelula, ids idvertice OF_DMUTEXV)
{   /*Recebe um identificador para um celula e um vertice, e inseri este vertice
     * dentro da celula */
     
    sCell *c = mesh->getCell(idcelula OF_MUTEXV);
    /* cria um ponteiro para uma celula e o associa celula de identificador passado */

    ids vids[3] =
        { c->getVertexId(0 OF_MUTEXV), c->getVertexId(1 OF_MUTEXV), c->getVertexId(2 OF_MUTEXV)};
    /* cria um vetor de vertices e coloca os vertices da celula neste*/
    
    ids newcell[3], cellsids[3];

    mesh->delCell(idcelula OF_MUTEXV); /* deleta a celula da triangulação */
    
    newcell[0] = idvertice;   // coloca na primeira posição do vetor o identificador do vertice a ser inserido
    newcell[1] = vids[0];     // coloca na segunda posição do vetor o primeiro vertice da celula excluida
    newcell[2] = vids[1];     // coloca na terceira posição do vetor o segundo vertice da celula excluida
    cellsids[0] = mesh->addCell(newcell OF_MUTEXV); // gera uma nova celula com estes vertices 

    newcell[0] = idvertice;  // coloca na primeira posição do vetor o identificador do vertice a ser inserido
    newcell[1] = vids[1];    // coloca na segunda posição do vetor o segundo vertice da celula excluida
    newcell[2] = vids[2];    // coloca na terceira posição do vetor o terceiro vertice da celula excluida
    cellsids[1] = mesh->addCell(newcell OF_MUTEXV); // gera uma nova celula com estes vertices 

    newcell[0] = idvertice; // coloca na primeira posição do vetor o identificador do vertice a ser inserido
    newcell[1] = vids[2];   // coloca na segunda posição do vetor o terceiro vertice da celula excluida
    newcell[2] = vids[0];   // coloca na terceira posição do vetor o primeiro vertice da celula excluida
    cellsids[2] = mesh->addCell(newcell OF_MUTEXV); // gera uma nova celula com estes vertices 

    /* assumindo que a celula original tenha uma orientação positiva, então a ordem 
     * que são geradas as novas celulas garante que estas tenham tambem orientação
     * positiva */
     
     // checa se a inserção do vertice fez alguma celulas vizinhas serem não delaunay 
    legalizeEdge(cellsids[0], mesh->getCell(cellsids[0] OF_MUTEXV)->getVertexIndex(idvertice OF_MUTEXV) OF_MUTEXV);
    legalizeEdge(cellsids[1], mesh->getCell(cellsids[1] OF_MUTEXV)->getVertexIndex(idvertice OF_MUTEXV) OF_MUTEXV);
    legalizeEdge(cellsids[2], mesh->getCell(cellsids[2] OF_MUTEXV)->getVertexIndex(idvertice OF_MUTEXV) OF_MUTEXV);
}

template <class _Traits>
void ofDelaunay2D<_Traits>::insertOut(ids c, ids v, int edge  OF_DMUTEXV)
{
     /* Recebe identificador para uma celula e para um vertice, e um referencia uma
      * uma aresta que é feita pelo vertice oposto a esta na celula */
      
    ofBoundaryCellCIterator2D<_Traits> ibc(mesh); 
    /* cria um objeto para iterar pelas celulas da borda de uma malha */
    sVertex *vs[3]; /* e um vetor para ponteiros de vertices */

    vs[2] = mesh->getVertex(v OF_MUTEXV); /* coloca na ultima posição do vetor gerado
                                            o vertice de identificador passado */
    ibc.initialize(c, edge);  /* inicializa o objeto iterador com inicio na celula
                               * passada e no vertice oposto a aresta */
                               
    /* coloca nas outras posições do vetor de vertices os outros vertices da celula 
     * passada, ou seja o vertice da aresta do bordo, no sentido inverso ao da celula*/
    vs[0] = mesh->getVertex(ibc->getVertexId((ibc.getEdge()+2)%3));
    vs[1] = mesh->getVertex(ibc->getVertexId((ibc.getEdge()+1)%3));

    while(geom.inLeft(vs[0], vs[1], vs[2]) == 1) /* vai iterando sobre as celulas do
     * bordo da malha até encontrar uma celula onde o vertice a ser inserido  não  esteja
     * a esquerda da aresta */
    {
        ++ibc;
        vs[0] = mesh->getVertex(ibc->getVertexId((ibc.getEdge()+2)%3));
        vs[1] = mesh->getVertex(ibc->getVertexId((ibc.getEdge()+1)%3));
    }

    //	ids tempc;
    int tempe, i; /* cria duas variaveis inteiras um para ser um contador simples,
                  * outra para armazenar identificador posivelmente */
    ofList<ids> lista, listaw; /* cria duas listas de identificadores */

    do /* coloca em lista todos os identificadores dos vertices que são visiveis
        * ao vertice a ser inserido, em sentido anti-horario, até chegar a uma
        * celula que já não seja visivel ao vertice */
    {
        --ibc; /* decremeta o iterador, ou seja volta para o espaço onde o ponto 
                * esta a esquerda da aresta, ou que o vertice é visivel a aresta */
        tempe = (ibc.getEdge()+1)%3; /* pega o vertice seguinte na orientação ao vertice
                                      * que identifica a aresta, ou seja o vertice interno */
        //		if(geom.inCircle(*ibc, vs[2]) > 0)
        //			searchInCircle(&ibc,tempe,v,&lista, &listaw);
        //		else
        //		{
         /* se o outro vertice não estiver na lista o inseri */
        if( !lista.inList(ibc->getVertexId((tempe+1)%3)) )
            lista.insert(ibc->getVertexId((tempe+1)%3));
        //		}
        vs[0] = mesh->getVertex(ibc->getVertexId((ibc.getEdge()+2)%3));
        vs[1] = mesh->getVertex(ibc->getVertexId((ibc.getEdge()+1)%3));

    } while( geom.inLeft(vs[0], vs[1], vs[2]) == 1); 


    //	for(i=0; i < listaw.size(); i++)
    //	{
    //		tempc = listaw.pos(i);
    //		mesh->delCell(tempc);
    //	}
    int vids[3]; /* cria este vetor auxiliar para armazenar os identificadores dos 
                 * vertice que irão compor as celulas a serem inseridas na triangu-
                 * lação */

    for(i=0; i < (lista.size()-1); i++)
    {  /* percorre a lista pegando sempre dois elementos seguindo e o vertice e
        * vai criando celulas na triangulação  e colocando os identificadores das celulas criadas
        8 na lista ' listaw, de modo a preservar a arientação positiva */
        vids[0] = v;
        vids[1] = lista.pos(i);
        vids[2] = lista.pos(i+1);

        listaw.insert(mesh->addCell(vids));
        //		mesh->addCell(vids);
    }

    for(i = 0; i < listaw.size(); i++)
    /* percorre as celulas criadas checando se alguma delas não satisfaz a propriedade
     *Delaunay */
        legalizeEdge(listaw.pos(i), mesh->getCell(listaw.pos(i))->getVertexIndex(v));

    lista.clear(); /* limpa as listas */
    listaw.clear();
}

template <class _Traits>
void ofDelaunay2D<_Traits>::insertOn(ids c, ids idvertice, int edge  OF_DMUTEXV)
{   /* Recebe identificadores para uma celula e pra um vertice, e um referencias para
     * uma aresta, o vertice a ser inserido na celula esta sobre a aresta passada */
    ids coposto; /* identificador para uma celula oposta */

    sCell *cell = mesh->getCell(c OF_MUTEXV), *cellop;
    /* cria um ponteiro para celula e e coloca nesta o ponteiro para a celula passada,
    e um ponteiro para uma celula que servirá para percorrer varias celulas */

    coposto = cell->getMateId(edge OF_MUTEXV);
    /* coposto recebe a outra celula que contem a aresta passada */

    if(coposto >= 0)
    {   
        /* se coposto existir, ou seja, seu identificador for valido */
        /* coloca em cellop o ponteiro para esta celula */
        cellop = mesh->getCell(coposto OF_MUTEXV);

        ids vids[4];  /* cria um vetor de a identificadores para vertices */

        vids[0] = cell->getVertexId(edge OF_MUTEXV); // na primeira posição coloca o vertice oposta a aresta
        vids[1] = cell->getVertexId((edge+1)%3 OF_MUTEXV); // na segunda o proximo vertice da celula
        vids[2] = cellop->getVertexMateId(c OF_MUTEXV); // na terceira o vertice da celula visinha oposto a celula passada
        vids[3] = cell->getVertexId((edge+2)%3 OF_MUTEXV); // e na quarta posição coloca o outro vertice da celula passada 

        ids newcell[3], cellsids[4];
        /* cria mais dois vetores para identificadores de celulas */

        mesh->delCell(c OF_MUTEXV);  /* deleta a celula passada e a celula oposta */
        mesh->delCell(coposto OF_MUTEXV);

        /* cria outras quatro celulas com os vertices das antigas celulas como é
         * de se esperar, sempre preservando a orientação positiva das celulas */
        newcell[0] = idvertice;
        newcell[1] = vids[0];
        newcell[2] = vids[1];
        cellsids[0] = mesh->addCell(newcell OF_MUTEXV);

        newcell[0] = idvertice;
        newcell[1] = vids[1];
        newcell[2] = vids[2];
        cellsids[1] = mesh->addCell(newcell OF_MUTEXV);

        newcell[0] = idvertice;
        newcell[1] = vids[2];
        newcell[2] = vids[3];
        cellsids[2] = mesh->addCell(newcell OF_MUTEXV);

        newcell[0] = idvertice;
        newcell[1] = vids[3];
        newcell[2] = vids[0];
        cellsids[3] = mesh->addCell(newcell OF_MUTEXV);

        /* checa se em alguma das celulas geradas as propriedades delaunay foram
         * quebradas */
        legalizeEdge(cellsids[0], mesh->getCell(cellsids[0] OF_MUTEXV)->getVertexIndex(idvertice OF_MUTEXV) OF_MUTEXV);
        legalizeEdge(cellsids[1], mesh->getCell(cellsids[1] OF_MUTEXV)->getVertexIndex(idvertice OF_MUTEXV) OF_MUTEXV);
        legalizeEdge(cellsids[2], mesh->getCell(cellsids[2] OF_MUTEXV)->getVertexIndex(idvertice OF_MUTEXV) OF_MUTEXV);
        legalizeEdge(cellsids[3], mesh->getCell(cellsids[3] OF_MUTEXV)->getVertexIndex(idvertice OF_MUTEXV) OF_MUTEXV);

    }
    else
    {   /* se não existir outra celula que contenha a aresta, então */ 
        /* cria um vetor de identificadores para celulas */
        ids vids[3];

        vids[0] = cell->getVertexId(edge OF_MUTEXV); /* na primeira posição coloca
                                                      * o vertice oposta à aresta */
        /* e nas outras posições coloca os outros vertices da celula passada */
        vids[1] = cell->getVertexId((edge+1)%3 OF_MUTEXV);       
        vids[2] = cell->getVertexId((edge+2)%3 OF_MUTEXV);

        ids newcell[3], cellsids[2]; /* cria dois vetores para identificadores para celulas */

        mesh->delCell(c OF_MUTEXV); /* deleta da malha a celula passada */

        /* com o novo vertice e os vertices da antiga celula, cria duas novas celulas
        * como era de se esperar, mantendo a orientação positiva das celulas */
        newcell[0] = idvertice;
        newcell[1] = vids[0];
        newcell[2] = vids[1];
        cellsids[0] = mesh->addCell(newcell OF_MUTEXV);

        newcell[0] = idvertice;
        newcell[1] = vids[2];
        newcell[2] = vids[0];
        cellsids[1] = mesh->addCell(newcell OF_MUTEXV);

        /* checa se em alguma das celulas geradas as propriedades delaunay foram
         * quebradas */
        legalizeEdge(cellsids[0], mesh->getCell(cellsids[0] OF_MUTEXV)->getVertexIndex(idvertice OF_MUTEXV) OF_MUTEXV);
        legalizeEdge(cellsids[1], mesh->getCell(cellsids[1] OF_MUTEXV)->getVertexIndex(idvertice OF_MUTEXV) OF_MUTEXV);
    }
}

#undef SPACE
    #undef IDS
    #undef SMESH

}

#endif
