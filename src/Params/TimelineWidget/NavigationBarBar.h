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

namespace CinderGwen { class NavigationBarBar : public Gwen::ControlsInternal::ScrollBarBar {
public:
	GWEN_CONTROL_INLINE( NavigationBarBar, Gwen::ControlsInternal::ScrollBarBar ){
		RestrictToParent( true );
		SetTarget( this );
	}
	void Render( Gwen::Skin::Base* skin ) {
		skin->DrawScrollBarBar(this, m_bDepressed, IsHovered(), m_bHorizontal );
		//BaseClass::Render( skin );
		( (Gwen::Skin::TexturedBase*) skin )->Textures.Scroller.ButtonV_Normal.Draw( skin->GetRender(), GetRenderBounds() );
	}
	void OnMouseMoved( int x, int y, int deltaX, int deltaY ){
		if ( !m_pTarget ) return;
		if ( !m_bDepressed ) return;

 
		if( mDragType == NavigationBarBar_Center ) {
			Gwen::Point p = Gwen::Point( x - m_HoldPos.x, y - m_HoldPos.y );
			if ( m_pTarget->GetParent() ) p = m_pTarget->GetParent()->CanvasPosToLocal( p );
			m_pTarget->MoveTo( p.x, p.y );
		}
		else  {
			Gwen::Point p = Gwen::Point( x, y );
			if( m_pTarget->GetParent() ) p = m_pTarget->GetParent()->CanvasPosToLocal( p );
			//m_pTarget->SetWidth( 50 );
			//m_pTarget->RestrictToParent( true );
			//m_pTarget->MoveTo( p.x, p.y );
			m_pTarget->SetUserData( new Gwen::Point( p ) );
		}
		//ci::app::console() << "Call" << std::endl;
		onDragged.Call( this );
	}
	void OnMouseClickLeft( int x, int y, bool bDown ){
		if ( !m_pTarget ) return;

		if ( bDown )
		{
			m_HoldPos = m_pTarget->CanvasPosToLocal( Gwen::Point( x, y ) );
			if( m_HoldPos.x > 10 && m_HoldPos.x < GetBounds().w - 10 ){
				mDragType = NavigationBarBar_Center;
			}
			else if( m_HoldPos.x < 10 ) {
				mDragType = NavigationBarBar_Left;
			}
			else if( m_HoldPos.x > GetBounds().w - 10 ) {
				mDragType = NavigationBarBar_Right;
			}
			mLastBarWidth = GetBounds().w;
			m_bDepressed = true;
			Gwen::MouseFocus = this;
		}
		else
		{
			m_bDepressed = false;

			Gwen::MouseFocus = NULL;
		}
	}

	enum {
		NavigationBarBar_Center,
		NavigationBarBar_Left,
		NavigationBarBar_Right
	};

	int mDragType;

	float mLastBarWidth;

	//void MoveTo(int x, int y){}

};}