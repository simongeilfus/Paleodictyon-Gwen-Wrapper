#include "TrackList.h"


namespace CinderGwen { namespace Widgets {
	
	float TrackList::mTrackHeight		= 24.0f;
	float TrackList::mLeftColumnWidth	= 150.0f;

	//------------------------------------------------------------------------------------------
	
	GWEN_CONTROL_CONSTRUCTOR( TrackButton ){
		
		SetKeyboardInputEnabled( false );
		SetIsToggle( true );
		Dock( Gwen::Pos::Top );
		SetSize( mLeftColumnWidth, 24 );
        SetBounds( GetPos().x, GetPos().y, mLeftColumnWidth, 24 );

		mIsOpen				= false;
		mTimeline			= NULL;
		mSelectedKeyframe	= -1;
		mHeight				= TrackList::mTrackHeight;
		mFullHeight			= mHeight * 4.0f;
		mLeftColumnWidth	= TrackList::mLeftColumnWidth;
		mMouseDown			= false;
		
		mTrackHeader		= new Gwen::Controls::Button( this );
        mTrackHeader->SetTextColorOverride( Gwen::Colors::GreyLight );
		mTrackHeader->Dock( Gwen::Pos::Left );
		mTrackHeader->SetIsToggle( true );
		mTrackHeader->SetPadding( Gwen::Padding( 4, 4, 4, 4 ) );
		mTrackHeader->SetAlignment( Gwen::Pos::Left | Gwen::Pos::Top );
		mTrackHeader->SetSize( mLeftColumnWidth, mHeight );
		mTrackHeader->SetText( "TrackName" );
		mTrackHeader->SetMouseInputEnabled( false );
		
		ci::gl::VboMesh::Layout layout;
		layout.setDynamicPositions();
		layout.setDynamicColorsRGBA();
		
		mNeedRenderUpdate = true;
		mGraphVboSamples = 200;
		mGraphVbo = ci::gl::VboMesh( mGraphVboSamples * 2 * 4, 0, layout, GL_LINES );
	}
	
	void TrackButton::Render( Gwen::Skin::Base* skin ){ 
		glDisable( GL_TEXTURE_2D );
	
		Gwen::Rect rect = GetRenderBounds();
		( (Gwen::Skin::TexturedBase*) skin )->Textures.Panel.Dark.Draw( skin->GetRender(), rect );
		
		// Draw Selection
		if( mMouseDown && mSelectionRect.w > 1.0f ){
			skin->GetRender()->SetDrawColor( skin->Colors.Label.Bright );
			Gwen::Rect selection( mLeftColumnWidth + rect.x + mSelectionRect.x, rect.y, mSelectionRect.w, rect.h );
			skin->GetRender()->DrawFilledRect( selection );
		}

		// Draw Start, End and current Time
		Gwen::Point start = CinderGwen::toGwen( mTimeline->getPosFromTime( 0 ) );
		Gwen::Point end = CinderGwen::toGwen( mTimeline->getPosFromTime( mTimeline->getEnd() ) );
		Gwen::Point current = CinderGwen::toGwen( mTimeline->getPosFromTime( mTimeline->getCurrentFrame() ) );
		skin->GetRender()->SetDrawColor( skin->Colors.Label.Dark );
		skin->GetRender()->DrawFilledRect( Gwen::Rect( mLeftColumnWidth + rect.x + start.x, rect.y+mHeight, 1, mHeight*3 ) );
		skin->GetRender()->DrawFilledRect( Gwen::Rect( mLeftColumnWidth + rect.x + end.x, rect.y+mHeight, 1, mHeight*3 ) );
		skin->GetRender()->DrawFilledRect( Gwen::Rect( mLeftColumnWidth + rect.x + current.x, rect.y+mHeight, 1, mHeight*3 ) );

		// Draw Track Details
		if( mIsOpen ){
			renderTrackDetails( skin );
		}

		// Draw Track Block
		Animation::ValueTrack* track = static_cast<Animation::ValueTrack*>( mTrackRef.get() );
		float x = mLeftColumnWidth + rect.x + mTimeline->getPosFromTime( track->getMinTime() ).x;
		float w = mLeftColumnWidth + rect.x + mTimeline->getPosFromTime( track->getMaxTime() ).x - x;
		( (Gwen::Skin::TexturedBase*) skin )->Textures.Panel.Normal.Draw( skin->GetRender(), Gwen::Rect( x, rect.y, w, mHeight ) );
		
	}
	void TrackButton::PostLayout( Gwen::Skin::Base* skin ){
		if ( mIsOpen ){
			SetHeight( mFullHeight );
		}
		else{
			SetHeight( mHeight );
		}
	};
	
