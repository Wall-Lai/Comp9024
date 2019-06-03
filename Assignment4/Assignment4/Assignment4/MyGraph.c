#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

// A vertex is a 2D point
typedef struct Vertex {
    int x; // x-coordinate
	int y; // y-coordinate
} Vertex;
// each edge is a pair of vertices (end-points)
typedef struct Edge {
	Vertex *p1; // first end point
	Vertex *p2; // second end point
    int weight; //weight for edge used in shortest path
} Edge;

typedef struct VertexNode {
	Vertex *v;
    int state; //-1:unexplored, 0:explored, 1:visit
    struct Node_in_list *linked_node;
    struct VertexNode *next;
} VertexNode;

typedef struct GraphRep *Graph;
typedef struct GraphRep { // graph header
	VertexNode *vertices; // an array of vertices or a linked list of vertices  
	int nV; // #vertices
	int nE; // #edges
} GraphRep;

typedef struct Node_in_list{
    Vertex *v;
    struct Node_in_list *next_vertex;
    struct VertexNode *coordinate;
}Node_in_List;

typedef struct Node_in_Queue{
    Vertex *v;
    struct Node_in_Queue *next;
}Node_in_Queue;

typedef struct QRep{
    int length;
    Node_in_Queue *head;
    Node_in_Queue *tail;
}QRep;

typedef struct QRep *Queue;

// A vertex node stores a vertex and other information, and you need to expand this type

//The above types serve as a starting point only. You need to expand them and add more types. 
// Watch the lecture video between 7:50pm-8:20 or so for my discussion about this assignment    

// the time complexity analysis is O(1)
Graph CreateEmptyGraph()
{
    Graph g = malloc(sizeof(GraphRep));
    assert(g != NULL);
    g->nE = 0;
    g->nV = 0;
    g->vertices = NULL;
    return g;
}


Queue CreateEmptyQueue()
{
    Queue Q = malloc(sizeof(QRep));
    assert(Q != NULL);
    Q->head = NULL;
    Q->tail = NULL;
    Q->length = 0;
    return Q;
}

VertexNode *CreateNewVNode(Vertex *value)
{
    VertexNode *new = malloc(sizeof(VertexNode));
    assert(new != NULL);
    new->v = value;
    new->linked_node = NULL;
    new->next = NULL;
    new->state = -1;
    return new;
}

Node_in_List *CreatNewNodeInList(Vertex *value)
{
    Node_in_List *new = malloc(sizeof(Node_in_List));
    assert(new != NULL);
    new->coordinate = NULL;
    new->next_vertex = NULL;
    new->v = value;
    return new;
}

void InsertQueue(Queue Q, Vertex *value)
{
    Node_in_Queue *new = malloc(sizeof(Node_in_Queue));
    assert(new != NULL);
    new->v = value;
    new->next = NULL;
    if (Q->tail != NULL) {
        Q->tail->next = new;
        Q->tail = new;
    }
    else{
        Q->head = new;
        Q->tail = new;
    }
//    printf("Qhead=%d,%d,Qtail=%d,%d",Q->head->v->x,Q->head->v->y,Q->tail->v->x,Q->tail->v->y);
    Q->length++;
}

Vertex *DeleteQueue(Queue Q)
{
    assert(Q->length > 0);
    Node_in_Queue *temp = Q->head;
    Q->head = Q->head->next;
    if (Q->head == NULL) {
        Q->tail = NULL;
    }
    Q->length--;
    Vertex *value = temp->v;
    free(temp);
    return value;
}

int inLL(Node_in_List *L, Vertex *n)
{
    if (L == NULL) {
        return 0;
    }
    else if (L->v->x == n->x && L->v->y == n->y){
        return 1;
    }
    return inLL(L->next_vertex, n);
}

Node_in_List *insertLL (Node_in_List *N, Vertex *v)
{
    if (inLL(N, v) == 1) {
        return N;
    }
    Node_in_List *new = CreatNewNodeInList(v);
    new->next_vertex = N;
    return new;
}

