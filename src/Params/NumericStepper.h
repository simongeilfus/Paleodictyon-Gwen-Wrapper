#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"

#include "Gwen/Gwen.h"
#include "Gwen/Skins/Simple.h"
#include "Gwen/Skins/TexturedBase.h"

#include "Gwen/BaseRender.h"
#include "Gwen/Controls/Base.h"
#include "Gwen/Controls/TextBox.h"

#include "cinder/Utilities.h"
#include "cinder/timeline.h"

#include <sstream>
#include <iomanip>

namespace CinderGwen {
    
    
    class NumberStepperBox : public Gwen::Controls::TextBox
    {
    public:
        
        GWEN_CONTROL_INLINE( NumberStepperBox, Gwen::Controls::TextBox ){
            SetText( L"0" );
            mPressed = false;
        }
        void OnMouseMoved( int x, int y, int deltaX, int deltaY ){
            if( mPressed ) {
                mDeltaY = deltaY;
                onDrag.Call( this );
            }
            else mDeltaY = 0.0f;
        }
        void OnLostKeyboardFocus(){
            onReturnPressed.Call( this );
        }
        void OnMouseClickLeft(int x, int y, bool bDown ){
            mPressed = bDown;
            BaseClass::OnMouseClickLeft( x, y, bDown );
        }
        
        Gwen::Event::Caller	onDrag;
        
        float   mDeltaY;
        
    private:
        
        bool 	mPressed;
        
        virtual bool IsTextAllowed( const Gwen::UnicodeString& str, int iPos ){
            const Gwen::UnicodeString& strString = GetText().GetUnicode();
            
            if ( str.length() == 0 )
                return true;
            
            for (size_t i=0; i<str.length(); i++)
            {
                if ( str[i] == L'-' )
                {
                    // Has to be at the start
                    if ( i != 0 || iPos != 0 )
                        return false;
                    
                    // Can only be one
                    if ( std::count( strString.begin(), strString.end(), L'-' ) > 0 )
                        return false;
                    
                    continue;
                }
                
                if ( str[i] == L'0' ) continue;
                if ( str[i] == L'1' ) continue;
                if ( str[i] == L'2' ) continue;
                if ( str[i] == L'3' ) continue;
                if ( str[i] == L'4' ) continue;
                if ( str[i] == L'5' ) continue;
                if ( str[i] == L'6' ) continue;
                if ( str[i] == L'7' ) continue;
                if ( str[i] == L'8' ) continue;
                if ( str[i] == L'9' ) continue;
                
                if ( str[i] == L'.' )
                {
                    // Already a fullstop
                    if ( std::count( strString.begin(), strString.end(), L'.' ) > 0 )
                        return false;
                    
                    continue;
                }
                
                return false;
            }
            
            return true;
        }
        
    };
    
    
    class NumericStepperUp : public Gwen::Controls::Button
    {
        GWEN_CONTROL_INLINE( NumericStepperUp, Gwen::Controls::Button )
        {
            SetSize( 10, 10 );
            mTimeOfPress = 10000.0f;
        }
        
        virtual void OnMouseClickLeft( int x, int y, bool bDown ){
            //virtual void OnPress(){
            mTimeOfPress = ci::app::getElapsedSeconds();
            BaseClass::OnMouseClickLeft( x, y, bDown );
        }
        virtual void Render( Gwen::Skin::Base* skin )
        {
            float timeDiff = ci::app::getElapsedSeconds() - mTimeOfPress;
            if( IsDepressed() && timeDiff > 0.5f ){
                onPress.Call( this );
            }
            //skin->DrawNumericUpDownButton( this, IsDepressed(), true );
            skin->DrawScrollButton( this, Gwen::Pos::Top, IsDepressed(), IsHovered(), IsDisabled() );
        }
        
        float mTimeOfPress;
    };
    
    class NumericStepperDown : public Gwen::Controls::Button
    {
        GWEN_CONTROL_INLINE( NumericStepperDown, Gwen::Controls::Button )
        {
            SetSize( 10, 10 );
            mTimeOfPress = 10000.0f;
        }
        
        virtual void OnMouseClickLeft( int x, int y, bool bDown ){
            //virtual void OnPress(){
            mTimeOfPress = ci::app::getElapsedSeconds();
            BaseClass::OnMouseClickLeft( x, y, bDown );
        }
        virtual void Render( Gwen::Skin::Base* skin )
        {
            float timeDiff = ci::app::getElapsedSeconds() - mTimeOfPress;
            if( IsDepressed() && timeDiff > 0.5f ){
                onPress.Call( this );
            }
            skin->DrawScrollButton( this, Gwen::Pos::Bottom, IsDepressed(), IsHovered(), IsDisabled() );
            //skin->DrawNumericUpDownButton( this, IsDepressed(), false );
        }
        float mTimeOfPress;
    };
    
    
    class NumericStepper : public Gwen::Controls::Base {
    public:
        
