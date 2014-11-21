/*
ui/IPanel.hpp
-----------------------
Copyright (c) 2014, theJ89

Description:
    IPanel is defined here.
    IPanel is an interface for panels, of which all of Brimstone's UI elements derive from.
*/

#ifndef BS_UI_IPANEL_HPP
#define BS_UI_IPANEL_HPP




//Includes
#include <brimstone/Point.hpp>                  //Brimstone::Point2i
#include <brimstone/Size.hpp>                   //Brimstone::Size2i
#include <brimstone/Bounds.hpp>                 //Brimstone::Bounds2i
#include <brimstone/ui/Events.hpp>              //Brimstone::IEvent
//#include <brimstone/graphics/IRenderable.hpp>   //Brimstone::IRenderable




namespace Brimstone {
class IContainer;
class RootPanel;
class IPanel : /*public IRenderable,*/ public virtual IEventHandler {
public:
    virtual void        setParent( IContainer* parent )                 = 0;
    virtual IContainer* getParent() const                               = 0;

    virtual RootPanel*  getRootPanel() const                            = 0;
    virtual IPanel*     getPanelAt( const Point2i localPos )            = 0;

    virtual void     setPos( const Point2i pos )                        = 0;
    virtual Point2i  getPos() const                                     = 0;
    virtual void     setSize( const Size2i size )                       = 0;
    virtual Size2i   getSize() const                                    = 0;
    virtual void     setVisible( const bool visible )                   = 0;
    virtual bool     isVisible() const                                  = 0;

    virtual void     focus()                                            = 0;
    virtual void     blur()                                             = 0;
    virtual bool     hasFocus() const                                   = 0;
    virtual void     setFocusable( const bool focusable )               = 0;
    virtual bool     isFocusable() const                                = 0;

    virtual void     captureMouse()                                     = 0;
    virtual void     releaseMouse()                                     = 0;
    virtual bool     isCapturingMouse() const                           = 0;

    virtual Point2i  localToWindow( const Point2i localCoords ) const   = 0;
    virtual Point2i  windowToLocal( const Point2i windowCoords ) const  = 0;
};

}




#endif //BS_UI_IPANEL_HPP