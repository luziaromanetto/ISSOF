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

#ifndef OFLIST_H
#define OFLIST_H

#include "ofMacros.h"

namespace of
{

/** Node of ofList
 */
template<class T>
class ofListNode
{
public:

    /* Constructor */
    ofListNode();

    /* Destructor */
    ~ofListNode();

    /* Define the element in list node */
    void	setInfo(T info)
    {
        atu=info;
    };

    /* Return the element in list node */
    T& getInfo()
    {
        return atu;
    };

    /* Define the next node */
    void	setNext(ofListNode<T> *prox)
    {
        next=prox;
    };

    /* Return the next node */
    ofListNode<T>*	getNext()
    {
        return next;
    };

protected:
    T				atu;
    ofListNode<T>*	next;
};

template<class T>
ofListNode<T>::ofListNode()
{
    next = NULL;
};

template<class T>
ofListNode<T>::~ofListNode()
{}
;


/**	Dynamic single linked list
 *
 * 	T is the type of element
 */
template<class T>
class ofList
{
public:

    /* Constructor */
    ofList();
    ofList(ofList<T> &list);

    /* Destructor */
    ~ofList();

    /** Return a element of the specified position
     *
     * 	\param pos: position of the element
     */
    T&	pos(int pos);

    /*	Return the first element */
    T&	first();

    /*  Return the last element (faster than pos(size() - 1) )
     */
    T&	last();

    /*	Return the number of elements in list */
    int size() const;

    /** Insert a element in list (in last position)
     *
     * 	\param novo: element will be inserted
     */
    void insert(T novo);

    /** Insert a element in list, only if this element isnt in list. (in last position)
     *
     * 	\param novo: element
     */
    bool insertOnlyOne(T novo);

    /** Insert a element in first position of list
     *
     * 	\param novo: element
     */
    void insertFirst(T novo);

    /** Insert a element in the specified position of list
     *
     * 	\param novo: element
     * 	\param pos: position
     */
    void insert(T novo, int pos);

    /** Delete a element
     *
     * 	\param pos: position of element
     */
    bool  erase(int pos);

    /** Return true if the element is inlist */
    bool	inList(T info);

    /**	Return the position of a element, -1 if isnt in list
     */
    int 	search(T info);

    /** Delete all element of list
     */
    void  clear();

    /* Return true if list dont have any element
     */
    bool  empty();

