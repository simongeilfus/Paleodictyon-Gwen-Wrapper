#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"

#include "Gwen/Gwen.h"
#include "Gwen/Skins/Simple.h"
#include "Gwen/Skins/TexturedBase.h"

#include "Gwen/BaseRender.h"
#include "Gwen/Controls/Base.h"
#include "Gwen/Controls/Text.h"
#include "Gwen/Controls.h"
#include "Gwen/Controls/WindowControl.h"
#include "Gwen/Controls/NumericUpDown.h"
#include "Gwen/Controls/ScrollControl.h"

#include "CinderGwen.h"
#include "Params/Keyframer.h"
#include "Params/TimelineWidget/TimelineWidget.h"

#include "AnimRef.h"
#include "Track.h"

namespace CinderGwen {
    
    class Param : public Gwen::Controls::Base {
    public:
        
        GWEN_CONTROL( Param, Gwen::Controls::Base );
        
        void setLabel( std::string text );
        void setControl( Gwen::Controls::Base* control );
        
        //void OnBoundsChanged(Gwen::Rect oldBounds)
       // {
         //   BaseClass::OnBoundsChanged( oldBounds );
        //}
    //private:
        
        
		void PostLayout( Gwen::Skin::Base* skin )
		{
            //BaseClass::Layout( skin );
			//bool resized =
            SizeToChildren( true, true );
           // mOnLayout.Call( this );
            
            //SizeToChildren();
            //GetParent()->SizeToChildren();
            //GetParent()->GetParent()->SizeToChildren();    
            //ci::app::console() << "Param sizetochilds " <<  resized << std::endl;
            //if( resized ){
            //    ci::app::console() << "      " <<  GetSize().x << " " << GetSize().y << std::endl;
            //}
		}
        
        void Render( Gwen::Skin::Base* skin );
        
        void valueChanged( Gwen::Controls::Base* control );
        void onNewKeyFrame( Gwen::Controls::Base* control );
        
        Gwen::Controls::Label*          mLabel;
        Gwen::Controls::Base*           mControl;
        Keyframer*                      mKeyframer;
        
        Gwen::Event::Caller             mOnLayout;
        
        AnimRef                         mAnimRef;
        
    };
    
    class ParamsWindow : public Gwen::Controls::WindowControl {
    public:
        
        GWEN_CONTROL( ParamsWindow, Gwen::Controls::WindowControl );
        
        void Invalidate(){
            BaseClass::Invalidate();
            mLayoutDone = false;
        }
		void PostLayout( Gwen::Skin::Base* skin )
		{
            if( !mLayoutDone ){
                bool resized = SizeToChildren( true, true );
                if(resized) {
                    SizeToChildren();
                }
                //mLayoutDone = true;
            }
        }
        
        bool mLayoutDone;
    };
	
    class Params : public Gwen::Event::Handler {
    public:
        Params();
        Params( const std::string &title, const ci::Vec2i &size );
        ~Params();
        
        void	show( bool visible = true );
        void	hide();
        bool	isVisible() const;
        
        void	addParam( const std::string &name, ci::Anim<bool> *boolParam, int layout = Gwen::Pos::Top, float colWidth = 120.0f, const std::string& group = "none", const std::string& tab = "none" );
        void	addParam( const std::string &name, ci::Anim<float> *floatParam, float min = -1000.0f, float max = 1000.0f, float step = 0.1f, int floatPrecision = 3, int layout = Gwen::Pos::Top, float colWidth = 120.0f, const std::string& group = "none", const std::string& tab = "none" );
        void	addParam( const std::string &name, ci::Anim<int32_t> *intParam, int min = -1000, int max = 1000, int step = 1, int layout = Gwen::Pos::Top, float colWidth = 120.0f, const std::string& group = "none", const std::string& tab = "none" );
        void	addParam( const std::string &name, ci::Anim<ci::Vec2f> *vectorParam, ci::Vec2f min = ci::Vec2f( -1000.0f, -1000.0f ), ci::Vec2f max = ci::Vec2f( 1000.0f, 1000.0f ), float step = 0.1f, int floatPrecision = 3, int layout = Gwen::Pos::Top, float colWidth = 120.0f, bool vertical = true, const std::string& group = "none", const std::string& tab = "none" );
        void	addParam( const std::string &name, ci::Anim<ci::Vec3f> *vectorParam, ci::Vec3f min = ci::Vec3f( -1000.0f, -1000.0f, -1000.0f ), ci::Vec3f max = ci::Vec3f( 1000.0f, 1000.0f, 1000.0f ), float step = 0.1f, int floatPrecision = 3, int layout = Gwen::Pos::Top, float colWidth = 120.0f, bool vertical = true, const std::string& group = "none", const std::string& tab = "none" );
        
        
        void	addParam( const std::string &name, ci::Anim<ci::Color> *colorParam, int layout = Gwen::Pos::Top, float colWidth = 120.0f, const std::string& group = "none", const std::string& tab = "none" );
        void	addParam( const std::string &name, ci::Anim<ci::ColorA> *colorParam, int layout = Gwen::Pos::Top, float colWidth = 120.0f, const std::string& group = "none", const std::string& tab = "none"  );
        void	addParam( const std::string &name, std::string *strParam, const std::string &optionsStr = "", bool readOnly = false );
        void    addParam( const std::string &name, const std::vector<std::string> &enumNames, ci::Anim<int> *param, const std::string &optionsStr = "", bool readOnly = false );
        void	addSeparator( const std::string &name = "", const std::string &optionsStr = "" );
        void	addText( const std::string &name = "", const std::string &optionsStr = "" );
        void	addButton( const std::string &name, const std::function<void()> &callback, const std::string &optionsStr = "" );
        
        void	setOptions( const std::string &name = "", const std::string &optionsStr = "" );
        
        Gwen::Controls::Base* getControl();
        
    private:
        void onLayout( Gwen::Controls::Base* control );
        
        
        Gwen::Controls::Base* mControl;
        ParamsWindow* mWindow;
        Gwen::Controls::ScrollControl* mScroll;
        
    };
    
    class Timeline : public Gwen::Event::Handler {
    public:
        Timeline();
        Timeline( const std::string &title, const ci::Vec2i &size, float totalTime, ci::TimelineRef timeline );
        
        void addTrack( Animation::TrackRef track );
        void addKeyframe( std::string trackName, AnimRef anim, Animation::KeyFrameRef keyframe );
        
        static void         setCurrentTimeline( Timeline* timeline ){ currentTimeline = timeline; }
        static Timeline*    getCurrentTimeline() { return currentTimeline; }
        
        void update();
        
    private:
        
        void onUpdate( Gwen::Controls::Base* control );
        
        Widgets::TimelineWidget*            mTimelineWidget;
        Gwen::Controls::Base*               mControl;
        std::vector< Animation::TrackRef >  mTrackList;
        ci::TimelineRef                     mTimeline;
        
        static Timeline*                    currentTimeline;
    };

};