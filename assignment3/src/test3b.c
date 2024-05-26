/*
  Bruce A. Maxwell
  Fall 2014

  Assignment 3 test program 2

  Creates a 2D subdivision surface out of lines.

*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Line.h"
#include "include/Image.h"

typedef struct {
  Line l;
  void *next;
} LineNode;

// returns a new line node
LineNode *newLineNode(float x1, float y1, float x2, float y2);
LineNode *newLineNode(float x1, float y1, float x2, float y2) {
  LineNode *ln = malloc(sizeof(LineNode));
  line_set2D( &(ln->l), x1, y1, x2, y2 );
  ln->next = NULL;

  return(ln);
}

// push the new node on the front of the list
LineNode *pushLineNode(LineNode *root, LineNode *ln);
LineNode *pushLineNode(LineNode *root, LineNode *ln) {
  ln->next = (void *)root;
  root = ln;

  return( root );
}

// draws all the lines into the image
int drawLines( Image *src, LineNode *root, Color c );
int drawLines( Image *src, LineNode *root, Color c ) {
  LineNode *p = root;
  while( p != NULL ) {
    line_draw( &(p->l), src, c );
    p = (LineNode *)(p->next);
  }

  return(0);
}

// takes in a list of lines and subdivides each line into two new ones
LineNode *subdivide( LineNode *root, float xc, float yc, float perturb );
LineNode *subdivide( LineNode *root, float xc, float yc, float perturb ) {
  LineNode *newlist = NULL;
  LineNode *p = root;
  
  while(p != NULL) {
    float xm, ym, da, db, d;
    LineNode *la, *lb;
    LineNode *q;

    // get parent distances from the center
    da = sqrt( (p->l.a.val[0] - xc)*(p->l.a.val[0] - xc) + 
               (p->l.a.val[1] - yc)*(p->l.a.val[1] - yc) );
    db = sqrt( (p->l.b.val[0] - xc)*(p->l.b.val[0] - xc) + 
               (p->l.b.val[1] - yc)*(p->l.b.val[1] - yc) );
    da = (da + db)*0.5;

    // calculate the midpoint location between the two parents
    xm = (p->l.a.val[0] + p->l.b.val[0])*0.5 - xc;
    ym = (p->l.a.val[1] + p->l.b.val[1])*0.5 - yc;

    // scale the midpoint location relative to the central point
    d = sqrt( xm*xm + ym*ym );
    xm *= (da + perturb*da*(drand48()-0.5))/d;
    ym *= (da + perturb*da*(drand48()-0.5))/d;
    xm += xc;
    ym += yc;

    // create two new lines
    la = newLineNode( p->l.a.val[0], p->l.a.val[1], xm, ym );
    lb = newLineNode( xm, ym, p->l.b.val[0], p->l.b.val[1] );

    // push the two new lines on the stack
    newlist = pushLineNode( newlist, la );
    newlist = pushLineNode( newlist, lb );

    // move to the next line and free the parent line.
    q = p;
    p = (LineNode *)(p->next);

    free( q );
  }

  // return the new list
  return( newlist );
}

int main(int argc, char *argv[]) {
  int MaxDivisions = 6;
  int i, j;
  Image *src;

  // hold the lines in a linked list
  LineNode *root = NULL;
  
  Color c;
  float perturb = 0.5;
	int seed;

  printf("Usage: %s <divisions> <perturbation> <random seed (optional)>\n", argv[0] );

  // check for divisions and perturbation arguments
  if(argc > 1) {
    int tmp = atoi( argv[1] );
    MaxDivisions = tmp < MaxDivisions ? tmp : MaxDivisions;
  }
  printf("Using %d divisions\n", MaxDivisions);

  if(argc > 2 ){
    float pert = atof( argv[2] );
    perturb = pert;
  }
  printf("Using %.2f perturbation\n", perturb);

	// seed the program if one is given
	if(argc > 3) {
		seed = atoi( argv[3] );
		srand48( seed );
		printf("Seeding using %d\n", seed);
	}
	else {
		printf("Using default random seed\n");
	}

  // create the image
  src = image_create( 500, 500 );

  // make 5 objects
  for(j=0;j<5;j++) {
    int x0, y0, x1, y1, ds;
    float ptmp = perturb;

    // random placement and size
    x0 = 100 + rand() % 150;
    y0 = 100 + rand() % 150;
    ds = 10 + rand() % 200;
    x1 = x0 + ds;
    y1 = y0 + ds;

    // create four lines that make a box
    root = pushLineNode( root, newLineNode( x0, y0, x0, y1 ) );
    root = pushLineNode( root, newLineNode( x0, y1, x1, y1 ) );
    root = pushLineNode( root, newLineNode( x1, y1, x1, y0 ) );
    root = pushLineNode( root, newLineNode( x1, y0, x0, y0 ) );

    // subdivide the lines
    for(i = 0;i < MaxDivisions;i++) {

      root = subdivide( root, (x1+x0)/2, (y1+y0)/2, ptmp );
      ptmp *= 0.5f;

    }

    // draw the object in a random color
    c.c[0] = drand48();
    c.c[1] = drand48();
    c.c[2] = drand48();
    drawLines( src, root, c );

    // delete the list of lines
    {
      LineNode *q, *p = root;
      while(p != NULL) {
        q = p;
        p = (LineNode *)p->next;
        free( q );
      }
      root = NULL;
    }
  }

  // write the image
  image_write( src, "test3b.ppm");

  // free the image
  image_free( src );

  return(0);
}
