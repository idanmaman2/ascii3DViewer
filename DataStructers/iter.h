//
// Created by idang on 9/21/22.
//

#ifndef IDSH_ITER_H
#define IDSH_ITER_H


typedef struct iter {
    void *  (*next) (void * cur , void * ds);
    void (*reset)(void * cur , void * ds);
    int  (*hasNext )(void * cur , void * ds);
    void * current ;
}iter;

void forEachIter(void (*func )(void * param , void * arg) , struct iter *  it ,void  * arg){
    it->reset(it->current,it);
    while(it->hasNext(it->current,it)){
        func(it->next(it->current,it),arg);
    }
}

void convertToVectorIter(struct iter * it , void * ds ){
}



void filterIter(int (*func ) , struct iter * it , void * ds  ){}

void reduceIter(int (*func ) , struct iter * it , void * ds ){}

#endif //IDSH_ITER_H
