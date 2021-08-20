
#include "trick/VariableServer.hh"
#include "trick/exec_proto.hh"

int Trick::VariableServer::init() {

    int ret ;

    /* start up a thread for the input processor variable server */
    if ( enabled ) {
        ret = listen_thread.check_and_move_listen_device() ;
        if ( ret != 0 ) {
            return ret ;
        }
        listen_thread.create_thread() ;
    }

    return(0) ;
}