	void TrackButton::OnMouseMoved( int x, int y, int deltaX, int deltaY ){
		if( mMouseDown ){
			Gwen::Point pos	= CanvasPosToLocal( Gwen::Point( x - mLeftColumnWidth, y ) );

			if( pos.y < mHeight ) onMainDrag( pos.x, pos.y );
			else if( pos.y < mHeight * 2.0f ) {
				mSelectionRect	= CinderGwen::toGwen( ci::Rectf( ci::math<float>::min( mMouseDownPos.x, mMouseDragPos.x ), ci::math<float>::min( mMouseDownPos.y, mMouseDragPos.y ), 
													ci::math<float>::max( mMouseDownPos.x, mMouseDragPos.x ), ci::math<float>::max( mMouseDownPos.y, mMouseDragPos.y ) ) );
				onSecondaryDrag( pos.x, pos.y );
			}
			else {
				mSelectionRect	= CinderGwen::toGwen( ci::Rectf( ci::math<float>::min( mMouseDownPos.x, mMouseDragPos.x ), ci::math<float>::min( mMouseDownPos.y, mMouseDragPos.y ), 
													ci::math<float>::max( mMouseDownPos.x, mMouseDragPos.x ), ci::math<float>::max( mMouseDownPos.y, mMouseDragPos.y ) ) );
				onDetailsDrag( pos.x, pos.y );
			}
			mMouseDragPos	= pos;

		}
	}
	void TrackButton::OnMouseClickLeft( int x, int y, bool bDown ){
		Gwen::Point pos = CanvasPosToLocal( Gwen::Point( x, y ) );
		if( pos.x > mLeftColumnWidth ){
			if( mSelection.size() <= 1 ) clearSelection();
			pos				= CanvasPosToLocal( Gwen::Point( x - mLeftColumnWidth, y ) );
			mMouseDownPos	= pos;
			
			if( bDown ){
				mMouseDown			= true;
				Gwen::MouseFocus	= this;
			}
			else{
				mMouseDown			= false;
				Gwen::MouseFocus	= NULL;
			}

			if( pos.y < mHeight ) onMainClickLeft( pos.x, pos.y, bDown );
			else if( pos.y < mHeight * 2.0f ) onSecondaryClickLeft( pos.x, pos.y, bDown );
			else onDetailsClickLeft( pos.x, pos.y, bDown );
			
		}
		else {
			BaseClass::OnMouseClickLeft( x, y, bDown );
		}
	}
	void TrackButton::OnMouseDoubleClickLeft( int x, int y ){
		mIsOpen = !mIsOpen;
		if ( mIsOpen ) {
			SetHeight( mFullHeight );
		}
		else {
			SetHeight( mHeight );
		}
		if( !GetToggleState() ) SetToggleState( true );
	}
	
	void TrackButton::onMainClickLeft( int x, int y, bool bDown ){
		if( bDown ){
			if( x >= mTimeline->getPosFromTime( mTrackRef->getMinTime() ).x && x <= mTimeline->getPosFromTime( mTrackRef->getMaxTime() ).x ){
				selectAll();
				float min = 1000000;
				mSelectedKeyframe = -1;
				for( size_t j = 0; j < mTrackRef->size(); j++ ){
					Animation::KeyFrameRef key = mTrackRef->at( j );
					float diff = mTimeline->getPosFromTime( key->mTime ).x - x;
					diff *= diff;
					if( diff < min ) {
						min = diff;
						mSelectedKeyframe = j;
					}
				}
			}
		}
		else {
			clearSelection();
			mSelectionDistances.clear();
		}
	}
	void TrackButton::onSecondaryClickLeft( int x, int y, bool bDown ){
		if( bDown ){
			if( mSelection.size() == 0 ){
				mSelectionRect = Gwen::Rect();
				clearSelection();
				makeSelection( x );
			}
			else {

				float min = 1000000;
				mSelectedKeyframe = -1;
				for( size_t j = 0; j < mTrackRef->size(); j++ ){
					Animation::KeyFrameRef key = mTrackRef->at( j );
					float diff = mTimeline->getPosFromTime( key->mTime ).x - x;
					diff *= diff;
					if( diff < 36 ){
						if( diff < min ) {
							min = diff;
							mSelectedKeyframe = j;
						}
					}
				}

			}
		}
		else {
			if( mSelection.size() == 0 ) makeSelection( x );
			else clearSelection();
			mSelectionDistances.clear();
		}
	}
	void TrackButton::onDetailsClickLeft( int x, int y, bool bDown ){
		if( bDown ){
			makeSelection( x );
		}
		else {
			clearSelection();
			makeSelection( x );
		}
	}

