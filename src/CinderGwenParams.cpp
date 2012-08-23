#include "CinderGwenParams.h"

#include "Gwen/Gwen.h"
#include "Gwen/Utility.h"
#include "Gwen/Skin.h"

#include "Params/NumericStepper.h"
#include "Params/ColorPicker.h"

#include "cinder/Cinder.h"

namespace CinderGwen {
    
    
    
    GWEN_CONTROL_CONSTRUCTOR( Param ){
        
        mLabel = new Gwen::Controls::Label( this );
        mLabel->SetPos( 15, 0 );
        mLabel->SetSize( 80, 15 );
        
        mKeyframer = new Keyframer( this );
        mKeyframer->SetPos( 4, 5 );
        mKeyframer->onNewKeyFrame.Add( this, &Param::onNewKeyFrame );
        
        SetMargin( Gwen::Margin( 0, 1.5f, 0, 1.5f ) );
    }
    
    void Param::onNewKeyFrame( Gwen::Controls::Base* control ){
        std::string typeName = mControl->GetTypeName();
        
#ifdef GWEN_TIMELINE
        if( Timeline::getCurrentTimeline() ){
            if( typeName == "CheckBox" ){
                Gwen::Controls::CheckBox* base = gwen_cast< Gwen::Controls::CheckBox >( mControl );
                Animation::KeyFrameRef keyframe = Animation::KeyFrame::create( (int) base->IsChecked(), 0.0f );
                Timeline::getCurrentTimeline()->addKeyframe( mLabel->GetText().c_str(), mAnimRef, keyframe );
            }
            else if( typeName == "NumericStepper" ){
                NumericStepper* base = gwen_cast< NumericStepper >( mControl );
                Animation::KeyFrameRef keyframe = Animation::KeyFrame::create( base->getValue(), 0.0f );
                Timeline::getCurrentTimeline()->addKeyframe( mLabel->GetText().c_str(), mAnimRef, keyframe );
            }
            else if( typeName == "VectorStepper2f" ){
                VectorStepper2f* base = gwen_cast< VectorStepper2f >( mControl );
                Animation::KeyFrameRef keyframe = Animation::KeyFrame::create( base->getValue(), 0.0f );
                Timeline::getCurrentTimeline()->addKeyframe( mLabel->GetText().c_str(), mAnimRef, keyframe );
            }
            else if( typeName == "VectorStepper3f" ){
                VectorStepper3f* base = gwen_cast< VectorStepper3f >( mControl );
                Animation::KeyFrameRef keyframe = Animation::KeyFrame::create( base->getValue(), 0.0f );
                Timeline::getCurrentTimeline()->addKeyframe( mLabel->GetText().c_str(), mAnimRef, keyframe );
            }
            else if( typeName == "ColorPicker" ){
                ColorPicker* base = gwen_cast< ColorPicker >( mControl );
                Animation::KeyFrameRef keyframe = Animation::KeyFrame::create( toCinder( base->GetColor() ), 0.0f );
                Timeline::getCurrentTimeline()->addKeyframe( mLabel->GetText().c_str(), mAnimRef, keyframe );
            }
        }
#endif
                                                    
    }
    
    void Param::setLabel( std::string text ){
        mLabel->SetText( text );
        //mLabel->SizeToContents();
    }
    void Param::setControl( Gwen::Controls::Base* control ){
        mControl = control;
        //mControl->SetPos( 80, mControl->GetPos().y );

        //SizeToChildren();
        //GetParent()->SizeToChildren();
        //GetParent()->GetParent()->SizeToChildren();
        //Invalidate();
        //GetParent()->Invalidate();
        //GetParent()->GetParent()->Invalidate();
        //ci::app::console() << "their size  : " << ChildrenSize().x << " " << ChildrenSize().y << std::endl;
        //ci::app::console() << "my size     : " << GetSize().x << " " << GetSize().y << std::endl;
        //ci::app::console() << "my dad size : " << GetParent()->GetSize().x << " " << GetParent()->GetSize().y << std::endl;
        //ci::app::console() << "his dad size : " << GetParent()->GetParent()->GetSize().x << " " << GetParent()->GetParent()->GetSize().y << std::endl << std::endl << std::endl;
        //Gwen::Point size = ChildrenSize() + Gwen::Point( 10.0f, 10.0f );
        //SetSize( size.x, size.y );
    }
    
