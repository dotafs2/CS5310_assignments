/*
Bruce A. Maxwell
Fall 2014

Example of a 3D scene model
 */

#include <stdio.h>
#include <stdlib.h>
#include "Module.h"
#include "Light.h"

/*
  Adds to the module a unit cylinder, aligned along the Y-axis

  Takes in the number of subdivisions to use
 */
void cylinder( Module *mod, int sides );
void cylinder( Module *mod, int sides ) {
  Polygon p;
  Point xtop, xbot;
  double x1, x2, z1, z2;
  int i;

  polygon_init( &p );
  point_set3D( &xtop, 0, 1.0, 0.0 );
  point_set3D( &xbot, 0, 0.0, 0.0 );

  // make a fan for the top and bottom sides
  // and quadrilaterals for the sides
  for(i=0;i<sides;i++) {
    Point pt[4];
    Vector n[4];
    int j;

    x1 = cos( i * M_PI * 2.0 / sides );
    z1 = sin( i * M_PI * 2.0 / sides );
    x2 = cos( ( (i+1)%sides ) * M_PI * 2.0 / sides );
    z2 = sin( ( (i+1)%sides ) * M_PI * 2.0 / sides );

    point_copy( &pt[0], &xtop );
    point_set3D( &pt[1], x1, 1.0, z1 );
    point_set3D( &pt[2], x2, 1.0, z2 );

    polygon_set( &p, 3, pt );
    for(j=0;j<3;j++)
	    vector_set( &(n[j]), 0, 1, 0 );
    polygon_setNormals( &p, 3, n );
    module_polygon( mod, &p );

    point_copy( &pt[0], &xbot );
    point_set3D( &pt[1], x1, 0.0, z1 );
    point_set3D( &pt[2], x2, 0.0, z2 );
    polygon_set( &p, 3, pt );
    for(j=0;j<3;j++)
	    vector_set( &(n[j]), 0, -1, 0 );
    polygon_setNormals( &p, 3, n );
    module_polygon( mod, &p );

    point_set3D( &pt[0], x1, 0.0, z1 );
    point_set3D( &pt[1], x2, 0.0, z2 );
    point_set3D( &pt[2], x2, 1.0, z2 );
    point_set3D( &pt[3], x1, 1.0, z1 );

    vector_set( &n[0], x1, 0.0, z1 );
    vector_set( &n[1], x2, 0.0, z2 );
    vector_set( &n[2], x2, 0.0, z2 );
    vector_set( &n[3], x1, 0.0, z1 );
    
    polygon_set( &p, 4, pt );
    polygon_setNormals( &p, 4, n );
    module_polygon( mod, &p );
  }

  polygon_clear( &p );
}

