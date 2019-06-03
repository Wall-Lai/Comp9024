#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
// all the basic data structures and functions are included in this template
// you can add your own auxiliary functions as you like
int max(int a,int b)
{
    if (a >= b){
        return a;
    }
    else{
        return b;
    }
}


// data type for avl tree nodes
typedef struct AVLTreeNode {
    int key; //key of this item
    int value;  //value (int) of this item
    int height; //height of the subtree rooted at this node
    struct AVLTreeNode *parent; //pointer to parent
    struct AVLTreeNode *left; //pointer to left child
    struct AVLTreeNode *right; //pointer to right child
} AVLTreeNode;

//data type for AVL trees
typedef struct AVLTree{
    int  size;      // count of items in avl tree
    AVLTreeNode *root; // root
} AVLTree;
int get_height(AVLTreeNode *node)
{
    if (node == NULL) {
        return -1;
    }
    else{
        return node->height;
    }
}
int balance(AVLTreeNode *node){
    if (!node) {
        return 0;
    }
    int ht = get_height(node->left) - get_height(node->right);
    return ht;
}
// create a new AVLTreeNode
AVLTreeNode *newAVLTreeNode(int k, int v )
{
    AVLTreeNode *new;
    new = malloc(sizeof(AVLTreeNode));
    assert(new != NULL);
    new->key = k;
    new->value = v;
    new->height = 0; // height of this new node is set to 0
    new->left = NULL; // this node has no child
    new->right = NULL;
    new->parent = NULL; // no parent
    return new;
}

// create a new empty avl tree
AVLTree *newAVLTree()
{
    AVLTree *T;
    T = malloc(sizeof (AVLTree));
    assert (T != NULL);
    T->size = 0;
    T->root = NULL;
    return T;
}

int InsertNode(AVLTree *T, int k, int v);
AVLTreeNode *Search(AVLTree *T, int k, int v);

// InsertNode is O(logN),so this time complexity is O(NlogN).
AVLTree *CreateAVLTree(const char *filename)
{
    AVLTree *T;
    T = newAVLTree();
    int sign, x ,y;
    int k = 0, v;
    int c = 0;

    FILE *fp;
    fp = fopen(filename, "r");
    
    if (strcmp(filename,"stdin") != 0) {
        if(fp == 0) {
            perror("fopen");
            exit(1);
        }
        while(!feof(fp)){
            fscanf(fp, "(%d,%d)\n", &x, &y);
            int a = x, b = y;
            sign = InsertNode(T, x, y);
            if (sign == 0){
                printf("The node (%d,%d) has already exist\n",a, b);
            }
        }
    }
    else{
        printf("please input the nodes:\n");
        char out[1024] = {'\0'};
        char buff[256];
        int i = 0;
        while (fgets(buff, sizeof buff, stdin) != NULL && buff[0] != '\n'){
            while(buff[i] != '\0'){
                if (isdigit(buff[i]) != 0 || buff[i] == '-'){
                    strcat(out, &buff[i]);
                } else if (buff[i] == ')' || buff[i] == ','){
                    c++;
                    if (c == 1){
                        k = atoi(out);
                    } else {
                        v = atoi(out);
                        InsertNode(T, k, v);
                        c = 0;
                    }
                    out[0] = '\0';
                }
                i += 1;
            }
            i = 0;
            fflush(stdout);
        }
        fclose(fp);
    }
    return T;
}

void TreeToList(AVLTreeNode *node,int key[],int value[],int *index)
{
    if(node == NULL) return;
    TreeToList(node->left,key,value,index);
    key[*index]= node->key;
    value[*index] = node->value;
    (*index)++;
    TreeToList(node->right,key,value,index);
}
//time complexity is O(n).
AVLTreeNode *CloneNode(AVLTreeNode *root)
{
    if (root == NULL) {
        return NULL;
    }
    AVLTreeNode *newNode = newAVLTreeNode(root->key, root->value);
    newNode->height = root->height;
    newNode->right = CloneNode(root->right);
    if (newNode->right) {
        newNode->right->parent = newNode;
    }
    newNode->left =CloneNode(root->left);
    if (newNode->left) {
        newNode->left->parent = newNode;
    }
    return newNode;
}
// time complexity analysis is O(N).
AVLTree *CloneAVLTree(AVLTree *T)
{
    AVLTree *T2;
    T2 = newAVLTree();
    if(T==NULL) return T2;
    AVLTreeNode *root = CloneNode(T->root);
    T2->root = root;
    T2->size = T->size;
    return T2;
}
//AVLTree to array
int Inorder_Traversal(AVLTreeNode *node, int Key_array[], int Value_array[], int i){
    if (node->left){
        i = Inorder_Traversal(node->left, Key_array, Value_array, i);
    }
    Key_array[i] = node->key;
    Value_array[i] = node->value;
    i++;
    if (node->right){
        i = Inorder_Traversal(node->right, Key_array, Value_array, i);
    }
    return i;
}

