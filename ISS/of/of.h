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

#ifndef OF_H
#define OF_H

// System includes ---------------------------------------------------------
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>

#ifdef OF_THREADS
#include <libpq-fe.h>
#include <pthread.h>
#endif

using namespace std;

// OF includes -------------------------------------------------------------
#include "ofMacros.h"
#include "ofBase.h"
#include "ofVector.h"
#include "ofList.h"
#include "ofKdTree.h"
#include "ofXmlParser.h"
#include "ofBooleanVector.h"
#include "ofObjectVector.h"

#include "ofVertex.h"
#include "ofSing.h"
#include "ofCell.h"

#include "ofSingularVertex.h"
#include "ofVertex2D.h"
#include "ofVertex3D.h"
#include "ofCell2D.h"
#include "ofCell3D.h"

#include "ofBoundaryCellIterator2D.h"
#include "ofBoundaryCellCIterator2D.h"
#include "ofBoundaryCellIterator3D.h"
#include "ofBoundaryCellIteratorSurface.h"
#include "ofCellsIterator.h"
#include "ofVerticesIterator.h"
#include "ofVertexStarIterator2D.h"
#include "ofVertexStarIterator3D.h"
#include "ofVertexStarIteratorSurface.h"
#include "ofEdgeStarIterator3D.h"

#include "ofMesh.h"
#include "ofMeshOper.h"
#include "ofMesh2D.h"
#include "ofMesh3D.h"
#include "ofMeshSurface.h"
#include "ofMeshNOSurface.h"
#include "ofDelaunay2D.h"
#include "ofGeometric.h"
#include "ofTopology.h"
#include "ofBinaryIO.h"

#include "ofReader.h"
#include "ofWriter.h"
#include "ofOfReader.h"
#include "ofVtkReader.h"
#include "ofOffReader.h"
#include "ofOffWriter.h"
#include "ofOfWriter.h"
#include "ofVtkWriter.h"

#include "ofTraits.h"

#include "ofTReader.h"
#include "ofPgReader.h"

#endif