	void TrackButton::onMainDrag( int x, int y ){
			onSecondaryDrag( x, y );
			/*if( mTrackRef != NULL && mTimeline != NULL ){
				ci::Vec2f p = mTimeline->getPosFromTime( mTrackRef->at( 0 )->mTime );
				float delta = p.x - x;
				for( size_t i = 0; i < mTrackRef->size(); i++ ){
					p = mTimeline->getPosFromTime( mTrackRef->at( i )->mTime );
					p.x -= delta;
					float time = mTimeline->getTimeFromPos( CinderGwen::toGwen( p ) );
					mTrackRef->at( i )->mTime = time;
				}
				mTrackRef->sort();
				mTrackRef->refresh();
				mNeedRenderUpdate = true;
			}*/
		
	}
	void TrackButton::onSecondaryDrag( int x, int y ){

		if( mSelection.size() && mTrackRef != NULL && mTimeline != NULL ){
			if( mSelection.size() == 1 ) mSelectedKeyframe = mSelection[ 0 ];

			ci::Vec2f selected = mTimeline->getPosFromTime( mTrackRef->at( mSelectedKeyframe )->mTime );

			if( mSelectionDistances.empty() ){
				for ( size_t i = 0; i < mSelection.size(); i++ ) {
					ci::Vec2f p = mTimeline->getPosFromTime( mTrackRef->at( mSelection[i] )->mTime );					
					mSelectionDistances.push_back( p.x - selected.x );
				}
			}

			selected.x -= (int) selected.x - (int) x;

			float time = mTimeline->getTimeFromPos( CinderGwen::toGwen( selected ) );
			mTrackRef->at( mSelectedKeyframe )->mTime = time;

			for( size_t i = 0; i < mSelection.size(); i++ ){
				ci::Vec2f p = selected + ci::Vec2f( mSelectionDistances[i], 0.0f );
				float time = mTimeline->getTimeFromPos( CinderGwen::toGwen( p ) );
				mTrackRef->at( mSelection[i] )->mTime = time;
			}
			mTrackRef->sort();
			mTrackRef->refresh();
			mNeedRenderUpdate = true;

			for ( size_t i = 0; i < mSelection.size(); i++ ) {
				//ci::Vec2f p = mTimeline->getPosFromTime( mTrackRef->at( mSelection[i] )->mTime );
			}
		}
		else if( mSelection.empty() ){
			//makeSelection( x );
		}
	}
	void TrackButton::onDetailsDrag( int x, int y ){
		onSecondaryDrag( x, y );
	}

	void TrackButton::select( size_t i ){
		clearSelection();
		addToSelection( i );
	}
	void TrackButton::selectAll(){
		clearSelection();
		for( size_t i = 0; i < mTrackRef->size(); i++ )
			addToSelection( i );
	}
	void TrackButton::addToSelection( size_t i ){
		mSelection.push_back( i );
	}
	void TrackButton::removeFromSelection( size_t i ){
		for( size_t i = 0; i < mTrackRef->size(); i++ )
			addToSelection( i );
	}
	void TrackButton::clearSelection(){
		mSelection.clear();
	}
	
	void TrackButton::makeSelection( int x ){
		if( mTrackRef != NULL && mTimeline != NULL ){
			
			if( mSelectionRect.w < 5.0f ){
				float min = 1000000;
				size_t i = -1;
				for( size_t j = 0; j < mTrackRef->size(); j++ ){
					Animation::KeyFrameRef key = mTrackRef->at( j );
					float diff = mTimeline->getPosFromTime( key->mTime ).x - x;
					diff *= diff;
					if( diff < 36 ){
						if( diff < min ) {
							min = diff;
							i = j;
						}
					}
				}
				if( i != -1 ) addToSelection( i );
			}
			else {
				ci::Rectf rect = CinderGwen::toCinder( mSelectionRect );
				for( size_t j = 0; j < mTrackRef->size(); j++ ){
					Animation::KeyFrameRef key = mTrackRef->at( j );
					float keyPos	= mTimeline->getPosFromTime( key->mTime ).x;
					if( keyPos >= rect.x1 && keyPos <= rect.x2 ){
						addToSelection( j );
					}
				}
				mSelectionRect = Gwen::Rect();
			}
		}

	}
	const std::vector<size_t>& TrackButton::getSelection(){
		return mSelection;
	}

	bool TrackButton::OnKeyBackspace( bool bDown ){
		return OnKeyDelete( bDown );
	}
	bool TrackButton::OnKeyDelete( bool bDown ){
		if( bDown ){
			if( mSelectedKeyframe != -1 ){
				mTrackRef->erase( mTrackRef->begin() + mSelectedKeyframe );
				mTrackRef->refresh();
				return true;
			}
		}
		return false;
	}
	void TrackButton::SetToggleState( bool b ){
		mTrackHeader->SetToggleState( b );
		BaseClass::SetToggleState( b );
	}
	void TrackButton::OnBoundsChanged( Gwen::Rect rect ){
		BaseClass::OnBoundsChanged( rect );
		mNeedRenderUpdate = true;
	}

	//-------------------------------------------------------------------------------------------------------------------------------

	GWEN_CONTROL_CONSTRUCTOR( ValueTrackButton ){
		
	}
	
