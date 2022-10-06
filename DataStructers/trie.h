//
// Created by idang on 9/19/22.
//

#ifndef IDSH_TRIE_H
#define IDSH_TRIE_H

#include <stdlib.h>
#include <memory.h>
#include "vector.h"


//{" ",'!','"','#','$','%','&',39,'(',')','*','+',',','-','.','/','0','1','2','3','4','5','6','7','8','9',':',';','<','=','>','?','@','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','[','\\',']','^','_','`','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','{','|','}','~'};
#define charsLen 95
#define firstOrd 32

typedef struct trieNode{
    void * end ;
    struct trieNode * next[charsLen];
}trieNode;

typedef struct trie{
    struct trieNode root;
    int size ;
}trie;


//functions
struct linkedNode * createTrieNode(){
    struct trieNode  * newHead = (struct trieNode * )calloc(1,sizeof(struct trieNode));
    return newHead;

}

static int convertToIndex(char ch){
    return ch-firstOrd;


}

static int nodeExsistChar(char ck , struct trieNode * nd){
    return nd->next[convertToIndex(ck)] ;
}

void putDataInTrieNode(struct trieNode * nd , int size , void * data){
    nd->end = malloc(size);
    memcpy(nd->end,data,size);
}

struct trie *  createTrie(size_t size ){
    struct trie *  stk = (struct trie * )malloc(sizeof (struct trie));
    memset(stk,0,sizeof(struct trie));
    stk->size = size ;
    return stk ;
}

void addWord (struct trie * nd , char * word , void * data ){
    int wordLen = strlen(word);
    struct trieNode * start = &nd->root;
    for(int i=0 ;i<wordLen;i++){
        size_t index  = convertToIndex(word[i]);
        if(!nodeExsistChar(word[i],start))
            start->next[index] = createTrieNode();
        start = start->next[index];
    }
    putDataInTrieNode(start,nd->size , data);
}

static void * searchForValueRec(struct trieNode * nd , char * word){
    if (!nd)
        return NULL;
    if(*word)
        return searchForValueRec(nd->next[convertToIndex(*word)],word+1);
    return nd->end;
}

void *  searchForvalue(struct trie * nd , char * word ){
    return searchForValueRec(&nd->root,word);
}

static void forEachRec(void (*callback)(void * key , void * data , size_t keyLen),struct trieNode * node,struct vector * untilNow){
    char value ;
    addVector(&value,untilNow);
    if(node->end) {
        callback(untilNow->arr, node->end, untilNow->len-1);
    }
    for(size_t  i=0 ,value = firstOrd ; i< charsLen ; i++,value++){
        if(nodeExsistChar(value,node)){
            setIndexVector(untilNow->len-1,&value,untilNow);
            forEachRec(callback,node->next[convertToIndex(value)],untilNow);
        }

    }
    deleteLastVector(untilNow);



}

void forEachTrie(void (*callback)(void * ), struct trie * nd ){
    struct vector * vec = createVector(sizeof(char));
    forEachRec(callback,&nd->root,vec);


}


#endif //IDSH_TRIE_H
