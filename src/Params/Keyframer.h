#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"

#include "Gwen/Gwen.h"
#include "Gwen/Skins/Simple.h"
#include "Gwen/Skins/TexturedBase.h"

#include "Gwen/BaseRender.h"
#include "Gwen/Controls/Base.h"
#include "Gwen/Controls/TextBox.h"
#include "Gwen/Controls/Menu.h"
#include "Gwen/Controls/MenuItem.h"

#include "CinderGwenParams.h"

namespace CinderGwen {
    
    
    class Keyframer : public Gwen::Controls::Button
    {
        GWEN_CONTROL_INLINE( Keyframer, Gwen::Controls::Button )
        {
            SetSize( 7, 7 );
            
          /*  Gwen::Controls::Menu* menu;
            menu = new Gwen::Controls::Menu( getCanvas() );
			menu->AddItem( L"Add Keyframe");
            menu->AddItem( L"Next Keyframe");
            menu->AddItem( L"Previous Keyframe");
            menu->AddItem( L"Delete Keyframe");
            menu->AddDivider();
            menu->AddItem( L"Add Track");
            menu->AddItem( L"Copy Track");
            menu->AddItem( L"Past Track");
            menu->AddItem( L"Delete Track");
            menu->SetWidth( 140 );
            menu->CloseAll();
            menu->Close();
            menu->SetPos( 0, 0 );
            menu->SetDisableIconMargin( true );*/
            SetKeyboardInputEnabled( true );
        }
        
        void OnPress(){
            onNewKeyFrame.Call( this );
            BaseClass::OnPress();
        }
        
        Gwen::Event::Caller onNewKeyFrame;
        
        virtual bool OnKeyPress( int iKey, bool bPress = true ){
            //ci::app::console() << iKey << std::endl;
        }
        virtual bool OnKeyRelease( int iKey ){
            
        }
        virtual void AcceleratePressed() {
            //ci::app::console() << "acc" << std::endl;
        };
        virtual bool AccelOnlyFocus() { 
            //ci::app::console() << iKey << std::endl;
            return false; 
        }
        
        virtual void Render( Gwen::Skin::Base* skin )
        {
            //Gwen::Rect bounds = GetRenderBounds();
            //skin->GetRender()->Translate( bounds );
            //ci::gl::drawStrokedCircle( ci::Vec2i( bounds.x, bounds.y ), 30 );
            skin->DrawRadioButton( this, IsDepressed(), true );
        }
        
        virtual void OnMouseMoved( int x, int y, int deltaX, int deltaY ){
            BaseClass::OnMouseMoved( x, y, deltaX, deltaY );
        }
        virtual bool OnMouseWheeled( int iDelta ){
            return BaseClass::OnMouseWheeled( iDelta );
        }
        virtual void OnMouseClickLeft( int x, int y, bool bDown ){
            BaseClass::OnMouseClickLeft( x, y, bDown );
        };
        virtual void OnMouseClickRight( int x, int y, bool bDown ){ 
            Gwen::Controls::Menu* menu = ContextMenu::getMenu();
            menu->ClearItems();
            menu->AddItem( L"Add Keyframe");
            menu->AddItem( L"Next Keyframe");
            menu->AddItem( L"Previous Keyframe");
            menu->AddItem( L"Delete Keyframe");
            menu->AddDivider();
            menu->AddItem( L"Add Track");
            menu->AddItem( L"Copy Track");
            menu->AddItem( L"Past Track");
            menu->AddItem( L"Delete Track");
            menu->SetWidth( 140 );
            menu->Open( 0 );
            //if( !menu->IsMenuOpen() ){
            //    menu->Open( 0 );
            //}
           // BaseClass::OnMouseClickRight( x, y, bDown );
            
        }
        virtual void OnMouseDoubleClickLeft( int x, int y ){ 
            OnMouseClickLeft( x, y, true ); 
        };
        virtual void OnMouseDoubleClickRight( int x, int y ){ 
            OnMouseClickRight( x, y, true ); 
        };
        
    };
   

};