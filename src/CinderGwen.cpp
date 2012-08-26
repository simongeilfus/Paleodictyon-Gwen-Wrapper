#include "CinderGwen.h"
#include "CinderGwenRenderer.h"

namespace CinderGwen {
	
	Canvas::Canvas(){
	}

	Canvas::Canvas( int width, int height, Gwen::Renderer::Base* renderer, Gwen::Skin::Base* skin, bool doRegisterEvents ){

		if( renderer == NULL ) mRenderer = new CinderGwen::Renderer();
		else mRenderer = renderer;
		
		if( skin == NULL ){
			mSkin = new Gwen::Skin::TexturedBase( mRenderer );
			mSkin->SetRender( mRenderer );
			static_cast<Gwen::Skin::TexturedBase*>(mSkin)->Init( "DefaultSkin.png" );
		}
		else {
			mSkin = skin;
			mSkin->SetRender( mRenderer );
		}

		
		mCanvas = new Gwen::Controls::Canvas( mSkin );		
		mCanvas->Initialize();
		mCanvas->SetSize( width, height );

		mLastMousePos = ci::app::AppBasic::get()->getMousePos();

		registerEvents();
	}
	
	Canvas::~Canvas(){
        unregisterEvents();
        mCanvas->ReleaseChildren();
		delete mCanvas;
	}

		
	void Canvas::registerEvents(){
		mCallbackIds.push_back( ci::app::AppBasic::get()->registerMouseDown( this, &Canvas::mouseDown ) );
		mCallbackIds.push_back( ci::app::AppBasic::get()->registerMouseUp( this, &Canvas::mouseUp ) );
		mCallbackIds.push_back( ci::app::AppBasic::get()->registerMouseMove( this, &Canvas::mouseMove ) );
		mCallbackIds.push_back( ci::app::AppBasic::get()->registerMouseDrag( this, &Canvas::mouseDrag ) );
		mCallbackIds.push_back( ci::app::AppBasic::get()->registerMouseWheel( this, &Canvas::mouseWheel ) );
		mCallbackIds.push_back( ci::app::AppBasic::get()->registerKeyDown( this, &Canvas::keyDown ) );
		mCallbackIds.push_back( ci::app::AppBasic::get()->registerKeyUp( this, &Canvas::keyUp ) );
		mCallbackIds.push_back( ci::app::AppBasic::get()->registerResize( this, &Canvas::resizeEvent ) );
	}
	void Canvas::unregisterEvents(){
		if( mCallbackIds.size() ){
			ci::app::AppBasic::get()->unregisterMouseDown(	mCallbackIds[ 0 ] );
			ci::app::AppBasic::get()->unregisterMouseUp(	mCallbackIds[ 1 ] );
			ci::app::AppBasic::get()->unregisterMouseMove(	mCallbackIds[ 2 ] );
			ci::app::AppBasic::get()->unregisterMouseDrag(	mCallbackIds[ 3 ] );
			ci::app::AppBasic::get()->unregisterMouseWheel( mCallbackIds[ 4 ] );
			ci::app::AppBasic::get()->unregisterKeyDown(	mCallbackIds[ 5 ] );
			ci::app::AppBasic::get()->unregisterKeyUp(		mCallbackIds[ 6 ] );
		}
	}

	void Canvas::draw(){
		mCanvas->RenderCanvas();
	}
	
	Gwen::Controls::Canvas* Canvas::getCanvas(){
		return mCanvas;
	}
	Gwen::Skin::Base* Canvas::getSkin(){
		return mSkin;
	}
	Gwen::Renderer::Base* Canvas::getRenderer(){
		return mRenderer;
	}

	bool Canvas::mouseDown( ci::app::MouseEvent event ){
		return mCanvas->InputMouseButton( event.isLeft() ? 0 : ( event.isRight() ? 1 : 2 ), true );
	}
	bool Canvas::mouseUp( ci::app::MouseEvent event ){
		return mCanvas->InputMouseButton( event.isLeft() ? 0 : ( event.isRight() ? 1 : 2 ), false );
	}
	bool Canvas::mouseDrag( ci::app::MouseEvent event ){
		ci::Vec2f delta = mLastMousePos - ci::Vec2f( event.getPos() ) ;
		//mCanvas->InputMouseButton( event.isLeft() ? 0 : ( event.isRight() ? 1 : 2 ), true );
		mLastMousePos = event.getPos();
		return mCanvas->InputMouseMoved( event.getX(), event.getY(), delta.x, delta.y );
	}
	bool Canvas::mouseMove( ci::app::MouseEvent event ){
		ci::Vec2f delta = mLastMousePos - ci::Vec2f( event.getPos() ) ;
		mLastMousePos = event.getPos();
		return mCanvas->InputMouseMoved( event.getX(), event.getY(), delta.x, delta.y );
	}
	bool Canvas::mouseWheel( ci::app::MouseEvent event ){
		return mCanvas->InputMouseWheel( event.getWheelIncrement() * 95 );
	}