AVLTreeNode *ArrayToAVLTree(int key_array[], int value_array[], int begin, int final)
{
    if (begin > final){
        return NULL;
    }
    
    int m = (begin + final)/2;
    AVLTreeNode *node = newAVLTreeNode(key_array[m], value_array[m]);
    
    node->left =  ArrayToAVLTree(key_array, value_array, begin, m-1);
    if (node->left){
        node->left->parent = node;
    }
    node->right = ArrayToAVLTree(key_array, value_array, m+1, final);
    if (node->right){
        node->right->parent = node;
    }
    
    node->height = max(get_height(node->left), get_height(node->right))+1;
    return node;
}

//time complexity for ALVTreesUNion() is O((m+n)log(m+n)).
AVLTree *AVLTreesIntersection(AVLTree *T1, AVLTree *T2)
{
    AVLTree *T3;
    if (T1->size == 0 || T2->size == 0){
        T3 = newAVLTree();
    }else {
        T3 = newAVLTree();
        int a = 0, b = 0, c = 0;
        
        int size_1 = T1->size, K1[size_1], V1[size_1]; //T1 array
        int size_2 = T2->size, K2[size_2], V2[size_2]; //T2 array
        int size_3 = T1->size + T2->size, K3[size_3], V3[size_3]; //Array for new tree
        //Store elements in arrays by Inorder Traversal
        Inorder_Traversal(T1->root, K1, V1, 0);
        Inorder_Traversal(T2->root, K2, V2, 0);
        
        //merging two sorted arrays of trees T1 and T2 into a new array T3
        while(a < size_1 && b < size_2){
            if (K1[a] > K2[b]){
                b += 1;
            }
            else if(K1[a] < K2[b]){
                a += 1;
            }
            else{
                if (V1[a]>V2[b]){
                    b +=1;
                }else if(V1[a]<V2[b]){
                    a +=1;
                }else{
                    K3[c] = K1[a];
                    V3[c] = V1[a];
                    a +=1;
                    b +=1;
                    c += 1;
                }
            }
        }
        //Create AVL Tree from array
        T3->root = ArrayToAVLTree(K3, V3, 0, c - 1);
    }
    return T3;
}