	void ValueTrackButton::renderTrackDetails( Gwen::Skin::Base* skin ){
		Gwen::Rect rect = GetRenderBounds();
		// Draw Separation
		//( (Gwen::Skin::TexturedBase*) skin )->Textures.Input.Button.Disabled.Draw( skin->GetRender(), Gwen::Rect( rect.x, rect.y+mHeight, rect.w, 1 ) );
		skin->GetRender()->SetDrawColor( skin->Colors.Label.Dark );
		skin->GetRender()->DrawFilledRect( Gwen::Rect( rect.x+5, rect.y+mHeight, rect.w, 1 ) );
		//skin->GetRender()->DrawFilledRect( Gwen::Rect( rect.x, rect.y+mHeight*2, rect.w, 1 ) );

		// Draw Keyframes
		for( size_t j = 0; j < mTrackRef->size(); j++ ){
			Animation::KeyFrameRef keyframe = mTrackRef->at( j );
			ci::Vec2f pos = mTimeline->getPosFromTime( keyframe->mTime );
			skin->GetRender()->SetDrawColor( skin->Colors.Tree.Lines );
			skin->GetRender()->DrawFilledRect( Gwen::Rect( mLeftColumnWidth + rect.x + pos.x, rect.y, 1, mFullHeight ) );
			( (Gwen::Skin::TexturedBase*) skin )->Textures.Input.Slider.H.Disabled.DrawCenter( skin->GetRender(), Gwen::Rect( mLeftColumnWidth + rect.x + pos.x - 5, rect.y + mHeight, 10, mHeight ) );
		}
		// Draw Selected Keyframes
		for( size_t j = 0; j < mSelection.size(); j++ ){
			Animation::KeyFrameRef keyframe = mTrackRef->at( mSelection[j] );
			ci::Vec2f pos = mTimeline->getPosFromTime( keyframe->mTime );
			skin->GetRender()->SetDrawColor( skin->Colors.Tree.Lines );
			skin->GetRender()->DrawFilledRect( Gwen::Rect( mLeftColumnWidth + rect.x + pos.x, rect.y, 1, mFullHeight ) );
			( (Gwen::Skin::TexturedBase*) skin )->Textures.Input.Slider.H.Down.DrawCenter( skin->GetRender(), Gwen::Rect( mLeftColumnWidth + rect.x + pos.x - 5, rect.y + mHeight, 10, mHeight ) );
		}
		glDisable( GL_TEXTURE_2D );

		// Draw Graph
		Gwen::Rect tr = GetRenderBounds();
		skin->GetRender()->Translate( tr );
		ci::TimelineRef timelineRef = mTrackRef->getTimelineRef();
		Animation::ValueTrack* track= static_cast<Animation::ValueTrack*>( mTrackRef.get() );
		float duration				= mTimeline->getEnd();
		float inc					= duration / (float) mGraphVboSamples;
		float min					= track->getMinValue();
		float max					= track->getMaxValue();
		int type					= track->getTarget().getType();
		ci::ColorA	red				= ci::ColorA( 1.0f, 0.411f, 0.411f ,1.0f );
		ci::ColorA	green			= ci::ColorA( 0.615f, 1.0f, 0.411f ,1.0f );
		ci::ColorA	blue			= ci::ColorA( 0.411f, 0.674f, 1.0f,1.0f );
		float graphMargin			= 5;
		if( type == AnimType_float ){
			if( mNeedRenderUpdate ) {
				float timeSave = timelineRef->getCurrentTime();
				ci::gl::VboMesh::VertexIter iter = mGraphVbo.mapVertexBuffer();
				for( float x = inc; x <= duration; x += inc ) {
					ci::Vec2f p = mTimeline->getPosFromTime( x - inc );
					timelineRef->stepTo( x - inc );
					float val = ci::lmap<float>( *track->getTarget().cast<float>(), min, max, mHeight * 2.0f - graphMargin, graphMargin );
					float xx = mLeftColumnWidth + tr.x + p.x;
					float yy = tr.y+mHeight*2.0f+val;
					iter.setPosition( ci::Vec3f( xx, yy, 0.0f ) );
					iter.setColorRGBA( ci::ColorA::white() );
					++iter;

					p = mTimeline->getPosFromTime( x );
					timelineRef->stepTo( x );
					val = ci::lmap<float>( *track->getTarget().cast<float>(), min, max, mHeight * 2.0f - graphMargin, graphMargin );
					xx = mLeftColumnWidth + tr.x + p.x;
					yy = tr.y+mHeight*2.0f+val;
						
					iter.setPosition( ci::Vec3f( xx, yy, 0.0f ) );
					iter.setColorRGBA( ci::ColorA::white() );
					++iter;
				}
				timelineRef->stepTo( timeSave );

				mNeedRenderUpdate = false;
			}
			ci::gl::drawArrays( mGraphVbo, 0, mGraphVboSamples * 2 );
		}
		else if( type == AnimType_Vec2f ){
			if( mNeedRenderUpdate ) {
				float timeSave = timelineRef->getCurrentTime();
				ci::gl::VboMesh::VertexIter iter = mGraphVbo.mapVertexBuffer();
				for( float x = inc; x <= duration; x += inc ) {
					ci::Vec2f p1 = mTimeline->getPosFromTime( x - inc );
					timelineRef->stepTo( x - inc );
					ci::Vec2f val1 = *track->getTarget().cast<ci::Vec2f>();
					ci::Vec2f p2 = mTimeline->getPosFromTime( x );
					timelineRef->stepTo( x );
					ci::Vec2f val2 = *track->getTarget().cast<ci::Vec2f>();
						
					iter.setPosition( ci::Vec3f( mLeftColumnWidth + tr.x + p1.x, tr.y+mHeight*2.0f+ci::lmap<float>( val1.x, min, max, mHeight * 2.0f - graphMargin, graphMargin ), 0.0f ) );
					iter.setColorRGBA( red );
					++iter;
						
					iter.setPosition( ci::Vec3f( mLeftColumnWidth + tr.x + p2.x, tr.y+mHeight*2.0f+ci::lmap<float>( val2.x, min, max, mHeight * 2.0f - graphMargin, graphMargin ), 0.0f ) );
					iter.setColorRGBA( red );
					++iter;
						
					iter.setPosition( ci::Vec3f( mLeftColumnWidth + tr.x + p1.x, tr.y+mHeight*2.0f+ci::lmap<float>( val1.y, min, max, mHeight * 2.0f - graphMargin, graphMargin ), 0.0f ) );
					iter.setColorRGBA( green );
					++iter;
						
					iter.setPosition( ci::Vec3f( mLeftColumnWidth + tr.x + p2.x, tr.y+mHeight*2.0f+ci::lmap<float>( val2.y, min, max, mHeight * 2.0f - graphMargin, graphMargin ), 0.0f ) );
					iter.setColorRGBA( green );
					++iter;
				}
				timelineRef->stepTo( timeSave );

				mNeedRenderUpdate = false;
			}
			ci::gl::drawArrays( mGraphVbo, 0, mGraphVboSamples * 2 * 2 );			
		}
		else if( type == AnimType_Vec3f ){
			if( mNeedRenderUpdate ) {
				float timeSave = timelineRef->getCurrentTime();
				ci::gl::VboMesh::VertexIter iter = mGraphVbo.mapVertexBuffer();
				for( float x = inc; x <= duration; x += inc ) {
					ci::Vec2f p1 = mTimeline->getPosFromTime( x - inc );
					timelineRef->stepTo( x - inc );
					ci::Vec3f val1 = *track->getTarget().cast<ci::Vec3f>();
					ci::Vec2f p2 = mTimeline->getPosFromTime( x );
					timelineRef->stepTo( x );
					ci::Vec3f val2 = *track->getTarget().cast<ci::Vec3f>();
						
					iter.setPosition( ci::Vec3f( mLeftColumnWidth + tr.x + p1.x, tr.y+mHeight*2.0f+ci::lmap<float>( val1.x, min, max, mHeight * 2.0f - graphMargin, graphMargin ), 0.0f ) );
					iter.setColorRGBA( red );
					++iter;
						
					iter.setPosition( ci::Vec3f( mLeftColumnWidth + tr.x + p2.x, tr.y+mHeight*2.0f+ci::lmap<float>( val2.x, min, max, mHeight * 2.0f - graphMargin, graphMargin ), 0.0f ) );
					iter.setColorRGBA( red );
					++iter;
						
					iter.setPosition( ci::Vec3f( mLeftColumnWidth + tr.x + p1.x, tr.y+mHeight*2.0f+ci::lmap<float>( val1.y, min, max, mHeight * 2.0f - graphMargin, graphMargin ), 0.0f ) );
					iter.setColorRGBA( green );
					++iter;
						
					iter.setPosition( ci::Vec3f( mLeftColumnWidth + tr.x + p2.x, tr.y+mHeight*2.0f+ci::lmap<float>( val2.y, min, max, mHeight * 2.0f - graphMargin, graphMargin ), 0.0f ) );
					iter.setColorRGBA( green );
					++iter;
						
					iter.setPosition( ci::Vec3f( mLeftColumnWidth + tr.x + p1.x, tr.y+mHeight*2.0f+ci::lmap<float>( val1.z, min, max, mHeight * 2.0f - graphMargin, graphMargin ), 0.0f ) );
					iter.setColorRGBA( blue );
					++iter;
						
					iter.setPosition( ci::Vec3f( mLeftColumnWidth + tr.x + p2.x, tr.y+mHeight*2.0f+ci::lmap<float>( val2.z, min, max, mHeight * 2.0f - graphMargin, graphMargin ), 0.0f ) );
					iter.setColorRGBA( blue );
					++iter;
				}
				timelineRef->stepTo( timeSave );

				mNeedRenderUpdate = false;
			}
			ci::gl::drawArrays( mGraphVbo, 0, mGraphVboSamples * 2 * 3 );	
			
		}
		else if( type == AnimType_Color ){
		if( mNeedRenderUpdate ) {
				float timeSave = timelineRef->getCurrentTime();
				ci::gl::VboMesh::VertexIter iter = mGraphVbo.mapVertexBuffer();
				for( float x = inc; x <= duration; x += inc ) {
					ci::Vec2f p1 = mTimeline->getPosFromTime( x - inc );
					timelineRef->stepTo( x - inc );
					ci::Color val1 = *track->getTarget().cast<ci::Color>();
					ci::Vec2f p2 = mTimeline->getPosFromTime( x );
					timelineRef->stepTo( x );
					ci::Color val2 = *track->getTarget().cast<ci::Color>();
						
					iter.setPosition( ci::Vec3f( mLeftColumnWidth + tr.x + p1.x, tr.y+mHeight*2.0f+ci::lmap<float>( val1.r, min, max, mHeight * 2.0f - graphMargin, graphMargin ), 0.0f ) );
					iter.setColorRGBA( red );
					++iter;
						
					iter.setPosition( ci::Vec3f( mLeftColumnWidth + tr.x + p2.x, tr.y+mHeight*2.0f+ci::lmap<float>( val2.r, min, max, mHeight * 2.0f - graphMargin, graphMargin ), 0.0f ) );
					iter.setColorRGBA( red );
					++iter;
						
					iter.setPosition( ci::Vec3f( mLeftColumnWidth + tr.x + p1.x, tr.y+mHeight*2.0f+ci::lmap<float>( val1.g, min, max, mHeight * 2.0f - graphMargin, graphMargin ), 0.0f ) );
					iter.setColorRGBA( green );
					++iter;
						
					iter.setPosition( ci::Vec3f( mLeftColumnWidth + tr.x + p2.x, tr.y+mHeight*2.0f+ci::lmap<float>( val2.g, min, max, mHeight * 2.0f - graphMargin, graphMargin ), 0.0f ) );
					iter.setColorRGBA( green );
					++iter;
						
					iter.setPosition( ci::Vec3f( mLeftColumnWidth + tr.x + p1.x, tr.y+mHeight*2.0f+ci::lmap<float>( val1.b, min, max, mHeight * 2.0f - graphMargin, graphMargin ), 0.0f ) );
					iter.setColorRGBA( blue );
					++iter;
						
					iter.setPosition( ci::Vec3f( mLeftColumnWidth + tr.x + p2.x, tr.y+mHeight*2.0f+ci::lmap<float>( val2.b, min, max, mHeight * 2.0f - graphMargin, graphMargin ), 0.0f ) );
					iter.setColorRGBA( blue );
					++iter;
				}
				timelineRef->stepTo( timeSave );

				mNeedRenderUpdate = false;
			}
			ci::gl::drawArrays( mGraphVbo, 0, mGraphVboSamples * 2 * 3 );	
		}
		else if( type == AnimType_ColorA ){
			if( mNeedRenderUpdate ) {
				float timeSave = timelineRef->getCurrentTime();
				ci::gl::VboMesh::VertexIter iter = mGraphVbo.mapVertexBuffer();
				for( float x = inc; x <= duration; x += inc ) {
					ci::Vec2f p1 = mTimeline->getPosFromTime( x - inc );
					timelineRef->stepTo( x - inc );
					ci::ColorA val1 = *track->getTarget().cast<ci::ColorA>();
					ci::Vec2f p2 = mTimeline->getPosFromTime( x );
					timelineRef->stepTo( x );
					ci::ColorA val2 = *track->getTarget().cast<ci::ColorA>();
						
					iter.setPosition( ci::Vec3f( mLeftColumnWidth + tr.x + p1.x, tr.y+mHeight*2.0f+ci::lmap<float>( val1.r, min, max, mHeight * 2.0f - graphMargin, graphMargin ), 0.0f ) );
					iter.setColorRGBA( red );
					++iter;
						
					iter.setPosition( ci::Vec3f( mLeftColumnWidth + tr.x + p2.x, tr.y+mHeight*2.0f+ci::lmap<float>( val2.r, min, max, mHeight * 2.0f - graphMargin, graphMargin ), 0.0f ) );
					iter.setColorRGBA( red );
					++iter;
						
					iter.setPosition( ci::Vec3f( mLeftColumnWidth + tr.x + p1.x, tr.y+mHeight*2.0f+ci::lmap<float>( val1.g, min, max, mHeight * 2.0f - graphMargin, graphMargin ), 0.0f ) );
					iter.setColorRGBA( green );
					++iter;
						
					iter.setPosition( ci::Vec3f( mLeftColumnWidth + tr.x + p2.x, tr.y+mHeight*2.0f+ci::lmap<float>( val2.g, min, max, mHeight * 2.0f - graphMargin, graphMargin ), 0.0f ) );
					iter.setColorRGBA( green );
					++iter;
						
					iter.setPosition( ci::Vec3f( mLeftColumnWidth + tr.x + p1.x, tr.y+mHeight*2.0f+ci::lmap<float>( val1.b, min, max, mHeight * 2.0f - graphMargin, graphMargin ), 0.0f ) );
					iter.setColorRGBA( blue );
					++iter;
						
					iter.setPosition( ci::Vec3f( mLeftColumnWidth + tr.x + p2.x, tr.y+mHeight*2.0f+ci::lmap<float>( val2.b, min, max, mHeight * 2.0f - graphMargin, graphMargin ), 0.0f ) );
					iter.setColorRGBA( blue );
					++iter;
						
					iter.setPosition( ci::Vec3f( mLeftColumnWidth + tr.x + p1.x, tr.y+mHeight*2.0f+ci::lmap<float>( val1.a, min, max, mHeight * 2.0f - graphMargin, graphMargin ), 0.0f ) );
					iter.setColorRGBA( ci::ColorA::white() );
					++iter;
						
					iter.setPosition( ci::Vec3f( mLeftColumnWidth + tr.x + p2.x, tr.y+mHeight*2.0f+ci::lmap<float>( val2.a, min, max, mHeight * 2.0f - graphMargin, graphMargin ), 0.0f ) );
					iter.setColorRGBA( ci::ColorA::white() );
					++iter;
				}
				timelineRef->stepTo( timeSave );

				mNeedRenderUpdate = false;
			}
			ci::gl::drawArrays( mGraphVbo, 0, mGraphVboSamples * 2 * 4 );	
			
		}
	}
	
	
	//-------------------------------------------------------------------------------------------------------------------------------