Node_in_List *deleteLL(Node_in_List *N, Vertex *v)
{
    if (N == NULL) {
        return NULL;
    }
    if (N->v->x == v->x && N->v->y == v->y) {
        N->coordinate = NULL;
        return N->next_vertex;
    }
    N->next_vertex = deleteLL(N->next_vertex, v);
    return N;
}

VertexNode *search(VertexNode *n, Vertex *v)
{
    while (n != NULL) {
        if (n->v->x == v->x && n->v->y == v->y) {
            return n;
        }
        n = n->next;
    }
    return NULL;
}
// the time complexity analysis is O(m+n)
int InsertEdge(Graph g, Edge *e)
{
    Node_in_List *temp1, *temp2;
    VertexNode *node1, *node2;
    node1 = search(g->vertices, e->p1);
    node2 = search(g->vertices, e->p2);
    if (node1 != NULL) {
        if (inLL(node1->linked_node, e->p2)) {
            return 0;
        }
        else{
            temp1 = insertLL(node1->linked_node, e->p2);
        }
        if (node2 == NULL) {
            node2 = CreateNewVNode(e->p2);
            node2->next = g->vertices; //node2 is the first of g's vertices
            g->vertices = node2;
            g->nV++;
        }
        temp1->coordinate = node2;
        node1->linked_node = temp1;
        temp2 = insertLL(node2->linked_node, e->p1);
        temp2->coordinate = node1;
        node2->linked_node = temp2;
    }
    else{
        if (node2 == NULL) {
            node2 = CreateNewVNode(e->p2);
            node2->next = g->vertices; //node2 is the first of g's vertices
            g->vertices = node2;
            g->nV++;
        }
        else{
            if (inLL(node2->linked_node, e->p1)) {
                return 0;
            }
        }
        node1 = CreateNewVNode(e->p1);
        node1->next = g->vertices;
        g->vertices = node1;
        g->nV++;
        temp2 = insertLL(node2->linked_node, e->p1);
        temp2->coordinate = node1;
        node2->linked_node = temp2;
        temp1 = insertLL(node1->linked_node, e->p2);
        temp1->coordinate = node2;
        node1->linked_node = temp1;
    }
    e->weight = sqrt(pow((e->p1->x - e->p2->x),2) + pow((e->p1->y - e->p2->y), 2));
    g->nE++;
    return 1;
}

// the time complexity analysis is O(m+n)
void DeleteEdge(Graph g, Edge *e)
{
    VertexNode *node1, *node2;
    node1 = search(g->vertices, e->p1);
    node2 = search(g->vertices, e->p2);
    if (node1 == NULL || node2 == NULL) {
        return;
    }
    if (inLL(node1->linked_node, e->p2) == 1 && inLL(node2->linked_node, e->p1)) {
        node1->linked_node = deleteLL(node1->linked_node, e->p2);
        node2->linked_node = deleteLL(node2->linked_node, e->p1);
        g->nE--;
    }
}
//the time complexity analysis is O(n)
void ShowQueue(Queue Q){
    Node_in_Queue *head, *first, *t, *p = NULL, *q;
    first = Q->head->next->next;
    head= Q->head->next;
    head->next = NULL;
    while(first!=NULL){
        for(t=first,q=head;(q!=NULL)&&(t->v->x>q->v->x||(t->v->x==q->v->x && t->v->y >= q->v->y));p=q,q=q->next);
        first = first->next;
        if(q==head) head=t;
        else p->next = t;
        t->next =q;
    }
    while(head->next!=NULL){
        printf("(%d,%d),",head->v->x,head->v->y);
        head=head->next;
    }
    printf("(%d,%d)",head->v->x,head->v->y);
}
// the time complexity analysis is O(mn)
void ReachableVertices(Graph g, Vertex *v)
{
    assert(g != NULL);
    VertexNode *temp1, *temp2, *new1, *new2;
    Vertex *pop;
    Node_in_List *p;
    Node_in_Queue * current = NULL;
    Queue q = CreateEmptyQueue();
    temp1 = search(g->vertices, v);
//    if (temp1 == NULL) {
//        return;
//    }
    temp1->state = 0;
    InsertQueue(q, temp1->v);
    if (q->length == 1) {
        current = q->head;
    }
    while (current != NULL) {
        pop = current->v;
        new1 = search(g->vertices, pop);
        p = new1->linked_node;
        while (p != NULL) {
            new2 = p->coordinate;
            if (new2->state == -1) {
                new2->state = 0;
                InsertQueue(q, new2->v);
            }
            p = p->next_vertex;
        }
        current = current->next;
    }
    printf("\nreachable vertices:\n");
    ShowQueue(q);
    
    temp2 = g->vertices;
    while (temp2 != NULL) {
        temp2->state = -1;
        temp2 = temp2->next;
    }
    
}

