/*
	Bruce A. Maxwell
	Fall 2013

  Reads data from a PLY file

  Returns...

  a list of polygons (complete with surface normals)
  a list of colors

  Blender can export to PLY files (but doesn't seem to save colors)

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graphics.h"

typedef enum {
	type_float32,
	type_uint8,
	type_int32,
	type_list,
	type_none
} ply_type;

typedef struct {
	ply_type type;
	ply_type listCardType;
	ply_type listDataType;
	char name[32];
	void *next;
} ply_property;

ply_type plyType(char *buffer);
ply_type plyType(char *buffer) {
	if(!strcmp(buffer, "float32"))
		return(type_float32);
  
	if(!strcmp(buffer, "uint8"))
		return(type_uint8);

	if(!strcmp(buffer, "int32"))
		return(type_int32);

	if(!strcmp(buffer, "list"))
		return(type_list);
  
	return(type_none);
}

#define MaxVertices (10)

int readPLY(char filename[], int *nPolygons, Polygon **plist, Color **clist, int estNormals) {
	char buffer[256];
	Point *vertex;
	Vector *normal;
	//  Point *texture;
	Color *color;
	Polygon *p;
	int numPoly;
	int numVertex;
	int vertexProp = 0;
	int faceProp = 0;
	ply_property *vertexproplist = NULL;
	ply_property *vertexproptail = NULL;
	ply_property *faceproplist = NULL;
	ply_property *faceproptail = NULL;
	int nv;
	int vid[MaxVertices];
	int i, j;
	Color tcolor;

	// first line ought to be "ply"
	// format ought to be "ascii 1.0"
	// comment lines are ignored

	// element creates an element structure and specifies how many
	// vertex is meaningful
	// float32 is %f
	// uint8 is %c
	// int32 is %d
	// x, y, z, nx, ny, nz, s, t, red, green, blue all have meaning

	// element face is meaningful
	// property list means # of elements, followed by type
	// vertex_indices means make polygons out of them

	// end_header means the first element type starts

	int doneWithHeader = 0;
	FILE *fp = fopen(filename, "r");
	if(fp) {
		// check if it's a .ply file
		fscanf(fp, "%s", buffer);
		if(strcmp(buffer, "ply")) {
			printf("%s doesn't look like a .ply file\n", filename);
			fclose(fp);
			return(-1);
		}

		while(!doneWithHeader) {
			fscanf(fp, "%s", buffer);
			switch(buffer[0]) {
			case 'f':
				// format statement
				for(;fgetc(fp) != '\n';);
				break;
	
			case 'c':
				// comment
				for(;fgetc(fp) != '\n';);
				break;

			case 'p':
				// property statement
			{
				ply_property *prop = malloc(sizeof(ply_property));
				prop->listCardType = type_none;
				prop->listDataType = type_none;
				prop->next = NULL;

				fscanf(fp, "%s", buffer); // get the data type
				prop->type = plyType(buffer);
				if(prop->type == type_list) {
					fscanf(fp, "%s", buffer); // get the first data type
					prop->listCardType = plyType(buffer);
					fscanf(fp, "%s", buffer); // get the first data type
					prop->listDataType = plyType(buffer);
				}
				else if(prop->type == type_none) {
					printf("Unrecognized property type %s", buffer);
					fclose(fp);
					return(-1);
				}
				printf("Read property type %d\n", prop->type);

				fscanf(fp, "%s", prop->name);
				printf("Read property name %s\n", prop->name);

				// add the property entry to the list
				if(vertexProp) {
					if(vertexproplist == NULL) {
						vertexproplist = prop;
						vertexproptail = prop;
					}
					else {
						vertexproptail->next = prop;
						vertexproptail = prop;
					}
				}
				else if(faceProp) {
					if(faceproplist == NULL) {
						faceproplist = prop;
						faceproptail = prop;
					}
					else {
						faceproptail->next = prop;
						faceproptail = prop;
					}
				}
			}
			break;

			case 'e':
				if(!strcmp(buffer, "end_header")) {
					doneWithHeader = 1;
					break;
				}

				// otherwise it's an element statement
				fscanf(fp, "%s", buffer);
				if(!strcmp(buffer, "vertex")) {
					printf("Read element vertex\n");
					vertexProp = 1;
					faceProp = 0;
					fscanf(fp, "%d", &numVertex);
				}
				else if(!strcmp(buffer, "face")) {
					printf("Read element face\n");
					faceProp = 1;
					vertexProp = 0;
					fscanf(fp, "%d", &numPoly);
				}
				break;

			default: // don't know what to do with it
				for(;fgetc(fp) != '\n';);
				break;
			}
		}
		// finished with the header
		vertex = malloc(sizeof(Point) * numVertex);
		normal = malloc(sizeof(Vector) * numVertex);
		// texture
		color = malloc(sizeof(Color) * numVertex); // apparently not written by Blender

		// read the vertices
		for(i=0;i<numVertex;i++) {
			for(j=0;j<3;j++)
				fscanf(fp, "%f", &(vertex[i].val[j]));
			vertex[i].val[3] = 1.0;

			for(j=0;j<3;j++)
				fscanf(fp, "%f", &(normal[i].val[j]));
			normal[i].val[3] = 0.0;

			for(j=0;j<2;j++)
				fscanf(fp, "%*f");
      
			for(j=0;j<3;j++) {
				fscanf(fp, "%f", &(color[i].c[j]));
				color[i].c[j] /= 255.0;
			}
		}

		p = malloc(sizeof(Polygon) * numPoly);
		*clist = malloc(sizeof(Color) * numPoly);

		// read the faces and build the polygons
		for(i=0;i<numPoly;i++) {
			polygon_init(&(p[i]));

			// read in the vertex indices
			nv = 0;
			fscanf(fp, "%d", &nv);

			if(nv > MaxVertices) {
				printf("Number of vertices is greater than MaxVertices (%d), terminating\n", nv);
				exit(-1);
			}

			for(j=0;j<nv;j++) {
				fscanf(fp, "%d", &(vid[j]));
			}

			// assign the polygon vertices and surface normals
			// not setting vertexWorld right now, because no Phong shading

			p[i].nVertex = nv;
//			p[i].zBufferFlag = 1;
			p[i].normal = malloc(sizeof(Vector)*nv);
			p[i].vertex = malloc(sizeof(Point)*nv);
			tcolor.c[0] = tcolor.c[1] = tcolor.c[2] = 0.0;
			//      printf("%d: ", nv);
			for(j=0;j<nv;j++) {
				//	printf("%d  ", vid[j]);
				p[i].vertex[j] = vertex[vid[j]];
				if(!estNormals) {
					p[i].normal[j] = normal[vid[j]];
				}
				tcolor.c[0] += color[vid[j]].c[0];
				tcolor.c[1] += color[vid[j]].c[1];
				tcolor.c[2] += color[vid[j]].c[2];
			}
			tcolor.c[0] /= (float)nv;
			tcolor.c[1] /= (float)nv;
			tcolor.c[2] /= (float)nv;

			if(estNormals) {
				Vector tx, ty, tn;

				tx.val[0] = p[i].vertex[0].val[0] - p[i].vertex[1].val[0];
				tx.val[1] = p[i].vertex[0].val[1] - p[i].vertex[1].val[1];
				tx.val[2] = p[i].vertex[0].val[2] - p[i].vertex[1].val[2];

				ty.val[0] = p[i].vertex[2].val[0] - p[i].vertex[1].val[0];
				ty.val[1] = p[i].vertex[2].val[1] - p[i].vertex[1].val[1];
				ty.val[2] = p[i].vertex[2].val[2] - p[i].vertex[1].val[2];

				vector_cross(&tx, &ty, &tn);
				vector_normalize(&tn);

				for(j=0;j<nv;j++)
					p[i].normal[j] = tn;
			}
      
			printf("(%.2f %.2f %.2f)\n", tcolor.c[0], tcolor.c[1], tcolor.c[2]);

			(*clist)[i] = tcolor;
		}

		*nPolygons = numPoly;
		*plist = p;

		free(vertex);
		free(normal);
		//    free(texture);
		free(color);

		{
			ply_property *q;

			while(vertexproplist != NULL) {
				q = (ply_property *)vertexproplist->next;
				free(vertexproplist);
				vertexproplist = q;
			}

			while(faceproplist != NULL) {
				q = (ply_property *)faceproplist->next;
				free(faceproplist);
				faceproplist = q;
			}
		}

		fclose(fp);
	}
	else {
		printf("Unable to open %s\n", filename);
		return(-1);
	}

	return(0);
}
