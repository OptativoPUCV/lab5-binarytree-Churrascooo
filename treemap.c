#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}

//------------------------------------------------------------------------
TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}
//------------------------------------------------------------------------
TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2))
{
    TreeMap * new = (TreeMap *)malloc(sizeof(TreeMap));
    new->root = NULL;
    new->current = NULL;
    new->lower_than = lower_than;
    return new;
}
//------------------------------------------------------------------------
void insertTreeMap(TreeMap * tree, void* key, void * value)
{
    if (tree == NULL || key == NULL || value == NULL) return;
    if (tree->root == NULL)
    {
        tree->root = createTreeNode(key, value);
    }
    else
    {
        TreeNode *aux = tree->root;
        while (aux != NULL)
        {
            if (is_equal(tree, key, aux->pair->key))
            {
                return;
            }
            if (tree->lower_than(key, aux->pair->key) == 1)
            {
                if (aux->left == NULL)
                {
                    TreeNode *new = createTreeNode(key, value);
                    new->parent = aux;
                    aux->left = new;
                    tree->current = new;
                    return;
                }
                else
                {
                    aux = aux->left;    
                }
            }
            else
            {
                if (aux->right == NULL)
                {
                    TreeNode *new = createTreeNode(key, value);
                    new->parent = aux;
                    aux->right = new;
                    tree->current = new;
                    return;
                }
                else
                {
                    aux = aux->right;
                }
            }
        }
    }
}
//------------------------------------------------------------------------
TreeNode * minimum(TreeNode * x)
{
    while (x->left != NULL)
    {
        x = x->left;
    }
    return x;
}
//------------------------------------------------------------------------
void removeNode(TreeMap * tree, TreeNode* node)
{
    if (tree == NULL || node == NULL) return;
    
    //Si no tiene hijos
    if (node->left == NULL && node->right == NULL)
    {
        if (node->parent->left == node)
        {
            node->parent->left = NULL;
        }
        else
        {
            node->parent->right = NULL;    
        }
    }
    else
    //Si tiene un hijo
    {
        if (node->left != NULL && node->right == NULL)
        {
            if (node->parent->left == node)
            {
                node->parent->left = node->left;
                node->left->parent = node->parent;
            }
            else
            {
                node->parent->right = node->left;
                node->left->parent = node->parent;
            }
        }
        if (node->right != NULL && node->left == NULL)
        {
            if (node->parent->left == node)
            {
                node->parent->left = node->right;
                node->right->parent= node->parent;
            }
            else
            {
                node->parent->right = node->right;
                node->right->parent = node->parent;
            }
        }
        //En caso de tener dos hijos
        if (node->left != NULL && node->right != NULL)
        {
            TreeNode *min = minimum(node->right);
            node->pair->key = min->pair->key;
            node->pair->value = min->pair->value;
            removeNode(tree, min);
        }
    }
    return;
}
//------------------------------------------------------------------------
void eraseTreeMap(TreeMap * tree, void* key)
{
    if (tree == NULL || tree->root == NULL) return;
    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);
}
//------------------------------------------------------------------------
Pair * searchTreeMap(TreeMap * tree, void* key)
{
    if (tree == NULL || key == NULL)
    {
        return NULL;
    }

    else
    {
        TreeNode *aux = tree->root;
        while (aux != NULL)
        {
            if (is_equal(tree, key, aux->pair->key))
            {
                tree->current = aux;
                return aux->pair;
            }
            if (tree->lower_than(key, aux->pair->key) == 1)
            {
                aux = aux->left;
            }
            else
            {
                aux = aux->right;
            }            
        }
        return NULL;
    }    
}
//------------------------------------------------------------------------
Pair * upperBound(TreeMap * tree, void* key)
{
    if (tree == NULL || key == NULL) return NULL;
    
    TreeNode *aux = tree->root;
    TreeNode *result = NULL;

    while (aux != NULL)
    {
        if (tree->lower_than(key, aux->pair->key))
        {
            result = aux;
            aux = aux->left;
        }
        else if (is_equal(tree, key, aux->pair->key))
        {
            return aux->pair;
        }
        else
        {
            aux = aux->right;
        }
    }
    if (result == NULL)
    {
        return NULL;
    }
    else
    {
        return result->pair;
    }
}
//------------------------------------------------------------------------
Pair * firstTreeMap(TreeMap * tree)
{
    if (tree == NULL) return NULL;
    tree->current = tree->root;
    while (tree->current->left != NULL)
    {
        tree->current = tree->current->left;
    }
    return tree->current->pair;
}
//------------------------------------------------------------------------
Pair * nextTreeMap(TreeMap * tree)
{
    if (tree == NULL || tree->current == NULL)
    {
        return NULL;
    }
    else
    {
        if (tree->current->right != NULL)
        {
            tree->current = tree->current->right;
            tree->current->right = tree->current->left;
            while (tree->current->left == NULL)
            {
                tree->current = tree->current->left;
            }
            return tree->current->pair;
        }
        else
        {
            TreeNode *parent = tree->current->parent;
            while (parent != NULL && tree->lower_than(tree->current->pair, parent->pair))
            {
                tree->current = parent;
            }
        }
        if (tree->current == NULL)
        {
            return NULL;
        }
        else
        {
            tree->current = tree->current->parent;
            return tree->current->parent->pair;
        }
    }
}
