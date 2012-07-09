#include "NavigationBar.h"

namespace CinderGwen { namespace Widgets {

	void NavigationBar::Render( Gwen::Skin::Base* skin ){
		glDisable( GL_TEXTURE_2D );

		Gwen::Rect rect = GetRenderBounds();
		//skin->GetRender()->Translate( rect );	

		if( mEnd == -1 ){			
			mStart  = mNormalizedStart * (float) GetBounds().w;
			mEnd	= mNormalizedEnd * (float) GetBounds().w;
		}

		mWidth = mEnd - mStart;

		ci::Rectf r( ci::Vec2f( rect.x + mStart, rect.y ), ci::Vec2f( rect.x  + mEnd, rect.y + rect.h) );	
		( (Gwen::Skin::TexturedBase*) skin )->Textures.Scroller.ButtonH_Hover.Draw( skin->GetRender(), GetRenderBounds() );
		if ( mMouseDown ) ( (Gwen::Skin::TexturedBase*) skin )->Textures.Scroller.ButtonH_Disabled.Draw( skin->GetRender(), CinderGwen::toGwen( r ) );
		else ( (Gwen::Skin::TexturedBase*) skin )->Textures.Scroller.ButtonH_Down.Draw( skin->GetRender(), CinderGwen::toGwen( r ) );

	}
		
		
	void NavigationBar::OnMouseMoved( int x, int y, int deltaX, int deltaY ){
		if( mMouseDown ){
			Gwen::Point p = CanvasPosToLocal( Gwen::Point( x, y ) );

			if( mDragType == NavigationBarBar_Center ) {
				mWidth		= mEnd - mStart;
				float start = p.x - mWidth * 0.5f;
				if( start < 0 ) start = 0;
				float end	= start + mWidth;
				if( end > GetBounds().w ){
					end = GetBounds().w;
					start = end - mWidth;
				}
				mStart = start;
				mEnd   = end;
				update();
			}
			else if( mDragType == NavigationBarBar_Left ) {
				mStart = p.x;
				if( mStart < 0 ) mStart = 0;
				else if( mStart > mEnd - mDraggerWidth * 3.0f ) mStart = mEnd - mDraggerWidth * 3.0f;
				update();
			}
			else if( mDragType == NavigationBarBar_Right ) {
				mEnd = p.x;
				if( mEnd > GetBounds().w ) mEnd = GetBounds().w;
				else if( mEnd < mStart + mDraggerWidth * 3.0f ) mEnd = mStart + mDraggerWidth * 3.0f;
				update();
			}
		}
	}
	void NavigationBar::OnMouseClickLeft( int x, int y, bool bDown ){
		if ( bDown ){
			mMouseDown			= true;
			Gwen::MouseFocus	= this;
			
			Gwen::Point p = CanvasPosToLocal( Gwen::Point( x, y ) );
			if( p.x > mStart + mDraggerWidth && p.x < mEnd - mDraggerWidth ){
				mDragType = NavigationBarBar_Center;
			}
			else if( p.x < mStart + mDraggerWidth ) {
				mDragType = NavigationBarBar_Left;
			}
			else if( p.x > mEnd - mDraggerWidth ) {
				mDragType = NavigationBarBar_Right;
			}
		}
		else{
			mMouseDown			= false;
			Gwen::MouseFocus	= NULL;
		}
	}
	
	void NavigationBar::update(){
		mNormalizedStart	= mStart / (float) GetBounds().w;
		mNormalizedEnd		= mEnd / (float) GetBounds().w;;
		mNormalizedWidth	= mWidth / (float) GetBounds().w;;
		
		mDraggedCallback.Call( this );
	}
	
	void NavigationBar::setNormalizedStart( float start ){
		if( mEnd == -1 ) mNormalizedStart = start;
		else{
			mStart = start * (float) GetBounds().w;
			update();
		}
	}
	void NavigationBar::setNormalizedEnd( float end ){
		if( mEnd == -1 ) mNormalizedEnd = end;
		else {
			mEnd = end * (float) GetBounds().w;
			update();
		}
	}
}}