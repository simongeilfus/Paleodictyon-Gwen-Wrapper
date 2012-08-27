#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"

#include "cinder/DataSource.h"
#include "cinder/DataTarget.h"
#include "cinder/Vector.h"
#include "cinder/Utilities.h"
#include "cinder/Json.h"

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
#include "Params/RenderCanvas.h"


#include <boost/any.hpp>
#include <vector>



#define GWEN_TIMELINE

#ifdef GWEN_TIMELINE

    #include "Track.h"
    #include "Params/TimelineWidget/TimelineWidget.h"

#endif

#define GWEN_FLOW

#ifdef GWEN_FLOW

    #include "Flow.h"

#endif

#include "AnimRef.h"

namespace CinderGwen {
    
    
    //-----------------------------------------------------------------------------
    //      Inspectable
    //-----------------------------------------------------------------------------
    
    class Inspectable {
    public:
        
        struct Property {
            Property(){}
            Property( const std::string &type, const boost::any& value ) : mValue( value ), mType( type ) {}
            boost::any  mValue;
            std::string mType;
        };
        
        typedef std::map< std::string, Property > PropertyMap;
        
        struct Parameter {
            
            Parameter( const std::string &name, boost::any animPtr, const std::string &group = "", const PropertyMap& properties = PropertyMap() ) : mValue( animPtr ), mName( name ), mGroup( group ), mProperties( properties ), mEditable( true ), mAnimatable( true ){
            }
            
            Parameter& prop( const std::string &type, const boost::any &value ){ mProperties[ type ] = Property( type, value );return *this; }
            Parameter& min( const boost::any& value ){ return prop( "min", value ); }
            Parameter& max( const boost::any& value ){ return prop( "max", value ); }
            Parameter& step( const boost::any& value ){ return prop( "step", value ); }
            
            Parameter& editable( bool b ){ mEditable = b; return *this; }
            Parameter& animatable( bool b ){ mAnimatable = b; return *this; }
            
            bool hasProperty( const std::string &type ){ return mProperties.count( type ); }
            
            template<typename T>
            T getProperty( const std::string &type ) throw()
            {
                T val;
                try {
                    val = boost::any_cast< T >( mProperties[ type ].mValue );
                }
                catch( boost::bad_any_cast exc ){
                    std::cout << "Casting " << mName << "." << type << " into " << typeid( T ).name() << " Failed " << std::endl;
                }
                return val;
            }
            
            boost::any              mValue;
            std::string             mName;
            std::string             mGroup;
            bool                    mEditable;
            bool                    mAnimatable;
            AnimRef                 mAnimRef;
            PropertyMap             mProperties;
        };
        
        typedef std::shared_ptr< Parameter > ParameterRef;
        
        static std::map< void*, ParameterRef > ParametersMap;
        
        Parameter& addParameter( const std::string &name, boost::any animPtr, const std::string &group = "", const PropertyMap& properties = PropertyMap() )
        {
            Inspectable::ParameterRef prop = Inspectable::ParameterRef( new Parameter( name, animPtr, group, properties ) );
            mParameters.push_back( prop );
            
            if( animPtr.type() == typeid( ci::Anim<float>* ) ) setupParameter<float>( prop, name, animPtr );
            else if( animPtr.type() == typeid( ci::Anim<int>* ) ) setupParameter<int>( prop, name, animPtr );
            else if( animPtr.type() == typeid( ci::Anim<bool>* ) ) setupParameter<bool>( prop, name, animPtr );
            else if( animPtr.type() == typeid( ci::Anim<ci::Vec2f>* ) ) setupParameter<ci::Vec2f>( prop, name, animPtr );
            else if( animPtr.type() == typeid( ci::Anim<ci::Vec3f>* ) ) setupParameter<ci::Vec3f>( prop, name, animPtr );
            else if( animPtr.type() == typeid( ci::Anim<ci::Color>* ) ) setupParameter<ci::Color>( prop, name, animPtr );
            else if( animPtr.type() == typeid( ci::Anim<ci::ColorA>* ) ) setupParameter<ci::ColorA>( prop, name, animPtr );
            else {
                std::cout << "Parameter::Unsupported Type " << std::endl;
            }
            
            return *prop.get();
        }
        
