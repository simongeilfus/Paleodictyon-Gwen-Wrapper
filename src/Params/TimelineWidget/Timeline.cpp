#include "Timeline.h"

namespace CinderGwen { namespace Widgets {

	void Timeline::OnMouseMoved( int x, int y, int deltaX, int deltaY ){
		if( mMouseDown ){
			mCurrentFrame = getTimeFromPos( CanvasPosToLocal( Gwen::Point( x, y ) ) );
			mDraggedCallback.Call( this );
		}

	}
	void Timeline::OnMouseClickLeft( int x, int y, bool bDown ){
		if ( bDown ){
			mCurrentFrame = getTimeFromPos( CanvasPosToLocal( Gwen::Point( x, y ) ) );
			mMouseDown			= true;
			Gwen::MouseFocus	= this;
			mDraggedCallback.Call( this );
		}
		else{
			mMouseDown			= false;
			Gwen::MouseFocus	= NULL;
		}
	}
		
	void Timeline::Render( Gwen::Skin::Base* skin ){ 
	
		Gwen::Rect rect = GetRenderBounds();
		skin->GetRender()->Translate( rect );		
		( (Gwen::Skin::TexturedBase*) skin )->Textures.Scroller.ButtonH_Normal.Draw( skin->GetRender(), GetRenderBounds() );
		glDisable( GL_TEXTURE_2D );
	
		update();
		float subFrameSize = (float) mNumFramesPerMark * mFrameSize / (float) mNumMarkSubdivisions;
	
		
		// Draw Frames Lines
		//------------------------------------------------------------------------------------------------------
		ci::gl::color( ci::ColorA::black() );
		for( size_t i = 0; i <= mNumFrames; i+= mNumFramesPerMark ){
			ci::Vec2f pos( rect.x + mMargins + i * mFrameSize, rect.y + rect.h );
			ci::gl::drawLine( pos, pos + ci::Vec2f( 0.0f, -mFrameSubLineHeight ) );
			for( size_t j = 0; j < mNumFrames; j++ ){
				ci::Vec2f trans = pos + ci::Vec2f( j * subFrameSize, 0.0f );
				if( mStartFrame + i + j * ( mNumFramesPerMark / mNumMarkSubdivisions ) < mEndFrame ) ci::gl::drawLine( trans, trans + ci::Vec2f( 0.0f, -mFrameLineHeight ) );
			}
		}
	
		// Draw Frames String
		//------------------------------------------------------------------------------------------------------
		for( size_t i = 0; i <= mNumFrames; i+= mNumFramesPerMark ){
			Gwen::Point charSize = skin->GetRender()->MeasureText( skin->GetDefaultFont(), ci::toString( i ) );
			Gwen::Point pos( mMargins + i * mFrameSize - charSize.x * 0.5f, rect.h - mFrameSubLineHeight - 3.0f - charSize.y );
			skin->GetRender()->SetDrawColor( skin->Colors.Label.Dark );
			skin->GetRender()->RenderText( skin->GetDefaultFont(), pos, ci::toString( mStartFrame + i ) );
		}

		// Draw Current Frame
		//------------------------------------------------------------------------------------------------------
		if( mCurrentFrame >= mStartFrame && mCurrentFrame <= mEndFrame ){
			ci::Vec2f pos = getPosFromTime( mCurrentFrame, rect );
			ci::Vec2f pos2 = getPosFromTime( mCurrentFrame, GetRenderBounds() );
			ci::Rectf r( pos2 - ci::Vec2f( 13, rect.h ), pos2 + ci::Vec2f( 13, 0 ) );
			( (Gwen::Skin::TexturedBase*) skin )->Textures.Input.Button.Hovered.Draw( skin->GetRender(), CinderGwen::toGwen( r ) );

			ci::gl::color( ci::ColorA::black() );
			ci::gl::drawLine( pos, pos + ci::Vec2f( 0.0f, -mFrameSubLineHeight ) );
			
			Gwen::Point currentFrameStringSize = skin->GetRender()->MeasureText( skin->GetDefaultFont(), ci::toString( (int) mCurrentFrame ) );
			Gwen::Point stringPos( pos2.x - currentFrameStringSize.x * 0.5f, rect.h - mFrameSubLineHeight - 3.0f - currentFrameStringSize.y );
			skin->GetRender()->SetDrawColor( skin->Colors.Label.Bright );
			skin->GetRender()->RenderText( skin->GetDefaultFont(), stringPos, ci::toString( (int) mCurrentFrame ) );
		}
	}

	void Timeline::setStart( uint32_t i ){ 
		mStartFrame = i; update(); 
	}
	void Timeline::setEnd( uint32_t i ){ 
		mEndFrame = i; update(); 
	}
	
	void Timeline::update(){
		mFullBarSize				= GetRenderBounds().w - 2.0f * mMargins;

		mNumFrames					= mEndFrame - mStartFrame;
		mFrameSize					= mFullBarSize / (float) mNumFrames;

		ci::Vec2i nice				= getNiceNumbers( mFullBarSize / 20 / mFrameSize );
	
		mNumFramesPerMark			= nice.x;
		mNumMarkSubdivisions		= nice.y;
		mNumFramesPerSubdvisions	= (int) ( (float) mNumFramesPerMark / (float) mNumMarkSubdivisions );
	}


	ci::Vec2i Timeline::getNiceNumbers( int original ){
		int nice[11] = { 1, 2, 4, 5, 10, 20, 50, 100, 200, 500, 1000 };
		//int subs[11] = { 1, 2, 4, 5, 10, 10, 10, 10, 10, 10, 10 };
		int closest = 10000;
		int result = 0;
		for( int i = 0 ; i < 8; i++ ){
			int diff = (int) ci::math<double>::abs( (double) original - nice[i] );
			if( diff <= closest ){
				closest = diff;
				result  = i;
			}
		}
		return ci::Vec2i( nice[result], ( nice[result] < 20 ) ? nice[result] : 10 );
	}


	
	float Timeline::getTimeFromPos( Gwen::Point pos ){
		Gwen::Point p	= pos;
		float frame		= ( (float) ( p.x - mMargins ) / mFullBarSize ) * mNumFrames + mStartFrame;
		if( frame < mStartFrame ) frame = mStartFrame;
		if( frame > mEndFrame ) frame = mEndFrame;

		if( mTimeFormat == TimeFormat_Frames )
			frame = (int) frame - ( (int) frame %  (int) mNumFramesPerSubdvisions );

		return frame;
	}
	ci::Vec2f Timeline::getPosFromTime( float frame, Gwen::Rect bounds ){
		return ci::Vec2f( bounds.x + mMargins + ( frame - mStartFrame ) * mFrameSize,  bounds.y + bounds.h );
	}
}}