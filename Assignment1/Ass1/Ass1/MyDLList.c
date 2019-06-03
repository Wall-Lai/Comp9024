//
//  main.m
//  Ass1
//
//  Created by z5189667 on 2019/3/14.
//  Copyright © 2019 Jay. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

// all the basic data structures and functions are included in this template
// you can add your own auxiliary functions as you like

// data structures representing DLList



// data type for nodes
typedef struct DLListNode {
    int  value;  // value (int) of this list item
    struct DLListNode *prev;
    // pointer previous node in list
    struct DLListNode *next;
    // pointer to next node in list
} DLListNode;

//data type for doubly linked lists
typedef struct DLList{
    int  size;      // count of items in list
    DLListNode *first; // first node in list
    DLListNode *last;  // last node in list
} DLList;

void PushNewNode(DLList *list, int it);

// create a new DLListNode
DLListNode *newDLListNode(int it)
{
    DLListNode *new;
    new = malloc(sizeof(DLListNode));
    assert(new != NULL);
    new->value = it;
    new->prev = new->next = NULL;
    return new;
}

// create a new empty DLList
DLList *newDLList()
{
    DLList *L;
    
    L = malloc(sizeof (struct DLList));
    assert (L != NULL);
    L->size = 0;
    L->first = NULL;
    L->last = NULL;
    return L;
}

// create a DLList from a text file
// time complexity analysis for CreateDLListFromFileDlist() is O(n).
DLList *CreateDLListFromFileDlist(const char *filename)
{
    
    DLList *List = newDLList();
    DLListNode *new;
    DLListNode *next;
    DLListNode *prev;
    next = List->first;
    prev = List->last;
    next = NULL;
    prev = NULL;
    
    int a = 0;
    FILE *fp;

    
    if (strcmp(filename,"stdin") != 0) {       //check the filename,list node from file
        fp = fopen(filename, "r");
        if(fp == 0) {
            perror("fopen");
            exit(1);
        }
        while(!feof(fp)){
            int res = fscanf(fp, "%d", &a);
            if(res == -1) {
                break;
            }
            new = newDLListNode(a);
            if (List->last == NULL) {           //initialize the first node of list
                List->first = List->last = new;
            }
            else{                               //initialize other nodes
                List->last->next = new;
                new->prev = List->last;
                List->last =new;
            }
            List->size++;                       //increase the size of new list
        }
        
        fclose(fp);
    }
    else{                               //list node from standard input
        char buff[50];
        while (fgets(buff, sizeof buff, stdin) != NULL && (buff[0] != '\n')){
            a = atoi(buff);
            if(List->size ==0){                       //initialize first node
                next = newDLListNode(a);
                List->first = List->last = next;
                List->size++;
            }
            else{
                while (next->next!=NULL){     //for each input number initialize other nodes and link it
                    next = next->next;
                }
                next->next = newDLListNode(a);
                prev = next;
                next = next->next;
                next->prev = prev;
                List->last = next;
            }
        }
        
        
    }
    
    return List;

}

// display items of a DLList
// Time complexity analysis for printDDList() is O(n).
void printDLList(DLList *u)
{
    if(u->size == 0) {                          //If list u is empty, print the prompt
         printf(" The Double Linked List you print is empty.\n");
    }
    else{                                       //Print from head to tail
        DLListNode *node = u->first;
        while(node->next !=NULL){
            printf("%d\n",node->value);
            node = node->next;
        }
        printf("%d\n",node->value);
    }
    printf("\n");
}

// clone a DLList
// time complexity analysis is O(n).
DLList *cloneList(struct DLList *u)
{
    DLList *NewL=newDLList();
    NewL->size = u->size;
    if (u == NULL){                                 //If list u is empty, return an empty list
        NewL = NULL;
    }
    else{
        DLListNode *node1;
        DLListNode *node2 = NULL;
        DLListNode *nodeu;
        int v;

        
        nodeu = u->first;
        if (nodeu != NULL) {                        //Clone the head node
            node2 = newDLListNode(nodeu->value);
            NewL->first = node2;
            node2->prev = NULL;
            nodeu = nodeu->next;
        }

        while (nodeu!=NULL){                         //Clone other nodes
            v = nodeu->value;
            node2->next = newDLListNode(v);
            node1 = nodeu;
            node2 = node2->next;
            node2->prev = node1;
            NewL->last = node2;
            nodeu = nodeu->next;
        }
    }
    return NewL;
}