        ParameterRef getParameter( int i ) ;
        ParameterRef getParameter( std::string name );
        
        template<typename T>
        ParameterRef getParameterValue( int i ){ return boost::any_cast<T>( getParameter( i )->mValue ); }
        
        template<typename T>
        ParameterRef getParameterValue( std::string name ){ return boost::any_cast<T>( getParameter( name )->mValue ); }
        
        int         getNumParameters();
        
        void readParametersFromJson( ci::DataSourceRef source );
        void writeParametersToJson( ci::DataTargetRef target );
        
        
    protected:
        
        template< typename T>
        void setupParameter( ParameterRef prop, const std::string &name, boost::any animPtr )
        {
            try {
                ci::Anim<T>* ptr = boost::any_cast<ci::Anim<T>*>( animPtr );
                prop->mAnimRef = AnimRef( ptr );
                prop->mAnimRef.setTrackRef( Animation::ValueTrack::create( prop->mAnimRef, name ) );
                ParametersMap[ (void*) ptr ] = prop;
            }
            catch( boost::bad_any_cast exc ){
                std::cout << "Parameter::bad_any_cast" << exc.what() << std::endl;
            }
        }
        
        template< typename T>
        void writeParameter( ci::JsonTree& jprop, ParameterRef prop, const std::string& typeName )
        {
            ci::Anim<T>* ptr = boost::any_cast<ci::Anim<T>*>( prop->mValue );
            if( ptr ){
                jprop.pushBack( ci::JsonTree( "Value", *ptr ) );
                jprop.pushBack( ci::JsonTree( "Type", typeName ) );
            }
            
            if( prop->mAnimRef.getTrackRef() ){
                const Animation::Track& track = *prop->mAnimRef.getTrackRef().get();
                ci::JsonTree vec = ci::JsonTree::makeArray("Track");
                
                for( int i = 0; i < track.size(); i++ ){
                    ci::JsonTree key;
                    key.pushBack( ci::JsonTree( "time", track[i]->mTime ) );
                    key.pushBack( ci::JsonTree( "type", track[i]->mType ) );
                    if( track[i]->mType == Animation::KeyFrame_Value ){
                        Animation::KeyFrame* keyFrame = static_cast<Animation::KeyFrame*>( track[i].get() );
                        key.pushBack( ci::JsonTree( "value", *keyFrame->mValue.cast<T>() ) );
                    }
                    vec.pushBack( key );
                }
                jprop.pushBack( vec );
            }
        }
        
        template< typename T >
        void readParameter( ci::JsonTree& jprop, ParameterRef prop )
        {
            ci::Anim<T>* ptr = boost::any_cast<ci::Anim<T>*>( prop->mValue );
            *ptr = jprop.getChild("Value").getValue<T>();
            
            ci::JsonTree track    = jprop.getChild("Track");
            ci::JsonTree::Container keyframes = track.getChildren();
            
            for( ci::JsonTree::Container::iterator it = keyframes.begin(); it != keyframes.end(); ++it ){
                ci::JsonTree key  = *it;
                float time  = key.getChild("time").getValue<float>();
                float value = key.getChild("value").getValue<T>();
                
                prop->mAnimRef.getTrackRef()->push_back( Animation::KeyFrame::create( value, time ) );
            }
        }
        
        
        std::vector< ParameterRef > mParameters;
    };
    
        
    
    //-----------------------------------------------------------------------------
    //      Params
    //-----------------------------------------------------------------------------
    
    class Params;
    
    class Param : public Gwen::Controls::Base {
    public:
        
        GWEN_CONTROL( Param, Gwen::Controls::Base );
        
        void setLabel( std::string text );
        void setControl( Gwen::Controls::Base* control );
    private:
        
        
		void PostLayout( Gwen::Skin::Base* skin )
		{
            SizeToChildren( true, true );
		}
        
