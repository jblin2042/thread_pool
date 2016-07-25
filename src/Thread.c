/*
 * Thread.c
 *
 *  Created on: 2016/7/25
 *      Author: j
 */

#include "Thread.h"
typedef struct _S_thread_pool {
	int i_arry_sz ;   // array 的數量
	pthread_t i_tid ;
	int i_thread_num ;  // thread 的編號

	pthread_cond_t i_cond ;
	pthread_mutex_t i_mutex ;
	List_handle* i_list ;
	int i_list_queue_num ;
} S_thread_pool ;

static void* Thread_pool_do ( void* arg ) {
	S_thread_pool* p_arg = ( S_thread_pool * ) arg ;
	S_thread_work *p_work = NULL ;

	while ( 1 ) {
		pthread_mutex_lock ( & p_arg->i_mutex ) ;
		if ( 0 >= p_arg->i_list_queue_num ) {
			pthread_cond_wait ( & p_arg->i_cond , & p_arg->i_mutex ) ;
		}
		p_work = ( S_thread_work* ) Dequeue ( & p_arg->i_list ) ;
		if ( ! p_work ) {   // 表示資料應該是有錯誤
			p_arg->i_list_queue_num = 0 ;
			pthread_mutex_unlock ( & p_arg->i_mutex ) ;
			continue ;
		}
		-- p_arg->i_list_queue_num ;
		pthread_mutex_unlock ( & p_arg->i_mutex ) ;
		p_work->Function ( p_work->arg ) ;
	}
	return NULL ;
}

Thread_pool_handle *Thread_pool_create ( int num_threads ) {
	if ( 0 >= num_threads )
		return NULL ;

	S_thread_pool *p_handle = ( S_thread_pool * ) calloc ( num_threads , sizeof(S_thread_pool) ) ;
	if ( ! p_handle )
		return NULL ;

	S_thread_pool *p_handle_tmp = NULL ;
	int i = 0 ;
	for ( i = 0 ; i < num_threads ; ++ i ) {
		p_handle_tmp = & p_handle [ i ] ;
		p_handle_tmp->i_arry_sz = num_threads ;
		p_handle_tmp->i_thread_num = i ;

		pthread_cond_init ( & p_handle_tmp->i_cond , NULL ) ;
		pthread_mutex_init ( & p_handle_tmp->i_mutex , NULL ) ;
		p_handle_tmp->i_list = Create_queue_head ( ) ;
		p_handle_tmp->i_list_queue_num = 0 ;
		pthread_create ( & p_handle_tmp->i_tid , NULL , Thread_pool_do , & p_handle [ i ] ) ;
	}
	return ( Thread_pool_handle* ) p_handle ;
}

typedef struct _S_priority {
	int i_arry_num ;
	int i_list_queue_num ;

} S_priority ;
static int Compar ( const void *s1 , const void *s2 ) {
	return ( ( S_priority * ) s1 )->i_list_queue_num - ( ( S_priority* ) s2 )->i_list_queue_num ;
}
static int Work_queue_num ( S_thread_pool* handle , S_priority *buf , int buf_sz ) {
	int i = 0 ;
	for ( i = 0 ; i < buf_sz ; ++ i ) {
		buf [ i ].i_arry_num = handle [ i ].i_thread_num ;
		buf [ i ].i_list_queue_num = handle [ i ].i_list_queue_num ;
	}
	qsort ( buf , buf_sz , sizeof(S_priority) , Compar ) ;
	return 1 ;
}

int Thread_pool_add_work ( Thread_pool_handle *handle , Thread_pool_function func , void* arg ) {
	if ( ( ! handle ) || ( ! func ) ) return - 1 ;


	S_thread_pool *p_handle = ( S_thread_pool * ) handle , *p_handle_tmp = NULL ;
	int arry_sz = p_handle [ 0 ].i_arry_sz , i = 0 ;
	S_thread_work work = { func , arg } ;

	// 找尋資料量少的 //
	S_priority buf [ arry_sz ] ;
	Work_queue_num ( p_handle , buf , arry_sz ) ;
	while ( 1 ) {
		for ( i = 0 ; i < arry_sz ; ++ i ) {
			p_handle_tmp = & p_handle [ buf [ i ].i_arry_num ] ;
			if ( 0 != pthread_mutex_trylock ( & p_handle_tmp->i_mutex ) ) { // 非阻塞 mutex lock
				continue ;
			}
			Enqueue ( & p_handle_tmp->i_list , & work , sizeof(S_thread_work) ) ;
			++ p_handle_tmp->i_list_queue_num ;
			pthread_mutex_unlock ( & p_handle_tmp->i_mutex ) ;
			pthread_cond_signal ( & p_handle_tmp->i_cond ) ;

//			for ( i = 0 ; i < arry_sz ; ++ i ) {
//				printf ( "array[%d]:%d " , p_handle [ i ].i_thread_num , p_handle [ i ].i_list_queue_num ) ;
//			}
//			printf ( "\n" ) ;
			return 1 ;
		}
	}
	return 1 ;
}

void Thread_pool_wait ( Thread_pool_handle *handle ) {
	if ( ! handle ) return ;

	S_thread_pool *p_handle = ( S_thread_pool * ) handle ;
	int arry_sz = p_handle [ 0 ].i_arry_sz , i = 0 ;
	int event = 0 ;
	do {
		event = 0 ;
		for ( i = 0 ; i < arry_sz ; ++ i ) {
			event += p_handle [ i ].i_list_queue_num ;

		}
	} while ( event ) ;
}

int Thread_pool_Destroy ( Thread_pool_handle **handle ) {
	if ( ( ! handle ) || ( ! * handle ) ) {
		return - 1 ;
	}
	S_thread_pool *p_tmp = ( S_thread_pool * ) * handle ;
	S_thread_work* p_list = NULL ;
	int i = 0 , sz = p_tmp->i_arry_sz ;
	for ( i = 0 ; i < sz ; ++ i ) {
		pthread_cancel ( p_tmp [ i ].i_tid ) ;
		pthread_mutex_destroy ( & p_tmp [ i ].i_mutex ) ;
		while ( ( p_list = ( S_thread_work* ) Dequeue ( & p_tmp [ i ].i_list ) ) ) {
			free ( p_list->arg ) ;
			p_list->arg = NULL ;

		}
		p_tmp [ i ].i_list_queue_num = 0 ;
		pthread_cancel ( p_tmp [ i ].i_tid ) ;
		p_tmp [ i ].i_tid = 0 ;
		p_tmp [ i ].i_arry_sz = 0 ;
	}
	return 1 ;
}