// time complexity for ALVTreesIntersection() is O(m+n+k log k).
AVLTree *AVLTreesUnion(AVLTree *T1, AVLTree *T2)
{
    AVLTree *T3;
    if (T1->size == 0 || T2->size == 0){
        T3 = newAVLTree();
    } else if(T2->size == 0){
        T3 = CloneAVLTree(T1);
    } else if (T1->size == 0){
        T3 = CloneAVLTree(T2);
    } else {
        T3 = newAVLTree();
        int a = 0, b = 0, c = 0, i;
        
        int size_1 = T1->size, K1[size_1], V1[size_1]; //T1 array
        int size_2 = T2->size, K2[size_2], V2[size_2]; //T2 array
        int size_3 = T1->size + T2->size, K3[size_3], V3[size_3]; //Array for new tree
        
        //Store elements in arrays by Inorder Traversal
        Inorder_Traversal(T2->root, K1, V1, 0);
        Inorder_Traversal(T1->root, K2, V2, 0);
        
        //merging two sorted arrays of trees T1 and T2 into a new array T3
        while(a < size_1 && b < size_2){
            if (K1[a] > K2[b]){
                K3[c] = K2[b];
                V3[c] = V2[b];
                b += 1;
                c += 1;
            } else if(K1[a] == K2[b]){
                if (V1[a]>V2[b]){
                    K3[c] = K2[b];
                    V3[c] = V2[b];
                    b +=1;
                    c +=1;
                }else if(V1[a]<V2[b]){
                    K3[c] = K1[a];
                    V3[c] = V1[a];
                    a +=1;
                    c +=1;
                    
                }else{
                    K3[c] = K1[a];
                    V3[c] = V1[a];
                    a +=1;
                    b +=1;
                    c +=1;
                }
            }
            else if(K1[a] < K2[b]){
                K3[c] = K1[a];
                V3[c] = V1[a];
                a += 1;
                c += 1;
            }
            
        }
        // If elements of 'a' still remain
        if(a < size_1){
            for(i = a; i <size_1; i += 1){
                K3[c] = K1[a];
                V3[c] = V1[a];
                c += 1;
            }
        }
        
        // If elements of 'b' still remain
        if(b < size_2){
            for(i = b; i <size_2; i += 1){
                K3[c] = K2[b];
                V3[c] = V2[b];
                c += 1;
            }
        }
        //Create AVL Tree from array
        T3->root = ArrayToAVLTree(K3, V3, 0, c-1);
    }
    return T3;
}
//Check if the node is parent's left child or right child.
int Linkparent(AVLTreeNode *parent, AVLTreeNode *node)
{
    if (parent->left && node->key == parent->left->key) {
        return 0;
    }
    else{
        return 1;
    }
}
//define single  rotation
AVLTreeNode *RRotate(AVLTree *T, AVLTreeNode *T1)
{
    assert(T1!=NULL);
    AVLTreeNode *T2;
    T2 = T1->left;
    AVLTreeNode *T3;
    T3 = T1->left->right;
    
    T2->right =T1;
    if (T1->parent == NULL) {
        T2->parent = NULL;
        T->root = T2;
    }
    else{
        T2->parent = T1->parent;
        int direct = Linkparent( T1->parent, T1);
        if (direct == 0) {
            T2->parent->left = T2;
        }
        else{
            T2->parent->right = T2;
        }
    }
    T1->parent = T2;
    T1->left = T3;
    if (T3 != NULL) {
        T3->parent = T1;
    }
    T1->height = max(get_height(T1->left),get_height(T1->right))+1;
    T2->height = max(get_height(T2->left),T1->height)+1;
    
    return T2;
}

AVLTreeNode *LRotate(AVLTree *T, AVLTreeNode *T1)
{
    assert(T1!=NULL);
    AVLTreeNode *T2;
    T2 = T1->right;
    AVLTreeNode *T3;
    T3 = T1->right->left;
    
    T2->left = T1;
    if (T1->parent == NULL) {
        T2->parent = NULL;
        T->root = T2;
    }
    else{
        T2->parent = T1->parent;
        int direct = Linkparent( T1->parent, T1);
        if (direct == 0) {
            T2->parent->left = T2;
        }
        else{
            T2->parent->right = T2;
        }
    }
    T1->parent = T2;
    T1->right = T3;


    if (T3 != NULL) {
        T3->parent = T1;
    }
   

    T1->height = max(get_height(T1->left),get_height(T1->right))+1;
    T2->height = max(get_height(T2->right),T1->height)+1;
    
    return T2;
}
//define double rotation
AVLTreeNode *LRRotate(AVLTree *T, AVLTreeNode *T1)
{    assert(T1!=NULL);
    T1->left = LRotate(T, T1->left);
    return RRotate(T, T1);
}

AVLTreeNode *RLRotate(AVLTree *T, AVLTreeNode *T1)
{    assert(T1!=NULL);
    T1->right = RRotate(T, T1->right);
    return LRotate(T, T1);
}

