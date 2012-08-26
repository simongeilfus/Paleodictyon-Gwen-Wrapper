#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"

#include "Gwen/Gwen.h"
#include "Gwen/Skins/Simple.h"
#include "Gwen/Skins/TexturedBase.h"

#include "Gwen/Controls.h"
#include "Gwen/Controls/WindowControl.h"
#include "Gwen/Controls/Menu.h"
#include "Gwen/Controls/MenuItem.h"

#include "CinderGwenRenderer.h"


#include <vector>

namespace CinderGwen {
	
	static inline Gwen::Point toGwen( const ci::Vec2f& v ){ return Gwen::Point( v.x, v.y ); }
	static inline Gwen::Color toGwen( const ci::ColorA& c ){ return Gwen::Color( c.r * 255, c.g * 255, c.b * 255, c.a * 255 ); }
	static inline Gwen::Rect toGwen( const ci::Rectf& r ){ return Gwen::Rect( r.x1, r.y1, r.getWidth(), r.getHeight() ); }
    
	static inline ci::Vec2f toCinder( const Gwen::Point& v ){ return ci::Vec2f( v.x, v.y ); }
	static inline ci::ColorA toCinder( const Gwen::Color& c ){ return ci::ColorA( (float) c.r / 255.0f, (float) c.g / 255.0f, (float) c.b / 255.0f, (float) c.a / 255.0f ); }
	static inline ci::Rectf toCinder( const Gwen::Rect& r ){ return ci::Rectf( r.x, r.y, r.x + r.w, r.y + r.h ); }
    
    //------------------------------------------------------------------------------------------------------------------------------------------------------

	class Canvas {
	public:
		Canvas();
		Canvas( int width, int height, Gwen::Renderer::Base* renderer = NULL, Gwen::Skin::Base* skin = NULL, bool doRegisterEvents = true );//new Gwen::Skin::TexturedBase() );
		~Canvas();
		
		void registerEvents();
		void unregisterEvents();

		void draw();

		Gwen::Controls::Canvas* getCanvas();
		Gwen::Skin::Base*		getSkin();
		Gwen::Renderer::Base*	getRenderer();

	private:

		bool mouseDown( ci::app::MouseEvent event );	
		bool mouseDrag( ci::app::MouseEvent event );	
		bool mouseUp( ci::app::MouseEvent event );	
		bool mouseMove( ci::app::MouseEvent event );	
		bool mouseWheel( ci::app::MouseEvent event );
		bool keyDown( ci::app::KeyEvent event );
		bool keyUp( ci::app::KeyEvent event );
		bool resizeEvent( ci::app::ResizeEvent event );

		int getGwenKey( ci::app::KeyEvent event );
		
		Gwen::Controls::Canvas*			mCanvas;
		Gwen::Skin::Base*				mSkin;
		Gwen::Renderer::Base*			mRenderer;
		ci::Vec2f						mLastMousePos;
		std::vector< ci::CallbackId >	mCallbackIds;
	};
    
    //------------------------------------------------------------------------------------------------------------------------------------------------------
    
    
    class CanvasSingleton {
    public:
        ~CanvasSingleton();
        
        static CanvasSingleton*         getInstance();
        static Canvas*         getCanvasInstance();
        
		static Gwen::Controls::Canvas*  getCanvas();
		static Gwen::Skin::Base*        getSkin();
		static Gwen::Renderer::Base*    getRenderer();
        
		static void    setSkin( Gwen::Skin::Base* skin );
		static void    setRenderer( Gwen::Renderer::Base* renderer );
        
        static void                     draw();
        
    private:
        CanvasSingleton();
        static void check();
        
        static CanvasSingleton*  mInstance;
        
        Canvas*     mGwenCanvas;
        Renderer*   mGwenRenderer;
    };
    
    static void draw(){ CanvasSingleton::draw(); }
    static Gwen::Controls::Canvas* getCanvas(){ return CanvasSingleton::getCanvas(); }
    static Gwen::Skin::Base* getSkin(){ return CanvasSingleton::getSkin(); }
    static Gwen::Renderer::Base* getRenderer(){ return CanvasSingleton::getRenderer(); }
    
    //------------------------------------------------------------------------------------------------------------------------------------------------------
    
    class ContextMenu {
    public:
        ~ContextMenu();
        
        static ContextMenu*             getInstance();
        static Gwen::Controls::Menu*    getMenu();
        
    private:
        ContextMenu();
        static void check();
        
        static ContextMenu* mInstance;
        
        Gwen::Controls::Menu* mMenu;
    };
    
    
};