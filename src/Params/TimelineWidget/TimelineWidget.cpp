#include "TimelineWidget.h"
#include "CinderGwen.h"


namespace CinderGwen { namespace Widgets {

    
    GWEN_CONTROL_CONSTRUCTOR( TimelineButton )
    {
        mDown = false;
        mType = 2;
    }
    int TimelineButton::getType()
    {
        return mType;
    }
    void TimelineButton::setType( int type )
    {
        mType = type;
    }
    void TimelineButton::Render( Gwen::Skin::Base* skin )
    {
        if( mType == PLAY ){
            if( GetToggleState() ) ( (Gwen::Skin::TexturedBase*) skin )->Textures.TimelineButtons.PlayDown.Draw( skin->GetRender(), GetRenderBounds() );
            else ( (Gwen::Skin::TexturedBase*) skin )->Textures.TimelineButtons.Play.Draw( skin->GetRender(), GetRenderBounds() );
        }
        else if( !mDown ){
            if( mType == START ) ( (Gwen::Skin::TexturedBase*) skin )->Textures.TimelineButtons.Start.Draw( skin->GetRender(), GetRenderBounds() );
            else if( mType == PREV ) ( (Gwen::Skin::TexturedBase*) skin )->Textures.TimelineButtons.Previous.Draw( skin->GetRender(), GetRenderBounds() );
            else if( mType == NEXT  ) ( (Gwen::Skin::TexturedBase*) skin )->Textures.TimelineButtons.Next.Draw( skin->GetRender(), GetRenderBounds() );
            else if( mType == END ) ( (Gwen::Skin::TexturedBase*) skin )->Textures.TimelineButtons.End.Draw( skin->GetRender(), GetRenderBounds() );
        }
        else {
            if( mType == START ) ( (Gwen::Skin::TexturedBase*) skin )->Textures.TimelineButtons.StartDown.Draw( skin->GetRender(), GetRenderBounds() );
            else if( mType == PREV ) ( (Gwen::Skin::TexturedBase*) skin )->Textures.TimelineButtons.PreviousDown.Draw( skin->GetRender(), GetRenderBounds() );
            else if( mType == NEXT  ) ( (Gwen::Skin::TexturedBase*) skin )->Textures.TimelineButtons.NextDown.Draw( skin->GetRender(), GetRenderBounds() );
            else if( mType == END ) ( (Gwen::Skin::TexturedBase*) skin )->Textures.TimelineButtons.EndDown.Draw( skin->GetRender(), GetRenderBounds() );
        }
    }
    