//rebalancing after insert/remove
void Restructure(AVLTree *T,AVLTreeNode *next)
{
    AVLTreeNode *parent;
    parent = next->parent;
    while (parent!=NULL){                      // balance fix
        parent->height = max(get_height(parent->left),get_height(parent->right))+1;
        int parent_bf = get_height(parent->left) - get_height(parent->right);
        if (parent_bf == 0) {
            break;
        }
        else if (parent_bf == 1 || parent_bf == -1){
            next = parent;
            parent = parent->parent;
        }
        else{                   // parent_bf == 2 || -2
            int node_pd = get_height(next->left) - get_height(next->right);
            if (parent_bf== 2){
                if (node_pd == -1)  {
                    parent = LRRotate(T, parent);
                }
                else {
                    parent = RRotate(T, parent);

                }
            }
            else{
                if(node_pd == -1) {
                    parent = LRotate(T, parent);
                }
                else {
                    parent = RLRotate(T, parent);
                    }

            }
            next = parent;
            parent = parent->parent;
        }
    }
}
// time complexity is O(logN)
int InsertNode(AVLTree *T, int k, int v)
{

    AVLTreeNode *node;
    AVLTreeNode *new;
    AVLTreeNode *parent = NULL;

    if (T->root == NULL) {
        new = newAVLTreeNode(k, v);
        T->root = new;
        T->size = 1;
        T->root->height = 0;
        return 1;
    }
    node = T->root;

    while (node != NULL) {
        parent = node;
        if (k < node->key) {
            node = node->left;
        }
        else if (k > node->key){
            node = node->right;
        }
        else{
            if (v < node->value) {
                node = node->left;
            }
            else if (v > node->value){
                node = node->right;
            }
            else{
                return 0;
            }
        }
    }

    new = newAVLTreeNode(k, v);
    T->size += 1;
    if (k > parent->key) {
        parent->right = new;
        new->parent = parent;
    }
    else if(k < parent->key){
        parent->left = new;
        new->parent = parent;

    }
    else{
        if (v > parent->value) {
            parent->right = new;
            new->parent = parent;
        }
        else{
            parent->left = new;
            new->parent = parent;
        }
    }
    
    new->height = 0;
    Restructure(T,new);

    return 1;
}
//find the minimal node of subtree.
AVLTreeNode *FindMinNode(AVLTreeNode *node)
{
    if (node) {
        while (node->left) {
            node = node->left;
        }
    }
    return node;
}
//find the maximal node of subtree.
AVLTreeNode *FindMaxNode(AVLTreeNode *node)
{
    if (node) {
        while (node->right) {
            node = node->right;
        }
    }
    return node;
}
//time complexity for DeleteNode() is O(log n).
int DeleteNode(AVLTree *T, int k, int v)
{
    AVLTreeNode *node;
    AVLTreeNode *parent;
    AVLTreeNode *child;
    AVLTreeNode *temp;
    if (T->root == NULL) {
        return 0;
    }
    node = Search(T, k, v);
    if (node == NULL) {
        return 0;
    }
    if (node == T->root) {
        if (node->height == 0) {
            T->root = NULL;
            T->size = 0;
            return 1;
        }
        else if(node->left == NULL){
            T->root = node->right;
            T->size -= 1;
            return 1;
        }
        else if(node->right == NULL){
            T->root = node->left;
            T->size -= 1;
            return 1;
        }
        else{
            if (get_height(node->left) >= get_height(node->right)) {
                temp = FindMaxNode(node->left);
                parent = temp->parent;
                node->key = temp->key;
                node->value = temp->value;
                free(temp);
                Restructure(T, parent);
            }
            else{
                temp = FindMinNode(node->right);
                parent = temp->parent;
                node->key = temp->key;
                node->value = temp->value;
                free(temp);
                Restructure(T, parent);
            }
            
        }
    }
    parent = node->parent;
    if (node->height == 0) {      //leaf node
        if (node->key > parent->key || (node->key == parent->key && node->value > parent->value)) {
            parent->right = NULL;
            child = parent->left;
            if (child) {
                child->parent = parent;
            }
        }
        else{
            parent->left = NULL;
            child = parent->right;
            if (child) {
                child->parent = parent;
            }
        }
        free(node);
        if (child) {
            Restructure(T, child);
        }
        
    }
    else if (node->left == NULL || node->right == NULL)   //node has only one child
    {
        child = node->left == NULL ? node->right : node->left;
        if (node->key > parent->key || (node->key == parent->key && node->value > parent->value)) {
            parent->right = child;
            child->parent = parent;
        }
        else{
            parent->left = child;
            child->parent = parent;
        }
        free(node);
        Restructure(T, child);
    }
    else{                                                   //node has two child
        if (get_height(node->left) >= get_height(node->right)) {
            temp = FindMaxNode(node->left);
            parent = temp->parent;
            node->key = temp->key;
            node->value = temp->value;
            free(temp);
            Restructure(T, parent);
        }
        else{
            temp = FindMinNode(node->right);
            parent = temp->parent;
            node->key = temp->key;
            node->value = temp->value;
            free(temp);
            Restructure(T, parent);
        }

    }
    T->size -= 1;
    return 1;
}
//
// time complexity analysis for Search() is O(log n).
AVLTreeNode *Search(AVLTree *T, int k, int v)
{
    AVLTreeNode *node;
    node = T->root;
    while (node != NULL) {
        if (k == node->key && v == node->value) {
            return node;
        }
        else if (k < node->key || (k == node->key && v < node->value))
        {
            node = node->left;
        }
        else{
            node = node->right;
        }
    }
    return NULL;
}