    void Param::Render( Gwen::Skin::Base* skin ){
       // BaseClass::Render( skin );
        
        std::string typeName = mControl->GetTypeName();
        
        if( typeName == "CheckBox" ){
            Gwen::Controls::CheckBox* base = gwen_cast< Gwen::Controls::CheckBox >( mControl );
            if( *mAnimRef.cast<bool>() != base->IsChecked() ){
                base->SetChecked( *mAnimRef.cast<bool>() );
            }
        }
        else if( typeName == "NumericStepper" ){
            NumericStepper* base = gwen_cast< NumericStepper >( mControl );
            if( *mAnimRef.cast<float>() != base->getValue() ){
                if( base->getUseIntegers() )
                    base->setValue( *mAnimRef.cast<int32_t>() );
                else
                    base->setValue( *mAnimRef.cast<float>() );
            }
        }
        else if( typeName == "VectorStepper2f" ){
            VectorStepper2f* base = gwen_cast< VectorStepper2f >( mControl );
            ci::Vec2f v = *mAnimRef.cast<ci::Vec2f>();
            if( v != base->getValue() ){
                base->setValue( v );
            }
        }
        else if( typeName == "VectorStepper3f" ){
            VectorStepper3f* base = gwen_cast< VectorStepper3f >( mControl ); 
            ci::Vec3f v = *mAnimRef.cast<ci::Vec3f>();
            if( v != base->getValue() ){
                base->setValue( v );
            }
        }
        else if( typeName == "ColorPicker" ){
            ColorPicker* base = gwen_cast< ColorPicker >( mControl );
            if( base->useAlpha() ){
                ci::ColorA c = *mAnimRef.cast<ci::ColorA>();
                if( c != toCinder( base->GetColor() ) ){
                    base->SetColor( toGwen( c ) );
                }
            }
            else {
                ci::Color c = *mAnimRef.cast<ci::Color>();
                if( c != toCinder( base->GetColor() ) ){
                    base->SetColor( toGwen( c ) );
                }
            }
        }
    }
    
    void Param::valueChanged( Gwen::Controls::Base* control ){
        std::string typeName = control->GetTypeName();
        //ci::app::console() << typeName <<std::endl;
        
        if( typeName == "CheckBox" ){
            Gwen::Controls::CheckBox* base = gwen_cast< Gwen::Controls::CheckBox >( control );
            mAnimRef = base->IsChecked();
        }
        else if( typeName == "NumericStepper" ){
            NumericStepper* base = gwen_cast< NumericStepper >( control );
            if( base->getUseIntegers() ){
                mAnimRef = (int32_t) base->getValue();
            }
            else{
                mAnimRef = base->getValue();
            }
        }
        else if( typeName == "VectorStepper2f" ){
            VectorStepper2f* base = gwen_cast< VectorStepper2f >( control );
            mAnimRef = base->getValue();
        }
        else if( typeName == "VectorStepper3f" ){
            VectorStepper3f* base = gwen_cast< VectorStepper3f >( control );
            mAnimRef = base->getValue();
        }
        else if( typeName == "ColorPicker" ){
            ColorPicker* base = gwen_cast< ColorPicker >( control );
            mAnimRef = toCinder( base->GetColor() );
        }
    }
    //------------------------------------------------------------------------------------------------------------------------
    
    GWEN_CONTROL_CONSTRUCTOR( ParamsWindow ){
        m_InnerPanel = new Gwen::Controls::ScrollControl( this );
        ( (Gwen::Controls::ScrollControl*) m_InnerPanel )->SetScroll( true, true );
        ( (Gwen::Controls::ScrollControl*) m_InnerPanel )->SetAutoHideBars( true );
        m_InnerPanel->Dock( Gwen::Pos::Fill );
        //new Gwen::Controls::Base( m_InnerPanel ); 
        m_InnerPanel->Show();
//        SetPadding( Gwen::Padding( 0, 0, 10, 10 ) );
        //m_InnerPanel->SetPadding( Gwen::Padding( 0, 0, 10, 10 ) );
        //m_InnerPanel->SetMargin( Gwen::Margin( 0, 0, 10, 10 ) );

        mLayoutDone = false;
    }
    