    void TimelineButton::OnMouseClickLeft(int x, int y, bool bDown )
    {
        BaseClass::OnMouseClickLeft( x, y, bDown );
        
        mDown = bDown;
    }
    
    
	GWEN_CONTROL_CONSTRUCTOR( TimelineWidget )
    {
		SetMouseInputEnabled( true );
		SetSize( 1000, 400 );

	
		/*Gwen::Controls::TreeControl* ctrl = new Gwen::Controls::TreeControl( this );

		ctrl->AllowMultiSelect( true );

		ctrl->AddNode( L"Node One" );
		Gwen::Controls::TreeNode* pNode = ctrl->AddNode( L"Node Two" );
		pNode->AddNode( L"Node Two Inside" );
		pNode->AddNode( L"Eyes" );
		Gwen::Controls::TreeNode* pNodeTwo = pNode->AddNode( L"Brown" )->AddNode( L"Node Two Inside" )->AddNode( L"Eyes" );
		pNodeTwo->AddNode( L"Brown" );
		pNodeTwo->AddNode( L"Green" );
		pNodeTwo->AddNode( L"Slime" );
		pNodeTwo->AddNode( L"Grass" );
		pNodeTwo->AddNode( L"Pipe" );
		pNode->AddNode( L"More" );
		pNode->AddNode( L"Nodes" );

		ctrl->AddNode( L"Node Three" );
		ctrl->Dock( Gwen::Pos::Left );
	
		ctrl->SetPos( 5, 60 );
		ctrl->SetBounds( 240, 30, 200, 200 );*/

	
		/*Gwen::Controls::PropertyTree* ptree = new Gwen::Controls::PropertyTree( this );
		ptree->SetBounds( 200, 10, 200, 200 );
		ptree->Dock( Gwen::Pos::Left );
	
		{
			Gwen::Controls::Properties* props = ptree->Add( L"Item One" );
			props->Add( L"Middle Name" );
			props->Add( L"Last Name" );
			props->Add( L"Four" );
		
				{
					{
						Gwen::Controls::PropertyRow* pRow = props->Add( L"First Name" );
					}

					props->Add( L"Middle Name" );
					props->Add( L"Last Name" );
				}
		}

		{
			Gwen::Controls::Properties* props = ptree->Add( L"Item Two" );
			props->Add( L"More Items" );
			props->Add( L"Bacon", new Gwen::Controls::Property::Checkbox( props ), L"1" );
			props->Add( L"To Fill" );
			props->Add( L"Colour", new Gwen::Controls::Property::ColorSelector( props ), L"255 0 0" );
			props->Add( L"Out Here" );
		}

		ptree->ExpandAll();*/



		/*
		
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// None
EaseNone


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Quadratic
EaseInQuad
EaseOutQuad
EaseInOutQuad
EaseOutInQuad

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Cubic
EaseInCubic
EaseOutCubic
EaseInOutCubic
EaseOutInCubic

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Quartic
EaseInQuart
EaseOutQuart
EaseInOutQuart
EaseOutInQuart

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Quintic
EaseInQuint
EaseOutQuint
EaseInOutQuint
EaseOutInQuint

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Sine
EaseInSine
EaseOutSine
EaseInOutSine
EaseOutInSine

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Exponential
EaseInExpo
EaseOutExpo
EaseInOutExpo
EaseOutInExpo

// Circular
EaseInCirc
EaseOutCirc
EaseInOutCirc
EaseOutInCirc


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Bounce
EaseInBounce
EaseOutBounce
EaseInOutBounce
EaseOutInBounce

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Back
EaseInBack
EaseOutBack
EaseInOutBack
EaseOutInBack



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Elastic
EaseInElastic
EaseOutElastic
EaseInOutElastic
EaseOutInElastic


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Atan
EaseInAtan
EaseOutAtan
EaseInOutAtan


*/
		
		// Add a menu
		//------------------------------------------------------------------
		mMenu = new Gwen::Controls::MenuStrip(this );
		mMenu->Dock( Gwen::Pos::Top );

		{
			Gwen::Controls::MenuItem* pRoot = mMenu->AddItem( L"Edit" );
			pRoot->GetMenu()->AddItem( L"Fold All", "" )->SetAction( this, &TimelineWidget::foldAll );
			pRoot->GetMenu()->AddItem( L"Unfold All", "" )->SetAction( this, &TimelineWidget::unfoldAll );
		}
		{
			Gwen::Controls::MenuItem* pRoot = mMenu->AddItem( L"Track" );
			pRoot->GetMenu()->AddItem( L"Delete Track", "" )->SetAction( this, &TimelineWidget::deleteTrack );
			pRoot->GetMenu()->AddItem( L"Delete Unused Tracks", "" )->SetAction( this, &TimelineWidget::deleteUnusedTracks );		
			pRoot->GetMenu()->AddItem( L"Clean Tracks", "" )->SetAction( this, &TimelineWidget::cleanTracks );		
		}
		{
			Gwen::Controls::MenuItem* pRoot = mMenu->AddItem( L"Key" );
			pRoot->GetMenu()->AddItem( L"Delete Key", "" )->SetAction( this, &TimelineWidget::deleteKey );
			pRoot->GetMenu()->AddDivider();
			pRoot->GetMenu()->AddItem( L"Goto Next Key", "" )->SetAction( this, &TimelineWidget::gotoNextKey );
			pRoot->GetMenu()->AddItem( L"Goto Previous Key", "" )->SetAction( this, &TimelineWidget::gotoPreviousKey );
			pRoot->GetMenu()->AddDivider();
			pRoot->GetMenu()->AddItem( L"Goto Start Time", "" )->SetAction( this, &TimelineWidget::gotoStartTime );
			pRoot->GetMenu()->AddItem( L"Goto End Time", "" )->SetAction( this, &TimelineWidget::gotoEndTime );
			pRoot->GetMenu()->AddItem( L"Goto Current Time", "" )->SetAction( this, &TimelineWidget::gotoCurrentTime );
		}
		{
			Gwen::Controls::MenuItem* pRoot = mMenu->AddItem( L"Curves" );
			{
			Gwen::Controls::MenuItem* easing = pRoot->GetMenu()->AddItem( L"None", "" )->SetAction( this, &TimelineWidget::easingFunctionSelected );
			easing->SetName( "None" );
			}

			{
			Gwen::Controls::MenuItem* easing = pRoot->GetMenu()->AddItem( L"Quadratic", "" );
			easing->GetMenu()->AddItem( L"In" )->SetAction( this, &TimelineWidget::easingFunctionSelected )->SetName( "EaseInElastic" );
			easing->GetMenu()->AddItem( L"Out" )->SetAction( this, &TimelineWidget::easingFunctionSelected )->SetName( "EaseOutQuad" );
			easing->GetMenu()->AddItem( L"InOut" )->SetAction( this, &TimelineWidget::easingFunctionSelected )->SetName( "EaseInOutQuad" );
			easing->GetMenu()->AddItem( L"OutIn" )->SetAction( this, &TimelineWidget::easingFunctionSelected )->SetName( "EaseOutInQuad" );
			}
			{
			Gwen::Controls::MenuItem* easing = pRoot->GetMenu()->AddItem( L"Cubic", "" );
			easing->GetMenu()->AddItem( L"In" )->SetAction( this, &TimelineWidget::easingFunctionSelected )->SetName( "EaseInCubic" );
			easing->GetMenu()->AddItem( L"Out" )->SetAction( this, &TimelineWidget::easingFunctionSelected )->SetName( "EaseOutCubic" );
			easing->GetMenu()->AddItem( L"InOut" )->SetAction( this, &TimelineWidget::easingFunctionSelected )->SetName( "EaseInOutCubic" );
			easing->GetMenu()->AddItem( L"OutIn" )->SetAction( this, &TimelineWidget::easingFunctionSelected )->SetName( "EaseOutInCubic" );
			}
			{
			Gwen::Controls::MenuItem* easing = pRoot->GetMenu()->AddItem( L"Quartic", "" );
			easing->GetMenu()->AddItem( L"In" )->SetAction( this, &TimelineWidget::easingFunctionSelected )->SetName( "EaseInQuart" );
			easing->GetMenu()->AddItem( L"Out" )->SetAction( this, &TimelineWidget::easingFunctionSelected )->SetName( "EaseOutQuart" );
			easing->GetMenu()->AddItem( L"InOut" )->SetAction( this, &TimelineWidget::easingFunctionSelected )->SetName( "EaseInOutQuart" );
			easing->GetMenu()->AddItem( L"OutIn" )->SetAction( this, &TimelineWidget::easingFunctionSelected )->SetName( "EaseOutInQuart" );
			}
			{
			Gwen::Controls::MenuItem* easing = pRoot->GetMenu()->AddItem( L"Quintic", "" );
			easing->GetMenu()->AddItem( L"In" )->SetAction( this, &TimelineWidget::easingFunctionSelected )->SetName( "EaseInQuint" );
			easing->GetMenu()->AddItem( L"Out" )->SetAction( this, &TimelineWidget::easingFunctionSelected )->SetName( "EaseOutQuint" );
			easing->GetMenu()->AddItem( L"InOut" )->SetAction( this, &TimelineWidget::easingFunctionSelected )->SetName( "EaseInOutQuint" );
			easing->GetMenu()->AddItem( L"OutIn" )->SetAction( this, &TimelineWidget::easingFunctionSelected )->SetName( "EaseOutInQuint" );
			}
			{
			Gwen::Controls::MenuItem* easing = pRoot->GetMenu()->AddItem( L"Sine", "" );
			easing->GetMenu()->AddItem( L"In" )->SetAction( this, &TimelineWidget::easingFunctionSelected )->SetName( "EaseInSine" );
			easing->GetMenu()->AddItem( L"Out" )->SetAction( this, &TimelineWidget::easingFunctionSelected )->SetName( "EaseOutSine" );
			easing->GetMenu()->AddItem( L"InOut" )->SetAction( this, &TimelineWidget::easingFunctionSelected )->SetName( "EaseInOutSine" );
			easing->GetMenu()->AddItem( L"OutIn" )->SetAction( this, &TimelineWidget::easingFunctionSelected )->SetName( "EaseOutInSine" );
			}
			{
			Gwen::Controls::MenuItem* easing = pRoot->GetMenu()->AddItem( L"Exponential", "" );
			easing->GetMenu()->AddItem( L"In", "" )->SetAction( this, &TimelineWidget::easingFunctionSelected )->SetName( "EaseInExpo" );
			easing->GetMenu()->AddItem( L"Out", "" )->SetAction( this, &TimelineWidget::easingFunctionSelected )->SetName( "EaseOutExpo" );
			easing->GetMenu()->AddItem( L"InOut", "" )->SetAction( this, &TimelineWidget::easingFunctionSelected )->SetName( "EaseInOutExpo" );
			easing->GetMenu()->AddItem( L"OutIn", "" )->SetAction( this, &TimelineWidget::easingFunctionSelected )->SetName( "EaseOutInExpo" );
			}
			{
			Gwen::Controls::MenuItem* easing = pRoot->GetMenu()->AddItem( L"Circular", "" );
			easing->GetMenu()->AddItem( L"In" )->SetAction( this, &TimelineWidget::easingFunctionSelected )->SetName( "EaseInCirc" );
			easing->GetMenu()->AddItem( L"Out" )->SetAction( this, &TimelineWidget::easingFunctionSelected )->SetName( "EaseOutCirc" );
			easing->GetMenu()->AddItem( L"InOut" )->SetAction( this, &TimelineWidget::easingFunctionSelected )->SetName( "EaseInOutCirc" );
			easing->GetMenu()->AddItem( L"OutIn" )->SetAction( this, &TimelineWidget::easingFunctionSelected )->SetName( "EaseOutInCirc" );
			}
			{
			Gwen::Controls::MenuItem* easing = pRoot->GetMenu()->AddItem( L"Bounce", "" );
			easing->GetMenu()->AddItem( L"In" )->SetAction( this, &TimelineWidget::easingFunctionSelected )->SetName( "EaseInBounce" );
			easing->GetMenu()->AddItem( L"Out" )->SetAction( this, &TimelineWidget::easingFunctionSelected )->SetName( "EaseOutBounce" );
			easing->GetMenu()->AddItem( L"InOut" )->SetAction( this, &TimelineWidget::easingFunctionSelected )->SetName( "EaseInOutBounce" );
			easing->GetMenu()->AddItem( L"OutIn" )->SetAction( this, &TimelineWidget::easingFunctionSelected )->SetName( "EaseOutInBounce" );
			}
			{
			Gwen::Controls::MenuItem* easing = pRoot->GetMenu()->AddItem( L"Back", "" );
			easing->GetMenu()->AddItem( L"In" )->SetAction( this, &TimelineWidget::easingFunctionSelected )->SetName( "EaseInBack" );
			easing->GetMenu()->AddItem( L"Out" )->SetAction( this, &TimelineWidget::easingFunctionSelected )->SetName( "EaseOutBack" );
			easing->GetMenu()->AddItem( L"InOut" )->SetAction( this, &TimelineWidget::easingFunctionSelected )->SetName( "EaseInOutBack" );
			easing->GetMenu()->AddItem( L"OutIn" )->SetAction( this, &TimelineWidget::easingFunctionSelected )->SetName( "EaseOutInBack" );
			}
			{
			Gwen::Controls::MenuItem* easing = pRoot->GetMenu()->AddItem( L"Elastic", "" );
			easing->GetMenu()->AddItem( L"In" )->SetAction( this, &TimelineWidget::easingFunctionSelected )->SetName( "EaseInElastic" );
			easing->GetMenu()->AddItem( L"Out" )->SetAction( this, &TimelineWidget::easingFunctionSelected )->SetName( "EaseOutElastic" );
			easing->GetMenu()->AddItem( L"InOut" )->SetAction( this, &TimelineWidget::easingFunctionSelected )->SetName( "EaseInOutElastic" );
			easing->GetMenu()->AddItem( L"OutIn" )->SetAction( this, &TimelineWidget::easingFunctionSelected )->SetName( "EaseOutInElastic" );
			}
			{
			Gwen::Controls::MenuItem* easing = pRoot->GetMenu()->AddItem( L"Atan", "" );
			easing->GetMenu()->AddItem( L"In" )->SetAction( this, &TimelineWidget::easingFunctionSelected )->SetName( "EaseInAtan" );
			easing->GetMenu()->AddItem( L"Out" )->SetAction( this, &TimelineWidget::easingFunctionSelected )->SetName( "EaseOutAtan" );
			easing->GetMenu()->AddItem( L"InOut" )->SetAction( this, &TimelineWidget::easingFunctionSelected )->SetName( "EaseInOutAtan" );
			}
		}
        
        float firstColumnWidth = 125;
        
		
		// Add the first row with the timeline and the navigation bar
		//------------------------------------------------------------------
		Gwen::Controls::Base* firstRow = new Gwen::Controls::Base( this );
		firstRow->Dock( Gwen::Pos::Top );
		firstRow->SetSize( firstColumnWidth, 45 );

		Gwen::Controls::Base* firstRowLeftColumn = new Gwen::Controls::Base( firstRow );
		firstRowLeftColumn->SetSize( firstColumnWidth, 45 );
		firstRowLeftColumn->Dock( Gwen::Pos::Left );
        
        mTimeStepper = new VectorStepper2f( firstRowLeftColumn );
        mTimeStepper->setUseIntegers(true);
        mTimeStepper->Dock( Gwen::Pos::Top );
        mTimeStepper->onChanged.Add( this, &TimelineWidget::timeStepperChanged );
        
        Gwen::Controls::Base* buttons = new Gwen::Controls::Base( firstRowLeftColumn );
        buttons->Dock( Gwen::Pos::Top );
        buttons->SetSize( 125, 21 );
        
        mStartButton = new TimelineButton( firstRowLeftColumn );
        mStartButton->SetPos( 0, 21 );
        mStartButton->SetSize( 27, 21 );
        mStartButton->setType( TimelineButton::START );
        mStartButton->onPress.Add( this, &TimelineWidget::handleTimelineButtons );
        
        mPrevButton = new TimelineButton( firstRowLeftColumn );
        mPrevButton->SetPos( 27, 21 );
        mPrevButton->SetSize( 23, 21 );
        mPrevButton->setType( TimelineButton::PREV );
        mPrevButton->onPress.Add( this, &TimelineWidget::handleTimelineButtons );
        
        mPlayButton = new TimelineButton( firstRowLeftColumn );
        mPlayButton->SetPos( 50, 21 );
        mPlayButton->SetIsToggle( true );
        mPlayButton->SetSize( 24, 21 );
        mPlayButton->setType( TimelineButton::PLAY );
        mPlayButton->onPress.Add( this, &TimelineWidget::handleTimelineButtons );
        
        mNextButton = new TimelineButton( firstRowLeftColumn );
        mNextButton->SetPos( 74, 21 );
        mNextButton->SetSize( 23, 21 );
        mNextButton->setType( TimelineButton::NEXT );
        mNextButton->onPress.Add( this, &TimelineWidget::handleTimelineButtons );
        
        mEndButton = new TimelineButton( firstRowLeftColumn );
        mEndButton->SetPos( 97, 21 );
        mEndButton->SetSize( 30, 21 );
        mEndButton->setType( TimelineButton::END );
        mEndButton->onPress.Add( this, &TimelineWidget::handleTimelineButtons );
                
		mTimeline = new Timeline( firstRow );
		mTimeline->SetPos( firstColumnWidth, 0 );
		mTimeline->Dock( Gwen::Pos::Top );
		mTimeline->mDraggedCallback.Add( this, &TimelineWidget::onTimelineDragged );

		mNavigationBar = new NavigationBar( firstRow );
		mNavigationBar->Dock( Gwen::Pos::Top );
		mNavigationBar->mDraggedCallback.Add( this, &TimelineWidget::onNavigationBarDragged );
	
		
		// Add the TrackList
		//------------------------------------------------------------------
		mTrackList = new TrackList( this );
		mTrackList->mTimeline = mTimeline;
        
        //new Gwen::Controls::Base( mTrackList );

		setTimeFormat( TimeFormat_Seconds );
		setPlayMode( PlayMode_Loop );
		setTime();

		mPlaying			= false;
		mScrubbing	= false;
	}
	
