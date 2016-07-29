#include "Include.h"

struct timeval g_timer_begin ;
struct timeval g_timer_end ;
void Begin_time ( void ) {
	gettimeofday ( & g_timer_begin , NULL ) ;
	return ;
}

long double End_time ( void ) {
	gettimeofday ( & g_timer_end , NULL ) ;
	long int d_s = g_timer_end.tv_sec - g_timer_begin.tv_sec ;
	long int d_us = g_timer_end.tv_usec - g_timer_begin.tv_usec ;
	if ( g_timer_end.tv_usec < g_timer_begin.tv_usec ) {
		-- d_s ;
		d_us = ( 1000000 - g_timer_begin.tv_usec ) + g_timer_end.tv_usec ;
	}
	return d_s + ( ( long double ) d_us / 10e5 ) ;
}

pthread_mutex_t g_m ;
int g_counter = 0 ;
#define D_max 65000
void *Function_1 ( void * arg ) {
	usleep ( 10000 ) ;
//	printf ( "%s\n" , ( char* ) arg ) ;
	pthread_mutex_lock ( & g_m ) ;
	++ g_counter ;
	pthread_mutex_unlock ( & g_m ) ;
	return NULL ;
}


#if 0
Thread_pool_destroy ( & p_handle ) ;
#endif
//int main ( void ) {
//	pthread_mutex_init ( & g_m , NULL ) ;
//
//	char buf [ D_max ] [ 100 ] ;
//	int i = 0 ;
//	while ( 1 ) {
//		Begin_time ( ) ;
//		Thread_pool_handle *p_handle = Thread_pool_create ( 500 ) ;
//		if ( ! p_handle ) {
//			perror ( "exit1" ) ;
//			exit ( 1 ) ;
//		}
//		for ( i = 0 ; i < D_max ; ++ i ) {
//			snprintf ( buf [ i ] , sizeof ( buf [ i ] ) , "%d%d%d%d" , i , i , i , i ) ;
//			Thread_pool_add_work ( p_handle , Function_1 , buf [ i ] ) ;
//		}
//
//		Thread_pool_destroy ( & p_handle ) ;
//		printf ( "g_counter:%d\n" , g_counter ) ;
//		printf ( "%Lf\n" , End_time ( ) ) ;
//	}
//	return 0 ;
//}

#if 0
Thread_pool_wait ( p_handle ) ;
#endif
//int main ( void ) {
//	pthread_mutex_init ( & g_m , NULL ) ;
//
//	char buf [ D_max ] [ 100 ] ;
//	int i = 0 ;
//
//	Thread_pool_handle *p_handle = Thread_pool_create ( 500 ) ;
//	if ( ! p_handle ) {
//		perror ( "exit1" ) ;
//		exit ( 1 ) ;
//	}
//
//	while ( 1 ) {
//		Begin_time ( ) ;
//		for ( i = 0 ; i < D_max ; ++ i ) {
//			snprintf ( buf [ i ] , sizeof ( buf [ i ] ) , "%d%d%d%d" , i , i , i , i ) ;
//			Thread_pool_add_work ( p_handle , Function_1 , buf [ i ] ) ;
//		}
//
//		Thread_pool_wait ( p_handle ) ;
//		printf ( "g_counter:%d\n" , g_counter ) ;
//		printf ( "%Lf\n" , End_time ( ) ) ;
//	}
//	Thread_pool_destroy ( & p_handle ) ;
//	return 0 ;
//}



#if 0
Thread_pool_wait ( p_handle ) ;
Thread_pool_destroy_no_wait ( & p_handle ) ;
#endif
//int main ( void ) {
//	pthread_mutex_init ( & g_m , NULL ) ;
//
//	char buf [ D_max ] [ 100 ] ;
//	int i = 0 ;
//	while ( 1 ) {
//		Begin_time ( ) ;
//		Thread_pool_handle *p_handle = Thread_pool_create ( 500 ) ;
//		if ( ! p_handle ) {
//			perror ( "exit1" ) ;
//			exit ( 1 ) ;
//		}
//		for ( i = 0 ; i < D_max ; ++ i ) {
//			snprintf ( buf [ i ] , sizeof ( buf [ i ] ) , "%d%d%d%d" , i , i , i , i ) ;
//			Thread_pool_add_work ( p_handle , Function_1 , buf [ i ] ) ;
//		}
//
//		Thread_pool_wait ( p_handle ) ;
//		Thread_pool_destroy_no_wait ( & p_handle ) ;
//
//		printf ( "g_counter:%d\n" , g_counter ) ;
//		printf ( "%Lf\n" , End_time ( ) ) ;
//	}
//	return 0 ;
//}

