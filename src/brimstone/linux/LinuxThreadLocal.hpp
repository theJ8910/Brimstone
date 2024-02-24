/*
linux/LinuxThreadLocal.hpp
--------------------------
Copyright (c) 2024, theJ89

Description:
    LinuxThreadLocal is defined here.
*/

#ifndef BS_LINUX_LINUXTHREADLOCAL_HPP
#define BS_LINUX_LINUXTHREADLOCAL_HPP




//Includes
#include <pthread.h>    //pthread_key_t




namespace Brimstone {
namespace Private {

class LinuxThreadLocal {
public:
    LinuxThreadLocal();
    ~LinuxThreadLocal();
    void  set( void* const value );
    void* get() const;
private:
    pthread_key_t m_key;
};

}
}

#endif //BS_LINUX_LINUXTHREADLOCAL_HPP