	void TimelineWidget::update(){
		if( mScrubbing ){
			mScrubbing	= false;
			mCurrentTime		= mManualTime;
			if( mPlaying ) play();
		}
		else if( mPlaying && !mTimeline->getMouseDown() ){
			if( mTimeFormat == TimeFormat_Seconds ){
				mCurrentTime = mLastStartTime + ( ci::app::App::get()->getElapsedSeconds() - mLastPlayingTime );
			}
			else if( mTimeFormat == TimeFormat_Frames ){
				mCurrentTime = mLastStartTime + ( ci::app::App::get()->getElapsedFrames() - mLastPlayingTime );
			}
		}
		if( mCurrentTime > mTotalTime ){
			if( mPlayMode == PlayMode_Normal ) {
				mPlaying = false;
				stepTo( mTotalTime );
			}
			else if( mPlayMode == PlayMode_Loop ){
				stepTo( 0 );
			}
		}
        mTimeStepper->setValue( ci::Vec2f( mCurrentTime, mTimeStepper->getValue().y ) );
		mTimeline->setCurrentFrame( mCurrentTime );
	}

	void TimelineWidget::Render( Gwen::Skin::Base* skin ){ 
		glDisable( GL_TEXTURE_2D );

		Gwen::Rect rect = GetRenderBounds();
		skin->GetRender()->Translate( rect );	
	}
	
