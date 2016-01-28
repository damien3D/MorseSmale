#include "stdafx.h"
#include "watchtime.h"
#include <time.h>
#include <iostream>

//_________________________________________________________


SystemWatchTime::SystemWatchTime() {
#ifdef WIN32
        start_ = GetTickCount() ;
#else
        clock_t init_user = times(&start_) ;
        while((start_user_ = times(&start_)) == init_user) ;
#endif
}

double SystemWatchTime::elapsed_user_time() const {
#ifdef WIN32
        return double(GetTickCount() - start_) / 1000.0 ;
#else
        clock_t end_user ;
        tms end ;
        end_user = times(&end) ;
        return double(end_user - start_user_) / 100.0 ;
#endif
}

double SystemWatchTime::now() {
#ifdef WIN32
        return double(GetTickCount()) / 1000.0 ;
#else
        clock_t end_user ;
        tms now_tms ;
        return double(times(&now_tms)) / 100.0 ;
#endif
}

void SystemWatchTime::print_elapsed_time() {
#ifdef WIN32
        std::cout << "---- Times (seconds) ----" << std::endl ;
        std::cout << "  Elapsed time: " << elapsed_user_time() << std::endl ;
#else
        clock_t end_user ;
        tms end ;
        end_user = times(&end) ;

        std::cout << "---- Times (seconds) ----" << std::endl ;
        std::cout << "  Real time: " 
                << double(end_user - start_user_) / 100.0 << std::endl ;

        std::cout << "  User time: " 
                << double(end.tms_utime - start_.tms_utime) / 100.0 
                << std::endl ;

        std::cout << "  Syst time: " 
                << double(end.tms_stime - start_.tms_stime) / 100.0 
                << std::endl ;    
#endif    
}