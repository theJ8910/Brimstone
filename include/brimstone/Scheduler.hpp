/*
Scheduler.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    This system schedules the execution of frames for other systems.
*/
#ifndef BS_SCHEDULER_HPP
#define BS_SCHEDULER_HPP




//Includes
#include <thread>                   //std::thread

#include <brimstone/Systems.hpp>    //AbstractSystem




namespace Brimstone {

class Scheduler : public AbstractSystem {
public:
    Scheduler();
    ~Scheduler();
    virtual void start();
    virtual void stop();

    virtual void preframe();
    virtual void frame();
    virtual void postframe();

    void setFrameDelay( int delayInMilliseconds );
    int  getFrameDelay() const;

            void run();
private:
    std::thread m_thread;
    bool        m_running;
    int         m_delay;
public:
    static Scheduler* get();
private:
    static Scheduler* m_singleton;
};

}

#endif //BS_SCHEDULER_HPP