	/*bool TimelineWidget::OnKeySpace( bool bDown ){
		if( bDown ) {
			if( mPlaying ) pause();
			else play();

			return false;
		}
		return false;
	}*/
    

    
	void TimelineWidget::onNavigationBarDragged( Gwen::Controls::Base* control ){
		NavigationBar* nav = (NavigationBar*) control;
		mTimeline->setStart( nav->mNormalizedStart * mTotalTime );
		mTimeline->setEnd( nav->mNormalizedEnd * mTotalTime );
		mTrackList->updateGraphVbos();
        onUpdate.Call( this );
	}

	
	void TimelineWidget::onTimelineDragged( Gwen::Controls::Base* control ){
		Timeline* timeline = (Timeline*) control;
		stepTo( timeline->getCurrentFrame() );
        onUpdate.Call( this );
	}

	
	void TimelineWidget::setTimeFormat( uint32_t timeFormat ){ 
		mTimeFormat = timeFormat; 
		mTimeline->setTimeFormat( timeFormat ); 
	}
	
	void TimelineWidget::setTime( float total, float start, float end, float current ){
		setTotalTime( total );
		setStartTime( start );
		setEndTime( end );
		setCurrentTime( current );
	}
	void TimelineWidget::setTotalTime( float total ){ 
		mTotalTime = total;
        mTimeStepper->setValue( ci::Vec2f( mTimeStepper->getValue().x, mTotalTime ) );
	}
	void TimelineWidget::setStartTime( float start ){ 
		mStartTime = start; 
		mTimeline->setStart( mStartTime );
		mNavigationBar->setNormalizedStart( mStartTime / mTotalTime );
	}
	void TimelineWidget::setEndTime( float end ){ 
		mEndTime = end; 
		mTimeline->setEnd( mEndTime );
		mNavigationBar->setNormalizedEnd( mEndTime / mTotalTime );
	}
	void TimelineWidget::setCurrentTime( float current ){ 
		mCurrentTime = current; 
		mTimeline->setCurrentFrame( mCurrentTime );
	}
	void TimelineWidget::setTimeline( Timeline* timeline ){
		mTrackList->setTimeline( timeline );
	}
	void TimelineWidget::setTrackList( std::vector< Animation::TrackRef >*	trackList ){
		mTrackList->setTrackList( trackList );
	}