// adds a cube centered on the origin of width 2x2x2 to the module
void cube( Module *mod );
void cube( Module *mod ) {
  Point pt[4];
  Polygon p;
  Vector n[4];
  int i;

  polygon_init( &p );
  point_set3D( &pt[0], -1, -1, -1 );
  point_set3D( &pt[1], -1, -1,  1 );
  point_set3D( &pt[2], -1,  1,  1 );
  point_set3D( &pt[3], -1,  1, -1 );
  polygon_set( &p, 4, pt );
  for(i=0;i<4;i++)
	  vector_set( &(n[i]), -1, 0, 0 );
  polygon_setNormals( &p, 4, n );
  module_polygon( mod, &p );

  point_set3D( &pt[0], 1, -1, -1 );
  point_set3D( &pt[1], 1, -1,  1 );
  point_set3D( &pt[2], 1,  1,  1 );
  point_set3D( &pt[3], 1,  1, -1 );
  polygon_set( &p, 4, pt );
  for(i=0;i<4;i++)
	  vector_set( &(n[i]), 1, 0, 0 );
  polygon_setNormals( &p, 4, n );
  module_polygon( mod, &p );

  point_set3D( &pt[0], -1, -1, -1 );
  point_set3D( &pt[1], -1, -1,  1 );
  point_set3D( &pt[2],  1, -1,  1 );
  point_set3D( &pt[3],  1, -1, -1 );
  polygon_set( &p, 4, pt );
  for(i=0;i<4;i++)
	  vector_set( &(n[i]), 0, -1, 0 );
  polygon_setNormals( &p, 4, n );
  module_polygon( mod, &p );

  point_set3D( &pt[0], -1, 1, -1 );
  point_set3D( &pt[1], -1, 1,  1 );
  point_set3D( &pt[2],  1, 1,  1 );
  point_set3D( &pt[3],  1, 1, -1 );
  polygon_set( &p, 4, pt );
  for(i=0;i<4;i++)
	  vector_set( &(n[i]), 0, 1, 0 );
  polygon_setNormals( &p, 4, n );
  module_polygon( mod, &p );

  point_set3D( &pt[0], -1, -1, -1 );
  point_set3D( &pt[1], -1,  1, -1 );
  point_set3D( &pt[2],  1,  1, -1 );
  point_set3D( &pt[3],  1, -1, -1 );
  polygon_set( &p, 4, pt );
  for(i=0;i<4;i++)
	  vector_set( &(n[i]), 0, 0, -1 );
  polygon_setNormals( &p, 4, n );
  module_polygon( mod, &p );

  point_set3D( &pt[0], -1, -1, 1 );
  point_set3D( &pt[1], -1,  1, 1 );
  point_set3D( &pt[2],  1,  1, 1 );
  point_set3D( &pt[3],  1, -1, 1 );
  polygon_set( &p, 4, pt );
  for(i=0;i<4;i++)
	  vector_set( &(n[i]), 0, 0, 1 );
  polygon_setNormals( &p, 4, n );
  module_polygon( mod, &p );

}

