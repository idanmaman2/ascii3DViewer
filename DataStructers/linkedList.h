//
// Created by idang on 9/18/22.
//

#ifndef IDSH_STACK_H
#define IDSH_STACK_H
#include <unistd.h>
#include <stdlib.h>
#include <memory.h>
#include <stdlib.h>
#include "iter.h"

typedef  struct linkedNode {
    struct  linkedNode * next ;
    struct  linkedNode * before ;
    void * content ;
}linkedNode;

typedef struct linkedList {
    struct iter it ;
    size_t len ;
    unsigned int size  ;
    struct linkedNode * head  ;
    struct linkedNode * tail ;
}linkedList;
//iter
void * iterNextLinkedList( struct linkedNode  * current,struct linkedList * ln){
    struct linkedNode * res = current ;
    ln->it.current=current->next ;
    return res->content;
}

int iterHasNextLinkedList(struct  linkedNode * current,struct linkedList * ln){
    return   ln->it.current;
}

void iterResetLinkedList(struct linked * current,struct linkedList * ln){
    ln->it.current = ln->head ;
}

//functions

void freeNode(struct linkedNode * node){
    free(node->content);
    free(node);
}

struct linkedNode * createNode(void * data,int size){
    struct linkedNode  * newHead = (struct linkedNode * )calloc(1,sizeof(struct linkedNode));
    newHead->content=malloc(size);
    memcpy(newHead->content,data,size);
    return newHead;

}

struct linkedList *  createLinkedList(unsigned int size ){
    struct linkedList *  stk = (struct linkedList * )calloc(1,sizeof (struct linkedList));
    stk->it.current = stk->head;
    stk->it.next = iterNextLinkedList ;
    stk->it.hasNext = iterHasNextLinkedList ;
    stk->it.reset =iterResetLinkedList ;
    stk->size = size ;
    return stk ;
}

void pushFront(struct linkedList * ln ,void * data){
    struct linkedNode  * newHead = createNode(data,ln->size);
    newHead->next=ln->head;
    if(ln->head)
        ln->head->before=newHead;
    else{
        ln->tail=newHead;
        ln->it.current=newHead;
    }
    ln->head=newHead;
}

void pushBack(struct linkedList * ln , void * data ){
    struct linkedNode  * newHead = createNode(data,ln->size);
    if(!ln->tail){
        ln->head=newHead;
        ln->tail=newHead;
        ln->it.current = ln->head;
        return ;
    }
    newHead->before = ln->tail ;
    ln->tail->next=newHead;
    ln->tail=newHead;
}

void *  getFront (struct linkedList * ln ){
    return ln->head->content;
}

void *  getBack(struct linkedList * ln ){
    return ln->tail->content;
}

void * popFront(struct linkedList * ln){
    struct linkedNode * toDel = ln->head;
    ln->head=ln->head->next;
    freeNode(toDel);

}

void * popBack(struct linkedList * ln){
    struct linkedNode * toDel = ln->tail;
    ln->tail=ln->tail->before;
    ln->tail->next=NULL;
    freeNode(toDel);

}

#endif //IDSH_STACK_H