	void TimelineWidget::foldAll( Gwen::Controls::Base* pControl ){
		if( !mTrackList->mTrackButtons.empty() ){
			for ( size_t i = 0; i < mTrackList->mTrackButtons.size(); i++ ) {
				mTrackList->mTrackButtons[ i ]->setIsOpen( false );
			}
		}
	}
	void TimelineWidget::unfoldAll( Gwen::Controls::Base* pControl ){
		if( !mTrackList->mTrackButtons.empty() ){
			for ( size_t i = 0; i < mTrackList->mTrackButtons.size(); i++ ) {
				mTrackList->mTrackButtons[ i ]->setIsOpen( true );
			}
		}
	}

	void TimelineWidget::deleteTrack( Gwen::Controls::Base* pControl ){
	}
	void TimelineWidget::deleteUnusedTracks( Gwen::Controls::Base* pControl ){
	}
	void TimelineWidget::cleanTracks( Gwen::Controls::Base* pControl ){
	}

	void TimelineWidget::deleteKey( Gwen::Controls::Base* pControl ){
	}
	void TimelineWidget::gotoNextKey( Gwen::Controls::Base* pControl ){
	}
	void TimelineWidget::gotoPreviousKey( Gwen::Controls::Base* pControl ){
	}
	void TimelineWidget::gotoStartTime( Gwen::Controls::Base* pControl ){
	}
	void TimelineWidget::gotoEndTime( Gwen::Controls::Base* pControl ){
	}
	void TimelineWidget::gotoCurrentTime( Gwen::Controls::Base* pControl ){
	}