//void *Function_2 ( void * arg ) {
//	usleep ( 10000 ) ;
//	printf ( "%s\n" , ( char* ) arg ) ;
//	pthread_mutex_lock ( & g_m ) ;
//	++ g_counter ;
//	pthread_mutex_unlock ( & g_m ) ;
//	return NULL ;
//}
//int main ( void ) {
//	pthread_mutex_init ( & g_m , NULL ) ;
//	char *p_str = "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000@" ;
//	int i = 0 ;
//	while ( 1 ) {
//		Begin_time ( ) ;
//		Thread_pool_handle *p_handle = Thread_pool_create ( 500 ) ;
//		if ( ! p_handle ) {
//			perror ( "exit1" ) ;
//			exit ( 1 ) ;
//		}
//		for ( i = 0 ; i < D_max ; ++ i ) {
//			Thread_pool_add_work ( p_handle , Function_2 , p_str ) ;
//		}
//
//		Thread_pool_destroy ( & p_handle ) ;
//		printf ( "g_counter:%d\n" , g_counter ) ;
//		printf ( "%Lf\n" , End_time ( ) ) ;
//	}
//	return 0 ;
//}


//typedef struct _S_test {
//	char buf [ 100 ] ;
//} S_test ;
//
//void *Function_3 ( void * arg ) {
//	S_test* p_arg = ( S_test* ) arg ;
//	usleep ( 10000 ) ;
//	printf ( "%s\n" , p_arg->buf ) ;
//	pthread_mutex_lock ( & g_m ) ;
//	++ g_counter ;
//	pthread_mutex_unlock ( & g_m ) ;
//	return NULL ;
//}
//
//int main ( void ) {
//	pthread_mutex_init ( & g_m , NULL ) ;
//	int i = 0 ;
//	while ( 1 ) {
//		Begin_time ( ) ;
//		Thread_pool_handle *p_handle = Thread_pool_create ( 50000 ) ;
//		if ( ! p_handle ) {
//			perror ( "exit1" ) ;
//			continue ;
//		}
//		for ( i = 0 ; i < D_max ; ++ i ) {
//			S_test test ;
//			snprintf ( test.buf , sizeof ( test.buf ) , "%d%d%d%d" , i , i , i , i ) ;
//			Thread_pool_add_work ( p_handle , Function_3 , & test ) ;
//		}
//
//		Thread_pool_destroy ( & p_handle ) ;
//		printf ( "g_counter:%d\n" , g_counter ) ;
//		printf ( "%Lf\n" , End_time ( ) ) ;
//	}
//	return 0 ;
//}
#if 0
extern void Thread_pool_pause ( Thread_pool_handle *handle ) ;
extern void Thread_pool_resume ( Thread_pool_handle *handle ) ;
#endif
int main ( void ) {
	pthread_mutex_init ( & g_m , NULL ) ;

	char buf [ D_max ] [ 100 ] ;
	int i = 0 ;
	while ( 1 ) {
		Begin_time ( ) ;
		Thread_pool_handle *p_handle = Thread_pool_create ( 500 ) ;
		if ( ! p_handle ) {
			perror ( "exit1" ) ;
			exit ( 1 ) ;
		}
		for ( i = 0 ; i < D_max ; ++ i ) {
			snprintf ( buf [ i ] , sizeof ( buf [ i ] ) , "%d%d%d%d" , i , i , i , i ) ;
			Thread_pool_add_work ( p_handle , Function_1 , buf [ i ] ) ;
		}
		Thread_pool_pause ( p_handle ) ;
		Thread_pool_resume ( p_handle ) ;
		Thread_pool_destroy ( & p_handle ) ;
		printf ( "g_counter:%d\n" , g_counter ) ;
		printf ( "%Lf\n" , End_time ( ) ) ;
	}
	return 0 ;
}










