/* var.c
 *
 * Instructions at the bottom of this file
 */
#define _GNU_SOURCE 1
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <jansson.h>
#include <flux/core.h>

const char default_service_name[] = "var";
static uint32_t rank, size;

static double get_power(){
	static int initialized = 0;
	if( !initialized ){
		srand( (unsigned int)13 );
		initialized = 1;
	}
	return rand()/(double)(RAND_MAX);
}

void timer_handler( flux_reactor_t *r, flux_watcher_t *w, int revents, void* arg ){
	static double min_watts=99999.0, mean_watts=0.0, max_watts=0.0;
	static uint64_t nsamples=0;
	char mykey[1025], myval[1025];
	double current_watts = get_power();
	nsamples++;

	mean_watts = mean_watts * ( (double)(nsamples-1) / nsamples ) + ( current_watts / nsamples );
	if( current_watts > max_watts ){ max_watts = current_watts; }
	if( current_watts < min_watts ){ min_watts = current_watts; }

	snprintf( mykey, 1024, "QQQ Node %06" PRIu32 " power ", rank );
	snprintf( myval, 1024, "QQQ latest %lf, max=%lf, min=%lf, mean=%lf, samples=%" PRIu64 ".", 
			current_watts, max_watts, min_watts, mean_watts, nsamples );

	flux_kvs_txn_t *t;
	flux_future_t *f;

	flux_log( (flux_t*)arg, LOG_NOTICE, mykey);
	flux_log( (flux_t*)arg, LOG_NOTICE, myval);

	t = flux_kvs_txn_create();
	flux_kvs_txn_put( t, 0, mykey, myval );
	f = flux_kvs_commit( (flux_t*)arg, "primary", 0, t );
	if (!f || flux_future_get (f, NULL) < 0) { flux_log_error ( (flux_t*)arg, "flux_service_register"); }
	flux_future_destroy (f);
	flux_kvs_txn_destroy(t);
}

int mod_main (flux_t *h, int argc, char **argv)
{
    flux_watcher_t* timer_watch_p;
    flux_future_t *f;

    // Get rank via flux_get_rank() and print results to stderr.
    flux_get_rank(h, &rank);
    flux_get_size(h, &size);
    flux_log(NULL, LOG_NOTICE, "QQQ Hello from rank %" PRIu32 " of %" PRIu32 ".\n", rank, size);
    
    // Set up and start the timer.
    timer_watch_p = flux_timer_watcher_create( flux_get_reactor(h), 1.0, 1.0, timer_handler, h); 
    if(!timer_watch_p){
        flux_log_error(h, "QQQ Failed to create timer.");
    }else{
	flux_watcher_start( timer_watch_p );
    }

    // Create the kvs namespace.  Nevermind.  Doesn't work.
    /*
    f = flux_kvs_namespace_create( h, "variorum", FLUX_USERID_UNKNOWN, 0 );
    if (!f || flux_future_get (f, NULL) < 0) { flux_log_error (h, "flux_service_register"); }
    flux_future_destroy (f);
    */

    // Start the service.
    flux_reactor_run (flux_get_reactor (h), 0);

    // Unregister service.
    f = flux_service_unregister (h, default_service_name);    
    if (!f || flux_future_get (f, NULL) < 0) { flux_log_error (h, "flux_service_register"); }
    flux_future_destroy (f);

    return 0;
}

MOD_NAME ("var");