    //------------------------------------------------------------------------------------------------------------------------
    
	Params::Params(){
        mControl = NULL;
    }
    
    Params::Params( const std::string &title, const ci::Vec2i &size ){
        mWindow = new ParamsWindow( CanvasSingleton::getCanvas() );
        mWindow->SetTitle( title );
        mWindow->SetSize( size.x, size.y );
        mWindow->SetClosable( false );
        
        //mScroll = new Gwen::Controls::ScrollControl( mWindow );
        //mScroll->Dock( Gwen::Pos::Fill );
        //mScroll->SetAutoHideBars( true );
        //mScroll->SetScroll( false, true );
        //mScroll->SetInnerSize( size.x, size.y );
        //mScroll->SetSize( size.x, size.y );
        
        //new Gwen::Controls::Base( mScroll ); 
        
        mControl            = mWindow;
    }
    Params::~Params(){
        if( mControl != NULL ) {
            mControl->RemoveAllChildren();
            delete mControl;
        }
    }
    
    
    void Params::addParam( const std::string &name, ci::Anim<bool> *boolParam, int layout, float colWidth, const std::string& group, const std::string& tab ){
        Param* param = new Param( mControl );
        param->Dock( layout );
        param->setLabel( name );
        param->mAnimRef = AnimRef( boolParam );
        
        Gwen::Controls::CheckBox* control = new Gwen::Controls::CheckBox( param );
        control->onCheckChanged.Add( param, &Param::valueChanged );
        control->SetSize( 15, 15 );
        control->SetPos( colWidth, 2.5f );
        param->setControl( control );
        
        mWindow->mLayoutDone = false;
    }
    
