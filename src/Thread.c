#include "Thread.h"
typedef struct _S_thread_pool {
	int i_arry_sz ;     // array 的數量
	int i_thread_num ;  // thread 的編號
	pthread_t i_tid ;
	bool i_working ;    // false 表示休眠中

	bool i_pause ;


	pthread_mutex_t i_mutex ;
	pthread_cond_t i_cond ;
	pthread_attr_t i_attr ;

	List_handle* i_list ;
	int i_list_queue_num ;

	bool i_shutdowned ;
} S_thread_pool ;

void* Thread_pool_do ( void* arg ) {
	S_thread_pool* p_arg = ( S_thread_pool * ) arg ;
	S_thread_work *p_work = NULL ;
	while ( 1 ) {
printf("111111111111111111111111111111111111111111111111111111111\n");
		pthread_mutex_lock ( & p_arg->i_mutex ) ;
		if ( ( 0 == p_arg->i_list_queue_num ) || ( true == p_arg->i_pause ) ) {
			p_arg->i_working = false ;
			pthread_cond_wait ( & p_arg->i_cond , & p_arg->i_mutex ) ;
			if ( true == p_arg->i_shutdowned ) {
				break ;
			}
		}
		p_arg->i_working = true ;
		p_work = ( S_thread_work* ) Dequeue ( & p_arg->i_list ) ;
		if ( ! p_work ) {   // 有時候是處理速度超過被配至速度
			pthread_mutex_unlock ( & p_arg->i_mutex ) ;
			continue ;
		}
		-- p_arg->i_list_queue_num ;
		pthread_mutex_unlock ( & p_arg->i_mutex ) ;
		p_work->Function ( p_work->arg ) ;
		free ( p_work ) ;
		p_work = NULL ;
	}
	pthread_mutex_unlock ( & p_arg->i_mutex ) ;
	pthread_exit ( NULL ) ;
}

