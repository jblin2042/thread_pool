#ifndef LIST_H_
#define LIST_H_

#include "Include.h"

typedef void List_handle ;
extern List_handle* Create_queue_head ( void ) ;
extern int Enqueue ( List_handle **handle , void *data , int data_byte ) ;
// 資料取出後要 free
extern void *Dequeue ( List_handle **handle ) ;

#endif /* LIST_H_ */
