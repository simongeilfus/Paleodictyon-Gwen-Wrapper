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

#include "NumericStepper.h"

#include <sstream>
#include <iomanip>

namespace CinderGwen {
    
    
    class HSVColorPicker : public Gwen::Controls::Base
    {
    public:
        GWEN_CONTROL_INLINE( HSVColorPicker, Gwen::Controls::Base )
        {
            SetMouseInputEnabled( true );
            SetSize( 256, 64 );
            SetCacheToTexture();
            
            m_LerpBox = new Gwen::Controls::ColorLerpBox( this );
            m_LerpBox->onSelectionChanged.Add( this, &HSVColorPicker::ColorBoxChanged );
            m_LerpBox->Dock( Gwen::Pos::Left );
            
            m_ColorSlider = new Gwen::Controls::ColorSlider( this );
            m_ColorSlider->SetPos( m_LerpBox->Width() + 15, 5 );
            m_ColorSlider->onSelectionChanged.Add( this, &HSVColorPicker::ColorSliderChanged );
            m_ColorSlider->Dock( Gwen::Pos::Left );
            
            m_After = new Gwen::ControlsInternal::ColorDisplay( this );
            m_After->SetSize( 48, 24 );
            m_After->SetPos( m_ColorSlider->X() + m_ColorSlider->Width() + 15, 5 );
            
            m_Before = new Gwen::ControlsInternal::ColorDisplay( this );
            m_Before->SetSize( 48, 24 );
            m_Before->SetPos( m_After->X(), 28 );
            
            int x = m_Before->X();
            int y = m_Before->Y() + 30;
            
            
            {
                Gwen::Controls::Label* label = new Gwen::Controls::Label( this );
                label->SetText(L"R:");
                label->SizeToContents();
                label->SetPos( x, y );
                
                NumericStepper* numeric = new NumericStepper( this );
                numeric->SetName( "RedBox" );
                numeric->setMin( 0 );
                numeric->setMax( 255 );
                numeric->SetPos( x + 15, y -1  );
                numeric->SetSize( 26, 16 );
                numeric->setUseIntegers( true );
                numeric->SetSelectAllOnFocus( true );
                numeric->onChanged.Add( this, &HSVColorPicker::NumericTyped );
                
            }
            
            y+= 20;
            
            {
                Gwen::Controls::Label* label = new Gwen::Controls::Label( this );
                label->SetText(L"G:");
                label->SizeToContents();
                label->SetPos( x, y );
                
                
                NumericStepper* numeric = new NumericStepper( this );
                numeric->SetName( "GreenBox" );
                numeric->setMin( 0 );
                numeric->setMax( 255 );
                numeric->SetPos( x + 15, y -1  );
                numeric->SetSize( 26, 16 );
                numeric->setUseIntegers( true );
                numeric->SetSelectAllOnFocus( true );
                numeric->onChanged.Add( this, &HSVColorPicker::NumericTyped );
            }
            
            y+= 20;
            
            {
                Gwen::Controls::Label* label = new Gwen::Controls::Label( this );
                label->SetText(L"B:");
                label->SizeToContents();
                label->SetPos( x, y );
                
                
                NumericStepper* numeric = new NumericStepper( this );
                numeric->SetName( "BlueBox" );
                numeric->setMin( 0 );
                numeric->setMax( 255 );
                numeric->SetPos( x + 15, y -1  );
                numeric->SetSize( 26, 16 );
                numeric->setUseIntegers( true );
                numeric->SetSelectAllOnFocus( true );
                numeric->onChanged.Add( this, &HSVColorPicker::NumericTyped );
            }
            
            y+= 20;
            
            {
                Gwen::Controls::Label* label = new Gwen::Controls::Label( this );
                label->SetText(L"A:");
                label->SizeToContents();
                label->SetPos( x, y );
                
                
                NumericStepper* numeric = new NumericStepper( this );
                numeric->SetName( "AlphaBox" );
                numeric->setMin( 0 );
                numeric->setMax( 255 );
                numeric->SetPos( x + 15, y -1  );
                numeric->SetSize( 26, 16 );
                numeric->setUseIntegers( true );
                numeric->SetSelectAllOnFocus( true );
                numeric->onChanged.Add( this, &HSVColorPicker::NumericTyped );
            }
        }
        
        Gwen::Color GetColor()
        {
            //return m_LerpBox->GetSelectedColor();
            return m_After->GetColor();
        }
        Gwen::Color GetDefaultColor() { return m_Before->GetColor(); }
        void SetColor( Gwen::Color color, bool onlyHue = false, bool reset = false )
        {
            UpdateControls( color );
            
            
            if ( reset )
                m_Before->SetColor( color );
            
            m_ColorSlider->SetColor( color );
            m_LerpBox->SetColor( color,  onlyHue );
            m_After->SetColor( color );
        }
        