	void TimelineWidget::play(){
		if( mTimeFormat == TimeFormat_Seconds ){
			mLastPlayingTime = ci::app::App::get()->getElapsedSeconds();
		}
		else if( mTimeFormat == TimeFormat_Frames ){
			mLastPlayingTime = ci::app::App::get()->getElapsedFrames();
		}
		mPlaying = true;
		mLastStartTime = mCurrentTime;
	}
	void TimelineWidget::pause(){
		mPlaying = !mPlaying;
		if( mPlaying ) play();
	}
	void TimelineWidget::stop(){
		mPlaying = false;
		mCurrentTime = 0;
	}

	void TimelineWidget::stepTo( float time, bool andPlay ){
		mScrubbing = true;
		mManualTime = time;
		if( andPlay ) mPlaying = true;
	}
    
    void TimelineWidget::handleTimelineButtons( Gwen::Controls::Base* pControl )
    {
        TimelineButton* button = gwen_cast<TimelineButton>( pControl );
        if( button->getType() == TimelineButton::START ){
            mCurrentTime = 0;
            mTimeline->setCurrentFrame( mCurrentTime );
            mTimeStepper->setValue( ci::Vec2f( mCurrentTime, mTimeStepper->getValue().y ) );
        }
        else if( button->getType() == TimelineButton::NEXT ){
            mCurrentTime+=1;
            mTimeline->setCurrentFrame( mCurrentTime );
            mTimeStepper->setValue( ci::Vec2f( mCurrentTime, mTimeStepper->getValue().y ) );
        }
        else if( button->getType() == TimelineButton::PREV ){
            mCurrentTime-=1;
            mTimeline->setCurrentFrame( mCurrentTime );
            mTimeStepper->setValue( ci::Vec2f( mCurrentTime, mTimeStepper->getValue().y ) );
        }
        else if( button->getType() == TimelineButton::END ){
            mCurrentTime = mTotalTime;
            mTimeline->setCurrentFrame( mCurrentTime );
            mTimeStepper->setValue( ci::Vec2f( mCurrentTime, mTimeStepper->getValue().y ) );
        }
        else if( button->getType() == TimelineButton::PLAY ){
            mPlaying = button->GetToggleState();
            if( mPlaying ) play();
        }
    }
    void TimelineWidget::timeStepperChanged( Gwen::Controls::Base* pControl )
    {
		VectorStepper2f* stepper = gwen_cast<VectorStepper2f>( pControl );
        //stepTo( stepper->getValue().x, mPlaying );
        if( mCurrentTime != stepper->getValue().x ){
            mCurrentTime = stepper->getValue().x;
            mTimeline->setCurrentFrame( mCurrentTime );
        }
        else if( mTotalTime != stepper->getValue().y ){
            setTotalTime( stepper->getValue().y );
            mTimeline->setStart( mNavigationBar->mNormalizedStart * mTotalTime );
            mTimeline->setEnd( mNavigationBar->mNormalizedEnd * mTotalTime );
            mTrackList->updateGraphVbos();
        }
    }

	
	void TimelineWidget::easingFunctionSelected( Gwen::Controls::Base* pControl ){
		Gwen::Controls::MenuItem* item = gwen_cast<Gwen::Controls::MenuItem>( pControl );
		std::string type = item->GetName();
		int trackI = mTrackList->mSelectedTrack;
		int keyframeI = mTrackList->mTrackButtons[trackI]->mSelectedKeyframe;
		if( trackI != -1 && keyframeI != -1 ){
			Animation::TrackRef trackRef = mTrackList->mTrackList->at( trackI );
			Animation::KeyFrameBase* keyframeBase = static_cast<Animation::KeyFrameBase*>( trackRef->at( keyframeI ).get() );
			if( keyframeBase->mType == Animation::KeyFrame_Value ){
				Animation::KeyFrame* keyframe = static_cast<Animation::KeyFrame*>( keyframeBase );
				keyframe->mEaseFn = easingStringToFunction( type );
				mTrackList->updateGraphVbos();
                std::cout << "CURVE " << std::endl;
				trackRef->refresh();
			}
		}
	}
	