    void Params::addParam( const std::string &name, ci::Anim<float> *floatParam, float min, float max, float step, int floatPrecision, int layout, float colWidth, const std::string& group, const std::string& tab ){
        Param* param = new Param( mControl );
        param->Dock( layout );
        param->setLabel( name );
        param->mAnimRef = AnimRef( floatParam );
        param->mOnLayout.Add( this, &Params::onLayout );
        
        NumericStepper* control = new NumericStepper( param );
        control->onChanged.Add( param, &Param::valueChanged );
        control->SetPos( colWidth, 0.0f );
        control->setMin( min );
        control->setMax( max );
        control->setStep( step );
        control->setFloatPrecision( floatPrecision );
        param->setControl( control );
        
        mWindow->mLayoutDone = false;
    }
    void Params::addParam( const std::string &name, ci::Anim<int> *intParam, int min, int max, int step, int layout, float colWidth, const std::string& group, const std::string& tab ){
        Param* param = new Param( mControl );
        param->Dock( layout );
        param->setLabel( name );
        param->mAnimRef = AnimRef( intParam );
        param->mOnLayout.Add( this, &Params::onLayout );
        
        NumericStepper* control = new NumericStepper( param );
        control->setUseIntegers( true );
        control->SetPos( colWidth, 0.0f );
        control->onChanged.Add( param, &Param::valueChanged );
        control->setMin( min );
        control->setMax( max );
        control->setStep( step );
        param->setControl( control );
        
        mWindow->mLayoutDone = false;
    }
    void Params::addParam( const std::string &name, ci::Anim<ci::Vec2f> *vectorParam, ci::Vec2f min, ci::Vec2f max, float step, int floatPrecision, int layout, float colWidth, bool vertical, const std::string& group, const std::string& tab ){
        Param* param = new Param( mControl );
        param->Dock( layout );
        param->setLabel( name );
        param->mAnimRef = AnimRef( vectorParam );
        param->mOnLayout.Add( this, &Params::onLayout );
        
        VectorStepper2f* control = new VectorStepper2f( param );
        control->onChanged.Add( param, &Param::valueChanged );
        control->SetPos( colWidth, 0.0f );
        control->setMin( min );
        control->setMax( max );
        control->setStep( step );
        control->setFloatPrecision( floatPrecision );
        if( vertical ) control->setVertical();
        param->setControl( control );
        
        mWindow->mLayoutDone = false;
    }
    void Params::addParam( const std::string &name, ci::Anim<ci::Vec3f> *vectorParam, ci::Vec3f min, ci::Vec3f max, float step, int floatPrecision, int layout, float colWidth, bool vertical, const std::string& group, const std::string& tab ){
        Param* param = new Param( mControl );
        param->Dock( layout );
        param->setLabel( name );
        param->mAnimRef = AnimRef( vectorParam );
        param->mOnLayout.Add( this, &Params::onLayout );
        
        VectorStepper3f* control = new VectorStepper3f( param );
        control->onChanged.Add( param, &Param::valueChanged );
        control->SetPos( colWidth, 0.0f );
        control->setMin( min );
        control->setMax( max );
        control->setStep( step );
        control->setFloatPrecision( floatPrecision );
        if( vertical ) control->setVertical();
        param->setControl( control );
        
        mWindow->mLayoutDone = false;
    }
    void Params::addParam( const std::string &name, ci::Anim<ci::Color> *colorParam, int layout, float colWidth, const std::string& group, const std::string& tab ){
        Param* param = new Param( mControl );
        param->setLabel( name );
        param->Dock( layout );
        param->mAnimRef = AnimRef( colorParam );
        param->mOnLayout.Add( this, &Params::onLayout );
        
        ColorPicker* control = new ColorPicker( param );
        control->onChanged.Add( param, &Param::valueChanged );
        control->SetPos( colWidth, 0.0f );
        param->setControl( control );
        
        mWindow->mLayoutDone = false;
    }
    void Params::addParam( const std::string &name, ci::Anim<ci::ColorA> *colorParam, int layout, float colWidth, const std::string& group, const std::string& tab ){
        Param* param = new Param( mControl );
        param->setLabel( name );
        param->Dock( layout );
        param->mAnimRef = AnimRef( colorParam );
        param->mOnLayout.Add( this, &Params::onLayout );
        
        ColorPicker* control = new ColorPicker( param );
        control->onChanged.Add( param, &Param::valueChanged );
        control->setUseAlpha();
        control->SetPos( colWidth, 0.0f );
        param->setControl( control );
        
        mWindow->mLayoutDone = false;
    }
    void Params::addParam( const std::string &name, std::string *strParam, const std::string &optionsStr, bool readOnly ){
        
    }
    void Params::addParam( const std::string &name, const std::vector<std::string> &enumNames, ci::Anim<int> *param, const std::string &optionsStr, bool readOnly ){
        
    }
    void Params::addSeparator( const std::string &name ){
        Gwen::Controls::Base* base = new Gwen::Controls::Base( mControl );
        base->Dock( Gwen::Pos::Top );
        base->SetSize( 200, 30 );

        //Gwen::Controls::SplitterBar* splitter = new Gwen::Controls::SplitterBar( base );
        //splitter->SetSize( 200, 5 );
        
    }
    void Params::addText( const std::string &name ){
        //Param* param = new Param( mControl );
        //param->setLabel( name );
        //param->Dock( Gwen::Pos::Top );
        
        Gwen::Controls::Base* labelContainer = new Gwen::Controls::Base( mControl );
        labelContainer->Dock( Gwen::Pos::Top );
        labelContainer->SetSize( 80, 25 );
        
        Gwen::Controls::Label* label = new Gwen::Controls::Label( labelContainer );
        label->Dock( Gwen::Pos::Top );
        label->SetSize( 80, 15 );
        label->SetText( name );
        label->MakeColorDark();
        label->SetMargin( Gwen::Margin( 2, 2, 0, 10 ) );
        
        //param->setControl( label );
    }
    void Params::addButton( const std::string &name, const std::function<void()> &callback ){
        std::shared_ptr<std::function<void ()> > callbackPtr( new std::function<void ()>( callback ) );
        mButtonCallbacks.push_back( callbackPtr );
        
        Gwen::Controls::Base* buttonContainer = new Gwen::Controls::Base( mControl );
        buttonContainer->Dock( Gwen::Pos::Top );
        buttonContainer->SetSize( 200, 20 );
        
        Gwen::Controls::Button* button = new Gwen::Controls::Button( buttonContainer );
        button->SetSize( 180, 20 );
        button->SetText( name );
        button->UserData.Set( "fn shared_ptr", callbackPtr );
        button->onPress.Add( this, &Params::buttonCallback );
        //button->SetSize( 80, 15 );
    }
    