        void Render( Gwen::Skin::Base* skin );
        
        void valueChanged( Gwen::Controls::Base* control );
        void onNewKeyFrame( Gwen::Controls::Base* control );
        
        Gwen::Controls::Label*          mLabel;
        Gwen::Controls::Base*           mControl;
        Keyframer*                      mKeyframer;
        
        Gwen::Event::Caller             mOnLayout;
        
        AnimRef                         mAnimRef;
        friend class Params;
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
        
        void    setInspectable( Inspectable* inspectable );
        void    addInspectable( Inspectable* inspectable );
        
        void	addParam( const std::string &name, ci::Anim<bool> *boolParam, int layout = Gwen::Pos::Top, float colWidth = 120.0f, const std::string& group = "none", const std::string& tab = "none" );
        void	addParam( const std::string &name, ci::Anim<float> *floatParam, float min = -1000.0f, float max = 1000.0f, float step = 0.1f, int floatPrecision = 3, int layout = Gwen::Pos::Top, float colWidth = 120.0f, const std::string& group = "none", const std::string& tab = "none" );
        void	addParam( const std::string &name, ci::Anim<int> *intParam, int min = -1000, int max = 1000, int step = 1, int layout = Gwen::Pos::Top, float colWidth = 120.0f, const std::string& group = "none", const std::string& tab = "none" );
        void	addParam( const std::string &name, ci::Anim<ci::Vec2f> *vectorParam, ci::Vec2f min = ci::Vec2f( -1000.0f, -1000.0f ), ci::Vec2f max = ci::Vec2f( 1000.0f, 1000.0f ), float step = 0.1f, int floatPrecision = 3, int layout = Gwen::Pos::Top, float colWidth = 120.0f, bool vertical = true, const std::string& group = "none", const std::string& tab = "none" );
        void	addParam( const std::string &name, ci::Anim<ci::Vec3f> *vectorParam, ci::Vec3f min = ci::Vec3f( -1000.0f, -1000.0f, -1000.0f ), ci::Vec3f max = ci::Vec3f( 1000.0f, 1000.0f, 1000.0f ), float step = 0.1f, int floatPrecision = 3, int layout = Gwen::Pos::Top, float colWidth = 120.0f, bool vertical = true, const std::string& group = "none", const std::string& tab = "none" );
        
        
        void	addParam( const std::string &name, ci::Anim<ci::Color> *colorParam, int layout = Gwen::Pos::Top, float colWidth = 120.0f, const std::string& group = "none", const std::string& tab = "none" );
        void	addParam( const std::string &name, ci::Anim<ci::ColorA> *colorParam, int layout = Gwen::Pos::Top, float colWidth = 120.0f, const std::string& group = "none", const std::string& tab = "none"  );
        void	addParam( const std::string &name, std::string *strParam, const std::string &optionsStr = "", bool readOnly = false );
        void    addParam( const std::string &name, const std::vector<std::string> &enumNames, ci::Anim<int> *param, const std::string &optionsStr = "", bool readOnly = false );
        
        void	addParam( const std::string &name, ci::DataSourceRef pathParam, int layout = Gwen::Pos::Top, float colWidth = 120.0f );
        
        
        void	addSeparator( const std::string &name = "" );
        void	addText( const std::string &name = "" );
        void	addButton( const std::string &name, const std::function<void()> &callback );
        
        void    setPosition( ci::Vec2f pos ){ mWindow->SetPos( pos.x, pos.y ); }
        void    setVisible( bool visible ){ mControl->SetHidden( !visible ); }
        bool    getVisible(){ return !mControl->Hidden(); }
        
        void    removeAll(){ mControl->Inner()->Inner()->RemoveAllChildren(); mButtonCallbacks.clear();}
        
        Gwen::Controls::Base* getControl();
        
    private:
        
