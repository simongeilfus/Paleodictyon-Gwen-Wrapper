#include "CinderGwenParams.h"

#include "Gwen/Gwen.h"
#include "Gwen/Utility.h"
#include "Gwen/Skin.h"

#include "cinder/Cinder.h"

namespace CinderGwen {
    
    
    
    GWEN_CONTROL_CONSTRUCTOR( Param ){
        
        mLabel = new Gwen::Controls::Label( this );
        mLabel->SetPos( 15, 0 );
        mLabel->SetSize( 80, 15 );
        
        mKeyframer = new Keyframer( this );
        mKeyframer->SetPos( 4, 5 );
        mKeyframer->onNewKeyFrame.Add( this, &Param::onNewKeyFrame );
        
        SetMargin( Gwen::Margin( 0, 1.5f, 0, 1.5f ) );
    }
    
    void Param::onNewKeyFrame( Gwen::Controls::Base* control ){
        std::string typeName = mControl->GetTypeName();
        
        if( Timeline::getCurrentTimeline() ){
            if( typeName == "CheckBox" ){
                Gwen::Controls::CheckBox* base = gwen_cast< Gwen::Controls::CheckBox >( mControl );
            }
            else if( typeName == "NumericStepper" ){
                NumericStepper* base = gwen_cast< NumericStepper >( mControl );
                Animation::KeyFrameRef keyframe = Animation::KeyFrame::create( base->getValue(), 0.0f );
                Timeline::getCurrentTimeline()->addKeyframe( mLabel->GetText().c_str(), mAnimRef, keyframe );
            }
            else if( typeName == "VectorStepper" ){
                VectorStepper* base = gwen_cast< VectorStepper >( mControl );
                Animation::KeyFrameRef keyframe = Animation::KeyFrame::create( base->getValue(), 0.0f );
                Timeline::getCurrentTimeline()->addKeyframe( mLabel->GetText().c_str(), mAnimRef, keyframe );
            }
        }
                                                    
    }
    
    void Param::setLabel( std::string text ){
        mLabel->SetText( text );
        mLabel->SizeToContents();
    }
    void Param::setControl( Gwen::Controls::Base* control ){
        mControl = control;
        //mControl->SetPos( 80, mControl->GetPos().y );

        //SizeToChildren();
        //GetParent()->SizeToChildren();
        //GetParent()->GetParent()->SizeToChildren();
        //Invalidate();
        //GetParent()->Invalidate();
        //GetParent()->GetParent()->Invalidate();
        //ci::app::console() << "their size  : " << ChildrenSize().x << " " << ChildrenSize().y << std::endl;
        //ci::app::console() << "my size     : " << GetSize().x << " " << GetSize().y << std::endl;
        //ci::app::console() << "my dad size : " << GetParent()->GetSize().x << " " << GetParent()->GetSize().y << std::endl;
        //ci::app::console() << "his dad size : " << GetParent()->GetParent()->GetSize().x << " " << GetParent()->GetParent()->GetSize().y << std::endl << std::endl << std::endl;
        //Gwen::Point size = ChildrenSize() + Gwen::Point( 10.0f, 10.0f );
        //SetSize( size.x, size.y );
    }
    
    void Param::Render( Gwen::Skin::Base* skin ){
        BaseClass::Render( skin );
        
        std::string typeName = mControl->GetTypeName();
        
        if( typeName == "CheckBox" ){
            Gwen::Controls::CheckBox* base = gwen_cast< Gwen::Controls::CheckBox >( mControl );
            if( *mAnimRef.cast<bool>() != base->IsChecked() ){
                base->SetChecked( *mAnimRef.cast<bool>() );
            }
        }
        else if( typeName == "NumericStepper" ){
            NumericStepper* base = gwen_cast< NumericStepper >( mControl );
            if( *mAnimRef.cast<float>() != base->getValue() ){
                base->setValue( *mAnimRef.cast<float>() );
            }
        }
        else if( typeName == "VectorStepper" ){
            VectorStepper* base = gwen_cast< VectorStepper >( mControl );
            ci::Vec3f v = *mAnimRef.cast<ci::Vec3f>();
            if( v != base->getValue() ){
                base->setValue( v );
            }
        }
    }
    
    void Param::valueChanged( Gwen::Controls::Base* control ){
        std::string typeName = control->GetTypeName();
        //ci::app::console() << typeName <<std::endl;
        
        if( typeName == "CheckBox" ){
            Gwen::Controls::CheckBox* base = gwen_cast< Gwen::Controls::CheckBox >( control );
            mAnimRef = base->IsChecked();
        }
        else if( typeName == "NumericStepper" ){
            NumericStepper* base = gwen_cast< NumericStepper >( control );
            mAnimRef = base->getValue();
        }
        else if( typeName == "VectorStepper" ){
            VectorStepper* base = gwen_cast< VectorStepper >( control );
            mAnimRef = base->getValue();
        }
        
    }
    //------------------------------------------------------------------------------------------------------------------------
    
