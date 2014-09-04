/*
Input.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Input system
*/

#ifndef BS_INPUT_HPP
#define BS_INPUT_HPP

//Includes
#include <brimstone/Systems.hpp>        //AbstractSystem

namespace Brimstone {

class Input : public AbstractSystem {
public:
    Input();
    ~Input();

    virtual void start();
    virtual void stop();

    virtual void preframe();
    virtual void frame();
    virtual void postframe();
public:
    static Input* get();
private:
    static Input* m_singleton;
};

}

#endif //BS_INPUT_HPP