Thread_pool_handle *Thread_pool_create ( int num_threads ) {
	if ( 0 >= num_threads ) {
		return NULL ;
	}
	S_thread_pool *p_handle = ( S_thread_pool * ) calloc ( num_threads , sizeof(S_thread_pool) ) ;
	if ( ! p_handle ) {
		return NULL ;
	}

	S_thread_pool *p_tmp = NULL ;
	int i = 0 ;
	for ( i = 0 ; i < num_threads ; ++ i ) {
		p_tmp = & p_handle [ i ] ;
		p_tmp->i_arry_sz = num_threads ;
		p_tmp->i_thread_num = i ;
		p_tmp->i_working = true ;
		p_tmp->i_pause = false ;

		pthread_mutex_init ( & p_tmp->i_mutex , NULL ) ;
		pthread_cond_init ( & p_tmp->i_cond , NULL ) ;

		// 等同 pthread_detach ( p_tmp->i_tid ) //
		pthread_attr_init ( & p_tmp->i_attr ) ;
		/* When a detached thread terminates, its resources are automatically released back to the  system */
		/* PTHREAD_CREATE_JOINABLE 配和 pthread_exit() and pthread_join() */
		pthread_attr_setdetachstate ( & p_tmp->i_attr , PTHREAD_CREATE_JOINABLE ) ;

		p_tmp->i_list = Create_queue_head ( ) ;
		p_tmp->i_list_queue_num = 0 ;
		p_tmp->i_shutdowned = false ;
		if ( 0 != pthread_create ( & p_tmp->i_tid , & p_tmp->i_attr , Thread_pool_do , p_tmp ) ) {
			p_handle [ 0 ].i_arry_sz = i + 1 ; // 因為 Thread_pool_destroy_no_wait 會取第一個 array 的數量
			Thread_pool_destroy_no_wait ( ( Thread_pool_handle** ) & p_handle ) ;
			return NULL ;
		}
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
/*
 * func 要清楚定 arg 要如何作處理包含 free 的部份
 */
int g_j = 0 ;
int Thread_pool_add_work ( Thread_pool_handle *handle , Thread_pool_function func , void* arg ) {
	if ( ! handle || ! func ) {
		return - 1 ;
	}
	S_thread_pool *p_handle = ( S_thread_pool * ) handle ;
	S_thread_pool *p_tmp = NULL ;
	int i = 0 ;
	int arry_sz = p_handle->i_arry_sz ;

	S_thread_work work = { func , arg } ;

	// 排序工作量 //
	S_priority priority [ arry_sz ] ;
	Work_queue_num ( ( S_thread_pool * ) handle , priority , arry_sz ) ;

	while ( 1 ) {
		for ( i = 0 ; i < arry_sz ; ++ i ) {
			p_tmp = & p_handle [ priority [ i ].i_arry_num ] ;

			if ( 0 != pthread_mutex_trylock ( & p_tmp->i_mutex ) ) {
				continue ;
			}

			if ( 1 != Enqueue ( & p_tmp->i_list , & work , sizeof(S_thread_work) ) ) {
				pthread_mutex_unlock ( & p_tmp->i_mutex ) ;
				continue ;
			}
			++ p_tmp->i_list_queue_num ;
			if ( false == p_tmp->i_working ) {
				pthread_cond_signal ( & p_tmp->i_cond ) ;
			}
			pthread_mutex_unlock ( & p_tmp->i_mutex ) ;
			return 1 ;
		}
	}
	return 1 ;
}

int Thread_pool_pause ( Thread_pool_handle *handle ) {
	if ( ! handle ) {
		return - 1 ;
	}
	S_thread_pool *p_tmp = NULL , *p_handle = ( S_thread_pool * ) handle ;
	int i = 0 , sz = p_handle->i_arry_sz ;
	bool event = false ;
	do {
		event = true ;
		for ( i = 0 ; i < sz ; ++ i ) {
			p_tmp = & p_handle [ i ] ;
			if ( true == p_tmp->i_pause ) {
				continue ;
			}
			if ( 0 != pthread_mutex_trylock ( & p_tmp->i_mutex ) ) {
				event = false ;
				continue ;
			}
			p_tmp->i_pause = true ;
			pthread_mutex_unlock ( & p_tmp->i_mutex ) ;
		}
	} while ( false == event ) ;
	return 1 ;
}
int Thread_pool_resume ( Thread_pool_handle *handle ) {
	if ( ! handle ) {
		return - 1 ;
	}
	S_thread_pool *p_tmp = NULL , *p_handle = ( S_thread_pool * ) handle ;
	int i = 0 , sz = p_handle->i_arry_sz ;
	bool event = false ;
	do {
		event = true ;
		for ( i = 0 ; i < sz ; ++ i ) {
			p_tmp = & p_handle [ i ] ;
			if ( false == p_tmp->i_pause ) {
				continue ;
			}
			if ( 0 != pthread_mutex_trylock ( & p_tmp->i_mutex ) ) {
				event = false ;
				continue ;
			}
			p_tmp->i_pause = false ;
			pthread_cond_signal ( & p_tmp->i_cond ) ;
			pthread_mutex_unlock ( & p_tmp->i_mutex ) ;
		}
	} while ( false == event ) ;
	return 1 ;

}







/*
 * 取得 mutex 的速度太快~會使其他來不及取得
 */
int Thread_pool_destroy ( Thread_pool_handle **handle ) {
	if ( ! handle || ! * handle ) {
		return - 1 ;
	}
	S_thread_pool *p_tmp = NULL , *p_handle = ( S_thread_pool * ) * handle ;
	int i = 0 , sz = p_handle->i_arry_sz ;
	bool event = false ;
	do {
		event = true ;
		for ( i = 0 ; i < sz ; ++ i ) {
			p_tmp = & p_handle [ i ] ;
			if ( true == p_tmp->i_shutdowned ) {
				continue ;
			}
			if ( 0 != pthread_mutex_trylock ( & p_tmp->i_mutex ) ) {
				event = false ;
				continue ;
			}
			if ( false == p_tmp->i_working ) {
				pthread_cond_signal ( & p_tmp->i_cond ) ;
			}
			if ( ( 0 != p_tmp->i_list_queue_num ) || ( true == p_tmp->i_working ) ) { //會跟上面的 pthread_cond_wait 有時間差的問題
				pthread_mutex_unlock ( & p_tmp->i_mutex ) ;
				event = false ;
				continue ;
			}
			p_tmp->i_shutdowned = true ;   // 上面已經解鎖定 ,等待 mutex unlock

			pthread_mutex_unlock ( & p_tmp->i_mutex ) ;

			pthread_join ( p_tmp->i_tid , NULL ) ;
			pthread_attr_destroy ( & p_tmp->i_attr ) ;
			pthread_mutex_destroy ( & p_tmp->i_mutex ) ;
			pthread_cond_destroy ( & p_tmp->i_cond ) ;

		}
	} while ( false == event ) ;

	free ( * handle ) ;
	* handle = NULL ;

	return 1 ;
}

int Thread_pool_wait ( Thread_pool_handle *handle ) {
	if ( ! handle ) {
		return - 1 ;
	}
	S_thread_pool *p_tmp = NULL , *p_handle = ( S_thread_pool * ) handle ;
	int i = 0 , sz = p_handle->i_arry_sz ;
	bool event = false ;
	do {
		event = true ;
		for ( i = 0 ; i < sz ; ++ i ) {
			p_tmp = & p_handle [ i ] ;
			if ( 0 != pthread_mutex_trylock ( & p_tmp->i_mutex ) ) {
				event = false ;
				continue ;
			}
			if ( ( 0 != p_tmp->i_list_queue_num ) || ( true == p_tmp->i_working ) ) { //會跟上面的 pthread_cond_wait 有時間差的問題
				pthread_mutex_unlock ( & p_tmp->i_mutex ) ;
				event = false ;
				continue ;
			}
			pthread_mutex_unlock ( & p_tmp->i_mutex ) ;
		}
	} while ( false == event ) ;
	return 1 ;
}

int Thread_pool_destroy_no_wait ( Thread_pool_handle **handle ) {
	if ( ! handle || ! * handle ) {
		return - 1 ;
	}
	S_thread_pool *p_tmp = NULL , *p_handle = ( S_thread_pool * ) * handle ;
	int i = 0 , sz = p_handle->i_arry_sz ;
	bool event = false ;
	S_thread_work* p_work = NULL ;
	do {
		event = true ;
		for ( i = 0 ; i < sz ; ++ i ) {
			p_tmp = & p_handle [ i ] ;
			if ( true == p_tmp->i_shutdowned ) {
				continue ;
			}
			if ( 0 != pthread_mutex_trylock ( & p_tmp->i_mutex ) ) {
				event = false ;
				continue ;
			}
			// 把資料全部都到出來
			do {
				p_work = ( S_thread_work* ) Dequeue ( & p_tmp->i_list ) ;
				if ( ! p_work ) {
					break ;
				}
				free ( p_work ) ;  // free NULL
			} while ( ! p_work ) ;
			p_tmp->i_list_queue_num = 0 ;

			if ( false == p_tmp->i_working ) {
				pthread_cond_signal ( & p_tmp->i_cond ) ;
			}
			if ( ( 0 != p_tmp->i_list_queue_num ) || ( true == p_tmp->i_working ) ) { //會跟上面的 pthread_cond_wait 有時間差的問題
				pthread_mutex_unlock ( & p_tmp->i_mutex ) ;
				event = false ;
				continue ;
			}
			p_tmp->i_shutdowned = true ;   // 上面已經解鎖定 ,等待 mutex unlock

			pthread_mutex_unlock ( & p_tmp->i_mutex ) ;

			pthread_join ( p_tmp->i_tid , NULL ) ;
			pthread_attr_destroy ( & p_tmp->i_attr ) ;
			pthread_mutex_destroy ( & p_tmp->i_mutex ) ;
			pthread_cond_destroy ( & p_tmp->i_cond ) ;

		}
	} while ( false == event ) ;

	free ( * handle ) ;
	* handle = NULL ;

	return 1 ;
}



























