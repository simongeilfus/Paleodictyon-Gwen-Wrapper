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

	class NavigationBar : public Gwen::Controls::Base {
	public:
		
		GWEN_CONTROL_INLINE( NavigationBar, Gwen::Controls::Base ){			
			SetHeight( 10 );

			mDraggerWidth		= 15;
			mStart				= 0;
			mEnd				= -1;
			mNormalizedStart	= 0;
			mNormalizedStart	= 1;
			mMouseDown			= false;
		}

		void Render( Gwen::Skin::Base* skin );
		void OnMouseMoved( int x, int y, int deltaX, int deltaY );
		void OnMouseClickLeft( int x, int y, bool bDown );
		void update();
		
		void setNormalizedStart( float start );
		void setNormalizedEnd( float end );
	
		enum {
			NavigationBarBar_Center,
			NavigationBarBar_Left,
			NavigationBarBar_Right
		} mDragType;

		float				mStart;
		float				mEnd;
		float				mWidth;
	
		float				mNormalizedStart;
		float				mNormalizedEnd;
		float				mNormalizedWidth;
	
		float				mDraggerWidth;
	
		bool				mMouseDown;
	
		Gwen::Event::Caller	mDraggedCallback;
	};
}
}