	GWEN_CONTROL_CONSTRUCTOR( CueTrackButton ){
		mFullHeight = mHeight * 2.0f;
	}
	
	void CueTrackButton::renderTrackDetails( Gwen::Skin::Base* skin ){
		Gwen::Rect rect = GetRenderBounds();
		
		// Draw Keyframes
		for( size_t j = 0; j < mTrackRef->size(); j++ ){
			Animation::KeyFrameRef keyframe = mTrackRef->at( j );
			ci::Vec2f pos = mTimeline->getPosFromTime( keyframe->mTime );
			skin->GetRender()->SetDrawColor( skin->Colors.Tree.Lines );
			skin->GetRender()->DrawFilledRect( Gwen::Rect( mLeftColumnWidth + rect.x + pos.x, rect.y, 1, mFullHeight ) );
			if( j == mSelectedKeyframe )( (Gwen::Skin::TexturedBase*) skin )->Textures.Input.Slider.H.Down.DrawCenter( skin->GetRender(), Gwen::Rect( mLeftColumnWidth + rect.x + pos.x - 4, rect.y + mHeight, 10, mHeight ) );
			else ( (Gwen::Skin::TexturedBase*) skin )->Textures.Input.Slider.H.Disabled.DrawCenter( skin->GetRender(), Gwen::Rect( mLeftColumnWidth + rect.x + pos.x - 4, rect.y + mHeight, 10, mHeight ) );
		}
		glDisable( GL_TEXTURE_2D );

		// Draw Separation
		skin->GetRender()->SetDrawColor( skin->Colors.Tree.Normal );
		skin->GetRender()->DrawFilledRect( Gwen::Rect( rect.x, rect.y+mHeight, rect.w, 1 ) );
		
	}
	
