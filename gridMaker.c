#include <stdio.h>
#include <stdlib.h>
#include "obstacles.h"
#include "display.h"
#include <math.h>

//function prototypes
unsigned char obstacleIntersect(Vertex *v1, Vertex *v2, Obstacle *obstacle);
void addNeighbour(Vertex *node,Vertex *find,Environment *env);
unsigned char checkinside(Vertex *v, Obstacle *obstacle);
void neiremove(Vertex *remnb);



// This is a helper function that determines whether or not line segment (v1---v2) intersects line segment (v3---v4)
unsigned char linesIntersect(short v1x, short v1y, short v2x, short v2y, short v3x, short v3y, short v4x, short v4y) {
  	float uA = ((v4x-v3x)*(v1y-v3y) - (v4y-v3y)*(v1x-v3x)) / (float)(((v4y-v3y)*(v2x-v1x) - (v4x-v3x)*(v2y-v1y)));
  	float uB = ((v2x-v1x)*(v1y-v3y) - (v2y-v1y)*(v1x-v3x)) / (float)(((v4y-v3y)*(v2x-v1x) - (v4x-v3x)*(v2y-v1y)));

  	// If uA and uB are between 0-1, there is an intersection
  	if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1) 
		return 1;

	return 0;
	}
//function to check if the node inside obstacle;	
unsigned char checkinside(Vertex *v, Obstacle *obstacle) {
    short p1 = obstacle->x; 
    short p2 = p1 + obstacle->w;
    short p3 = obstacle->y - obstacle->h ;
    short p4 = obstacle->y;
    
    unsigned char result = v->x >= p1 && v->x <= p2 && v->y >= p3 && v->y <= p4;
     
    return result;
}

unsigned char obstacleIntersect(Vertex *v1, Vertex *v2, Obstacle *obstacle) {
  //r1-r4 is 4 edge of the obstacle;
	unsigned char r1 =linesIntersect(v1->x, v1->y, v2->x, v2->y, obstacle->x,  obstacle->y, (obstacle->x+obstacle->w), obstacle->y);
	unsigned char r2 = linesIntersect(v1->x, v1->y, v2->x, v2->y, (obstacle->x+obstacle->w),  obstacle->y, (obstacle->x+obstacle->w),( obstacle->y- obstacle->h));
	unsigned char r3=linesIntersect(v1->x, v1->y, v2->x, v2->y, (obstacle->x+obstacle->w), (obstacle->y- obstacle->h), obstacle->x, ( obstacle->y- obstacle->h));
	unsigned char r4=linesIntersect(v1->x, v1->y, v2->x, v2->y, obstacle->x, ( obstacle->y- obstacle->h),obstacle->x, obstacle->y);
    
    //  line intersection of the 4 edge of the obstacle and the line
    return r1|| r2|| r3|| r4;
}
// Create a grid of vertices with a horizontal and vertical gap between them of env->gridSize pixels
void createGrid(Environment *env) {
	int apart = env->gridSize;
	//firstvertex is a pointer
	//malloc first vertex at the left down button
	Vertex **tail = NULL;
	 tail = &env->firstVertex;
		
	for(int y = 0; y < ENV_HEIGHT; y += apart){
		for(int x = 0 ; x < ENV_WIDTH; x += apart){
			Vertex *newVertex = (Vertex *)malloc(sizeof(Vertex));
			if(newVertex==NULL){
				printf("Error malloc");
				exit(0);
			}
			newVertex->x = x ;
			newVertex->y = y ;
			newVertex->firstNeighbour = NULL;
			newVertex->lastNeighbour = NULL;
			newVertex->nextVertex = NULL;
			*tail = newVertex;
			tail = &(*tail)->nextVertex;
			
		}
	}
	
	
//find valid neighbour
	Vertex *node = NULL ;
	Vertex *find = NULL;
	node = env->firstVertex;
	while(node!=NULL){
	find = env->firstVertex;
        //thats possible node
	while(find!=NULL){
        float distance = sqrt(pow(abs(((find->x)-(node->x))),2) + pow(abs(((find->y)-(node->y))),2));
	   if(distance==apart){
                //before adding, check obstcle;
                //printf("found (%d,%d) neighbor (%d,%d)\n",node->x,node->y,find->x,find->y);
            addNeighbour(node,find,env);
    	  }
		  //find next
         find = find->nextVertex;
	}
	//next node
    	node = node->nextVertex;
    }
    	
}
void addNeighbour(Vertex *node,Vertex *find,Environment *env){
//check intersection
	for (int i = 0; i < env->numObstacles; i++) {
        	if (obstacleIntersect(node, find, &env->obstacles[i])) { 
            	return;	
        }
	}		
//adding neighbour
	Neighbour *neighbour = (Neighbour *) malloc(sizeof(Neighbour));
	if(neighbour==NULL){
				printf("Error malloc");
				exit(0);
			}
				neighbour->vertex = NULL;
                neighbour->next=NULL;
				neighbour->vertex = find;

                if(node->firstNeighbour==NULL){
                    node->firstNeighbour = neighbour; 
                    node->lastNeighbour = neighbour;
                }
                else{
                    //mew tail inserted
                    node->lastNeighbour = neighbour;
                    Neighbour *current = node->firstNeighbour;
                    while(current->next!=NULL){
                    	current = current->next;
                    }
                    current->next = neighbour;
                }         
  }