	ci::EaseFn TimelineWidget::easingStringToFunction( std::string func ){
		if( func == "None" ) return ci::EaseNone();
		else if ( func == "EaseInQuad" ) return ci::EaseInQuad();
		else if ( func == "EaseOutQuad" ) return ci::EaseOutQuad();
		else if ( func == "EaseInOutQuad" ) return ci::EaseInOutQuad();
		else if ( func == "EaseOutInQuad" ) return ci::EaseOutInQuad();
		else if ( func == "EaseInCubic" ) return ci::EaseInCubic();
		else if ( func == "EaseOutCubic" ) return ci::EaseOutCubic();
		else if ( func == "EaseInOutCubic" ) return ci::EaseInOutCubic();
		else if ( func == "EaseOutInCubic" ) return ci::EaseOutInCubic();
		else if ( func == "EaseInQuart" ) return ci::EaseInQuart();
		else if ( func == "EaseOutQuart" ) return ci::EaseOutQuart();
		else if ( func == "EaseInOutQuart" ) return ci::EaseInOutQuart();
		else if ( func == "EaseOutInQuart" ) return ci::EaseOutInQuart();
		else if ( func == "EaseInQuint" ) return ci::EaseInQuint();
		else if ( func == "EaseOutQuint" ) return ci::EaseOutQuint();
		else if ( func == "EaseInOutQuint" ) return ci::EaseInOutQuint();
		else if ( func == "EaseOutInQuint" ) return ci::EaseOutInQuint();
		else if ( func == "EaseInSine" ) return ci::EaseInSine();
		else if ( func == "EaseOutSine" ) return ci::EaseOutSine();
		else if ( func == "EaseInOutSine" ) return ci::EaseInOutSine();
		else if ( func == "EaseOutInSine" ) return ci::EaseOutInSine();
		else if ( func == "EaseInExpo" ) return ci::EaseInExpo();
		else if ( func == "EaseOutExpo" ) return ci::EaseOutExpo();
		else if ( func == "EaseInOutExpo" ) return ci::EaseInOutExpo();
		else if ( func == "EaseOutInExpo" ) return ci::EaseOutInExpo();
		else if ( func == "EaseInCirc" ) return ci::EaseInCirc();
		else if ( func == "EaseOutCirc" ) return ci::EaseOutCirc();
		else if ( func == "EaseInOutCirc" ) return ci::EaseInOutCirc();
		else if ( func == "EaseOutInCirc" ) return ci::EaseOutInCirc();
		else if ( func == "EaseInBounce" ) return ci::EaseInBounce();
		else if ( func == "EaseOutBounce" ) return ci::EaseOutBounce();
		else if ( func == "EaseInOutBounce" ) return ci::EaseInOutBounce();
		else if ( func == "EaseOutInBounce" ) return ci::EaseOutInBounce();
		else if ( func == "EaseInBack" ) return ci::EaseInBack();
		else if ( func == "EaseOutBack" ) return ci::EaseOutBack();
		else if ( func == "EaseInOutBack" ) return ci::EaseInOutBack();
		else if ( func == "EaseOutInBack" ) return ci::EaseOutInBack();
		else if ( func == "EaseInElastic" ) return ci::EaseInElastic( 2.0f, 1.0f );
		else if ( func == "EaseOutElastic" ) return ci::EaseOutElastic( 1.0f, 4.0f );
		else if ( func == "EaseInOutElastic" ) return ci::EaseInOutElastic( 2.0f, 1.0f );
		else if ( func == "EaseOutInElastic" ) return ci::EaseOutInElastic( 1.0f, 4.0f );
		else if ( func == "EaseInAtan" ) return ci::EaseInAtan();
		else if ( func == "EaseOutAtan" ) return ci::EaseOutAtan();
		else if ( func == "EaseInOutAtan" ) return ci::EaseInOutAtan();
		return ci::EaseNone();
	}
}}