    ofList<T> &operator=( ofList<T> &list);

protected:
    int lsize;
    int ilastpos;
    ofListNode<T>*  lastpos;
    ofListNode<T>*	lfirst;
    ofListNode<T>*  llast;
};

template<class T>
ofList<T>::ofList() //construtor
{
    lfirst = NULL;
    llast = NULL;
    lsize = 0;
    ilastpos = -1;
    lastpos = NULL;
};

template<class T>
ofList<T>::ofList(ofList<T> &list) //construtor
{
    lfirst = NULL;
    llast = NULL;
    lsize = 0;
    ilastpos = -1;
    lastpos = NULL;

    for(int i=0; i< list.size(); i++)
        insert( list.pos(i) );
};

template<class T>
ofList<T>::~ofList()
{
    clear();
};

template<class T>
T& ofList<T>::pos(int pos)
{
    OF_ASSERT((pos >=0)&&(pos < lsize));

    ofListNode<T> *p = lfirst;

    if(( ilastpos >= 0 )&&( ilastpos <= pos))
    {
        p = lastpos;

        for(int i=ilastpos; i<pos; i++)
        {
            OF_ASSERT(p);
            p = p->getNext();
        }
    }
    else
    {
        for(int i=0; i<pos; i++)
        {
            OF_ASSERT(p);
            p = p->getNext();
        }
    }

    ilastpos = pos;
    lastpos = p;

    return p->getInfo();
};

template<class T>
T& ofList<T>::first()
{
    OF_ASSERT(lfirst);
    return lfirst->getInfo();
};

template<class T>
T& ofList<T>::last()
{
    OF_ASSERT(llast);
    return llast->getInfo();
};

template<class T>
int ofList<T>::size() const
{
    return lsize;
}

template<class T>
void ofList<T>::insert(T novo)
{
    ofListNode<T> *t = new ofListNode<T>();

    OF_ASSERT(t);

    t->setInfo(novo);
    if (lsize==0)
        lfirst = t;
    else
    {
        OF_ASSERT(llast);
        llast->setNext(t);
    }
    llast = t;
    lsize++;
    ilastpos = -1;
};

template<class T>
bool ofList<T>::insertOnlyOne(T novo)
{
    ilastpos = -1;

    if (lsize==0)
    {
        ofListNode<T> *t = new ofListNode<T>();
        OF_ASSERT(t);

        t->setInfo(novo);
        t->setNext(NULL);
        lfirst = t;
        llast = t;
    }
    else
    {
        ofListNode<T> *point = lfirst;
        OF_ASSERT(point);

        while((point->getNext()!=NULL)&&(point->getInfo() != novo))
        {
            point = point->getNext();
            OF_ASSERT(point);
        }

        if(point->getInfo() != novo)
        {
            ofListNode<T> *t = new ofListNode<T>();

            OF_ASSERT(t);

            t->setInfo(novo);
            t->setNext(NULL);
            point->setNext(t);
            llast = t;
        }
        else
            return false;
    };
    lsize++;

    return true;
};

template<class T>
void ofList<T>::insertFirst(T novo)
{
    ofListNode<T> *t = new ofListNode<T>();

    OF_ASSERT(t);

    t->setInfo(novo);
    t->setNext(lfirst);
    lfirst = t;
    if(lsize==0)
        llast = lfirst;
    lsize++;
    ilastpos = -1;
};

template<class T>
void ofList<T>::insert(T novo, int pos)
{
    OF_ASSERT((pos >=0)&&(pos <= lsize));

    ofListNode<T> *t = new ofListNode<T>();

    OF_ASSERT(t);

    t->setInfo(novo);
    if ((pos==0)||(lsize==0))
    {
        t->setNext(lfirst);
        lfirst = t;
        if (lsize==0)
            llast = t;
    }
    else
    {
        ofListNode<T> *point=lfirst;
        OF_ASSERT(point);
        int i;
        for(i=1; (i<pos)&&(point->getNext()!=NULL); i++)
        {
            point = point->getNext();
            OF_ASSERT(point);
        }
        t->setNext(point->getNext());
        point->setNext(t);
        if (pos>=lsize)
            llast = t;
    };
    lsize++;
    ilastpos = -1;
};

template<class T>
bool ofList<T>::erase(int pos)
{
    OF_ASSERT((pos >=0)&&(pos < lsize));

    ofListNode<T> *p, *a;

    if (pos ==0)
    {
        p = lfirst;
        OF_ASSERT(lfirst);
        lfirst = lfirst->getNext();
        if (lsize==1)
            llast = NULL;
        delete p;
    }
    else
    {
        a = lfirst;
        OF_ASSERT(lfirst);
        p = lfirst->getNext();
        for(int i=1; i<pos; i++)
        {
            a = p;
            OF_ASSERT(p);
            p = p->getNext();
        };

        a->setNext(p->getNext());
        if(pos==(lsize-1))
            llast = a;
        delete p;
    };
    lsize--;
    ilastpos = -1;
    return true;
};

template<class T>
bool ofList<T>::inList(T info)
{
    if (lsize==0)
        return false;

    ofListNode<T> *p=lfirst;

    OF_ASSERT(p);

    for(int i=0; (p->getInfo()!=info)&&(i<lsize);i++)
    {
        if (p->getNext() != NULL)
            p=p->getNext();
        OF_ASSERT(p);
    }
    return (p->getInfo()==info);
};

template<class T>
int ofList<T>::search(T info)
{
    int i;

    if (lsize==0)
        return -1;

    ofListNode<T> *p=lfirst;

    OF_ASSERT(p);

    for(i=0; (p->getInfo()!=info)&&(i<lsize);i++)
    {
        if (p->getNext() != NULL)
            p=p->getNext();
        OF_ASSERT(p);
    }
    if(p->getInfo()==info)
        return i;
    else
        return -1;
};

template<class T>
void ofList<T>::clear()
{
    while(lsize > 0)
        erase(0);

    lsize = 0;
    lfirst=NULL;
    llast=NULL;
    ilastpos = -1;
};

template<class T>
bool ofList<T>::empty()
{
    return (lsize == 0);
};

template<class T>
ofList<T>& ofList<T>::operator=( ofList<T> &list)
{
    clear();

    for(int i=0; i< list.size(); i++)
        insert( list.pos(i) );

    return *this;
};
}

#endif
