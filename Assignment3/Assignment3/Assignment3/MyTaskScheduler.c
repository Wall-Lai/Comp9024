

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>

// all the basic data structures and functions are included in this template
// you can add your own auxiliary functions as you like

// data type for heap nodes
typedef struct HeapNode {
    // each node stores the priority (key), name, execution time,
    //  release time and deadline of one task
    int key; //key of this item
    int TaskName;  // task name
    int Etime; //execution time
    int Rtime; // release time
    int Dline; // deadline
    struct HeapNode *parent; //pointer to parent
    struct HeapNode *left; //pointer to left child
    struct HeapNode *right; //pointer to right child
} HeapNode;

//data type for a priority queue (heap)
typedef struct Heap{ //this is heap header
    int  size;      // count of items in the heap
    HeapNode *LastNode; // last node pointer
    HeapNode *root; // pointer to the root of heap
} Heap;

// create a new heap node to store an item (task)
HeapNode *newHeapNode(int k, int n, int c, int r, int d, HeapNode *L, HeapNode *R, HeapNode *P)
{ // k:key, n:task name, c: execution time, r:release time, d:deadline
    // L, R, L: pointers to left child, right child and parent, respectively
    HeapNode *new;
    new = malloc(sizeof(HeapNode));
    assert(new != NULL);
    new->key = k;
    new->TaskName = n;
    new->Etime = c;
    new->Rtime = r;
    new->Dline = d;
    new->left = L; // left child
    new->right = R; // right child
    new->parent = P; // parent
    return new;
}

// create a new empty heap-based priority queue
Heap *newHeap()
{ // this function creates an empty heap-based priority queue
    Heap *T;
    T = malloc(sizeof(Heap));
    assert (T != NULL);
    T->size = 0;
    T->LastNode=NULL;
    T->root = NULL;
    return T;
}

//This structure is used to read the input from the file
typedef struct {
    int TaskName;
    int Etime;
    int Rtime;
    int Dline;
} values;

//This structure defines certain attributes of a core.
typedef struct {
    int name;
    int status;
    int start_time;
    int finish_time;
}Core;


//This function exchanges two nodes parameter
//Time Complexity = O(1)
void exchange(HeapNode *Node1, HeapNode *Node2, Heap *T){

    int t_key = Node2->key;
    int t_taskname = Node2->TaskName;
    int t_etime = Node2->Etime;
    int t_rtime = Node2->Rtime;
    int t_dline = Node2->Dline;
    
    Node2->key = Node1->key;
    Node2->TaskName = Node1->TaskName;
    Node2->Etime = Node1->Etime;
    Node2->Rtime = Node1->Rtime;
    Node2->Dline = Node1->Dline;

    Node1->key = t_key;
    Node1->TaskName = t_taskname;
    Node1->Etime = t_etime;
    Node1->Rtime = t_rtime;
    Node1->Dline = t_dline;
    
}

//Find the parent of new last node
HeapNode *FindLastNodeParent(Heap *T){
    HeapNode *current = T->LastNode;
    while(current->parent && current->parent->left != current)
    {
        current = current->parent;
    }
    current = current->parent;
    if (current->right == NULL)
    {
        return current;
    }
    current = current->right;
    while(current->left)
    {
        current = current->left;
    }
    return current;
}

//Find the position if the new last node, after removal
HeapNode *FindLastNode(Heap *T){
    HeapNode *current = T->LastNode;
    while(current->parent && current->parent->right != current)
    {
        current = current->parent;
    }
    current = current->parent->left; //Move over to the left child
    while(current->right)
    {
        current = current->right;
    }
    return current;
}
//Time complexity = O(log(n))
void Godown(HeapNode *node, Heap *T){
    if(node->left){ //If at least left child of node exists (if left child does not exist, naturally the right child will not exist)
        if(node->right && node->right->key < node->key && node->right->key < node->left->key)
        {
            exchange(node->right, node, T);
            Godown(node->right, T);
        }
        else if(node->left->key < node->key)
        {
            exchange(node->left, node, T);
            Godown(node->left, T);
        }
    }
}

//Time complexity = O(log(n))
void Goup(HeapNode *node, Heap *T){
    if (node->parent && node->parent->key > node->key)
    {
        exchange(node, node->parent, T);
        Goup(node->parent, T);
    }
}


//Check if size is of the form 2^n - 1
int size_2n_1(int size){
    size += 1;
    while(size % 2 == 0){
        size /= 2;
    }
    return size == 1 ? 1 : 0;
}


// This function inserts a node in a binary heap.
//Time complexity: log(n)
void Insert(Heap *T, int k, int n, int c, int r, int d)
{ // k: key, n: task name, c: execution time, r: release time, d:deadline
    // You don't need to check if this item already exists in T
    HeapNode *node = newHeapNode(k, n, c, r, d, NULL, NULL, NULL);
    if (!T->root)
    { //if no root exists
        T->root = node;
    }
    else if(!T->root->left)
    { //if only root exists
        T->root->left = node;
        T->root->left->parent = T->root;
    }
    else if(!T->root->right)
    { // if the there 2 elements, i.e. root's right child does not exist
        T->root->right = node;
        T->root->right->parent = T->root;
    }
    else
    {
        HeapNode *current;
        
        if(size_2n_1(T->size) == 1)
        { //If size = 1, 3, 7, 15, 31.. etc.
            current = T->root;
            while(current->left)
            {
                current = current ->left;
            }
            current->left = node;
            node->parent = current;
        }
        else
        {
            //Find position of new last node;
            HeapNode *NewLastNodeParent = FindLastNodeParent(T);
            if (!NewLastNodeParent->left)
            {
                NewLastNodeParent->left = node;
            }
            else
            {
                NewLastNodeParent->right = node;
            }
            node->parent = NewLastNodeParent;
        }
    }
    T->LastNode = node;
    Goup(T->LastNode, T); //Bubble up the newly inserted node
    T->size += 1;
}

