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
extern void Thread_pool_wait ( Thread_pool_handle *handle ) ;
extern int Thread_pool_Destroy ( Thread_pool_handle **handle ) ;



#if 0
#define D_max 65000
void *Function_1 ( void * arg ) {
//	char*p_arg = ( char* ) arg ;
	int i = 0 ;
	for ( i = 0 ; i < 50 ; ++ i ) {
//		printf ( "%s\n" , p_arg ) ;
	}
	return NULL ;
}

int main ( void ) {

	Begin_time ( ) ;
	Thread_pool_handle *p_handle = Thread_pool_create ( 500 ) ;
	if ( ! p_handle ) {
		exit ( 1 ) ;
	}
	char buf [ D_max ] [ 100 ] ;
	int i = 0 ;
	for ( i = 0 ; i < D_max ; ++ i ) {
		snprintf ( buf [ i ] , sizeof ( buf [ i ] ) , "%d%d%d%d" , i , i , i , i ) ;
		Thread_pool_add_work ( p_handle , Function_1 , buf [ i ] ) ;
	}

	Thread_pool_wait ( p_handle ) ;

	Thread_pool_Destroy ( & p_handle ) ;

	printf ( "%Lf\n" , End_time ( ) ) ;

	return 0 ;
}
#endif



#endif /* THREAD_H_ */
