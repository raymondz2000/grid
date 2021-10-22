
// This represents a rectangular obstacle
typedef struct obst {
	short   	 x;
	short   	 y; 
	short   	 w;
	short   	 h;
} Obstacle;


// This represents a vertex of the graph
typedef struct vert {
    short          x;
    short          y;
    struct neigh  *firstNeighbour;
    struct neigh  *lastNeighbour;
	struct vert   *nextVertex;
} Vertex;


// This represents a neighbour of a vertex in the graph
typedef struct neigh {
	Vertex        *vertex;
	struct neigh  *next;
} Neighbour;


// This represents an environment
typedef struct {
	Obstacle   *obstacles;
	short       numObstacles;
	Vertex     *firstVertex;
	int			gridSize;
} Environment;
	

// These are the external functions being used by the test program
extern void createGrid(Environment *);
extern void removeInvalidVertices(Environment *);
extern void addCrossEdges(Environment *);
extern void cleanupEverything(Environment *);