        GWEN_CONTROL_INLINE( NumericStepper, Gwen::Controls::Base )
        {
            
            
            mTextBox = new NumberStepperBox( this );
            mTextBox->SetSize( 50, 20 );
            mTextBox->SetText( "0.0" );
            mTextBox->onDrag.Add( this, &NumericStepper::OnBoxDrag );
            //mTextBox->onTextChanged.Add( this, &NumericStepper::OnBoxUpdate );
            mTextBox->onReturnPressed.Add( this, &NumericStepper::OnBoxUpdate );

            NumericStepperUp* pButtonUp = new NumericStepperUp( this );
            pButtonUp->onPress.Add( this, &NumericStepper::OnButtonUp );
            //pButtonUp->onDrag.Add( this,  &NumericStepper::OnButtonUpDrag );
            pButtonUp->SetTabable( false );
            pButtonUp->SetPos( 49, 0 );
            
            NumericStepperDown* pButtonDown = new NumericStepperDown( this );
            pButtonDown->onPress.Add( this, &NumericStepper::OnButtonDown );
            //pButtonDown->onDrag.Add( this,  &NumericStepper::OnButtonDownDrag );
            pButtonDown->SetTabable( false );
            pButtonDown->SetPos( 49, 10 );
            
            mMax            = 1000;
            mMin            = -1000;
            mValue          = 0;
            
            mUseIntegers    = false;
            
            mStep           = 0.1f;
            mFloatPrecision = 3;
            
            SetMouseInputEnabled( true );
            
            SetMargin( Gwen::Margin( 0, 1, 4, 1 ) );
        }
        
        void SetSelectAllOnFocus( bool selectAll = true ){
            mTextBox->SetSelectAllOnFocus( selectAll );
        }
        
        virtual void Render( Gwen::Skin::Base* skin )
        {
        }
        
        void setMin( float i )
        {
            mMin = i;
        }
        
        void setMax( float i )
        {
            mMax = i;
        }
        
        void setValue( float i )
        {
            if ( i > mMax ) i = mMax;
            if ( i < mMin ) i = mMin;
            
            if ( mValue == i )
            {		
                return;
            }
            
            mValue = mUseIntegers ? (int) i : i;
            
            SyncTextFromNumber();
            OnChange();
        }
        
        float getValue(){
            return mValue;
        }
        void setUseIntegers( bool integers ){
            mUseIntegers = integers;
            if( mStep < 1.0f ) mStep = 1.0f;
            SyncTextFromNumber();
        }
        void setStep( float step ){
            mStep = step;
        }
        void setFloatPrecision( int precision ) {
            mFloatPrecision = precision;
        }
		void Layout( Gwen::Skin::Base* skin )
		{
         	//bool resized =
            SizeToChildren( true, true );
        }
        
        Gwen::Event::Caller	onChanged;
        
    private:
        void OnBoxDrag( Gwen::Controls::Base* control )
        {
            NumberStepperBox* box = gwen_cast< NumberStepperBox >( control );
            SyncNumberFromText();
            setValue( mValue + box->mDeltaY * mStep );
        }
        void OnBoxUpdate( Gwen::Controls::Base* control )
        {
            SyncNumberFromText();
        }
        void OnButtonUp( Gwen::Controls::Base* /*control*/ )
        {
          //  SyncNumberFromText();
            setValue( mValue + mStep );
        }
        
        void OnButtonDown( Gwen::Controls::Base* /*control*/ )
        {
         //   SyncNumberFromText();
            setValue( mValue - mStep );
        }
        
        std::string toString( float number ){
            // Problem with ci::toString
            std::ostringstream ss;
            ss << std::fixed << std::setprecision( mUseIntegers ? 0 : mFloatPrecision );
            ss << number;
            std::string str = ss.str();
            
            return str;
        }
        
        void SyncTextFromNumber()
        {
            mTextBox->SetText( toString( mUseIntegers ? (int) mValue : mValue ) );
        }
        
        void SyncNumberFromText()
        {
            if( !mTextBox->GetText().Get().empty() ) setValue( ci::fromString<float>( mTextBox->GetText().c_str() ) );
        }
        
        
        
        void OnChange()
        {
            onChanged.Call( this );
        }
        
        void OnTextChanged()
        {
          //  mTextBox->OnTextChanged();
            
            SyncNumberFromText();
        }
        
        void OnEnter()
        {
            SyncNumberFromText();
            SyncTextFromNumber();
            
            //BaseClass::OnEnter();
        }

        
        
        float               mValue;
        float               mMax;
        float               mMin;
        float               mStep;
        
        NumberStepperBox*   mTextBox;
        
        bool                mUseIntegers;
        
        int                 mFloatPrecision;
        
        
    };
    
    class VectorStepper2f : public Gwen::Controls::Base {
    public:
        
        GWEN_CONTROL_INLINE( VectorStepper2f, Gwen::Controls::Base )
        {
            mXStepper = new NumericStepper( this );
            mXStepper->onChanged.Add( this, &VectorStepper2f::OnChange );
            mXStepper->Dock( Gwen::Pos::Left );
            //mXStepper->SetPos( 0, 0 );
            mYStepper = new NumericStepper( this );
            mYStepper->onChanged.Add( this, &VectorStepper2f::OnChange );
            mYStepper->Dock( Gwen::Pos::Left );
            //mYStepper->SetPos( 65, 0 );
            
            //SizeToChildren();
            SetSize( 2 * 60, 22 );
        }
        
