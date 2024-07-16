/*
  Bruce Maxwell
  Fall 2014

  A simple test function for Module_draw that creates 3 X-wing fighters as line drawings

  Requires the following functions to be defined

  element_create
  element_init
  element_delete

  module_create
  module_add
  module_delete
  module_translate2D
  module_scale2D
  module_point
  module_line
  module_module
  module_draw

  drawstate_create
  drawstate_setColor
  drawstate_copy

  vector_set

  point_set2D
  point_copy

  line_set2D
  line_set
  line_copy

  matrix_setView2D
 */
#include <stdio.h>
#include <stdlib.h>
#include "Module.h"
#include <time.h>

int main(int argc, char *argv[]) {
  View2D view;
  Matrix vtm;
  Matrix gtm;
  Module *scene;
  Module *wing;
  Module *engine;
  Module *body;
  Module *xwing;
  Module *formation;
  int i;
  Point vrp;
  Vector xaxis;
  Point p[8];
  Line l;
  DrawState *ds;
  Image *src;

  srand((unsigned int)time(NULL));
  // setup gtm
  matrix_identity(&gtm);
  point_set2D(&vrp, 0, 0);
  vector_set(&xaxis, 1, 0, 0);
  view.x = xaxis;
  view.vrp = vrp;
  view.dx = 2;
  view.screenx = 640;
  view.screeny = 360;
  matrix_setView2D(&vtm, &view);
  // create a body
  body = module_create();

  point_set2D(&p[0], 0, 0);
  point_set2D(&p[1], 2, .1);
  point_set2D(&p[2], 2.2, 0.25 );
  point_set2D(&p[3], 2, 0.4 );
  point_set2D(&p[4], 0, .5 );

  for(i=0;i<5;i++) {
    int a = i;
    int b = (i+1) % 5;

    line_set( &l, p[a], p[b] );
    module_line( body, &l );
  }
  line_set2D( &l, 0.6, 0.05, 0.6, 0.45 );
  module_line( body, &l );
  line_set2D( &l, 1.1, 0.08, 1.1, 0.42 );
  module_line( body, &l );

  // create an engine
  engine = module_create();

  point_set2D( &p[0], 0, 0 );
  point_set2D( &p[1], .6, 0 );
  point_set2D( &p[2], .6, .2 );
  point_set2D( &p[3], 0, .2 );

  for(i=0;i<4;i++) {
    int a = i;
    int b = (i+1) % 4;

    line_set( &l, p[a], p[b] );
    module_line( engine, &l );
  }

  // make a wing
  wing = module_create();

  point_set2D(&p[0], 0.5, 0);
  point_set2D(&p[1], 0.3, 1.5);
  point_set2D(&p[2], 0.7, 1.5 );
  point_set2D(&p[3], 0, 1.5 );
  point_set2D(&p[4], 0, 0 );

  for(i=0;i<5;i++) {
    int a = i;
    int b = (i+1) % 5;

    line_set( &l, p[a], p[b] );
    module_line( wing, &l );
  }
  module_scale2D( wing, 1.5, 1.0 );
  module_translate2D( wing, -0.05, 0.05 );
  module_module( wing, engine );

  // make an x-wing
  xwing = module_create();

  module_module(xwing, body );
  module_translate2D( xwing, 0, .5 );
  module_module( xwing, wing);
  module_identity(xwing);
  module_scale2D( xwing, 1, -1 );
  module_translate2D( xwing, 0, 0 );
  module_module( xwing, wing );

  // make a formation
  formation = module_create();

  module_module(formation, xwing );
  module_translate2D(formation, -4, 3 );
  module_module( formation, xwing );
  module_translate2D(formation, 0, -5 );
  module_module( formation, xwing );

  // make a scene
  scene = module_create();
  module_scale2D( scene, 0.1, 0.1 );
  module_translate2D( scene, 0.2, 0 );
  module_module( scene, formation );

	// draw stars into the scene
  module_identity(scene);
  for(i=0;i<30;i++) {
      point_set2D(&(p[0]), ((double)rand() / RAND_MAX) * 2 - 1, ((double)rand() / RAND_MAX) * 1 - 0.5);
    module_point( scene, &(p[0]) );
  }


	// create the image and draw the module
  src = image_create( view.screeny, view.screenx );
  ds = drawstate_create(); // default color is white
  module_draw( scene, &vtm, &gtm, ds, NULL, src );

	// write out the image
  image_write( src, "xwings.ppm" );

	// free modules
  module_delete( scene );
  module_delete( formation );
  module_delete( xwing );
  module_delete( body );
  module_delete( wing );

	// free drawstate
  free( ds );

	// free image
  image_free( src );

  return(0);
}