// the time complexity analysis is O(mn)
void ShortestPath(Graph g, Vertex *u, Vertex *v)
{
    assert(g != NULL);
    VertexNode *temp1, *temp2, *new1, *new2, *temp3;
    Vertex *pop;
    Node_in_List *p;
    Node_in_Queue * current = NULL;
    Queue q = CreateEmptyQueue();
    temp1 = search(g->vertices, u);
    temp2 = search(g->vertices, v);
    if (temp1 == NULL || temp2 == NULL) {
        return;
    }
    temp1->state = 0;
    InsertQueue(q, temp1->v);
    if (q->length == 1) {
        current = q->head;
    }
    while (current != NULL) {
        pop = current->v;
        new1 = search(g->vertices, pop);
        p = new1->linked_node;
        while (p != NULL) {
            new2 = p->coordinate;
            if (new2->state == -1) {
                new2->state = 0;
                InsertQueue(q, new2->v);
            }
            p = p->next_vertex;
        }
        if (current->v->x == temp2->v->x && current->v->y == temp2->v->y) {
            printf("The shortest path between (%d,%d) and (%d,%d) is:\n", u->x, u->y, v->x, v->y);
            printf("(%d,%d),", u->x, u->y);
            ShowQueue(q);
            printf("\n");
            temp3 = g->vertices;
            while (temp3 != NULL) {
                temp3->state = -1;
                temp3 = temp3->next;
            }
            return;
        }
        else{
            current = current->next;
        }
    }
    temp3 = g->vertices;
    while (temp3 != NULL) {
        temp3->state = -1;
        temp3 = temp3->next;
    }
}

void FreeLL(Node_in_List *N)
{
    if (N != NULL) {
        FreeLL(N->next_vertex);
        free(N);
    }
}
// the time complexity analysis is O(m+n)
void FreeGraph(Graph g)
{
    assert(g != NULL);
    VertexNode *node;
    node = g->vertices;
    while (node != NULL) {
        FreeLL(node->linked_node);
        node = node->next;
    }
    free(g);
}

// the time complexity analysis is O(mn)
void ShowGraph(Graph g)
{
    assert(g != NULL);
    Vertex *pop;
    Node_in_List *p;
    Queue q = CreateEmptyQueue();
    VertexNode *temp1, *temp2, *new1, *new2;
    temp1 = g->vertices;
    printf("Show graph:\n");
    while (temp1 != NULL) {
        if (temp1->state == -1) {
            temp1->state = 0;
            InsertQueue(q, temp1->v);
            while (q->length != 0) {
                pop = DeleteQueue(q);
                new1 = search(g->vertices, pop);
                new1->state = 1;
                p = new1->linked_node;
                while (p != NULL) {
                    new2 = p->coordinate;
                    if (new2->state == -1) {
                        new2->state = 0;
                        printf("(%d,%d),(%d,%d) ", new1->v->x,new1->v->y,new2->v->x,new2->v->y);
                        InsertQueue(q, new2->v);
                    }
                    else if(new2->state == 0){
                        printf("(%d,%d),(%d,%d) ", new1->v->x,new1->v->y,new2->v->x,new2->v->y);
                    }
                    p = p->next_vertex;
                }
            }
        }
        temp1 = temp1->next;
    }
    temp2 = g->vertices;
    while (temp2 != NULL) {
        temp2->state = -1;
        temp2 = temp2->next;
    }
    printf("\n");
}