// remove and free a vertex thats not valid;
void neiremove(Vertex *remnb){
    Neighbour *neighbour =NULL;
    Neighbour *tmp=NULL;
    
    Neighbour *prvnode = NULL; 
    Neighbour *currnode =NULL;
    Neighbour *tmp2=NULL;
    
    for (neighbour = remnb->firstNeighbour; neighbour != NULL; tmp = neighbour, neighbour = neighbour->next,free(tmp)) {

        for (currnode = neighbour->vertex->firstNeighbour; currnode != NULL; prvnode = currnode, currnode = currnode->next) {
        //node in neighbour structure
        //Find the node which we want to remove;
        	if (currnode->vertex == remnb) {
        	  tmp2=currnode;
            // update pointer
            	if (neighbour->vertex->firstNeighbour == currnode) {
                    neighbour->vertex->firstNeighbour = currnode->next;
            	}
            	if (neighbour->vertex->lastNeighbour == currnode) {
                    neighbour->vertex->lastNeighbour = prvnode;
                 
            	}
            	if (prvnode != NULL) {
                    prvnode->next = currnode->next;
            	}
            	
                free(tmp2);
                break;
       		}
   	    }
   	    
    }
       free(remnb);
  
}
//remove invalid
void removeInvalidVertices(Environment *env) {
    Vertex *prevnode = NULL;
    Vertex *tmp = NULL;
    int  found;
    Vertex *currentnode = env->firstVertex;
    
    while(currentnode != NULL) {
        found = 0;
        for (int i = 0; i < env->numObstacles; i++) {
            if (checkinside(currentnode, &env->obstacles[i])) {
            //found what be remove;
			 	tmp = currentnode; // store which should be remove
                if (env->firstVertex == currentnode) {
                    env->firstVertex = currentnode->nextVertex; 
                } 
                //release neighbour first
                if (prevnode != NULL) {
                   prevnode->nextVertex = currentnode->nextVertex;
                   
                }
                currentnode=currentnode->nextVertex; // go to next node
               	neiremove(tmp);//freeing
                found=1;
                break;
            }
           
        }
        if(!found){
         prevnode = currentnode;
         currentnode = currentnode->nextVertex;
         }   
       
    }
}

// Add diagonal edges to the graph, assuming that they don't intersect an obstacle
void addCrossEdges(Environment *env) {
	int apart = env->gridSize;
 	Vertex *node2 = env->firstVertex;
	Vertex *find2 = NULL;

	while(node2!=NULL){
		Vertex *find2 = env->firstVertex;
		 
    //thats possible node
	while(find2!=NULL){
        float distance2 = (pow(abs(((find2->x)-(node2->x))),2) + pow(abs(((find2->y)-(node2->y))),2));//diagonal distance
              
	   if(distance2==(pow(apart,2)+pow(apart,2))){
                
            //printf("found (%d,%d) neighbor (%d,%d)\n",node->x,node->y,find->x,find->y);
            addNeighbour(node2,find2,env);
    	}
        find2 = find2->nextVertex;
	}
    	node2 = node2->nextVertex;
    	}
}


// This procedure cleans up everything by creeing all alocated memory
void cleanupEverything(Environment *env) {
	Vertex *v = env->firstVertex;
	
	 //freeing
    Vertex *tmp3=NULL;
    Neighbour *currnode =NULL;
    Neighbour *tmp4=NULL;
    while(v!=NULL){
    	tmp3=v;
    	
    	currnode = v->firstNeighbour;
    	while(currnode!= NULL){
    		tmp4=currnode;
    		currnode=currnode->next;
    		free(tmp4);
    	}
    	v= v->nextVertex;
    	
    	free(tmp3);
    }
  	free(env->obstacles);
}
