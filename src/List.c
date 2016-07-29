/*
 * List.c
 *
 *  Created on: 2016/7/21
 *      Author: j
 */

#include"List.h"

typedef struct _List {
	unsigned char *data ;
	struct _List *next ;
} List ;

List_handle* Create_queue_head ( void ) {
	return NULL ;
}
int Enqueue ( List_handle **handle , void *data , int data_byte ) {
	if ( ( ! handle ) || ( ! data ) || ( 0 >= data_byte ) ) return - 1 ;

	List *new_node = ( List * ) calloc ( 1 , sizeof(List) ) ;
	if ( ! new_node ) return - 2 ;

	new_node->data = ( unsigned char* ) calloc ( data_byte , sizeof(unsigned char) ) ;
	if ( ! new_node->data ) {
		free ( new_node ) ; new_node = NULL ;
		return - 3 ;
	}
	memcpy ( new_node->data , ( unsigned char* ) data , data_byte ) ;
	new_node->next = NULL ;

	if ( ! * ( List ** ) handle ) {
		* ( List ** ) handle = new_node ;
		return 1 ;
	}

	List* p_tmp = * ( List ** ) handle ;
	while ( p_tmp->next )
		p_tmp = p_tmp->next ;

	p_tmp->next = new_node ;
	return 1 ;
}

void *Dequeue ( List_handle **handle ) {
	if ( ! * handle || ! handle ) {
		return NULL ;
	}

	List **p_head = ( List ** ) handle , *p_node = * p_head ;

	unsigned char* p_data = p_node->data ;
	* p_head = ( * p_head )->next ;

	free ( p_node ) ;
	p_node = NULL ;

	return ( void* ) p_data ;
}

#if 0
typedef struct _Test_data {
	void*(*Functione) ( void* ) ;
	void *arg ;
} Test_data ;

void * Function ( void *arg ) {
	printf ( "argv :%s\n" , ( char* ) arg ) ;
	return NULL ;
}



int main ( void ) {

	Test_data data ;
	void* head = Create_queue_head ( ) ;


	while(1){
	int i = 0 ;
	for ( i = 0 ; i < 10 ; ++ i ) {

		data.Functione = Function ;
		data.arg = ( void* ) calloc ( 100 , sizeof(char) ) ;

		snprintf ( ( char* ) data.arg , 100 , "%d%d%d%d" , i , i , i , i ) ;
		if ( ! data.arg ) {
			printf ( "calloc error\n" ) ;
			break ;
		}
		Enqueue ( & head , & data , sizeof(Test_data) ) ;
	}

	Test_data* p_data = NULL ;
	while ( 1 ) {
		p_data = ( Test_data* ) Dequeue ( & head ) ;
		if ( ! p_data ) {
			printf ( "NULL\n" ) ;
			break ;
		}

		p_data->Functione ( p_data->arg ) ;
		free ( p_data->arg ) ; p_data->arg = NULL ;
		free ( p_data ) ; p_data = NULL ;
	}

	}
	return 0 ;
}
#endif
#if 0
typedef struct _Test_data {
	void*(*Functione) ( void* ) ;
	void *arg ;
} Test_data ;

void * Function ( void *arg ) {
//	printf ( "argv :%s\n" , ( char* ) arg ) ;
	return NULL ;
}

int main ( void ) {
	Test_data data ;
	void* head = Create_queue_head ( ) ;
	while ( 1 ) {
		int i = 0 ;
		for ( i = 0 ; i < 5000 ; ++ i ) {
			data.Functione = Function ;
			data.arg = ( void* ) calloc ( 100 , sizeof(char) ) ;
			snprintf ( ( char* ) data.arg , 100 , "%d%d%d%d" , i , i , i , i ) ;
			if ( ! data.arg ) {
				printf ( "calloc error\n" ) ;
				break ;
			}
			Enqueue ( & head , & data , sizeof(Test_data) ) ;
		}

		Test_data* p_data = NULL ;
		while ( 1 ) {
			p_data = ( Test_data* ) Dequeue ( & head ) ;
			if ( ! p_data ) {
//				printf ( "NULL\n" ) ;
				break ;
			}

			p_data->Functione ( p_data->arg ) ;
			free ( p_data->arg ) ;
			p_data->arg = NULL ;
			free ( p_data ) ;
			p_data = NULL ;
		}
	}
	return 0 ;
}
#endif
