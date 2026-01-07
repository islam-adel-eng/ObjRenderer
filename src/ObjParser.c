/*  OBJ file parser
 *
 * This program takes in an OBJ file with a maximum number of vertices and edges.
 * It then converts it to arrays of structs of 3D points and edges (pairs of points).
 *
 * It is intended to be used as a library.
 *
 * Limitations:
 *  - Ignores vt and vn, only parses faces and vertices.
 *  - Fixed size arrays for vertices, edges and vertices per face.
 *  - Produces double edges (no duplicate edge removal)
 *
 * Author: Islam Adel
 */

#include <stdio.h>
#include <Types.h>

#define MAX_VERTICES 10000
#define MAX_EDGES 20000
#define MAX_FACE_VERTICES 12 // I seriously don't know why I chose 12, there is no way you want to go beyond that...

void ParseObj(char *file, point3D *vertices, edge *edges, int *vertexCount, int *edgeCount)
{
    FILE *ObjectFile = fopen(file, "r");
    if(!ObjectFile) // probably wrong path provided
    {
        printf("Failed to open file\n");
        return;
    }
    char line[2048]; // buffer for line, 2048 characters should be enough :D
    int j = 0; // face elements counter
    int n = 0; // used to store how many characters were read by sscanf("%n")
    int face[MAX_FACE_VERTICES]; // temporary face array

    while(fgets(line, 2048, ObjectFile) != NULL)
    {
        // read vertices
        if(line[0] == 'v' && line[1] == ' ') // second condition is to ensure vt and vn are ignored
        {
            sscanf(line, "v %f %f %f", &vertices[*vertexCount].x, &vertices[*vertexCount].y, &vertices[*vertexCount].z);
            (*vertexCount)++;
        }

        // read faces
        if (line[0] == 'f')
        {
            j = 0;
            for (int i = 1; line[i]; )  // i = 1 to skip the "f" at the beginning of line
            {
                // read the first number before any '/'
                if (sscanf(line + i, "%d%n", &face[j], &n) == 1)
                {
                    j++;
                    i += n;

                    // skip everything until next space (ignoring /vt/vn)
                    while (line[i] && line[i] != ' ') i++;
                    if (line[i] == ' ') i++;
                }
                else break;
            }

            // convert faces into edges (duplicates are NOT removed, leads to double the edge count)
            for (n = 0; n < j; n++)
            {
                edges[*edgeCount].a = face[n]-1;
                edges[*edgeCount].b = face[(n + 1) % j]-1;
                (*edgeCount)++;
            }
        }

        // stop reading if max edges OR max vertices are exceeded
        if((*vertexCount >= MAX_VERTICES) || (*edgeCount >= MAX_EDGES))
        {
            printf("Edges or vertices in %s exceeds the Vertices(%d) or Edges(%d) limit :(\n", file, MAX_VERTICES, MAX_EDGES);
            break;
        }

    }
    fclose(ObjectFile);
}


// int main(int argc, char **arg)
// {
//     point3D points[MAX_VERTICES];
//     edge edges[MAX_EDGES];
//     int vertexCount = 0;
//     int edgeCount = 0;
//     ParseObj(arg[1], points, edges, &vertexCount, &edgeCount);
//     int i = 0;
//     int j = 0;
//
//     if((vertexCount < MAX_VERTICES) && (edgeCount < MAX_EDGES))
//     {
//         printf("\nVertices (%d):\n", vertexCount);
//         for(int i = 0; i<vertexCount; i++)
//         {
//             printf("[%f, %f, %f]\n", points[i].x, points[i].y, points[i].z);
//         }
//         printf("\nEdges (%d):", edgeCount);
//
//         for(int j = 0; j<edgeCount; j++)
//         {
//             printf("\n[%d, %d]", edges[j].a, edges[j].b);
//         }
//     }
//     return 0;
// }