// put your time complexity for RemoveMin() here
// This function the root of the binary heap, modifies the heap and returns the root. We travel up each level, move over to the left child and move to the rightmost child of that node.
// Time complexity = O(log(n))
HeapNode *RemoveMin(Heap *T){
    HeapNode *node;
    if(T->size == 1)
    { //If root exists
        node = T->root;
        T->root = NULL;
        T->LastNode = NULL;
    }
    else if(T->size == 2)
    { //It 2 elements are present in the tree
        node = T->root;
        T->root = T->root->left;
        T->root->parent = NULL;
    }
    else
    {
        HeapNode *current = T->root;
        exchange(T->LastNode, T->root, T);
        node = T->LastNode;
        if (size_2n_1(T->size - 1) == 1)
        { //If size after decrementing will be 1,3,7,15,.....,(2^n - 1)
            //Travel to right most child and make it the last node
            while (current->right)
            {
                current = current->right;
            }
            T->LastNode = current;
        }
        else
        {
            //Else find position of new last node
            T->LastNode = FindLastNode(T);
        }
        if (node->parent->left == node)
        {
            node->parent->left = NULL;
        }
        else
        {
            node->parent->right = NULL;
        }
        //Bubble down the root node to maintain heap order
        Godown(T->root, T);
    }
    T->size -= 1;
    node->parent = NULL;
    return node;
}

int MinHelp(Heap *T)
{
    if (T->size !=0)
        return T->root->Dline-T->root->Etime;
    else return -1;
}

int TaskScheduler(char *f1, char *f2, int m )
{
    int a,b,c,d;
    HeapNode *releasenode = NULL,*readynode = NULL;
    FILE *fp = fopen(f1,"r");
    if (fp==NULL)
    {
        printf("%s does not exist",f1);
        exit(1);
    }
    Heap *F;                      //read data from file1
    F = newHeap();
    while(!feof(fp))
    {
        fscanf(fp,"%d %d %d %d\n",&a,&b,&c,&d);
        Insert(F,c,a,b,c,d);      //insert item in heap F,with release time as the keys.
    }
    fclose(fp);
    
    FILE *fp2 = fopen(f2,"w");    //write data to file2
    int target=0,k=0,j;
    int core[m];                  //use array to record the usage of core
    
    for (j=0;j<m;j++){            //initialize the array
        core[j]=0;
    }
    Heap *R;
    R = newHeap();
    while(F->size!=0 || R->size!=0 )
    {
        while(F->size!=0 && F->root->key<=k)
        {     //check whether the tasks is ready
            releasenode = RemoveMin(F);        //insert the task into heap R,with deadline as keys
            Insert(R,releasenode->Dline,releasenode->TaskName,releasenode->Etime,releasenode->Rtime,releasenode->Dline);
        }
        for(int j=0;j<m;j++)
        {               //find suitable core for each task
            int lateststart = MinHelp(R);    //get the latest start time for earliest task in
            if (lateststart==-1) break;
            if(R->size!=0 && core[j]<=lateststart  )
            {    // check whether the core is suitable for the task
                readynode = RemoveMin(R);
                if( core[j]<readynode->Rtime) core[j]=readynode->Rtime;
                fprintf(fp2,"%d core%d %d ",readynode->TaskName,j,core[j]);
                core[j]+=readynode->Etime;
            }
            if (j==0) target =core[j];
            if (core[j]>target) target=core[j];
        }
        if(target>k) k=target;
        else k++;
        if(F->size==0 && k>releasenode->Dline)
        {
            fprintf(fp2, "\nWhere task %d misses its deadline", readynode->TaskName);
            return 0;
        }
    }
    return 1;
}

int main() //sample main for testing
{
    int i;
    i=TaskScheduler("samplefile1.txt", "feasibleschedule1.txt", 4);
    if (i==0) printf("ANo feasible schedule!\n");
    /* There is a feasible schedule on 4 cores */
    i=TaskScheduler("samplefile1.txt", "feasibleschedule2.txt", 3);
    if (i==0) printf("BNo feasible schedule!\n");
    /* There is no feasible schedule on 3 cores */
    i=TaskScheduler("samplefile2.txt", "feasibleschedule3.txt", 5);
    if (i==0) printf("CNo feasible schedule!\n");
    /* There is a feasible schedule on 5 cores */
    i=TaskScheduler("samplefile2.txt", "feasibleschedule4.txt", 4);
    if (i==0) printf("DNo feasible schedule!\n");
    /* There is no feasible schedule on 4 cores */
    i=TaskScheduler("samplefile3.txt", "feasibleschedule5.txt", 2);
    if (i==0) printf("ENo feasible schedule!\n");
    /* There is no feasible schedule on 2 cores */
    i=TaskScheduler("samplefile4.txt", "feasibleschedule6.txt", 2);
    if (i==0) printf("FNo feasible schedule!\n");
    /* There is a feasible schedule on 2 cores */
    return 0;
}