	int Canvas::getGwenKey( ci::app::KeyEvent event ){
		int key = -1;
		if ( event.isShiftDown() ) key = Gwen::Key::Shift;
		else if ( event.getCode() == ci::app::KeyEvent::KEY_RETURN ) key = Gwen::Key::Return;
		else if ( event.getCode() == ci::app::KeyEvent::KEY_BACKSPACE ) key = Gwen::Key::Backspace;
		else if ( event.getCode() == ci::app::KeyEvent::KEY_DELETE ) key = Gwen::Key::Delete;
		else if ( event.getCode() == ci::app::KeyEvent::KEY_LEFT ) key = Gwen::Key::Left;
		else if ( event.getCode() == ci::app::KeyEvent::KEY_RIGHT ) key = Gwen::Key::Right;
		else if ( event.getCode() == ci::app::KeyEvent::KEY_TAB ) key = Gwen::Key::Tab;
		else if ( event.getCode() == ci::app::KeyEvent::KEY_SPACE ) key = Gwen::Key::Space;
		else if ( event.getCode() == ci::app::KeyEvent::KEY_HOME ) key = Gwen::Key::Home;
		else if ( event.getCode() == ci::app::KeyEvent::KEY_END ) key = Gwen::Key::End;
		else if ( event.isControlDown() || event.isMetaDown() ){
            key = Gwen::Key::Control;
        }
		else if ( event.getCode() == ci::app::KeyEvent::KEY_SPACE ) key = Gwen::Key::Space;
		else if ( event.getCode() == ci::app::KeyEvent::KEY_UP ) key = Gwen::Key::Up;
		else if ( event.getCode() == ci::app::KeyEvent::KEY_DOWN ) key = Gwen::Key::Down;
        
		return key;
	}
	bool Canvas::keyDown( ci::app::KeyEvent event ){
		int key = getGwenKey( event );
		if( key == -1 ) {
			key = event.getNativeKeyCode();
			Gwen::UnicodeChar chr = (Gwen::UnicodeChar) event.getChar();
			return mCanvas->InputCharacter( chr );
		}
		return mCanvas->InputKey(  key, true );
	}
	bool Canvas::keyUp( ci::app::KeyEvent event ){
		int key = getGwenKey( event );
		if( key == -1 ) key = event.getCode();
		return mCanvas->InputKey(  key, false );
	}

	bool Canvas::resizeEvent( ci::app::ResizeEvent event ){
		mCanvas->SetSize( event.getWidth(), event.getHeight() );
		return false;
	}
    
    
    //------------------------------------------------------------------------------------------------------------------------------------------------------
    
    
    CanvasSingleton* CanvasSingleton::getInstance(){
        check();
        return mInstance;
    }
    
    Canvas* CanvasSingleton::getCanvasInstance()
    {
        check();
        return mInstance->mGwenCanvas;
    }
    Gwen::Controls::Canvas* CanvasSingleton::getCanvas(){
        check();
        return mInstance->mGwenCanvas->getCanvas();
        
    }
    Gwen::Skin::Base* CanvasSingleton::getSkin(){
        check();
        return mInstance->mGwenCanvas->getSkin();
    }
    Gwen::Renderer::Base* CanvasSingleton::getRenderer(){
        check();
        return mInstance->mGwenRenderer;
    }
    
    void CanvasSingleton::setSkin( Gwen::Skin::Base* skin )
    {
        check();
       // mInstance->m
    }
    void CanvasSingleton::setRenderer( Gwen::Renderer::Base* renderer )
    {
        
    }
    void CanvasSingleton::draw(){
        check();
        mInstance->mGwenCanvas->draw();
    }
    
    CanvasSingleton::CanvasSingleton(){
        mGwenRenderer	= new Renderer();
        Gwen::Skin::TexturedBase* skin = new Gwen::Skin::TexturedBase( mGwenRenderer );
        skin->Init( "DefaultSkin.png" );
        //skin->SetDefaultFont( L"Helvetica-Light", 11 );
        mGwenCanvas		= new Canvas( ci::app::getWindowWidth(), ci::app::getWindowHeight(), mGwenRenderer, skin, true );        
    }
    CanvasSingleton::~CanvasSingleton(){
        delete mGwenRenderer;
        delete mGwenCanvas;
    }
    void CanvasSingleton::check(){
        if( !mInstance ) mInstance = new CanvasSingleton();
    }
    
    CanvasSingleton* CanvasSingleton::mInstance = NULL;    
    
    
    //------------------------------------------------------------------------------------------------------------------------------------------------------
    
    
    ContextMenu* ContextMenu::getInstance(){
        check();
        return mInstance;
    }
    Gwen::Controls::Menu* ContextMenu::getMenu(){
        check();
        return mInstance->mMenu;        
    }
    
    ContextMenu::~ContextMenu(){
        //mMenu
    }
    
    ContextMenu::ContextMenu(){
        mMenu = new Gwen::Controls::Menu( getCanvas() );
        mMenu->CloseAll();
        mMenu->Close();
       // mMenu->SetDisableIconMargin( true );
    }
    
    void ContextMenu::check(){
        if( !mInstance ) mInstance = new ContextMenu();
    }
    
    ContextMenu* ContextMenu::mInstance = NULL; 
    
};