// compute the union of two DLLists u and v
// time complexity analysis is O(u*v).
DLList *setUnion(struct DLList *u, struct DLList *v)
{
    if (u->size == 0 && v->size == 0) {             //check if any list u or v is empty
        return NULL;
    }
    if (u->size == 0) {
        return v;
    }
    if (v->size == 0) {
        return u;
    }
    
    DLList *NewL = cloneList(u);;                   //initialize new list to save the result
    DLListNode* node1 = NewL->first;
    DLListNode* node2 = v->first;
    DLListNode* temp;

    while (node2 != NULL) {
        while (node1->next != NULL) {
            if (node2->value == node1->value) {
                goto Have_found;                    //Check if the value in v is also in list u
            }
            node1 = node1->next;
        }
        temp = node1;
        node1->next = newDLListNode(node2->value);
        node1 = node1->next;
        node1->prev = temp;
        NewL->size ++;
    Have_found:
        node2 = node2->next;
        node1 = NewL->first;
    }
    
    return NewL;
}

// compute the insection of two DLLists u and v
// Time complexity analysis for intersection() is O(u*v).
DLList *setIntersection(struct DLList *u, struct DLList *v)
{
    if (u->size == 0 & v->size == 0) {          //check if any list u or v is empty
        return NULL;
    }
    if (u->size == 0) {
        return u;
    }
    if (v->size == 0) {
        return v;
    }
    
    DLList *NewL = newDLList();                 //initialize an empty list to save the result
    DLListNode *nodeL = NewL->first;
    DLListNode *nodeu = u->first;
    DLListNode *nodev = v->first;
    DLListNode *temp;
    int head = 0;
    
    while(nodev->next != NULL){
        while (nodeu->next != NULL) {
            if (nodev->value == nodeu->value) {
                while(!head){                   //This part only run once
                    nodeL = newDLListNode(nodev->value);
                    NewL->first = nodeL;
                    head = 1;
                    goto havehead;              //avoid add head node twice
                }
                temp = nodeL;
                nodeL->next = newDLListNode(nodev->value);
                nodeL = nodeL->next;
                nodeL->prev = temp;
                NewL->size++;
            }
        havehead:
            nodeu = nodeu->next;
        }
        nodev = nodev->next;
        nodeu = u->first;
    }
    return NewL;
}

// free up all space associated with list
// time complexity analysis is O(n).
void freeDLList(struct DLList *L)
{
    if (L->size == 0) {
        return;
    }
    assert(L != NULL);                      //assert L is not null
    DLListNode *curr, *prev;                //generate node current/previous
    curr = L->first;
    while (curr != NULL) {
        prev = curr;
        curr = curr->next;
        free(prev);                         //free node
    }
    free(L);                                //free list
}



void PushNewNode(DLList *list, int it)
{
    DLListNode *node;
    node = malloc(sizeof(DLListNode));
    assert(node != NULL);
    node->value = it;
    node->prev = list->last;
    if (list->last != NULL) {
        list->last->next = node;
    }
    list->last = node;
    if (list->size == 0) {
        list->first = node;
    }
    list->size++;
    
    
}




int main()
{
    
    DLList *list1, *list2, *list3, *list4;

    list1=CreateDLListFromFileDlist("File1.txt");
    printDLList(list1);
        list2=CreateDLListFromFileDlist("File2.txt");
    printDLList(list2);

    list3=setUnion(list1, list2);
    printDLList(list3);

    list4=setIntersection(list1, list2);
    printDLList(list4);

    freeDLList(list1);
    freeDLList(list2);
    freeDLList(list3);
    freeDLList(list4);

    printf("please type all the integers of list1\n");
    list1=CreateDLListFromFileDlist("stdin");

    printf("please type all the integers of list2\n");
    list2=CreateDLListFromFileDlist("stdin");

    list3=cloneList(list1);
    printDLList(list3);
    list4=cloneList(list2);
    printDLList(list4);

    freeDLList(list1);
    freeDLList(list2);
    freeDLList(list3);
    freeDLList(list4);
    
    return 0;
}