    GWEN_CONTROL_CONSTRUCTOR( ParamsWindow ){
        m_InnerPanel = new Gwen::Controls::ScrollControl( this );
        ( (Gwen::Controls::ScrollControl*) m_InnerPanel )->SetScroll( true, true );
        ( (Gwen::Controls::ScrollControl*) m_InnerPanel )->SetAutoHideBars( true );
        m_InnerPanel->Dock( Gwen::Pos::Fill );
        //new Gwen::Controls::Base( m_InnerPanel ); 
        m_InnerPanel->Show();
//        SetPadding( Gwen::Padding( 0, 0, 10, 10 ) );
        //m_InnerPanel->SetPadding( Gwen::Padding( 0, 0, 10, 10 ) );
        //m_InnerPanel->SetMargin( Gwen::Margin( 0, 0, 10, 10 ) );

        mLayoutDone = false;
    }
    
    //------------------------------------------------------------------------------------------------------------------------
    
	Params::Params(){
        mControl = NULL;
    }
    
    Params::Params( const std::string &title, const ci::Vec2i &size ){
        mWindow = new ParamsWindow( CanvasSingleton::getCanvas() );
        mWindow->SetTitle( title );
        mWindow->SetSize( size.x, size.y );
        mWindow->SetClosable( false );
        
        //mScroll = new Gwen::Controls::ScrollControl( mWindow );
        //mScroll->Dock( Gwen::Pos::Fill );
        //mScroll->SetAutoHideBars( true );
        //mScroll->SetScroll( false, true );
        //mScroll->SetInnerSize( size.x, size.y );
        //mScroll->SetSize( size.x, size.y );
        
        //new Gwen::Controls::Base( mScroll ); 
        
        mControl            = mWindow;
    }
    Params::~Params(){
        if( mControl != NULL ) {
            mControl->RemoveAllChildren();
            delete mControl;
        }
    }
    
    
    void Params::addParam( const std::string &name, ci::Anim<bool> *boolParam, int layout, float colWidth, const std::string& group, const std::string& tab ){
        Param* param = new Param( mControl );
        param->Dock( layout );
        param->setLabel( name );
        param->mAnimRef = AnimRef( boolParam );
        
        Gwen::Controls::CheckBox* control = new Gwen::Controls::CheckBox( param );
        control->onCheckChanged.Add( param, &Param::valueChanged );
        control->SetSize( 15, 15 );
        control->SetPos( colWidth, 2.5f );
        param->setControl( control );
        
        mWindow->mLayoutDone = false;
    }
    
    void Params::addParam( const std::string &name, ci::Anim<float> *floatParam, float min, float max, float step, int floatPrecision, int layout, float colWidth, const std::string& group, const std::string& tab ){
        Param* param = new Param( mControl );
        param->Dock( layout );
        param->setLabel( name );
        param->mAnimRef = AnimRef( floatParam );
        param->mOnLayout.Add( this, &Params::onLayout );
        
        NumericStepper* control = new NumericStepper( param );
        control->onChanged.Add( param, &Param::valueChanged );
        control->SetPos( colWidth, 0.0f );
        control->setMin( min );
        control->setMax( max );
        control->setStep( step );
        control->setFloatPrecision( floatPrecision );
        param->setControl( control );
        
        mWindow->mLayoutDone = false;
    }
    void Params::addParam( const std::string &name, ci::Anim<int32_t> *intParam, int min, int max, int step, int layout, float colWidth, const std::string& group, const std::string& tab ){
        Param* param = new Param( mControl );
        param->Dock( layout );
        param->setLabel( name );
        param->mAnimRef = AnimRef( intParam );
        param->mOnLayout.Add( this, &Params::onLayout );
        
        NumericStepper* control = new NumericStepper( param );
        control->setUseIntegers( true );
        control->SetPos( colWidth, 0.0f );
        control->onChanged.Add( param, &Param::valueChanged );
        control->setMin( min );
        control->setMax( max );
        control->setStep( step );
        param->setControl( control );
        
        mWindow->mLayoutDone = false;
    }
    void Params::addParam( const std::string &name, ci::Anim<ci::Vec3f> *vectorParam, ci::Vec3f min, ci::Vec3f max, float step, int floatPrecision, int layout, float colWidth, bool vertical, const std::string& group, const std::string& tab ){
        Param* param = new Param( mControl );
        param->Dock( layout );
        param->setLabel( name );
        param->mAnimRef = AnimRef( vectorParam );
        param->mOnLayout.Add( this, &Params::onLayout );
        
        VectorStepper* control = new VectorStepper( param );
        control->onChanged.Add( param, &Param::valueChanged );
        control->SetPos( colWidth, 0.0f );
        control->setMin( min );
        control->setMax( max );
        control->setStep( step );
        control->setFloatPrecision( floatPrecision );
        if( vertical ) control->setVertical();
        param->setControl( control );
        
        mWindow->mLayoutDone = false;
    }
    void Params::addParam( const std::string &name, ci::Anim<ci::Color> *quatParam, const std::string &optionsStr, bool readOnly ){
        
    }
    void Params::addParam( const std::string &name, ci::Anim<ci::ColorA> *quatParam, const std::string &optionsStr, bool readOnly ){
        
    }
    void Params::addParam( const std::string &name, std::string *strParam, const std::string &optionsStr, bool readOnly ){
        
    }
    void Params::addParam( const std::string &name, const std::vector<std::string> &enumNames, ci::Anim<int> *param, const std::string &optionsStr, bool readOnly ){
        
    }
    void Params::addSeparator( const std::string &name, const std::string &optionsStr ){
        //Gwen::Controls::Base* base = new Gwen::Controls::Base( mControl );
        //base->Dock( Gwen::Pos::Top );
        //base->SetSize( 10, 30 );
        
        Gwen::Controls::Label* label = new Gwen::Controls::Label( mControl );
        label->Dock( Gwen::Pos::Top );
        label->SetSize( 80, 15 );
        label->SetText( name );
        label->MakeColorDark();
        label->SetMargin( Gwen::Margin( 2, 2, 0, 10 ) );
        
    }
    void Params::addText( const std::string &name, const std::string &optionsStr ){
        
    }
    void Params::addButton( const std::string &name, const std::function<void()> &callback, const std::string &optionsStr ){
        
    }
    