        template<typename T>
        void addInspectableParam( Inspectable::ParameterRef param ){
            try {
                ci::Anim<T>* anim = param->mAnimRef.cast<T>();
                if( param->hasProperty( "min" ) && param->hasProperty( "max" ) && param->hasProperty( "step" ) )
                    addParam( param->mName, anim, param->getProperty<T>( "min" ), param->getProperty<T>( "max" ), param->getProperty<float>( "step" ) );
                else if( param->hasProperty( "min" ) && param->hasProperty( "max" ) )
                    addParam( param->mName, anim, param->getProperty<T>( "min" ), param->getProperty<T>( "max" ) );
                else if( param->hasProperty( "min" ) )
                    addParam( param->mName, anim, param->getProperty<T>( "min" ) );
                else
                    addParam( param->mName, anim );
            }
            catch( boost::bad_any_cast exc ){
                std::cout << "addInspectableParam: " << exc.what() << std::endl;
            }
        }
        
        void onLayout( Gwen::Controls::Base* control );
        void buttonCallback( Gwen::Controls::Base* control );
        
        
        Gwen::Controls::Base* mControl;
        ParamsWindow* mWindow;
        std::vector<std::shared_ptr<std::function<void()> > >	mButtonCallbacks;
        
    };
    
    //-----------------------------------------------------------------------------
    //      Menu
    //-----------------------------------------------------------------------------
    
    class MenuEvent {
    public:
        MenuEvent( const std::string& name, const std::string& subname, bool checked = false );
        
        const std::string& getName();
        const std::string& getSubName();
        
        bool isChecked();
    private:
        
        std::string mName;
        std::string mSubName;
        bool        mChecked;
    };
    
    class Menu : public Gwen::Event::Handler {
    public:
        Menu();
        
        void addSeparator( const std::string &menu );
        void addItem( const std::string &menu, const std::string &item, std::function<void( MenuEvent )> callback, bool checkable = false, bool checked = true );
        
    protected:
        void select( Gwen::Controls::Base* pControl );
        
        Gwen::Controls::MenuStrip*                          mMenuStrip;
        std::map< std::string, Gwen::Controls::MenuItem* >  mMenus;
    };
    
    class StatusBar {
    public:
        StatusBar();
        void setStatus( const std::string& status );
    private:
        Gwen::Controls::StatusBar* mStatusBar;
    };
    
    class Explorer : public Gwen::Event::Handler {
    public:
        Explorer( const std::string &title, const ci::Vec2i &size );
        
        void    setPosition( ci::Vec2f pos ){ mWindow->SetPos( pos.x, pos.y ); }
        void    setVisible( bool visible ){ mWindow->SetHidden( !visible ); }
        bool    getVisible(){ return !mWindow->Hidden(); }
        
        void addItem( const std::string &name, std::function<void( const std::string& )> callback );
        void removeItem( const std::string & name );
        
    protected:
        void select( Gwen::Controls::Base* pControl );
        
        ParamsWindow* mWindow;
        Gwen::Controls::ListBox* mListBox;
        std::map< std::string, std::shared_ptr<std::function< void( const std::string& ) > > > mCallbacks;
        
    };
    
    
    //-----------------------------------------------------------------------------
    //      Timeline
    //-----------------------------------------------------------------------------
    
#ifdef GWEN_TIMELINE
    class Timeline : public Gwen::Event::Handler {
    public:
        
		enum {
			TimeFormat_Frames = 0,
			TimeFormat_Seconds
		};
        
        Timeline();
        Timeline( const std::string &title, const ci::Vec2i &size, float totalTime, ci::TimelineRef timeline );
        
        void setInspectable( Inspectable* inspectable );
        void addInspectable( Inspectable* inspectable );
        
        void addTrack( Animation::TrackRef track );
        void addKeyframe( std::string trackName, AnimRef anim, Animation::KeyFrameRef keyframe );
        
        void setTimeFormat( int format ){ mTimelineWidget->setTimeFormat( format ); }
        
        static void         setCurrentTimeline( Timeline* timeline ){ currentTimeline = timeline; }
        static Timeline*    getCurrentTimeline() { return currentTimeline; }
        
        void update();
        
