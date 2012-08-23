#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"

#include "Gwen/Gwen.h"
#include "Gwen/Skins/Simple.h"
#include "Gwen/Skins/TexturedBase.h"

#include "Gwen/BaseRender.h"
#include "Gwen/Controls/Base.h"
#include "Gwen/Controls/TextBox.h"
#include "Gwen/Controls/Menu.h"
#include "Gwen/Controls/MenuItem.h"

#include "CinderGwenParams.h"

namespace CinderGwen {
    
    
    class RenderCanvas : public Gwen::Controls::Base
    {
        GWEN_CONTROL_INLINE( RenderCanvas, Gwen::Controls::Base )
        {
            SetKeyboardInputEnabled( true );
            SetMouseInputEnabled( true );
            mMouseDown = false;
        }
        
        
        virtual void Render( Gwen::Skin::Base* skin )
        {
            glDisable( GL_TEXTURE_2D );
            Gwen::Rect bounds = GetRenderBounds();
            skin->GetRender()->Translate( bounds );
            
            glColor4ub( 255, 255, 255, 255 );
            ci::gl::color( ci::ColorA::white() );
            ci::gl::pushMatrices();
            ci::gl::translate( bounds.x, bounds.y );
            
            if( mRenderFct ){
                mRenderFct();
            }
            
            ci::gl::popMatrices();
        }
        
        virtual void OnMouseMoved( int x, int y, int deltaX, int deltaY ){
            if( mMouseDown ){
                Gwen::Point p = CanvasPosToLocal( Gwen::Point( x, y ) );
                ci::app::MouseEvent event( ci::app::MouseEvent::LEFT_DOWN, p.x, p.y, 0, 0, 0 );
                mMouseDragFct( event );
            }
        }
        virtual void OnMouseClickLeft( int x, int y, bool bDown ){
            Gwen::Point p = CanvasPosToLocal( Gwen::Point( x, y ) );
            ci::app::MouseEvent event( ci::app::MouseEvent::LEFT_DOWN, p.x, p.y, 0, 0, 0 );
            if( bDown )
                mMouseDownFct( event );
            else
                mMouseUpFct( event );
            
            mMouseDown = bDown;
        };
        virtual void OnMouseClickRight( int x, int y, bool bDown ){
            Gwen::Point p = CanvasPosToLocal( Gwen::Point( x, y ) );
            ci::app::MouseEvent event( ci::app::MouseEvent::RIGHT_DOWN, p.x, p.y, 0, 0, 0 );
            if( bDown )
                mMouseDownFct( event );
            else
                mMouseUpFct( event );
            
            mMouseDown = bDown;
        }
        
        
        template<typename T>
        void registerRenderFunction( T *obj, void ( T::*callback )() ){
            mRenderFct = std::bind( callback, obj );
        }
        void unregisterRenderFunction(){
            mRenderFct = std::function< void() >();
        }
        
        template<typename T>
        void registerMouseDownFunction( T *obj, void ( T::*callback )( ci::app::MouseEvent ) ){
            mMouseDownFct = std::bind1st( std::mem_fun( callback ), obj );
        }
        void unregisterMouseDownFunction(){
            mMouseDownFct = std::function< void(ci::app::MouseEvent) >();
        }
        
        template<typename T>
        void registerMouseUpFunction( T *obj, void ( T::*callback )( ci::app::MouseEvent ) ){
            mMouseUpFct = std::bind1st( std::mem_fun( callback ), obj );
        }
        void unregisterMouseUpFunction(){
            mMouseUpFct = std::function< void(ci::app::MouseEvent) >();
        }
        
        template<typename T>
        void registerMouseDragFunction( T *obj, void ( T::*callback )( ci::app::MouseEvent ) ){
            mMouseDragFct = std::bind1st( std::mem_fun( callback ), obj );
        }
        void unregisterMouseDragFunction(){
            mMouseDragFct = std::function< void(ci::app::MouseEvent) >();
        }
        
    protected:
        bool                                        mMouseDown;
        std::function<void()>                       mRenderFct;
        std::function<void(ci::app::MouseEvent)>    mMouseDownFct;
        std::function<void(ci::app::MouseEvent)>    mMouseUpFct;
        std::function<void(ci::app::MouseEvent)>    mMouseDragFct;
        
    };
   

};