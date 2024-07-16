#include "Polygon.h"
#include "Module.h"
#ifdef __cplusplus
extern "C" {
#endif

// Function to create an empty Polygon
Polygon *polygon_create() {
    Polygon *p = (Polygon *)malloc(sizeof(Polygon));
    if (p == NULL) return NULL;
    p->numVertex = 0;
    p->vertex = NULL;
    p->color = NULL;
    p->normal = NULL;
    p->zBuffer = 1;
    p->oneSided = 0;
    return p;
}

// Function to create a Polygon with a given number of vertices
Polygon *polygon_createp(int numV, Point *vlist) {
    Polygon *p = polygon_create();
    if (p == NULL) return NULL;
    polygon_set(p, numV, vlist);
    return p;
}

// Function to free the memory of a Polygon
void polygon_free(Polygon *p) {
    if (p == NULL) return;
    if(p->vertex)
        free(p->vertex);
    if(p->color)
        free(p->color);
    if(p->normal)
        free(p->normal);
    free(p);
}

// Function to initialize an existing Polygon to an empty Polygon
void polygon_init(Polygon *p) {
    if (p == NULL) return;
    p->numVertex = 0;
    p->vertex = NULL;
    p->color = NULL;
    p->normal = NULL;
    p->zBuffer = 1;
    p->oneSided = 0;
}



// Function to set the vertices of a Polygon
	void polygon_set( Polygon *p, int numV, Point *vlist ) {
	if(p != NULL) {
		polygon_clear(p);
		p->vertex = malloc(sizeof(Point) * numV );
		p->numVertex = numV;
		memcpy( p->vertex, vlist, sizeof(Point) * numV );
	}
}


// Function to clear the internal data of a Polygon
void polygon_clear(Polygon *p) {
    if (p == NULL) return;
    if (p->vertex != NULL) {
       free(p->vertex);
    	p->vertex = NULL;
    }
    if (p->color != NULL)   free(p->color);
    if (p->normal != NULL)  free(p->normal);
    p->numVertex = 0;
}

// Function to set the oneSided field of a Polygon
void polygon_setSided(Polygon *p, int oneSided) {
    if (p == NULL) return;
    p->oneSided = oneSided;
}

// Function to set the colors of a Polygon
void polygon_setColors(Polygon *p, int numV, Color *clist) {
    if (p == NULL) return;
    if (p->color) free(p->color);
    p->color = (Color *)malloc(numV * sizeof(Color));
    if (p->color == NULL) return;
    for (int i = 0; i < numV; i++) {
        p->color[i] = clist[i];
    }
}

// Function to set the normals of a Polygon
void polygon_setNormals(Polygon *p, int numV, Vector *nlist) {
    if (p == NULL) return;
    if (p->normal) free(p->normal);
    p->normal = (Vector *)malloc(numV * sizeof(Vector));
    if (p->normal == NULL) return;
    for (int i = 0; i < numV; i++) {
        p->normal[i] = nlist[i];
    }
}

// Function to set all properties of a Polygon
void polygon_setAll(Polygon *p, int numV, Point *vlist, Color *clist, Vector *nlist, int zBuffer, int oneSided) {
    if (p == NULL) return;
    polygon_set(p, numV, vlist);
    polygon_setColors(p, numV, clist);
    polygon_setNormals(p, numV, nlist);
    p->zBuffer = zBuffer;
    p->oneSided = oneSided;
}
static int compYStart( const void *a, const void *b ) {
    EdgeRec *ea = (EdgeRec *)a;
    EdgeRec *eb = (EdgeRec *)b;

    return(ea->yStart - eb->yStart);
}

// Function to set the z-buffer flag of a Polygon
void polygon_zBuffer(Polygon *p, int flag) {
    if (p == NULL) return;
    p->zBuffer = flag;
}

// Function to copy one Polygon to another
void polygon_copy(Polygon *to, Polygon *from) {
    if ((to == NULL ) || (from == NULL)) return;
    // Allocate new memory and copy the data
    to->vertex = (Point *)malloc(from->numVertex * sizeof(Point));
    if (to->vertex == NULL) return;
    memcpy(to->vertex, from->vertex, from->numVertex * sizeof(Point));

    if (from->color) {
        to->color = (Color *)malloc(from->numVertex * sizeof(Color));
        if (to->color == NULL) return;
        memcpy(to->color, from->color, from->numVertex * sizeof(Color));
    } else {
        to->color = NULL;
    }
    if (from->normal) {
        to->normal = (Vector *)malloc(from->numVertex * sizeof(Vector));
        if (to->normal == NULL) return;
        memcpy(to->normal, from->normal, from->numVertex * sizeof(Vector));
    } else {
        to->normal = NULL;
    }

    to->numVertex = from->numVertex;
    to->zBuffer = from->zBuffer;
    to->oneSided = from->oneSided;
}
// Function to print the data of a Polygon
void polygon_print(Polygon *p, FILE *fp) {
    if ( (p == NULL) || (fp == NULL)) return;
    fprintf(fp, "Polygon with %d vertices:\n", p->numVertex);
    for (int i = 0; i < p->numVertex; i++) {
        fprintf(fp, "  Vertex %d: (%f, %f, %f, %f)\n", i, p->vertex[i].val[0], p->vertex[i].val[1], p->vertex[i].val[2], p->vertex[i].val[3]);
    }
}

// Function to normalize the vertices of a Polygon
void polygon_normalize(Polygon *p) {
    if (p == NULL) return;
    for (int i = 0; i < p->numVertex; i++) {
        point_normalize(&p->vertex[i]);
    }
}


static EdgeRec *makeEdgeRec( Point start, Point end,
			  Color c1, Color c2,
			  Image *src, DrawState *ds)
{
  EdgeRec *edge;
  float adjust;
  float dscan = end.val[1] - start.val[1];

  if( start.val[1] >= src->rows || end.val[1] <= 0.0 || start.val[1] == end.val[1]) {
    return( NULL );
  }

  edge = malloc(sizeof(EdgeRec));
  memset( edge, 0, sizeof(EdgeRec) );
  edge->x0 = start.val[0];
  edge->y0 = start.val[1];
  edge->x1 = end.val[0];
  edge->y1 = end.val[1];

  edge->yStart = (int)(start.val[1] + 0.5);

  edge->yEnd = (int)(end.val[1] + 0.5) - 1;
  edge->yEnd = edge->yEnd >= src->rows ? src->rows-1 : edge->yEnd;

  edge->dxPerScan = (end.val[0] - start.val[0] ) / dscan;
  edge->dzPerScan = (1.0/end.val[2] - 1.0/start.val[2]) / dscan;
  adjust = start.val[1] - floor(start.val[1]);
  adjust = adjust <= 0.5 ? 0.5 - adjust : (1.0 - adjust) + 0.5;
  edge->xIntersect = start.val[0] + adjust*edge->dxPerScan;
  edge->zIntersect = 1.0/start.val[2] + adjust*edge->dzPerScan;

	//printf("makeEdgeRec(): %.3f %.3f\n", start.val[1], edge->dzPerScan);


  if( ds->shade == ShadeFlat ) {
    edge->dcPerScan.c[0] = 0.0;
    edge->dcPerScan.c[1] = 0.0;
    edge->dcPerScan.c[2] = 0.0;

    edge->cIntersect = c1;
  }

  if( edge->y0 < 0 ) {
    edge->xIntersect += edge->dxPerScan * (-edge->y0);
    edge->zIntersect += edge->dzPerScan * (-edge->y0);

    edge->y0 = 0.0;
    edge->x0 = edge->xIntersect;
    edge->yStart = 0;
  }

  edge->xIntersect = edge->dxPerScan > 0.0 && edge->xIntersect > edge->x1 ? edge->x1 : edge->xIntersect;
  edge->xIntersect = edge->dxPerScan < 0.0 && edge->xIntersect < edge->x1 ? edge->x1 : edge->xIntersect;

   printf("makeEdgeRec(): %.3f %.3f\n", edge->zIntersect, edge->dzPerScan);

  return( edge );
}

	static int compXIntersect( const void *a, const void *b ) {
	EdgeRec *ea = (EdgeRec *)a;
	EdgeRec *eb = (EdgeRec *)b;

	if( ea->xIntersect < eb->xIntersect )
		return(-1);
	else if(ea->xIntersect > eb->xIntersect )
		return(1);

	return(0);
}

static LinkedList *setupEdgeList( Polygon *p, Image *src, DrawState *ds ) {
    LinkedList *edges = NULL;
    Point v1, v2;
    Color c1, c2;

    edges = ll_new();

    v1 = p->vertex[p->numVertex-1];

    if(p->color == NULL)
       c1 = ds->color;
    else
        c1 = p->color[p->numVertex - 1];

    for(int i=0 ; i < p->numVertex ; i++ ) {

        v2 = p->vertex[i];
        if(p->color == NULL)
            c2 = ds->color;
        else
            c2 = p->color[p->numVertex - 1];


        if( (int)(v1.val[1]+0.5) != (int)(v2.val[1]+0.5) ) {
            EdgeRec *edge;

            if( v1.val[1] < v2.val[1] )
                edge = makeEdgeRec( v1, v2, c1, c2, src, ds );
            else
                edge = makeEdgeRec( v2, v1, c2, c1, src, ds );

            if( edge )
                ll_insert( edges, edge, compYStart );
        }
        v1 = v2;
        c1 = c2;
    }

    if( ll_empty( edges ) ) {
        ll_delete( edges, NULL );
        edges = NULL;
    }

    return(edges);
}

static void fillScan( int scan, LinkedList *active, DrawState *ds, Image *src, Lighting *lights ) {
  EdgeRec *p1, *p2;
  int i, f;
  float curz, dzPerCol;
  Color c = ds->color;
  Color dcPerCol = {{0, 0, 0}};
  float dx;
  float avgz;
  float curs=0, curt=0, dsPerCol=0, dtPerCol=0;

  p1 = ll_head( active );
  while(p1) {
	  p2 = ll_next( active );
	  if( p2 == NULL ) {
		  printf("bad bad bad (your edges are not coming in pairs)\n");
		  break;
	  }

	  if( p2->xIntersect == p1->xIntersect ) {
		  p1 = ll_next( active );
		  continue;
	  }

	  // set up the z values
	  dx = p2->xIntersect - p1->xIntersect;
	  dzPerCol = (p2->zIntersect - p1->zIntersect) / dx;
	  curz = p1->zIntersect;

  	printf("p1 zIntersect %.3f p2 zIntersect %.3f\n", p1->zIntersect, p2->zIntersect);
  	printf("curz %.3f  dzPerCol %.3f\n", curz, dzPerCol);
  	printf("dx %.3f  \n", dx);
  	// 214.97 216.5

	  avgz = (p1->zIntersect + p2->zIntersect) * 0.5;
	  switch(ds->shade) {

	  case ShadeConstant:
			c = ds->color;
			dzPerCol = 0.0;
			curz = 1.0;
		  break;

		case ShadeFlat:
			c = p1->cIntersect;
			dcPerCol.c[0] = 0.0;
			dcPerCol.c[1] = 0.0;
			dcPerCol.c[2] = 0.0;
			break;

	  case ShadeDepth:
		  c.c[0] = p1->zIntersect;
		  dcPerCol.c[0] = dzPerCol;
		  break;

	  default:
			printf("Invalid Shade Method\n");
		  break;
	  }

	  i = (int)(p1->xIntersect);

	  if(i < 0) {
		  curz += -i*dzPerCol;
		  c.c[0] += -i*dcPerCol.c[0];
		  c.c[1] += -i*dcPerCol.c[1];
		  c.c[2] += -i*dcPerCol.c[2];
		  curs += -i*dsPerCol;
		  curt += -i*dtPerCol;
		  i = 0;
	  }

	  f = (int)(p2->xIntersect);

	  f = f > src->cols ? src->cols : f;

	  for(;i<f;i++) {

		  if( ds->shade == ShadeConstant || (curz - 0.0001*avgz > image_getz( src, scan, i ) && curz < 1000.0) ) {
			  switch(ds->shade) {
			  case ShadeConstant:
			  case ShadeFlat:
				  image_setColor( src, scan, i, c );
				  image_setz( src, scan, i, curz );
				  break;

			  case ShadeDepth:
			  {
				  Color ct;

				  ct.c[0] = ct.c[1] = ct.c[2] = 1.0 - (1.0 / c.c[0]);
				  image_setColor( src, scan, i, ct );
				  image_setz( src, scan, i, curz );

			  }
			  break;

			  default:
				  break;
			  }
		  }
		  curz += dzPerCol;
		  c.c[0] += dcPerCol.c[0];
		  c.c[1] += dcPerCol.c[1];
		  c.c[2] += dcPerCol.c[2];
		  curs += dsPerCol;
		  curt += dtPerCol;

	  }
	  p1 = ll_next( active );
  }
}

    void swap_int(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}
void draw_horizontal_line(Image *src, int y, int x1, int x2, Color c) {
    if (x1 > x2) swap_int(&x1, &x2);
    for (int x = x1; x <= x2; x++) {
        FPixel pixel = { {c.c[0], c.c[1], c.c[2]}};
        src->data[y * src->cols + x] = pixel;
    }
}

    float edge_slope(Point a, Point b) {
    if (a.val[1] == b.val[1]) return 0;
    return (b.val[0] - a.val[0]) / (b.val[1] - a.val[1]);
}
// only shadeConstant could call this method !!!!!
void polygon_drawFill( Polygon *p, Image *src, Color c ) {
	DrawState ds;
	ds.color = c;
	ds.shade = ShadeConstant;
	polygon_drawShade( p, src, &ds, NULL );
}



void polygon_draw(Polygon *p, Image *src, Color c) {
    if (!(int)p || !(int)src || p->numVertex < 2) return;

    Line line;
    for (int i = 0; i < p->numVertex; i++) {
        int next = (i + 1) % p->numVertex;
        line_set(&line, p->vertex[i], p->vertex[next]);
        line_draw(&line, src, c);
    }
}

// Function to fill a Polygon using the Barycentric coordinates algorithm
// TODO: did not fit z-buffer right now
void polygon_drawFillB(Polygon *p, Image *src, Color c) {
    if ( (p == NULL) || (src == NULL) || p->numVertex < 3) return;

    int minX = src->cols, minY = src->rows, maxX = 0, maxY = 0;
    for (int i = 0; i < p->numVertex; i++) {
        if (p->vertex[i].val[0] < minX) minX = (int)p->vertex[i].val[0];
        if (p->vertex[i].val[0] > maxX) maxX = (int)p->vertex[i].val[0];
        if (p->vertex[i].val[1] < minY) minY = (int)p->vertex[i].val[1];
        if (p->vertex[i].val[1] > maxY) maxY = (int)p->vertex[i].val[1];
    }

    for (int y = minY; y <= maxY; y++) {
        for (int x = minX; x <= maxX; x++) {
            float alpha, beta, gamma;
            if (barycentric(p->vertex, x, y, &alpha, &beta, &gamma)) {
                FPixel pixel = { {c.c[0], c.c[1], c.c[2]}};
                src->data[y * src->cols + x] = pixel;
            }
        }
    }
}

// Helper function to compute Barycentric coordinates
int barycentric(Point *vlist, int px, int py, float *alpha, float *beta, float *gamma) {
    double denom = (vlist[1].val[1] - vlist[2].val[1]) * (vlist[0].val[0] - vlist[2].val[0]) +
                  (vlist[2].val[0] - vlist[1].val[0]) * (vlist[0].val[1] - vlist[2].val[1]);

    *alpha = ((vlist[1].val[1] - vlist[2].val[1]) * (px - vlist[2].val[0]) +
              (vlist[2].val[0] - vlist[1].val[0]) * (py - vlist[2].val[1])) / denom;
    *beta = ((vlist[2].val[1] - vlist[0].val[1]) * (px - vlist[2].val[0]) +
             (vlist[0].val[0] - vlist[2].val[0]) * (py - vlist[2].val[1])) / denom;
    *gamma = 1.0f - *alpha - *beta;

    return (*alpha >= 0.0f && *beta >= 0.0f && *gamma >= 0.0f);
}

 // process the edge list, assumes the edges list has at least one entry
static int processEdgeList( LinkedList *edges, Image *src, DrawState *ds, Lighting *lights ) {
	LinkedList *active = NULL;
	LinkedList *tmplist = NULL;
	LinkedList *transfer = NULL;
	EdgeRec *current;
	EdgeRec *tedge;
	int scan = 0;

	active = ll_new( );
	tmplist = ll_new( );

	// get a pointer to the first item on the list and reset the current pointer
	current = ll_head( edges );

	// start at the first scanline and go until the active list is empty
	for(scan = current->yStart;scan < src->rows;scan++ ) {
		printf("starting line %d , \n", scan );
		// grab all edges starting on this row
		while( current != NULL && current->yStart == scan ) {
			ll_insert( active, current, compXIntersect );
			current = ll_next( edges );
		}
		// current is either null, or the first edge to be handled on some future scanline

		if( ll_empty(active) ) {
			break;
		}
		// if there are active edges
		// fill out the scanline
		fillScan( scan, active, ds, src, lights );

		// remove any ending edges and update the rest
		for( tedge = ll_pop( active ); tedge != NULL; tedge = ll_pop( active ) ) {

			// keep anything that's not ending
			if( tedge->yEnd > scan ) {
				float a = 1.0;

				// update the edge information with the dPerScan values
				tedge->xIntersect += tedge->dxPerScan;

				// adjust in the case of partial overlap
				if( tedge->dxPerScan < 0.0 && tedge->xIntersect < tedge->x1 ) {
					a = (tedge->xIntersect - tedge->x1) / tedge->dxPerScan;
					tedge->xIntersect = tedge->x1;
				}
				else if( tedge->dxPerScan > 0.0 && tedge->xIntersect > tedge->x1 ) {
					a = (tedge->xIntersect - tedge->x1) / tedge->dxPerScan;
					tedge->xIntersect = tedge->x1;
				}

				tedge->zIntersect += a*tedge->dzPerScan;
				tedge->cIntersect.c[0] += a*tedge->dcPerScan.c[0];
				tedge->cIntersect.c[1] += a*tedge->dcPerScan.c[1];
				tedge->cIntersect.c[2] += a*tedge->dcPerScan.c[2];

				ll_insert( tmplist, tedge, compXIntersect );
			}
		}

		transfer = active;
		active = tmplist;
		tmplist = transfer;
	}

	// get rid of the lists, but not the edge records
	ll_delete(active, NULL);
	ll_delete(tmplist, NULL);

	return(0);
}

void polygon_drawShade(Polygon *p, Image *src, DrawState *ds, Lighting *light) {
        if (p == NULL || src == NULL || ds == NULL) return;

        LinkedList *edges = NULL;

        // set up the edge list
        edges = setupEdgeList( p, src, ds );
        if( edges == NULL )
            return;
	    printf("hahahaha , %d\n", edges->size);
        // process the edge list (should be able to take an arbitrary edge list)
        processEdgeList( edges, src, ds, light );
        // clean up
        ll_delete( edges, (void (*)(const void *))free );

        return;
}





#ifdef __cplusplus
}
#endif