        void    setPosition( ci::Vec2f pos ){ mControl->SetPos( pos.x, pos.y ); }
        void    setVisible( bool visible ){ mControl->SetHidden( !visible ); }
        bool    getVisible(){ return mControl->Hidden(); }
    private:
        
        void onUpdate( Gwen::Controls::Base* control );
        
        Widgets::TimelineWidget*            mTimelineWidget;
        Gwen::Controls::Base*               mControl;
        std::vector< Animation::TrackRef >  mTrackList;
        ci::TimelineRef                     mTimeline;
        
        static Timeline*                    currentTimeline;
    };
#endif
    
    
    //-----------------------------------------------------------------------------
    //      FlowGraph
    //-----------------------------------------------------------------------------
    
#ifdef GWEN_FLOW
    class Flow : public Gwen::Event::Handler {
    public:
        Flow();
        Flow( const std::string &title, const ci::Vec2i &size );
        
        void Render();// Gwen::Skin::Base* skin );
        
        void mouseDown( ci::app::MouseEvent event );
        void mouseDrag( ci::app::MouseEvent event );
        void mouseUp( ci::app::MouseEvent event );
        
        void update(){ mGraph.update(); }
        
        flow::Graph& getGraph(){ return mGraph; }
        void    setPosition( ci::Vec2f pos ){ mControl->SetPos( pos.x, pos.y ); }
        void    setVisible( bool visible ){ mControl->SetHidden( !visible ); }
        bool    getVisible(){ return !mControl->Hidden(); }
        
    private:
        
        void createNode( Gwen::Controls::Base* base );
        
        flow::Graph                     mGraph;
        RenderCanvas*                   mRenderCanvas;
        Gwen::Controls::WindowControl*  mControl;
        Gwen::Controls::MenuStrip*      mMenu;
    };
#endif
    
    
    
    //--------------------------------------------------------------------------------------------------------------------------------
    
    
    
    //-----------------------------------------------------------------------------
    //      Inspectable Template Specialization
    //-----------------------------------------------------------------------------
    
    template<>
    inline void Inspectable::writeParameter<ci::Vec2f>( ci::JsonTree& jprop, ParameterRef prop, const std::string& typeName )
    {
        ci::Anim<ci::Vec2f>* ptr = boost::any_cast<ci::Anim<ci::Vec2f>*>( prop->mValue );
        if( ptr ){
            ci::JsonTree vec = ci::JsonTree::makeArray("Value");
            vec.pushBack( ci::JsonTree( "x", ((ci::Vec2f)*ptr).x ) );
            vec.pushBack( ci::JsonTree( "y", ((ci::Vec2f)*ptr).y ) );
            jprop.pushBack( vec );
            jprop.pushBack( ci::JsonTree( "Type", typeName ) );
        }
        if( prop->mAnimRef.getTrackRef() ){
            const Animation::Track& track = *prop->mAnimRef.getTrackRef().get();
            ci::JsonTree vec = ci::JsonTree::makeArray("Track");
            
            for( int i = 0; i < track.size(); i++ ){
                ci::JsonTree key;
                key.pushBack( ci::JsonTree( "time", track[i]->mTime ) );
                key.pushBack( ci::JsonTree( "type", track[i]->mType ) );
                if( track[i]->mType == Animation::KeyFrame_Value ){
                    Animation::KeyFrame* keyFrame = static_cast<Animation::KeyFrame*>( track[i].get() );
                    
                    ci::JsonTree value = ci::JsonTree::makeArray("Value");
                    value.pushBack( ci::JsonTree( "x", ((ci::Vec2f)*keyFrame->mValue.cast<ci::Vec2f>()).x ) );
                    value.pushBack( ci::JsonTree( "y", ((ci::Vec2f)*keyFrame->mValue.cast<ci::Vec2f>()).y ) );
                    key.pushBack( value );
                }
                vec.pushBack( key );
            }
            jprop.pushBack( vec );
        }
    }
    
    
    template<>
    inline void Inspectable::writeParameter<ci::Vec3f>( ci::JsonTree& jprop, ParameterRef prop, const std::string& typeName )
    {
        ci::Anim<ci::Vec3f>* ptr = boost::any_cast<ci::Anim<ci::Vec3f>*>( prop->mValue );
        if( ptr ){
            ci::JsonTree vec = ci::JsonTree::makeArray("Value");
            ci::Vec3f v = *ptr;
            vec.pushBack( ci::JsonTree( "x", v.x ) );
            vec.pushBack( ci::JsonTree( "y", v.y ) );
            vec.pushBack( ci::JsonTree( "z", v.z ) );
            jprop.pushBack( vec );
            jprop.pushBack( ci::JsonTree( "Type", typeName ) );
        }
        if( prop->mAnimRef.getTrackRef() ){
            const Animation::Track& track = *prop->mAnimRef.getTrackRef().get();
            ci::JsonTree vec = ci::JsonTree::makeArray("Track");
            
            for( int i = 0; i < track.size(); i++ ){
                ci::JsonTree key;
                key.pushBack( ci::JsonTree( "time", track[i]->mTime ) );
                key.pushBack( ci::JsonTree( "type", track[i]->mType ) );
                if( track[i]->mType == Animation::KeyFrame_Value ){
                    Animation::KeyFrame* keyFrame = static_cast<Animation::KeyFrame*>( track[i].get() );
                    
                    ci::JsonTree value = ci::JsonTree::makeArray("Value");
                    value.pushBack( ci::JsonTree( "x", (*keyFrame->mValue.cast<ci::Vec3f>()).value().x ) );
                    value.pushBack( ci::JsonTree( "y", (*keyFrame->mValue.cast<ci::Vec3f>()).value().y ) );
                    value.pushBack( ci::JsonTree( "z", (*keyFrame->mValue.cast<ci::Vec3f>()).value().z ) );
                    key.pushBack( value );
                }
                vec.pushBack( key );
            }
            jprop.pushBack( vec );
        }
    }
    
