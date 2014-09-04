/*
Graphics.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    Graphics system
*/

#ifndef BS_GRAPHICS_HPP
#define BS_GRAPHICS_HPP




//Includes
#include <brimstone/Systems.hpp>        //AbstractSystem




namespace Brimstone {

class Graphics : public AbstractSystem {
public:
    Graphics();
    ~Graphics();

    virtual void start();
    virtual void stop();

    virtual void preframe();
    virtual void frame();
    virtual void postframe();
public:
    static Graphics* get();
private:
    static Graphics* m_singleton;
};

}




#endif //BS_GRAPHICS_HPP