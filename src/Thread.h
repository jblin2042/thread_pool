/*
 * Thread.h
 *
 *  Created on: 2016/7/25
 *      Author: j
 */

#ifndef THREAD_H_
#define THREAD_H_

#include "Include.h"

typedef struct _S_thread_work {
	void * (*Function) ( void *arg ) ;
	void *arg ;
} S_thread_work ;

typedef void Thread_pool_handle ;
typedef void*(*Thread_pool_function) ( void * ) ;



extern Thread_pool_handle *Thread_pool_create ( int num_threads ) ;
extern int Thread_pool_add_work ( Thread_pool_handle *handle , Thread_pool_function func , void* arg ) ;


extern int Thread_pool_pause ( Thread_pool_handle *handle ) ;
extern int Thread_pool_resume ( Thread_pool_handle *handle ) ;

extern int Thread_pool_destroy ( Thread_pool_handle **handle ) ;

extern int Thread_pool_wait ( Thread_pool_handle *handle ) ;
extern int Thread_pool_destroy_no_wait ( Thread_pool_handle **handle ) ;



#endif /* THREAD_H_ */