    template<>
    inline void Inspectable::writeParameter<ci::Color>( ci::JsonTree& jprop, ParameterRef prop, const std::string& typeName )
    {
        ci::Anim<ci::Color>* ptr = boost::any_cast<ci::Anim<ci::Color>*>( prop->mValue );
        if( ptr ){
            ci::JsonTree vec = ci::JsonTree::makeArray("Value");
            vec.pushBack( ci::JsonTree( "r", ((ci::Color)*ptr).r ) );
            vec.pushBack( ci::JsonTree( "g", ((ci::Color)*ptr).g ) );
            vec.pushBack( ci::JsonTree( "b", ((ci::Color)*ptr).b ) );
            jprop.pushBack( vec );
            jprop.pushBack( ci::JsonTree( "Type", typeName ) );
        }
        if( prop->mAnimRef.getTrackRef() ){
            const Animation::Track& track = *prop->mAnimRef.getTrackRef().get();
            ci::JsonTree vec = ci::JsonTree::makeArray("Track");
            
            for( int i = 0; i < track.size(); i++ ){
                ci::JsonTree key;
                key.pushBack( ci::JsonTree( "time", track[i]->mTime ) );
                key.pushBack( ci::JsonTree( "type", track[i]->mType ) );
                if( track[i]->mType == Animation::KeyFrame_Value ){
                    Animation::KeyFrame* keyFrame = static_cast<Animation::KeyFrame*>( track[i].get() );
                    
                    ci::JsonTree value = ci::JsonTree::makeArray("Value");
                    value.pushBack( ci::JsonTree( "r", ((ci::ColorA)*keyFrame->mValue.cast<ci::Color>()).r ) );
                    value.pushBack( ci::JsonTree( "g", ((ci::ColorA)*keyFrame->mValue.cast<ci::Color>()).g ) );
                    value.pushBack( ci::JsonTree( "b", ((ci::ColorA)*keyFrame->mValue.cast<ci::Color>()).b ) );
                    key.pushBack( value );
                }
                vec.pushBack( key );
            }
            jprop.pushBack( vec );
        }
    }
    