    void Params::buttonCallback( Gwen::Controls::Base* control )
    {
        std::shared_ptr<std::function<void ()> > callbackPtr = control->UserData.Get< std::shared_ptr<std::function<void ()> > >( "fn shared_ptr" );
        callbackPtr->operator()();
    }
    
    Gwen::Controls::Base* Params::getControl(){
        return mControl;
    }
    
    
    void Params::onLayout( Gwen::Controls::Base* control ){
        //mWindow->GetChild( 0 )->SizeToChildren(false, true);
       // mWindow->SizeToChildren(false, true);
       // mWindow->Invalidate();
        //mScroll->SizeToChildren();
        //ci::app::console() << "Params sizetochilds" << std::endl;
    }
    
    //------------------------------------------------------------------------------------------------------------------------
    
    MenuEvent::MenuEvent( const std::string& name, const std::string& subname, bool checked ) : mName( name ), mSubName( subname ), mChecked( checked )
    {
    }
    
    const std::string& MenuEvent::getName()
    {
        return mName;
    }
    const std::string& MenuEvent::getSubName()
    {
        return mSubName;    
    }
    
    bool MenuEvent::isChecked()
    {
        return mChecked;    
    }
    
    Menu::Menu()
    {
        mMenuStrip = new Gwen::Controls::MenuStrip( CanvasSingleton::getCanvas() );
    }
    
    void Menu::addSeparator( const std::string &menu )
    {
        if( !mMenus.count( menu ) ){
            mMenus[ menu ] = mMenuStrip->AddItem( menu );
        }
        
        mMenus[ menu ]->GetMenu()->AddDivider();
    }
    
    void Menu::addItem( const std::string &menu, const std::string &item, std::function<void( MenuEvent )> callback, bool checkable, bool checked )
    {
        if( !mMenus.count( menu ) ){
            mMenus[ menu ] = mMenuStrip->AddItem( menu );
        }
        
        Gwen::Controls::MenuItem* menuItem = mMenus[ menu ]->GetMenu()->AddItem( item );
        menuItem->SetName( item );
        menuItem->UserData.Set( "Callback", std::shared_ptr<std::function< void( MenuEvent ) > >( new std::function< void( MenuEvent ) >( callback ) ) );
        menuItem->SetAction( this, &Menu::select );
        
        if( checkable ){
            menuItem->SetCheckable( true );
            menuItem->SetChecked( checked );
        }
    }
    
    void Menu::select( Gwen::Controls::Base* pControl )
    {
        Gwen::Controls::MenuItem* pMenuItem = (Gwen::Controls::MenuItem*) pControl;
        
        std::shared_ptr<std::function<void ( MenuEvent )> > callbackPtr = pMenuItem->UserData.Get< std::shared_ptr<std::function<void ( MenuEvent )> > >( "Callback" );
        callbackPtr->operator()( MenuEvent( "", pMenuItem->GetName(), pMenuItem->GetChecked() ) );
    }
    
    //------------------------------------------------------------------------------------------------------------------------
    
    StatusBar::StatusBar()
    {
        mStatusBar = new Gwen::Controls::StatusBar( CanvasSingleton::getCanvas() );
    }
    void StatusBar::setStatus( const std::string& status )
    {
        mStatusBar->SetText( status );
    }
    
    //------------------------------------------------------------------------------------------------------------------------
    
    Explorer::Explorer( const std::string &title, const ci::Vec2i &size )
    {
        mWindow = new ParamsWindow( CanvasSingleton::getCanvas() );
        mWindow->SetTitle( title );
        mWindow->SetSize( size.x, size.y );
        mWindow->SetClosable( false );
        
        mListBox = new Gwen::Controls::ListBox( mWindow );
        mListBox->Dock( Gwen::Pos::Fill );
        mListBox->SetAllowMultiSelect( false );
        mListBox->onRowSelected.Add( this, &Explorer::select );
    }
    
    void Explorer::addItem( const std::string &name, std::function<void( const std::string& )> callback )
    {
        mCallbacks[ name ] = std::shared_ptr<std::function< void( const std::string& ) > >( new std::function< void( const std::string& ) >( callback ) );
        mListBox->AddItem( name );
    }
    void Explorer::removeItem( const std::string & name )
    {
        if( mCallbacks.count( name ) ){
            mCallbacks[ name ] = std::shared_ptr<std::function< void( const std::string& ) > >();
            mCallbacks.erase( mCallbacks.find( name ) );
        }
    }
    void Explorer::select( Gwen::Controls::Base* pControl )
    {
        std::string name = mListBox->GetSelectedRow()->GetText( 0 ).m_String;
        mCallbacks[ name ]->operator()( name );
    }
    