void FreeAVLTreeNode(AVLTreeNode *node)
{
    if (node == NULL) {
        return;
    }
    FreeAVLTreeNode(node->left);
    FreeAVLTreeNode(node->right);
    free(node);
}
// time complexity is O(n).
void FreeAVLTree(AVLTree *T)
{
    if (T == NULL) {
        return;
    }
    FreeAVLTreeNode(T->root);
    free(T);
}



void PrintAVLTreeNode(AVLTreeNode *node)
{
    if (node != NULL)
    {
        PrintAVLTreeNode(node->left);
        printf("(%d,%d),%d\n",node->key,node->value,node->height);
        PrintAVLTreeNode(node->right);
    }
}
// time complexity is O(n).
void PrintAVLTree(AVLTree *T)
{
    if (T != NULL) {
        PrintAVLTreeNode(T->root);
    }
    printf("\n");
}



int main() //sample main for testing
{
     int i,j;
     AVLTree *tree1, *tree2, *tree3, *tree4, *tree5, *tree6, *tree7, *tree8;
     AVLTreeNode *node1;

    tree1=CreateAVLTree("stdin");
    PrintAVLTree(tree1);
    FreeAVLTree(tree1);
    
     tree2=CreateAVLTree("File1.txt");
     PrintAVLTree(tree2);
     tree3=CloneAVLTree(tree2);
     PrintAVLTree(tree3);
     FreeAVLTree(tree2);
     FreeAVLTree(tree3);
     //Create tree4
     tree4=newAVLTree();
     j=InsertNode(tree4, 10, 10);
     for (i=0; i<15; i++)
      {
       j=InsertNode(tree4, i, i);
       if (j==0) printf("(%d, %d) already exists\n", i, i);
      }
      PrintAVLTree(tree4);
      node1=Search(tree4,20,20);
      if (node1!=NULL)
        printf("key= %d value= %d\n",node1->key,node1->value);
      else
        printf("Key 20 does not exist\n");
    
      for (i=17; i>0; i--)
      {
        j=DeleteNode(tree4, i, i);
        if (j==0)
          printf("Key %d does not exist\n",i);
        PrintAVLTree(tree4);
      }
     FreeAVLTree(tree4);
     //Create tree5
     tree5=newAVLTree();
     j=InsertNode(tree5, 6, 25);
     j=InsertNode(tree5, 6, 10);
     j=InsertNode(tree5, 6, 12);
     j=InsertNode(tree5, 6, 20);
     j=InsertNode(tree5, 9, 25);
     j=InsertNode(tree5, 10, 25);
     PrintAVLTree(tree5);
     //Create tree6
     tree6=newAVLTree();
     j=InsertNode(tree6, 6, 25);
     j=InsertNode(tree6, 5, 10);
     j=InsertNode(tree6, 6, 12);
     j=InsertNode(tree6, 6, 20);
     j=InsertNode(tree6, 8, 35);
     j=InsertNode(tree6, 10, 25);
     PrintAVLTree(tree6);
     tree7=AVLTreesIntersection(tree5, tree6);
     tree8=AVLTreesUnion(tree5,tree6);
     PrintAVLTree(tree7);
     PrintAVLTree(tree8);
    return 0;
}