    Gwen::Controls::Base* Params::getControl(){
        return mControl;
    }
    
    
    void Params::onLayout( Gwen::Controls::Base* control ){
        //mWindow->GetChild( 0 )->SizeToChildren(false, true);
       // mWindow->SizeToChildren(false, true);
       // mWindow->Invalidate();
        //mScroll->SizeToChildren();
        //ci::app::console() << "Params sizetochilds" << std::endl;
    }
    
    //------------------------------------------------------------------------------------------------------------------------
    
	Timeline::Timeline(){
        mControl    = NULL;
        mTimelineWidget   = NULL;
    }
    
    Timeline::Timeline( const std::string &title, const ci::Vec2i &size, float totalTime, ci::TimelineRef timeline ){
        Gwen::Controls::WindowControl* control = new Gwen::Controls::WindowControl( CanvasSingleton::getCanvas() );
        control->SetTitle( title );
        control->SetSize( size.x, size.y );
        control->SetClosable( false );
        
        mControl    = control;
        
        mTimelineWidget   = new Widgets::TimelineWidget( mControl );
        mTimelineWidget->Dock( Gwen::Pos::Fill );
        mTimelineWidget->setTime( totalTime, 0.0f, totalTime );
        mTimelineWidget->onUpdate.Add( this, &Timeline::onUpdate );
        mTimeline = timeline;
        
        setCurrentTimeline( this );
    }
    
    void Timeline::addTrack( Animation::TrackRef track ){
        
    }
    void Timeline::addKeyframe( std::string trackName, AnimRef anim, Animation::KeyFrameRef keyframe ){
        if( mTimeline ){
            std::shared_ptr<Animation::KeyFrame> key = std::static_pointer_cast<Animation::KeyFrame>( keyframe );
            float fl = *key->mValue.cast<float>();
            ci::app::console() << fl << " at " << mTimelineWidget->getCurrentTime() << std::endl;
            keyframe->mTime = mTimelineWidget->getCurrentTime();
        
            bool found = false;
            int i = 0;
            for( ; i < mTrackList.size(); i++ ){
                if( mTrackList[i]->getName() == trackName ){
                    found = true;
                    break;
                }
            }
            
            if( found ) {
                mTrackList[i]->push_back( keyframe );
                mTrackList[i]->addToTimeline( mTimeline );
                mTimelineWidget->updateTrackList();
            }
            else {
                Animation::TrackRef track = Animation::ValueTrack::create( anim, trackName );
                track->push_back( keyframe );
                track->addToTimeline( mTimeline );
                mTrackList.push_back( track );
                mTimelineWidget->setTrackList( &mTrackList );
            }
        }
    }
    
    void Timeline::update(){
        mTimelineWidget->update();
        mTimeline->stepTo( mTimelineWidget->getCurrentTime() );
    }
    
    void Timeline::onUpdate( Gwen::Controls::Base* control ){
        update();
    }
    
    Timeline* Timeline::currentTimeline = NULL;
};