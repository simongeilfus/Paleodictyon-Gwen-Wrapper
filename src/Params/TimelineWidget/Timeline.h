#pragma once

#include "cinder/gl/Texture.h"
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/Utilities.h"

#include "Gwen/Controls/Base.h"
#include "Gwen/Controls/Button.h"
#include "Gwen/Controls/ScrollBar.h"
#include "Gwen/Controls/ScrollBarBar.h"
#include "Gwen/Controls/HorizontalSlider.h"
#include "Gwen/Controls/HorizontalScrollBar.h"
#include "Gwen/Skins/TexturedBase.h"

#include "CinderGwen.h"

namespace CinderGwen { namespace Widgets {

	class Timeline : public Gwen::Controls::Base {
	public:

		GWEN_CONTROL_INLINE( Timeline, Gwen::Controls::Base ){
			SetMouseInputEnabled( true );
			SetBounds( 0, 0, 100, 30 );
			

			mTimeFormat			= TimeFormat_Frames;
			mMargins			= 25;
			mMouseDown			= false;
			mFrameLineHeight	= 3;
			mFrameSubLineHeight	= 10;
			
			setStart( 0 );
			setEnd( 300 );
		}

		void OnMouseMoved( int x, int y, int deltaX, int deltaY );
		void OnMouseClickLeft( int x, int y, bool bDown );
		void Render( Gwen::Skin::Base* skin );
		void setStart( uint32_t i );
		void setEnd( uint32_t i );
		float getStart() { return mStartFrame; }
		float getEnd() { return mEndFrame; }

		void update();
		void Invalidate(){ update(); }

		float getCurrentFrame(){ return mCurrentFrame; }
		void setCurrentFrame( float current ){ mCurrentFrame = current; }
		void setTimeFormat( uint32_t timeFormat ){ mTimeFormat = timeFormat; }

		bool getMouseDown(){ return mMouseDown; }
		
		enum {
			TimeFormat_Frames = 0,
			TimeFormat_Seconds
		};
		
		float		getTimeFromPos( Gwen::Point pos );
		ci::Vec2f getPosFromTime( float frame, Gwen::Rect bounds = Gwen::Rect() );

		Gwen::Event::Caller	mDraggedCallback;

	protected:

		ci::Vec2i	getNiceNumbers( int original );

		float		mStartFrame;
		float		mEndFrame;
		float		mCurrentFrame;
		float		mNumFrames;
		float		mNumFramesPerMark;
		float		mNumMarkSubdivisions;
		float		mNumFramesPerSubdvisions;

		float		mFrameSize;
		float		mMargins;
		float		mFullBarSize;
		float		mFrameLineHeight;
		float		mFrameSubLineHeight;

		uint32_t	mTimeFormat;

		bool		mMouseDown;
		
		friend class TrackList;
		friend class TrackButton;
		friend class ValueTrackButton;
		friend class TimelineWidget;
	};

}}