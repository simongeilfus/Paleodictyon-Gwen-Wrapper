#pragma once

#include "cinder/gl/Texture.h"
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/Utilities.h"

#include "Gwen/Controls/Base.h"
#include "Gwen/Controls/Button.h"
#include "Gwen/Controls/TreeControl.h"
#include "Gwen/Controls/CollapsibleList.h"
#include "Gwen/Controls/Properties.h"
#include "Gwen/Controls/PropertyTree.h"
#include "Gwen/Controls/Property/ColorSelector.h"
#include "Gwen/Controls/Property/Checkbox.h"
#include "Gwen/Controls/ScrollBar.h"
#include "Gwen/Controls/ScrollBarBar.h"
#include "Gwen/Controls/ScrollControl.h"
#include "Gwen/Controls/HorizontalSlider.h"
#include "Gwen/Controls/HorizontalScrollBar.h"
#include "Gwen/Controls/MenuStrip.h"
#include "Gwen/Skins/TexturedBase.h"

#include "Timeline.h"
#include "NavigationBar.h"
#include "TrackList.h"
#include "NumericStepper.h"

namespace CinderGwen { namespace Widgets {

    
    
    class TimelineButton : public Gwen::Controls::Button {
    public:
        enum { START, PREV, PLAY, NEXT, END };
        GWEN_CONTROL( TimelineButton, Gwen::Controls::Button );
        
        void setType( int type );
        int getType();
        void Render( Gwen::Skin::Base* skin );
        
        void OnMouseClickLeft(int x, int y, bool bDown );
    protected:
        
        bool    mDown;
        int     mType;
    };
    
	class TimelineWidget : public Gwen::Controls::Base {

	public:
        
        
		GWEN_CONTROL( TimelineWidget, Gwen::Controls::Base );

		void update();
		
		void Render( Gwen::Skin::Base* skin );

		//bool OnKeySpace( bool bDown );
		
		void onNavigationBarDragged( Gwen::Controls::Base* control );
		void onTimelineDragged( Gwen::Controls::Base* control );

		void setTimeFormat( uint32_t timeFormat );
		void setPlayMode( uint32_t playMode ){ mPlayMode = playMode; }

		void setTime( float total = 1.0f, float start = 0.0f, float end = 1.0f, float current = 0.0f );
		void setTotalTime( float total );
		void setStartTime( float start );
		void setEndTime( float end );
		void setCurrentTime( float current );
		
		float getTotalTime(){   return mTotalTime; }
		float getStartTime(){ 	return mStartTime; }
		float getEndTime(){     return mEndTime; }
		float getCurrentTime(){ return mCurrentTime; }

		void play();
		void pause();
		void stop();
        
        bool isPlaying(){ return mPlaying; }
        bool isScrubbing(){ return mScrubbing; }

		void stepTo( float time, bool andPlay = false );

		void setTimeline( Timeline* timeline );
		void setTrackList( std::vector< Animation::TrackRef >*	trackList );
		
		void foldAll( Gwen::Controls::Base* pControl );
		void unfoldAll( Gwen::Controls::Base* pControl );

		void deleteTrack( Gwen::Controls::Base* pControl );
		void deleteUnusedTracks( Gwen::Controls::Base* pControl );
		void cleanTracks( Gwen::Controls::Base* pControl );

		void deleteKey( Gwen::Controls::Base* pControl );
		void gotoNextKey( Gwen::Controls::Base* pControl );
		void gotoPreviousKey( Gwen::Controls::Base* pControl );
		void gotoStartTime( Gwen::Controls::Base* pControl );
		void gotoEndTime( Gwen::Controls::Base* pControl );
		void gotoCurrentTime( Gwen::Controls::Base* pControl );
        
        void updateTrackList(){
            mTrackList->Invalidate();
        }
		
		enum {
			TimeFormat_Frames,
			TimeFormat_Seconds
		};

		enum {
			PlayMode_Normal,
			PlayMode_Loop,
			PlayMode_BackAndForth
		};
        
        Gwen::Event::Caller onUpdate;

	protected:
		void easingFunctionSelected( Gwen::Controls::Base* pControl );
		ci::EaseFn easingStringToFunction( std::string func );
        void timeStepperChanged( Gwen::Controls::Base* pControl );
        void handleTimelineButtons( Gwen::Controls::Base* pControl );
        

		bool			mPlaying;
		bool			mScrubbing;
		
		uint32_t		mTimeFormat;
		uint32_t		mPlayMode;

		float			mCurrentTime;
		float			mStartTime;
		float			mEndTime;
		float			mTotalTime;
		float			mManualTime;
		
		float			mLastPlayingTime;
		float			mLastStartTime;

		Gwen::Controls::MenuStrip* mMenu;
		Timeline*		mTimeline;
		NavigationBar*	mNavigationBar;
		TrackList*		mTrackList;
        VectorStepper2f*mTimeStepper;
        TimelineButton*         mStartButton;
        TimelineButton*         mPrevButton;
        TimelineButton*         mPlayButton;
        TimelineButton*         mNextButton;
        TimelineButton*         mEndButton;
	};

}}