	//-------------------------------------------------------------------------------------------------------------------------------

	GWEN_CONTROL_CONSTRUCTOR( BlockTrackButton ){
		mFullHeight = mHeight * 2.0f;		
        
	}
	
	void BlockTrackButton::renderTrackDetails( Gwen::Skin::Base* skin ){
		Gwen::Rect rect = GetRenderBounds();
		
		// Draw Keyframes
		for( size_t j = 0; j < mTrackRef->size(); j++ ){
			Animation::BlockFrame* keyframe = static_cast<Animation::BlockFrame*>( mTrackRef->at( j ).get() );
			ci::Vec2f pos = mTimeline->getPosFromTime( keyframe->mTime );
			skin->GetRender()->SetDrawColor( skin->Colors.Tree.Lines );
			skin->GetRender()->DrawFilledRect( Gwen::Rect( mLeftColumnWidth + rect.x + pos.x, rect.y, 1, mFullHeight ) );
			if( j == mSelectedKeyframe )( (Gwen::Skin::TexturedBase*) skin )->Textures.Input.Slider.H.Down.DrawCenter( skin->GetRender(), Gwen::Rect( mLeftColumnWidth + rect.x + pos.x - 4, rect.y + mHeight, 10, mHeight ) );
			else ( (Gwen::Skin::TexturedBase*) skin )->Textures.Input.Slider.H.Disabled.DrawCenter( skin->GetRender(), Gwen::Rect( mLeftColumnWidth + rect.x + pos.x - 4, rect.y + mHeight, 10, mHeight ) );
		}
		glDisable( GL_TEXTURE_2D );

		// Draw Separation
		skin->GetRender()->SetDrawColor( skin->Colors.Tree.Normal );
		skin->GetRender()->DrawFilledRect( Gwen::Rect( rect.x, rect.y+mHeight, rect.w, 1 ) );
	}