// makes 3 X-wing fighters in a loose formation
int main(int argc, char *argv[]) {

  Image *src;
  Module *scene;
  Module *engine;
  Module *wing;
  Module *wings;
  Module *laser;
  Module *body;
  Polygon p;
  Point pt[4];
  Vector n[4];
  View3D view;
  Matrix vtm, gtm;
  DrawState *ds;
  Lighting *light;
  Color Flame;
  Color Red;
  Color Grey;
  Color Dark;
	Color White;
  float bodyWidth = 2.0;
  int i;

	color_set(&White, 1.0, 1.0, 1.0 );
	color_set(&Flame, 1.0, 0.7, 0.2 );
	color_set(&Red, 1.0, 0.2, 0.1 );
	color_set(&Grey, 0.6, 0.65, 0.67 );
	color_set(&Dark, 0.2, 0.2, 0.2 );

  // set up the view
  point_set3D( &(view.vrp), 15, 30, 100 );
  vector_set( &(view.vpn), -20, -30, -100 );
  vector_set( &(view.vup), 0, 1, 0 );
  view.d = 2.2;
  view.du = 1.6;
  view.dv = 0.9;
  view.f = 1;
  view.b = 200;
  view.screenx = 1280;
  view.screeny = 720;

  matrix_setView3D( &vtm, &view );
  matrix_identity( &gtm );

  // engine
  engine = module_create();
  module_scale( engine, 1.3, 6, 1.3);
  module_rotateX( engine, 0, 1 );
  cylinder( engine, 10 );
  module_scale( engine, .8, .8, 1.1 );
  module_bodyColor( engine, &Flame );
  module_surfaceColor( engine, &Dark );
  cylinder( engine, 10 );

  // laser
  laser = module_create();
  module_scale( laser, 0.5, 5, 0.5 );
  module_rotateX( laser, 0, 1 );
  cylinder( laser, 6 );
  module_scale( laser, 0.4, 0.4, 1.0 );
  module_translate( laser, 0, 0, 4.5 );
  module_bodyColor( laser, &Red );
  cylinder( laser, 10 );

  // wing
  wing = module_create();
  polygon_init( &p );
  point_set3D( &pt[0], 0.0, 0.0, 0.0 );
  point_set3D( &pt[1], 0.0, 0.0, 5.0 );
  point_set3D( &pt[2], 15.0, 0.0, 3.0 );
  point_set3D( &pt[3], 15.0, 0.0, 0.0 );
  polygon_set( &p, 4, pt );
  for(i=0;i<4;i++)
	  vector_set( &(n[i]), 0, -1, 0 );
  polygon_setNormals( &p, 4, n );
  module_polygon( wing, &p );

  module_translate( wing, 0, 0.5, 0 );
  for(i=0;i<4;i++)
	  vector_set( &(n[i]), 0, 1, 0 );
  polygon_setNormals( &p, 4, n );
  module_polygon( wing, &p );
  
  module_identity(wing);
  module_translate( wing, 3, 1.6, -1 );
  module_module( wing, engine );

  module_identity(wing);
  point_set3D( &pt[0], 15.0, 0.0, 3.0 );
  point_set3D( &pt[1], 15.0, 0.0, 0.0 );
  point_set3D( &pt[2], 15.0, 0.5, 0.0 );
  point_set3D( &pt[3], 15.0, 0.5, 3.0 );
  polygon_set( &p, 4, pt );
  for(i=0;i<4;i++)
	  vector_set( &(n[i]), 1, 0, 0 );
  polygon_setNormals( &p, 4, n );
  module_polygon( wing, &p );

  point_set3D( &pt[0], 15.0, 0.0, 0.0 );
  point_set3D( &pt[1], 0.0, 0.0, 0.0 );
  point_set3D( &pt[2], 0.0, 0.5, 0.0 );
  point_set3D( &pt[3], 15.0, 0.5, 0.0 );
  polygon_set( &p, 4, pt );
  for(i=0;i<4;i++)
	  vector_set( &(n[i]), 0, 0, -1 );
  polygon_setNormals( &p, 4, n );
  module_polygon( wing, &p );

  point_set3D( &pt[0], 15.0, 0.0, 3.0 );
  point_set3D( &pt[1], 15.0, 0.5, 3.0 );
  point_set3D( &pt[2], 0.0, 0.5, 5.0 );
  point_set3D( &pt[3], 0.0, 0.0, 5.0 );
  polygon_set( &p, 4, pt );
  for(i=0;i<4;i++)
	  vector_set( &(n[i]), 2, 0, 15 );
  polygon_setNormals( &p, 4, n );
  module_polygon( wing, &p );

  module_translate( wing, 15, 0.25, 0 );
  module_module( wing, laser );

  // 4 wings
  wings = module_create();
  module_bodyColor( wings, &Grey );
  module_rotateZ(wings, cos( 0.3 ), sin( 0.3 ) );
  module_translate(wings, bodyWidth, 0, 0 );
  module_module( wings, wing );

  module_identity( wings );
  module_scale( wings, 1, -1, 1 );
  module_rotateZ(wings, cos( -0.3) , sin(-0.3) );
  module_translate(wings, bodyWidth, 0, 0 );
  module_module( wings, wing );

  module_identity( wings );
  module_scale( wings, -1, 1, 1 );
  module_rotateZ(wings, cos( -0.3) , sin(-0.3) );
  module_translate( wings, -bodyWidth, 0, 0 );
  module_module( wings, wing );

  module_identity( wings );
  module_scale( wings, -1, -1, 1 );
  module_rotateZ(wings, cos( 0.3) , sin(0.3) );
  module_translate( wings, -bodyWidth, 0, 0 );
  module_module( wings, wing );

  // body
  body = module_create();
  module_bodyColor( body, &White);
  module_surfaceColor( body, &Dark );

  module_module( body, wings );

  module_scale(body, bodyWidth, bodyWidth, 8 );
  module_translate(body, 0, 0, 4 );
  cube( body );


  module_identity(body);
  point_set3D( &pt[0], bodyWidth, bodyWidth, 12 );
  point_set3D( &pt[1], bodyWidth, -bodyWidth, 12 );
  point_set3D( &pt[2], bodyWidth*0.5, -bodyWidth*0.3, 35 );
  point_set3D( &pt[3], bodyWidth*0.5, bodyWidth*0.3, 35 );
  polygon_set( &p, 4, pt );
  for(i=0;i<4;i++)
	  vector_set( &(n[i]), 23, 0, 0.5*bodyWidth );
  polygon_setNormals( &p, 4, n );
  module_polygon( body, &p );

  point_set3D( &pt[0], -bodyWidth, bodyWidth, 12 );
  point_set3D( &pt[1], -bodyWidth, -bodyWidth, 12 );
  point_set3D( &pt[2], -bodyWidth*0.5, -bodyWidth*0.3, 35 );
  point_set3D( &pt[3], -bodyWidth*0.5, bodyWidth*0.3, 35 );
  polygon_set( &p, 4, pt );
  for(i=0;i<4;i++)
	  vector_set( &(n[i]), -23, 0, 0.5*bodyWidth );
  polygon_setNormals( &p, 4, n );
  module_polygon( body, &p );

  point_set3D( &pt[0], -bodyWidth, bodyWidth, 12 );
  point_set3D( &pt[1],  bodyWidth, bodyWidth, 12 );
  point_set3D( &pt[2],  bodyWidth*0.5, bodyWidth*0.3, 35 );
  point_set3D( &pt[3], -bodyWidth*0.5, bodyWidth*0.3, 35 );
  polygon_set( &p, 4, pt );
  for(i=0;i<4;i++)
	  vector_set( &(n[i]), 0, 23, 0.5*bodyWidth );
  polygon_setNormals( &p, 4, n );
  module_polygon( body, &p );

  point_set3D( &pt[0], -bodyWidth, -bodyWidth, 12 );
  point_set3D( &pt[1],  bodyWidth, -bodyWidth, 12 );
  point_set3D( &pt[2],  bodyWidth*0.5, -bodyWidth*0.3, 35 );
  point_set3D( &pt[3], -bodyWidth*0.5, -bodyWidth*0.3, 35 );
  polygon_set( &p, 4, pt );
  for(i=0;i<4;i++)
	  vector_set( &(n[i]), 0, -23, 0.5*bodyWidth );
  polygon_setNormals( &p, 4, n );
  module_polygon( body, &p );

  point_set3D( &pt[0], -bodyWidth*0.5,  bodyWidth*0.3, 35 );
  point_set3D( &pt[1],  bodyWidth*0.5,  bodyWidth*0.3, 35 );
  point_set3D( &pt[2],  bodyWidth*0.5, -bodyWidth*0.3, 35 );
  point_set3D( &pt[3], -bodyWidth*0.5, -bodyWidth*0.3, 35 );
  polygon_set( &p, 4, pt );
  for(i=0;i<4;i++)
	  vector_set( &(n[i]), 0, 0, 1 );
  polygon_setNormals( &p, 4, n );
  module_polygon( body, &p );

  // scene with three ships (body module)
  scene = module_create();
	// draw into the scene

	light = lighting_create();
	lighting_add(light, LightPoint, &White, NULL, &(view.vrp), 0.0, 0.0);
	module_addLight(scene,light);
  module_scale( scene, 0.7, 0.7, 0.7 );
  module_rotateZ( scene, cos( 0.5 ), sin( 0.5 ) );
  module_rotateY( scene, cos(-0.1), sin(-0.1) );
  module_rotateX( scene, cos( 0.2), sin(0.2) );
  module_translate( scene, -15, -10, -10 );
  module_module( scene, body );

  module_identity(scene);
  module_scale( scene, 0.7, 0.7, 0.7 );
  module_rotateZ( scene, cos( 0.3 ), sin( 0.3 ) );
  module_rotateY( scene, cos(-0.05), sin(-0.05) );
  module_rotateX( scene, cos( 0.2), sin(0.2) );
  module_translate( scene, 5, -1, -50 );
  module_module( scene, body );

  module_identity(scene);
  module_scale( scene, 0.7, 0.7, 0.7 );
  module_rotateZ( scene, cos( 0.1 ), sin( 0.1 ) );
  module_rotateX( scene, cos( 0.2), sin(0.2) );
  module_translate( scene, -35, -3, -60 );
  module_module( scene, body );

  // image
  src = image_create( view.screeny, view.screenx );
  ds = drawstate_create();
  point_copy(&(ds->viewer), &(view.vrp) );
  ds->shade = ShadeGouraud;

  module_draw( scene, &vtm, &gtm, ds, NULL, src );

  image_write( src, "test9b.ppm" );

  polygon_clear( &p );
  module_delete( scene );
  module_delete( wing );
  module_delete( wings );
  module_delete( laser );
  module_delete( body );
  module_delete( engine );
  image_free( src );

  return(0);
}
