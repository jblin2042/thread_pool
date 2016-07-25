/*
 * List.h
 *
 *  Created on: 2016/7/21
 *      Author: j
 */

#ifndef LIST_H_
#define LIST_H_
#include "Include.h"

typedef void List_handle ;


extern List_handle* Create_queue_head ( void ) ;
extern int Enqueue ( List_handle **handle , void *data , int data_byte ) ;
extern void *Dequeue ( List_handle **handle ) ;

#endif /* LIST_H_ */