	//-------------------------------------------------------------------------------------------------------------------------------
	

	GWEN_CONTROL_CONSTRUCTOR( TrackList ){
		Dock( Gwen::Pos::Top | Gwen::Pos::Fill );
		SetMouseInputEnabled( true );
		SetScroll( false, true );
		SetAutoHideBars( false );

		mTrackList			= NULL;
		mTimeline			= NULL;
		
		mSelectedTrack		= -1;
		mSelectedKeyframe	= -1;

		mMouseDown			= false;
		
	}
	
	void TrackList::updateGraphVbos(){
		Base::List& children = GetChildren();
		for ( Base::List::iterator iter = children.begin(); iter != children.end(); ++iter ){
			TrackButton* pChild = dynamic_cast<TrackButton*>(*iter);
			if ( pChild ) pChild->setNeedRenderUpdate();
		}
	}
	void TrackList::OnBoundsChanged( Gwen::Rect rect ){
		updateGraphVbos();
		Gwen::Controls::ScrollControl::OnBoundsChanged( rect );
	}
	void TrackList::Invalidate(){
		updateGraphVbos();
		Gwen::Controls::ScrollControl::Invalidate();
	};
	void TrackList::VBarMoved(Gwen::Controls::Base * control){
		updateGraphVbos();
		Gwen::Controls::ScrollControl::VBarMoved( control );
	}
	bool TrackList::OnMouseWheeled( int iDelta ){
		updateGraphVbos();
		return Gwen::Controls::ScrollControl::OnMouseWheeled( iDelta );
	}
	void TrackList::setTimeline( Timeline* timeline ){
		mTimeline = timeline;
	}
	void TrackList::setTrackList( std::vector< Animation::TrackRef >*	trackList ){
		//RemoveAllChildren();
		Base::List& children = GetChildren();
		for ( Base::List::iterator iter = children.begin(); iter != children.end(); ++iter ){
			TrackButton* pChild = dynamic_cast<TrackButton*>(*iter);
			if ( pChild ) RemoveChild(*iter);
		}

		mTrackList = trackList;
		for( size_t i = 0; i < trackList->size(); i++ ){
			TrackButton* track = NULL;
			int trackType = trackList->at( i )->getType();
			if( trackType == Animation::Track_Value ){
				track = new ValueTrackButton( this );
			}
			else if( trackType == Animation::Track_Cue ){
				track = new CueTrackButton( this );
			}
			else if( trackType == Animation::Track_Block ){
				track = new BlockTrackButton( this );
			}

			if( track != NULL ){
				track->addOnSelectionCallback( this, &TrackList::onSelection );
				track->addOnDeselectionCallback( this, &TrackList::onDeselection );
				track->setTrackRef( trackList->at( i ) );
				track->setTimelinePtr( mTimeline );
				track->setId( i );
				track->setName( trackList->at( i )->getName() );
				track->setNeedRenderUpdate();
				mTrackButtons.push_back( track );
			}
		}
	}
	void TrackList::onSelection( Gwen::Controls::Base* control ){
		TrackButton* selected = dynamic_cast<TrackButton*>( control );
		if( !selected ) return;

		Base::List& children = GetChildren();
		for ( Base::List::iterator iter = children.begin(); iter != children.end(); ++iter ){
			TrackButton* pChild = dynamic_cast<TrackButton*>(*iter);
			if ( pChild && pChild->getId() != selected->getId() ) {
				pChild->SetToggleState( false );
			}
		}

		mSelectedTrack = selected->getId();
	}
	void TrackList::onDeselection( Gwen::Controls::Base* control ){
		mSelectedTrack =  -1;
	}