int main() //sample main for testing 
{ 
 Graph g1;
 Edge *e_ptr; 
 Vertex *v1, *v2;
  
 // Create an empty graph g1;
 g1=CreateEmptyGraph();
  
 // Create first connected component 
 // Insert edge (0,0)-(0,10)
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=0;
 v1->y=0;
 v2->x=0;
 v2->y=10;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
 
 // Insert edge (0,0)-(5,6)
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=0;
 v1->y=0;
 v2->x=5;
 v2->y=6;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
 
 // Insert edge (0, 10)-(10, 10)
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=0;
 v1->y=10;
 v2->x=10;
 v2->y=10;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
 
 // Insert edge (0,10)-(5,6)
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=0;
 v1->y=10;
 v2->x=5;
 v2->y=6;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
 
 // Insert edge (0,0)-(5,4)
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=0;
 v1->y=0;
 v2->x=5;
 v2->y=4;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
 
 // Insert edge (5, 4)-(10, 4)
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=5;
 v1->y=4;
 v2->x=10;
 v2->y=4;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
 
 // Insert edge (5,6)-(10,6)
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=5;
 v1->y=6;
 v2->x=10;
 v2->y=6;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
 
 // Insert edge (10,10)-(10,6)
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=10;
 v1->y=10;
 v2->x=10;
 v2->y=6;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
 
 // Insert edge (10, 6)-(10, 4)
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=10;
 v1->y=6;
 v2->x=10;
 v2->y=4;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
 
 // Create second connected component
 // Insert edge (20,4)-(20,10)
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=20;
 v1->y=4;
 v2->x=20;
 v2->y=10;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
 
 // Insert edge (20,10)-(30,10)
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=20;
 v1->y=10;
 v2->x=30;
 v2->y=10;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
 
 // Insert edge (25,5)-(30,10) 	
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=25;
 v1->y=5;
 v2->x=30;
 v2->y=10;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n"); 
 
 //Display graph g1
 ShowGraph(g1);
//
 // Find the shortest path between (0,0) and (10,6)
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=0;
 v1->y=0;
 v2->x=10;
 v2->y=6;
 ShortestPath(g1, v1, v2);
 free(v1);
 free(v2);

//  Delete edge (0,0)-(5, 6)
 e_ptr = (Edge*) malloc(sizeof(Edge));
 assert(e_ptr != NULL);
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=0;
 v1->y=0;
 v2->x=5;
 v2->y=6;
 e_ptr->p1=v1;
 e_ptr->p2=v2;
 DeleteEdge(g1, e_ptr);
 free(e_ptr);
 free(v1);
 free(v2);

 // Display graph g1
 ShowGraph(g1);

 // Find the shortest path between (0,0) and (10,6)
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=0;
 v1->y=0;
 v2->x=10;
 v2->y=6;
 ShortestPath(g1, v1, v2);
 free(v1);
 free(v2);

 // Find the shortest path between (0,0) and (25,5)
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v2=(Vertex *) malloc(sizeof(Vertex));
 assert(v2 != NULL);
 v1->x=0;
 v1->y=0;
 v2->x=25;
 v2->y=5;
 ShortestPath(g1, v1, v2);
 free(v1);
 free(v2);

//  Find reachable vertices of (0,0)
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v1->x=0;
 v1->y=0;
 ReachableVertices(g1, v1);
 free(v1);
//
 // Find reachable vertices of (20,4)
 v1=(Vertex*) malloc(sizeof(Vertex));
 assert(v1 != NULL);
 v1->x=20;
 v1->y=4;
 ReachableVertices(g1, v1);
 free(v1);

 // Free graph g1
 FreeGraph(g1);

 return 0;
}
