/*  main program source code
 *
 * This is the core program, it's purpose is to take in parsed OBJ files and render them.
 * It uses weak perspective projection [https://en.wikipedia.org/wiki/3D_projection#Weak_perspective_projection].
 *
 * Author: Islam Adel
 */

#include <SDL3/SDL.h>
#include <ObjParser.h>
#include <math.h>
#include <stdio.h>

// Screen variables
SDL_Renderer* renderer;
SDL_Window* window;
int width = 1920;
int height = 1080;

//-----------Functions-----------\\

float f = 200.0f; // defined outside to be modified at runtime
point2D project(point3D p, float distance)
{
    point2D p_projected;
    float z = p.z + distance; // z + distance from camera
    p_projected.x = width/2.0f + (p.x/z)*f; // division by 2.0f is to center the object on the screen
    p_projected.y = height/2.0f - (p.y/z)*f;

    return p_projected;
};

point3D rotate3D(point3D p, float ax, float ay, float az)
{
    // temporary variables to not overrwite values mid-calculation
    float x = p.x;
    float y = p.y;
    float z = p.z;

    // Rotate around Z
    float x1 = x * cosf(az) - y * sinf(az);
    float y1 = x * sinf(az) + y * cosf(az);
    float z1 = z;

    // Rotate around Y
    float x2 = x1 * cosf(ay) + z1 * sinf(ay);
    float y2 = y1;
    float z2 = -x1 * sinf(ay) + z1 * cosf(ay);

    // Rotate around X
    float x3 = x2;
    float y3 = y2 * cosf(ax) - z2 * sinf(ax);
    float z3 = y2 * sinf(ax) + z2 * cosf(ax);

    return (point3D){x3, y3, z3};
}

//----------------------\\




int main(int argc, char *argv[])
{
    //-----------Object Parsing-----------\\

    point3D vertices[MAX_VERTICES];
    point2D points2D[MAX_VERTICES];
    edge edges[MAX_EDGES];
    int vertexCount = 0;
    int edgeCount = 0;
    ParseObj(argv[1], vertices, edges, &vertexCount, &edgeCount);
    //----------------------\\

    float ax = 0, ay = 0, az = 0;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer("ObjRenderer", width, height, SDL_WINDOW_FULLSCREEN, &window, &renderer);
    SDL_SetRenderLogicalPresentation(renderer, width, height, SDL_LOGICAL_PRESENTATION_INTEGER_SCALE);

    //-----------Loop Variables-----------\\

    float CurrentTime;
    float LastTime;
    float Delta;
    LastTime = SDL_GetTicks();
    char running = 1;
    SDL_Event e;
    //----------------------\\

    //-----------Main Loop-----------\\

    while (running)
    {
        CurrentTime = SDL_GetTicks();
        Delta = (CurrentTime - LastTime)/1000.0f;
        LastTime = CurrentTime;

        //-----------Event management-----------\\

        while(SDL_PollEvent(&e))
        {
            if (e.type == SDL_EVENT_QUIT)
            {
                SDL_Quit();
                running = false;
            }

            if(e.type == SDL_EVENT_KEY_DOWN) // Keyboard input management
            {

                switch (e.key.key) {
                    case SDLK_ESCAPE: running = false; break;
                    case SDLK_Z: f+=100; break;
                    case SDLK_X: f = ((f-100)<200) ? f : f-100; break; // ensure fov doesn't go below 200
                }

            }
        }
        //----------------------\\

        SDL_SetRenderDrawColor(renderer, 0,0,0,255);
        SDL_RenderClear(renderer);

        //-----------Drawing-----------\\

        //ax += 1*Delta;
        ay += 1*Delta;
        //az += 1*Delta;

        SDL_SetRenderDrawColor(renderer, 255, 166, 0, 255);
        for(int i = 0; i < vertexCount; i++) points2D[i] = project(rotate3D(vertices[i], ax, ay, az), 4);
        for(int i = 0; i < edgeCount; i++) SDL_RenderLine(renderer, points2D[edges[i].a].x, points2D[edges[i].a].y, points2D[edges[i].b].x, points2D[edges[i].b].y);

        //----------------------\\

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // cap FPS to ~60
    }
    //----------------------\\

    printf("Vertex count: %d\n", vertexCount);
    printf("Edge count: %d\n", edgeCount);
    return 0;
}
