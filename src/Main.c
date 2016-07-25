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