    template<>
    inline void Inspectable::writeParameter<ci::ColorA>( ci::JsonTree& jprop, ParameterRef prop, const std::string& typeName )
    {
        
        ci::Anim<ci::ColorA>* ptr = boost::any_cast<ci::Anim<ci::ColorA>*>( prop->mValue );
        if( ptr ){
            ci::JsonTree vec = ci::JsonTree::makeArray("Value");
            vec.pushBack( ci::JsonTree( "r", ((ci::ColorA)*ptr).r ) );
            vec.pushBack( ci::JsonTree( "g", ((ci::ColorA)*ptr).g ) );
            vec.pushBack( ci::JsonTree( "b", ((ci::ColorA)*ptr).b ) );
            vec.pushBack( ci::JsonTree( "a", ((ci::ColorA)*ptr).a ) );
            jprop.pushBack( vec );
            jprop.pushBack( ci::JsonTree( "Type", typeName ) );
        }
        if( prop->mAnimRef.getTrackRef() ){
            const Animation::Track& track = *prop->mAnimRef.getTrackRef().get();
            ci::JsonTree vec = ci::JsonTree::makeArray("Track");
            
            for( int i = 0; i < track.size(); i++ ){
                ci::JsonTree key;
                key.pushBack( ci::JsonTree( "time", track[i]->mTime ) );
                key.pushBack( ci::JsonTree( "type", track[i]->mType ) );
                if( track[i]->mType == Animation::KeyFrame_Value ){
                    Animation::KeyFrame* keyFrame = static_cast<Animation::KeyFrame*>( track[i].get() );
                    
                    ci::JsonTree value = ci::JsonTree::makeArray("Value");
                    value.pushBack( ci::JsonTree( "r", ((ci::ColorA)*keyFrame->mValue.cast<ci::ColorA>()).r ) );
                    value.pushBack( ci::JsonTree( "g", ((ci::ColorA)*keyFrame->mValue.cast<ci::ColorA>()).g ) );
                    value.pushBack( ci::JsonTree( "b", ((ci::ColorA)*keyFrame->mValue.cast<ci::ColorA>()).b ) );
                    value.pushBack( ci::JsonTree( "a", ((ci::ColorA)*keyFrame->mValue.cast<ci::ColorA>()).a ) );
                    key.pushBack( value );
                }
                vec.pushBack( key );
            }
            jprop.pushBack( vec );
        }
    }
    
    
    template<>
    inline void Inspectable::readParameter<ci::Vec2f>( ci::JsonTree& jprop, ParameterRef prop )
    {
        ci::Anim<ci::Vec2f>* ptr = boost::any_cast<ci::Anim<ci::Vec2f>*>( prop->mValue );
        ci::Vec2f v;
        v.x = jprop.getChild("Value").getChild("x").getValue<float>();
        v.y = jprop.getChild("Value").getChild("y").getValue<float>();
        *ptr = v;
        
        ci::JsonTree track    = jprop.getChild("Track");
        ci::JsonTree::Container keyframes = track.getChildren();
        
        for( ci::JsonTree::Container::iterator it = keyframes.begin(); it != keyframes.end(); ++it ){
            ci::JsonTree key  = *it;
            float time  = key.getChild("time").getValue<float>();
            float x = key.getChild("value").getChild("x").getValue<float>();
            float y = key.getChild("value").getChild("y").getValue<float>();
            
            prop->mAnimRef.getTrackRef()->push_back( Animation::KeyFrame::create( ci::Vec2f( x, y ), time ) );
        }
    }
    template<>
    inline void Inspectable::readParameter<ci::Vec3f>( ci::JsonTree& jprop, ParameterRef prop )
    {
        ci::Anim<ci::Vec3f>* ptr = boost::any_cast<ci::Anim<ci::Vec3f>*>( prop->mValue );
        ci::Vec3f v;
        v.x = jprop.getChild("Value").getChild("x").getValue<float>();
        v.y = jprop.getChild("Value").getChild("y").getValue<float>();
        v.z = jprop.getChild("Value").getChild("z").getValue<float>();
        *ptr = v;
        
        ci::JsonTree track    = jprop.getChild("Track");
        ci::JsonTree::Container keyframes = track.getChildren();
        
        for( ci::JsonTree::Container::iterator it = keyframes.begin(); it != keyframes.end(); ++it ){
            ci::JsonTree key  = *it;
            float time  = key.getChild("time").getValue<float>();
            float x = key.getChild("value").getChild("x").getValue<float>();
            float y = key.getChild("value").getChild("y").getValue<float>();
            float z = key.getChild("value").getChild("z").getValue<float>();
            
            prop->mAnimRef.getTrackRef()->push_back( Animation::KeyFrame::create( ci::Vec3f( x, y, z ), time ) );
        }
    }
    template<>
    inline void Inspectable::readParameter<ci::Color>( ci::JsonTree& jprop, ParameterRef prop )
    {
        ci::Anim<ci::Color>* ptr = boost::any_cast<ci::Anim<ci::Color>*>( prop->mValue );
        ci::Color c;
        c.r = jprop.getChild("Value").getChild("r").getValue<float>();
        c.g = jprop.getChild("Value").getChild("g").getValue<float>();
        c.b = jprop.getChild("Value").getChild("b").getValue<float>();
        *ptr = c;
        
        ci::JsonTree track    = jprop.getChild("Track");
        ci::JsonTree::Container keyframes = track.getChildren();
        
        for( ci::JsonTree::Container::iterator it = keyframes.begin(); it != keyframes.end(); ++it ){
            ci::JsonTree key  = *it;
            float time  = key.getChild("time").getValue<float>();
            float r = key.getChild("value").getChild("r").getValue<float>();
            float g = key.getChild("value").getChild("g").getValue<float>();
            float b = key.getChild("value").getChild("b").getValue<float>();
            
            prop->mAnimRef.getTrackRef()->push_back( Animation::KeyFrame::create( ci::Color( r, g, b ), time ) );
        }
    }
    template<>
    inline void Inspectable::readParameter<ci::ColorA>( ci::JsonTree& jprop, ParameterRef prop )
    {
        ci::Anim<ci::ColorA>* ptr = boost::any_cast<ci::Anim<ci::ColorA>*>( prop->mValue );
        ci::ColorA c;
        c.r = jprop.getChild("Value").getChild("r").getValue<float>();
        c.g = jprop.getChild("Value").getChild("g").getValue<float>();
        c.b = jprop.getChild("Value").getChild("b").getValue<float>();
        c.a = jprop.getChild("Value").getChild("a").getValue<float>();
        *ptr = c;
        
        ci::JsonTree track    = jprop.getChild("Track");
        ci::JsonTree::Container keyframes = track.getChildren();
        
        for( ci::JsonTree::Container::iterator it = keyframes.begin(); it != keyframes.end(); ++it ){
            ci::JsonTree key  = *it;
            float time  = key.getChild("time").getValue<float>();
            float r = key.getChild("value").getChild("r").getValue<float>();
            float g = key.getChild("value").getChild("g").getValue<float>();
            float b = key.getChild("value").getChild("b").getValue<float>();
            float a = key.getChild("value").getChild("a").getValue<float>();
            
            prop->mAnimRef.getTrackRef()->push_back( Animation::KeyFrame::create( ci::ColorA( r, g, b, a ), time ) );
        }
    }
    
    //---------
    
    template<>
    inline void Params::addInspectableParam< bool >( Inspectable::ParameterRef param ){
        ci::Anim<bool>* anim = param->mAnimRef.cast<bool>();
        addParam( param->mName, anim );
    }
    
    
    template<>
    inline void Params::addInspectableParam< ci::ColorA >( Inspectable::ParameterRef param ){
        ci::Anim<ci::ColorA>* anim = param->mAnimRef.cast<ci::ColorA>();
        addParam( param->mName, anim );
    }
    
    template<>
    inline void Params::addInspectableParam< ci::Color >( Inspectable::ParameterRef param ){
        ci::Anim<ci::Color>* anim = param->mAnimRef.cast<ci::Color>();
        addParam( param->mName, anim );
    }

    

};