    //------------------------------------------------------------------------------------------------------------------------
    
#ifdef GWEN_TIMELINE
	Timeline::Timeline(){
        mControl    = NULL;
        mTimelineWidget   = NULL;
    }
    
    Timeline::Timeline( const std::string &title, const ci::Vec2i &size, float totalTime, ci::TimelineRef timeline ){
        Gwen::Controls::WindowControl* control = new Gwen::Controls::WindowControl( CanvasSingleton::getCanvas() );
        control->SetTitle( title );
        control->SetSize( size.x, size.y );
        control->SetClosable( false );
        
        mControl    = control;
        
        mTimelineWidget   = new Widgets::TimelineWidget( mControl );
        mTimelineWidget->Dock( Gwen::Pos::Fill );
        mTimelineWidget->setTime( totalTime, 0.0f, totalTime );
        mTimelineWidget->onUpdate.Add( this, &Timeline::onUpdate );
        mTimeline = timeline;
        
        setCurrentTimeline( this );
    }
    
    void Timeline::addTrack( Animation::TrackRef track ){
        
    }
    void Timeline::addKeyframe( std::string trackName, AnimRef anim, Animation::KeyFrameRef keyframe ){
        if( mTimeline ){
            std::shared_ptr<Animation::KeyFrame> key = std::static_pointer_cast<Animation::KeyFrame>( keyframe );
            //float fl = *key->mValue.cast<float>();
            //ci::app::console() << fl << " at " << mTimelineWidget->getCurrentTime() << std::endl;
            keyframe->mTime = mTimelineWidget->getCurrentTime();
        
            bool found = false;
            int i = 0;
            for( ; i < mTrackList.size(); i++ ){
                if( mTrackList[i]->getName() == trackName ){
                    found = true;
                    break;
                }
            }
            
            if( found ) {
                mTrackList[i]->push_back( keyframe );
                mTrackList[i]->addToTimeline( mTimeline );
                mTimelineWidget->updateTrackList();
            }
            else {
                Animation::TrackRef track = Animation::ValueTrack::create( anim, trackName );
                track->push_back( keyframe );
                track->addToTimeline( mTimeline );
                mTrackList.push_back( track );
                mTimelineWidget->setTrackList( &mTrackList );
            }
        }
    }
    
    void Timeline::update(){
        mTimelineWidget->update();
        mTimeline->stepTo( mTimelineWidget->getCurrentTime() );
    }
    
    void Timeline::onUpdate( Gwen::Controls::Base* control ){
        update();
    }
    
    Timeline* Timeline::currentTimeline = NULL;
#endif
    
    
#ifdef GWEN_FLOW
    
