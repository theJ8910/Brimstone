/*
Audio.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Audio system
*/

#ifndef BS_AUDIO_HPP
#define BS_AUDIO_HPP




//Includes
#include <brimstone/Systems.hpp>        //AbstractSystem




namespace Brimstone {

class Audio : public AbstractSystem {
public:
    Audio();
    ~Audio();

    virtual void start();
    virtual void stop();

    virtual void preframe();
    virtual void frame();
    virtual void postframe();
public:
    static Audio* get();
private:
    static Audio* m_singleton;
};

}




#endif //BS_AUDIO_HPP