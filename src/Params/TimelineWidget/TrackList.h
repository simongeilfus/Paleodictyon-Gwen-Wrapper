#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Vbo.h"
#include "cinder/CinderMath.h"
#include "cinder/Rand.h"

#include "Gwen/Controls/Base.h"
#include "Gwen/Controls/CollapsibleList.h"
#include "Gwen/Controls/Button.h"
#include "Gwen/Skins/TexturedBase.h"

#include "Timeline.h"
#include "Track.h"

namespace CinderGwen { namespace Widgets {
	
	class TrackButton : public Gwen::Controls::Button {
	public:
		GWEN_CONTROL( TrackButton, Gwen::Controls::Button );
		
		void Render( Gwen::Skin::Base* skin );
		void PostLayout( Gwen::Skin::Base* skin );
		
		void OnMouseMoved( int x, int y, int deltaX, int deltaY );
		void OnMouseClickLeft( int x, int y, bool bDown );
		void OnMouseDoubleClickLeft( int x, int y );
		
		bool OnKeyBackspace( bool bDown );
		bool OnKeyDelete( bool bDown );
		void OnBoundsChanged( Gwen::Rect rect );
		
		void onMainClickLeft( int x, int y, bool bDown );
		void onSecondaryClickLeft( int x, int y, bool bDown );
		void onDetailsClickLeft( int x, int y, bool bDown );

		void onMainDrag( int x, int y );
		void onSecondaryDrag( int x, int y );
		void onDetailsDrag( int x, int y );

		void select( size_t i );
		void selectAll();
		void addToSelection( size_t i );
		void removeFromSelection( size_t i );
		void clearSelection();

		void makeSelection( int x );
		const std::vector<size_t>& getSelection();
		
		void SetToggleState( bool b );

		void setId( int id ){ mId = id; }
		int getId(){ return mId; }

		void setName( std::string name ){ mTrackHeader->SetText( name ); }
		 
		void setIsOpen( bool open ){ mIsOpen = open; }
		bool getIsOpen(){ return mIsOpen; }
		
		template <typename T>
		void addOnSelectionCallback( Gwen::Event::Handler* ob, T f ){ onToggleOn.Add( ob, f ); }
		template <typename T>
		void addOnDeselectionCallback( Gwen::Event::Handler* ob, T f ){ onToggleOff.Add( ob, f ); }

		void setTrackRef( Animation::TrackRef trackRef ){ mTrackRef = trackRef; }
		void setTimelinePtr( Timeline* timeline ){ mTimeline = timeline; }

		void setNeedRenderUpdate( bool update = true ){ mNeedRenderUpdate = update; }

		virtual void renderTrackDetails( Gwen::Skin::Base* skin ) = 0;

	protected:
		bool					mIsOpen;
		Gwen::Controls::Button*	mTrackHeader;
		Animation::TrackRef				mTrackRef;
		Timeline*				mTimeline;
		float					mMinTime;
		float					mMaxTime;
		int						mId;

		bool					mMouseDown;
		Gwen::Point				mMouseDownPos;
		Gwen::Point				mMouseDragPos;

		ci::gl::VboMesh			mGraphVbo;
		bool					mNeedRenderUpdate;
		int						mGraphVboSamples;
		
		float					mHeight;
		float					mFullHeight;
		float					mLeftColumnWidth;
		
		int						mSelectedKeyframe;
		std::vector<size_t>		mSelection;
		std::vector<float>		mSelectionDistances;
		Gwen::Rect				mSelectionRect;

		friend class TimelineWidget;
	};
		
	//-------------------------------------------------------------------------------------------------------------------------------------


	class ValueTrackButton : public TrackButton {
	public:
		GWEN_CONTROL( ValueTrackButton, TrackButton );
		
		void renderTrackDetails( Gwen::Skin::Base* skin );
		
	protected:
		friend class TimelineWidget;
	};

	//-------------------------------------------------------------------------------------------------------------------------------------


	class CueTrackButton : public TrackButton {
	public:
		GWEN_CONTROL( CueTrackButton, TrackButton );
		
		void renderTrackDetails( Gwen::Skin::Base* skin );
		
	protected:
		friend class TimelineWidget;
	};

	//-------------------------------------------------------------------------------------------------------------------------------------


	class BlockTrackButton : public TrackButton {
	public:
		GWEN_CONTROL( BlockTrackButton, TrackButton );
		
		void renderTrackDetails( Gwen::Skin::Base* skin );
		
	protected:
		friend class TimelineWidget;
	};
		
	//-------------------------------------------------------------------------------------------------------------------------------------

	class TrackList : public Gwen::Controls::ScrollControl {
	public:

		GWEN_CONTROL( TrackList, Gwen::Controls::ScrollControl );
	
		void OnBoundsChanged( Gwen::Rect rect );
		void Invalidate();
		void VBarMoved( Gwen::Controls::Base * control );
		bool OnMouseWheeled( int iDelta );
		
	/*	void Render( Gwen::Skin::Base* skin );
		
		void OnMouseMoved( int x, int y, int deltaX, int deltaY );
		void OnMouseClickLeft( int x, int y, bool bDown );
		void OnMouseClickRight( int x, int y, bool bDown );
		
		bool OnKeyBackspace( bool bDown );
		bool OnKeyDelete( bool bDown );*/
		
		void setTimeline( Timeline* timeline );
		void setTrackList( std::vector< Animation::TrackRef >*	trackList );

		void updateGraphVbos();

	protected:
		
		void onSelection( Gwen::Controls::Base* control );
		void onDeselection( Gwen::Controls::Base* control );

		bool							mMouseDown;

		int								mSelectedTrack;
		int								mSelectedKeyframe;

		static float					mTrackHeight;
		static float					mLeftColumnWidth;

		Timeline*						mTimeline;
		std::vector< Animation::TrackRef >*		mTrackList;
		std::vector< TrackButton* >		mTrackButtons;
		
		friend class Timeline;
		friend class TrackButton;
		friend class TimelineWidget;
	};

}}