    Flow::Flow( const std::string &title, const ci::Vec2i &size )
    {
        
        Gwen::Controls::WindowControl* control = new Gwen::Controls::WindowControl( CanvasSingleton::getCanvas() );
        control->SetTitle( title );
        control->SetSize( size.x, size.y );
        control->SetClosable( false );
        control->m_TitleBar->SetMargin( Gwen::Margin( 0, 0, 0 ,0 ) );
        control->SetPadding( Gwen::Padding( -5, 0, -5 ,0 ) );
        control->SetMouseInputEnabled( true );
        
        mControl = control;
        
		mMenu = new Gwen::Controls::MenuStrip( control );
		mMenu->Dock( Gwen::Pos::Top );
        mMenu->SetMargin( Gwen::Margin( 0, 0, 0, 0 ) );
        mMenu->SetPadding( Gwen::Padding( 0, 0, 0, 0 ) );
        
		{
			Gwen::Controls::MenuItem* pRoot = mMenu->AddItem( L"Audio" );
			pRoot->GetMenu()->AddItem( L"AudioInput", "" )->SetAction( this, &Flow::createNode );
			pRoot->GetMenu()->AddItem( L"AudioTrack", "" )->SetAction( this, &Flow::createNode );
			pRoot->GetMenu()->AddItem( L"AudioFft", "" )->SetAction( this, &Flow::createNode );
		}
        
		{
			Gwen::Controls::MenuItem* pRoot = mMenu->AddItem( L"Math" );
			pRoot->GetMenu()->AddItem( L"Average", "" )->SetAction( this, &Flow::createNode );
			pRoot->GetMenu()->AddItem( L"Constant", "" )->SetAction( this, &Flow::createNode );
		}
        
		{
			Gwen::Controls::MenuItem* pRoot = mMenu->AddItem( L"Constants" );
			pRoot->GetMenu()->AddItem( L"Int", "" )->SetAction( this, &Flow::createNode );
			pRoot->GetMenu()->AddItem( L"Float", "" )->SetAction( this, &Flow::createNode );
			pRoot->GetMenu()->AddItem( L"Bool", "" )->SetAction( this, &Flow::createNode );
		}
        
		{
			Gwen::Controls::MenuItem* pRoot = mMenu->AddItem( L"Utils" );
			pRoot->GetMenu()->AddItem( L"Range", "" )->SetAction( this, &Flow::createNode );
			pRoot->GetMenu()->AddItem( L"Visualizer", "" )->SetAction( this, &Flow::createNode );
		}
        
        mRenderCanvas = new RenderCanvas( control );
        mRenderCanvas->Dock( Gwen::Pos::Fill );
        mRenderCanvas->registerRenderFunction( this, &Flow::Render );
        mRenderCanvas->registerMouseDownFunction( this, &Flow::mouseDown );
        mRenderCanvas->registerMouseDragFunction( this, &Flow::mouseDrag );
        mRenderCanvas->registerMouseUpFunction( this, &Flow::mouseUp );
    }
    
    void Flow::Render()// Gwen::Skin::Base* skin )
    {
        mGraph.draw();
    }
    
    void Flow::mouseDown( ci::app::MouseEvent event )
    {
        mGraph.mouseDown( event );
    }
    void Flow::mouseDrag( ci::app::MouseEvent event )
    {
        mGraph.mouseDrag( event );
    }
    void Flow::mouseUp( ci::app::MouseEvent event )
    {
        mGraph.mouseUp( event );
    }
    
    void Flow::createNode( Gwen::Controls::Base* base )
    {
        if( base->GetValue().m_String == "AudioInput" ){
            mGraph.addNode( AudioInputNode::createRef( ci::Vec2f( 10, 10 ), ci::Vec2f( 150, 39 ) ) );
        }
        else if( base->GetValue().m_String == "AudioTrack" ){
            mGraph.addNode( AudioTrackNode::createRef( ci::Vec2f( 10, 10 ), ci::Vec2f( 150, 39 ) ) );
        }
        else if( base->GetValue().m_String == "AudioFft" ){
            mGraph.addNode( AudioFftNode::createRef( ci::Vec2f( 10, 10 ), ci::Vec2f( 150, 39 ) ) );
        }
        else if( base->GetValue().m_String == "Average" ){
            mGraph.addNode( AverageNode::createRef( ci::Vec2f( 10, 10 ), ci::Vec2f( 150, 39 ) ) );
        }
        else if( base->GetValue().m_String == "Int" ){
            mGraph.addNode( ConstantNode::createRef( 0, ci::Vec2f( 10, 10 ), ci::Vec2f( 150, 39 ) ) );
        }
        else if( base->GetValue().m_String == "Float" ){
            mGraph.addNode( ConstantNode::createRef( 0.0f, ci::Vec2f( 10, 10 ), ci::Vec2f( 150, 39 ) ) );
        }
        else if( base->GetValue().m_String == "Bool" ){
            mGraph.addNode( ConstantNode::createRef( false, ci::Vec2f( 10, 10 ), ci::Vec2f( 150, 39 ) ) );
        }
        else if( base->GetValue().m_String == "Range" ){
            mGraph.addNode( RangeNode::createRef( ci::Vec2f( 10, 10 ), ci::Vec2f( 150, 39 ) ) );
        }
        else if( base->GetValue().m_String == "Visualizer" ){
            mGraph.addNode( VisualizerNode::createRef( ci::Vec2f( 10, 10 ), ci::Vec2f( 150, 39 ) ) );
        }
    }
        
#endif
    
    
};