	//void TrackList::Render( Gwen::Skin::Base* skin ){ 
	//	glDisable( GL_TEXTURE_2D );
	//
	//	Gwen::Rect rect = GetRenderBounds();
	//	( (Gwen::Skin::TexturedBase*) skin )->Textures.Scroller.ButtonH_Normal.Draw( skin->GetRender(), rect );
	//	//skin->GetRender()->Translate( rect );
	//	if( mTrackList != NULL && mTimeline != NULL ){
	//		for( size_t i = 0; i < mTrackList->size(); i++ ){
	//			/*
	//			Gwen::Rect r( rect.x, rect.y + i * mTrackHeight, rect.w, mTrackHeight );
	//			( (Gwen::Skin::TexturedBase*) skin )->Textures.Scroller.ButtonH_Hover.Draw( skin->GetRender(), r );
	//			for( size_t j = 0; j < mTrackList->at( i )->size(); j++ ){
	//				Animation::KeyFrameRef keyframe = mTrackList->at( i )->at( j );
	//				Gwen::Point pos = mTimeline->getPosFromTime( keyframe->mTime );
	//				if( i == mSelectedTrack && j == mSelectedKeyframe ) ( (Gwen::Skin::TexturedBase*) skin )->Textures.Input.Slider.H.Down.DrawCenter( skin->GetRender(), Gwen::Rect( rect.x + pos.x - 5, rect.y + i * mTrackHeight, 10, mTrackHeight ) );
	//				else ( (Gwen::Skin::TexturedBase*) skin )->Textures.Input.Slider.H.Disabled.DrawCenter( skin->GetRender(), Gwen::Rect( rect.x + pos.x - 5, rect.y + i * mTrackHeight, 10, mTrackHeight ) );
	//			}*/
	//		}
	//	}
	////}
	//
	//void TrackList::OnMouseMoved( int x, int y, int deltaX, int deltaY ){
	//	
	//}
	//void TrackList::OnMouseClickLeft( int x, int y, bool bDown ){

	//	
	//}
	//void TrackList::OnMouseClickRight( int x, int y, bool bDown ){
	//}

	//
	//bool TrackList::OnKeyBackspace( bool bDown ){
	//	return OnKeyDelete( bDown );
	//}
	//bool TrackList::OnKeyDelete( bool bDown ){
	//	return false;
	//}
}}
