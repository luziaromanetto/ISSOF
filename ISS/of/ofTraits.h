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

#ifndef OFTRAITS_H
#define OFTRAITS_H

#include "ofVertex2D.h"
#include "ofVertex3D.h"
#include "ofCell2D.h"
#include "ofCell3D.h"
#include "ofMesh2D.h"
#include "ofMesh3D.h"
#include "ofMeshSurface.h"
#include "ofMeshNOSurface.h"
#include "ofGeometric.h"
#include "ofTopology.h"

/** Traits class
 */

namespace of
{

struct ofDefault2D
{
    typedef ofDefault2D							sTraits;
    typedef double								space;
    typedef int									ids;

    typedef ofVertex2D<sTraits>					sVertex;
    typedef ofCell2D<sTraits>					sCell;

    typedef ofMesh<sTraits>						sMesh;
    typedef ofMesh2D<sTraits>					sOper;

    typedef ofGeometric<sTraits>				sGeometric;
    typedef ofTopology<sTraits>					sTopology;

};

struct ofDefaultLong2D
{
    typedef ofDefaultLong2D						sTraits;
    typedef long double							space;
    typedef long 								ids;

    typedef ofVertex2D<sTraits>					sVertex;
    typedef ofCell2D<sTraits>					sCell;

    typedef ofMesh<sTraits>						sMesh;
    typedef ofMesh2D<sTraits>					sOper;

    typedef ofGeometric<sTraits>					sGeometric;
    typedef ofTopology<sTraits>					sTopology;
};

struct ofDefault3D
{
    typedef ofDefault3D							sTraits;
    typedef double								space;
    typedef int									ids;

    typedef ofVertex3D<sTraits>					sVertex;
    typedef ofCell3D<sTraits>					sCell;

    typedef ofMesh<sTraits>						sMesh;
    typedef ofMesh3D<sTraits>					sOper;

    typedef ofGeometric<sTraits>				sGeometric;
    typedef ofTopology<sTraits>					sTopology;
};

struct ofDefaultSurface
{
    typedef ofDefaultSurface					sTraits;
    typedef double								space;
    typedef int									ids;

    typedef ofVertex3D<sTraits>					sVertex;
    typedef ofCell2D<sTraits>					sCell;

    typedef ofMesh<sTraits>						sMesh;
    typedef ofMeshSurface<sTraits>				sOper;

    typedef ofGeometric<sTraits>				sGeometric;
    typedef ofTopology<sTraits>					sTopology;
};

struct ofDefaultNOSurface
{
    typedef ofDefaultNOSurface					sTraits;
    typedef double								space;
    typedef int									ids;

    typedef ofVertex3D<sTraits>					sVertex;
    typedef ofCell2D<sTraits>					sCell;

    typedef ofMesh<sTraits>						sMesh;
    typedef ofMeshNOSurface<sTraits>			sOper;

    typedef ofGeometric<sTraits>				sGeometric;
    typedef ofTopology<sTraits>					sTopology;
};

}

#endif
