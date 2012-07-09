#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/TextureFont.h"
#include "cinder/gl/Fbo.h"
#include "cinder/ImageIo.h"
#include "cinder/Font.h"
#include "cinder/Text.h"
#include "cinder/Surface.h"
#include "cinder/Utilities.h"

#include "Gwen/Gwen.h"
#include "Gwen/Font.h"
#include "Gwen/Texture.h"
#include "Gwen/Skins/Simple.h"
#include "Gwen/Skins/TexturedBase.h"
//#include "Gwen/Input/Windows.h"

#include <iostream>
#include <string>
#include <map>

namespace CinderGwen {


	class Renderer : public Gwen::Renderer::Base {
	public:		
		Renderer();
		~Renderer();

		void Begin();
		void End();
	
		void DrawFilledRect( Gwen::Rect rect );

		void SetDrawColor( Gwen::Color color );

		void StartClip();
		void EndClip();

		void DrawTexturedRect( Gwen::Texture* pTexture, Gwen::Rect rect, float u1, float v1, float u2, float v2 );
		void DrawMissingImage( Gwen::Rect pTargetRect );

		void LoadTexture( Gwen::Texture* pTexture );
		void FreeTexture( Gwen::Texture* pTexture );

		void LoadFont( Gwen::Font* pFont );
		void FreeFont( Gwen::Font* pFont );
		void RenderText( Gwen::Font* pFont, Gwen::Point pos, const Gwen::UnicodeString& text );
		Gwen::Point MeasureText( Gwen::Font* pFont, const Gwen::UnicodeString& text );

		Gwen::Color PixelColour( Gwen::Texture* pTexture, unsigned int x, unsigned int y, const Gwen::Color& col_default = Gwen::Color( 255, 255, 255, 255 ) );


		
		class FboCTT : public Gwen::Renderer::ICacheToTexture {
		public:
			void Initialize();
			void ShutDown();
			void SetupCacheTexture( Gwen::Controls::Base* control );
			void FinishCacheTexture( Gwen::Controls::Base* control );
			void DrawCachedControlTexture( Gwen::Controls::Base* control );
			void CreateControlCacheTexture( Gwen::Controls::Base* control );
			void UpdateControlCacheTexture( Gwen::Controls::Base* control );
			void SetRenderer( Gwen::Renderer::Base* renderer );

			
		//protected:
			Renderer::Base* mRenderer;
			ci::gl::Fbo		mFbo;
		};

		
		Gwen::Renderer::ICacheToTexture* GetCTT() {
			if( mFboCTT == NULL ){
				mFboCTT = new FboCTT();
				mFboCTT->SetRenderer( this );
			}
			return (Gwen::Renderer::ICacheToTexture*) mFboCTT; 
		}

		ci::ColorA										mColor;
		std::map< std::string, ci::gl::Texture >		mTextures;
		std::map< std::string, ci::Font >				mFonts;
		std::map< std::string, ci::gl::TextureFontRef >	mTextureFonts;
		FboCTT*											mFboCTT;
	};

};