        void ColorBoxChanged( Gwen::Controls::Base* pControl )
        {
            onColorChanged.Call( this );
            Gwen::Color newColor = m_LerpBox->GetSelectedColor();
            newColor.a = m_After->GetColor().a;
            UpdateControls( newColor );
            Invalidate();
        }
        void ColorSliderChanged( Gwen::Controls::Base* pControl )
        {
            if ( m_LerpBox )
                m_LerpBox->SetColor( m_ColorSlider->GetSelectedColor(),  true );
            Invalidate();
        }
        void NumericTyped( Gwen::Controls::Base* control )
        {
            NumericStepper* box = gwen_cast<NumericStepper>(control);
            if ( !box ) return;
            
            //if ( box->GetText() == L"" )	return;
            
            int textValue = box->getValue();//atoi( box->GetText().c_str()  );
            if ( textValue < 0 ) textValue = 0;
            if ( textValue > 255 ) textValue = 255;
            
            Gwen::Color newColor = GetColor();
            
            if ( box->GetName().find( "Red" ) != Gwen::String::npos )
            {
                newColor.r = textValue;
            }
            else if ( box->GetName().find( "Green" ) != Gwen::String::npos )
            {
                newColor.g = textValue;
            }
            else if ( box->GetName().find( "Blue" ) != Gwen::String::npos )
            {
                newColor.b = textValue;
            }
            else if ( box->GetName().find( "Alpha" ) != Gwen::String::npos )
            {
                newColor.a = textValue;
            }
            
            //SetColor( newColor );
            m_After->SetColor( newColor );
            onColorChanged.Call( this );
        }
        
        
        void UpdateControls( Gwen::Color color )
        {
            // What in the FUCK
            
            NumericStepper* redBox = gwen_cast<NumericStepper>(	FindChildByName( "RedBox",   false ) );
            if ( redBox )    redBox->setValue( (int) color.r );//redBox->SetText( Gwen::Utility::ToString( (int)color.r), false );
            
            NumericStepper* greenBox = gwen_cast<NumericStepper>(	FindChildByName( "GreenBox",   false ) );
            if ( greenBox )  greenBox->setValue( (int) color.g );//greenBox->SetText( Gwen::Utility::ToString( (int)color.g ), false );
            
            NumericStepper* blueBox = gwen_cast<NumericStepper>(	FindChildByName( "BlueBox",   false ) );
            if ( blueBox )   blueBox->setValue( (int) color.b );//blueBox->SetText( Gwen::Utility::ToString( (int)color.b ), false );
            
            NumericStepper* alphaBox = gwen_cast<NumericStepper>(	FindChildByName( "AlphaBox",   false ) );
            if ( alphaBox )   alphaBox->setValue( (int) color.a );//alphaBox->SetText( Gwen::Utility::ToString( (int)color.a ), false );
            
            m_After->SetColor( color );
        }
        
        Gwen::Event::Caller	onColorChanged;
        
    protected:
        Gwen::Controls::ColorLerpBox* m_LerpBox;
        Gwen::Controls::ColorSlider* m_ColorSlider;
        Gwen::ControlsInternal::ColorDisplay* m_Before;
        Gwen::ControlsInternal::ColorDisplay* m_After;
    };
    
    class ColorPicker : public Gwen::Controls::Button
    {
    public:
        
        GWEN_CONTROL_INLINE( ColorPicker, Gwen::Controls::Button ){
            m_Color = Gwen::Colors::Black;
            SetText( "" );
            SetSize( 40, 18 );
            mAlpha = false;
        }
        
        void Render( Gwen::Skin::Base* skin )
        {
            skin->GetRender()->SetDrawColor( m_Color );
            //skin->GetRender()->DrawFilledRect( GetRenderBounds() );
            skin->DrawColorDisplay( this, m_Color );
            skin->GetRender()->DrawShavedCornerRect( GetRenderBounds() );
        }
        Gwen::Color GetColor(){ return m_Color; }
        void SetColor( const Gwen::Color& col ){
            //std::cout << "set color" <<  toCinder( col ) << std::endl;
            m_Color = col; }
        
        void OnPress(){
            Gwen::Controls::Menu* pMenu = new Gwen::Controls::Menu( GetCanvas() );
            pMenu->SetSize( 260, 180 );
            pMenu->SetDeleteOnClose( true );
            pMenu->SetDisableIconMargin( true );
            pMenu->Open( Gwen::Pos::Right | Gwen::Pos::Top );
            
            Gwen::Controls::Base* truc = new Gwen::Controls::Base( pMenu );
            truc->Dock( Gwen::Pos::Fill );
            truc->SetSize( 260, mAlpha ? 160 : 135 );
            truc->SetPadding( Gwen::Padding( 10, 10, 10, 10 ) );
            
            HSVColorPicker* picker = new HSVColorPicker( truc );
            picker->Dock( Gwen::Pos::Fill );
            picker->SetSize( 256, 128 );
            picker->SetCacheToTexture( false );
            //std::cout << toCinder( m_Color ) << std::endl;
            picker->SetColor( m_Color, false, true );
            picker->onColorChanged.Add( this, &ThisClass::ColorChanged );
            
            pMenu->Layout( GetSkin() );
        }
        
        
        void ColorChanged( Gwen::Controls::Base* control )
        {
            HSVColorPicker* picker = gwen_cast<HSVColorPicker>( control );
            
            //Gwen::String colorStr;
            //colorStr += Gwen::Utility::ToString( ( int )picker->GetColor().r ) + " ";
            //colorStr += Gwen::Utility::ToString( ( int )picker->GetColor().g ) + " ";
            //colorStr += Gwen::Utility::ToString( ( int )picker->GetColor().b );
            
            //m_TextBox->SetText( colorStr );
            //DoChanged();
            if( picker ){
                m_Color = picker->GetColor();
                onChanged.Call( this );
            }
        }
        
        void setUseAlpha( bool alpha = true ){ mAlpha = alpha; }
        bool useAlpha(){ return mAlpha; }
        
        Gwen::Event::Caller onChanged;
    private:
        
        Gwen::Color	m_Color;
        bool        mAlpha;
    };

};