        void setVertical( bool vertical = true ){
            if( vertical ){
                mXStepper->Dock( Gwen::Pos::Top );
                mYStepper->Dock( Gwen::Pos::Top );
                SetSize( 63, 2 * 22 );
            }
            else {
                mXStepper->Dock( Gwen::Pos::Left );
                mYStepper->Dock( Gwen::Pos::Left );
                SetSize( 2 * 60, 22 );
            }
        }
        
        
        void OnChange()
        {
            onChanged.Call( this );
        }
        
        
        void setMin( ci::Vec2f v )
        {
            mXStepper->setMin( v.x );
            mYStepper->setMin( v.y );
        }
        
        void setMax( ci::Vec2f v )
        {
            mXStepper->setMax( v.x );
            mYStepper->setMax( v.y );
        }
        
        void setValue( ci::Vec2f v )
        {
            mXStepper->setValue( v.x );
            mYStepper->setValue( v.y );
        }
        
        ci::Vec2f getValue(){
            return ci::Vec2f( mXStepper->getValue(), mYStepper->getValue() );
        }
        void setStep( float step ){
            mXStepper->setStep( step );
            mYStepper->setStep( step );
        }
        
        void setFloatPrecision( int precision ) {
            mXStepper->setFloatPrecision( precision );
            mYStepper->setFloatPrecision( precision );
        }
        
        Gwen::Event::Caller onChanged;
        
    private:
        void Layout( Gwen::Skin::Base* skin )
		{
         	//bool resized =
            SizeToChildren( true, true );
        }
        
        NumericStepper* mXStepper;
        NumericStepper* mYStepper;
    };
    
    class VectorStepper3f : public Gwen::Controls::Base {
    public:
        
        GWEN_CONTROL_INLINE( VectorStepper3f, Gwen::Controls::Base )
        {
            mXStepper = new NumericStepper( this );
            mXStepper->onChanged.Add( this, &VectorStepper3f::OnChange );
            mXStepper->Dock( Gwen::Pos::Left );
            //mXStepper->SetPos( 0, 0 );
            mYStepper = new NumericStepper( this );
            mYStepper->onChanged.Add( this, &VectorStepper3f::OnChange );
            mYStepper->Dock( Gwen::Pos::Left );
            //mYStepper->SetPos( 65, 0 );
            mZStepper = new NumericStepper( this );
            mZStepper->onChanged.Add( this, &VectorStepper3f::OnChange );
            mZStepper->Dock( Gwen::Pos::Left );
            //mZStepper->SetPos( 130, 0 );
            
            //SizeToChildren();
            SetSize( 3 * 60, 22 );
        }
        
        void setVertical( bool vertical = true ){
            if( vertical ){
                mXStepper->Dock( Gwen::Pos::Top );
                mYStepper->Dock( Gwen::Pos::Top );
                mZStepper->Dock( Gwen::Pos::Top );                
                SetSize( 63, 3 * 22 );
            }
            else {
                mXStepper->Dock( Gwen::Pos::Left );
                mYStepper->Dock( Gwen::Pos::Left );
                mZStepper->Dock( Gwen::Pos::Left );                
                SetSize( 3 * 60, 22 );
            }
        }
        
        
        void OnChange()
        {
            onChanged.Call( this );
        }
        
        
        void setMin( ci::Vec3f v )
        {            
            mXStepper->setMin( v.x );
            mYStepper->setMin( v.y );
            mZStepper->setMin( v.z );
        }
        
        void setMax( ci::Vec3f v )
        {            
            mXStepper->setMax( v.x );
            mYStepper->setMax( v.y );
            mZStepper->setMax( v.z );
        }
        
        void setValue( ci::Vec3f v )
        {            
            mXStepper->setValue( v.x );
            mYStepper->setValue( v.y );
            mZStepper->setValue( v.z );
        }
        
        ci::Vec3f getValue(){
            return ci::Vec3f( mXStepper->getValue(), mYStepper->getValue(), mZStepper->getValue() );
        }
        void setStep( float step ){
            mXStepper->setStep( step );
            mYStepper->setStep( step );
            mZStepper->setStep( step );
        }
        
        void setFloatPrecision( int precision ) {
            mXStepper->setFloatPrecision( precision );
            mYStepper->setFloatPrecision( precision );
            mZStepper->setFloatPrecision( precision );        
        }
        
        Gwen::Event::Caller onChanged;
        
    private:
        void Layout( Gwen::Skin::Base* skin )
		{
         	//bool resized =
            SizeToChildren( true, true );
        }
        
        NumericStepper* mXStepper;
        NumericStepper* mYStepper;
        NumericStepper* mZStepper;
        NumericStepper* mWStepper;
    };

};