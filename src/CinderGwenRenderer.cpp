#include "CinderGwenRenderer.h"
#include "GwenResources.h"
	
namespace CinderGwen {


	Renderer::Renderer() : mFboCTT( NULL ) {
	}

	Renderer::~Renderer(){
	}

	void Renderer::Begin(){
		//gl::enableAlphaBlending();
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		glAlphaFunc( GL_GREATER, 1.0f );	
		glEnable ( GL_BLEND );
	}

	void Renderer::End(){
		glBindTexture( GL_TEXTURE_2D, 0 );
		glDisable( GL_TEXTURE_2D );
		glDisable( GL_BLEND );
	}
	
	void Renderer::DrawFilledRect( Gwen::Rect rect ){
		GLboolean texturesOn;
		glGetBooleanv(GL_TEXTURE_2D, &texturesOn);
		if ( texturesOn ){
			glDisable(GL_TEXTURE_2D);
		}	
		
		Translate( rect );
		ci::gl::color( mColor );
		//ci::gl::drawSolidRect( ci::Rectf( rect.x, rect.y, rect.x + rect.w, rect.y + rect.w ) );

		glBegin( GL_QUADS );
		glVertex2f( rect.x, rect.y );
		glVertex2f( rect.x + rect.w, rect.y );
		glVertex2f( rect.x + rect.w, rect.y + rect.h );
		glVertex2f( rect.x, rect.y + rect.h);
		glEnd();
		
	}

	void Renderer::SetDrawColor(Gwen::Color color){
		//mColor = ci::ColorA( color.r, color.g, color.b, color.a );
		mColor = ci::ColorA( color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f );
	}

	void Renderer::StartClip(){
		Gwen::Rect rect = ClipRegion();
		GLint view[4];
		glGetIntegerv( GL_VIEWPORT, &view[0] );
		rect.y = view[3] - (rect.y + rect.h);

		glScissor( rect.x * Scale(), rect.y * Scale(), rect.w * Scale(), rect.h * Scale() );
		glEnable( GL_SCISSOR_TEST );
	};

	void Renderer::EndClip(){
		glDisable( GL_SCISSOR_TEST );
	};

	
	void Renderer::DrawTexturedRect( Gwen::Texture* pTexture, Gwen::Rect rect, float u1, float v1, float u2, float v2 ){
		std::string name;
		name.assign( pTexture->name.GetUnicode().begin(), pTexture->name.GetUnicode().end() );

		if( mTextures.count( name ) == 0 ) return;

		ci::gl::Texture tex = mTextures.find( name )->second;

		if ( !tex ){
			DrawMissingImage( rect );
			return;
		}

		Translate( rect );

		ci::gl::color( ci::ColorA( 1,1,1,1 ) );
		
		GLuint boundtex;

		GLboolean texturesOn;
		glGetBooleanv(GL_TEXTURE_2D, &texturesOn);
		glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint *)&boundtex);
		if ( !texturesOn || tex.getId() != boundtex ){
			tex.enableAndBind();
		}		

		glBegin( GL_TRIANGLES );
		
		glTexCoord2f( u1, v1 );
		glVertex2f( rect.x, rect.y );
		glTexCoord2f( u2, v1 );
		glVertex2f( rect.x+rect.w, rect.y);
		glTexCoord2f( u1, v2 );
		glVertex2f( rect.x, rect.y + rect.h);

		glTexCoord2f( u2, v1 );
		glVertex2f( rect.x+rect.w, rect.y);
		glTexCoord2f( u2, v2 );
		glVertex2f( rect.x+rect.w, rect.y+rect.h);
		glTexCoord2f( u1, v2 );	
		glVertex2f( rect.x, rect.y + rect.h );

		glEnd();
	
	}

	
	void Renderer::DrawMissingImage( Gwen::Rect pTargetRect ){
		Translate( pTargetRect );
		ci::gl::color( ci::ColorA( 1.0f, 0.0f, 0.0f, 1.0f ) );

		glBegin( GL_QUADS );
		glVertex2f( pTargetRect.x, pTargetRect.y );
		glVertex2f( pTargetRect.x + pTargetRect.w, pTargetRect.y );
		glVertex2f( pTargetRect.x + pTargetRect.w, pTargetRect.y + pTargetRect.h );
		glVertex2f( pTargetRect.x, pTargetRect.y + pTargetRect.h);
		glEnd();
		
	}

	void Renderer::LoadTexture( Gwen::Texture* pTexture )	{
		std::string path;
		path.assign( pTexture->name.GetUnicode().begin(), pTexture->name.GetUnicode().end() );

		if( mTextures.count( path ) == 0 ){
			ci::gl::Texture texture;
			try {

	#ifdef GWEN_RESOURCES
				if( GWEN_RESOURCE_MAP.count( path ) > 0 ) texture = ci::gl::Texture( ci::loadImage( ci::app::loadResource( path, GWEN_RESOURCE_MAP[ path ], "IMAGE" ) ) );
				else texture = ci::gl::Texture( ci::loadImage( ci::app::getAssetPath( path ) ) );
	#else
				texture = ci::gl::Texture( ci::loadImage( ci::app::getAssetPath( path ) ) );
	#endif
			}
			catch( ... ){
				pTexture->failed = true;
				return;
			}
		
			mTextures[ path ] = texture;
			pTexture->width = texture.getWidth();
			pTexture->height = texture.getHeight();
		}
	}

	void Renderer::FreeTexture( Gwen::Texture* pTexture )	{
		std::string name;
		name.assign( pTexture->name.GetUnicode().begin(), pTexture->name.GetUnicode().end() );

		if( mTextures.count( name ) == 0 ) return;

		ci::gl::Texture tex = mTextures.find( name )->second;
		if ( !tex ) return;
		
		mTextures.erase( mTextures.find( name ) );

		pTexture->data = NULL;
	}


	void Renderer::LoadFont( Gwen::Font* pFont ){
		std::string name;
		name.assign( pFont->facename.begin(), pFont->facename.end());
		std::string nameExt = ci::getPathExtension( name );

		if( mFonts.count( name ) == 0 ){
			pFont->realsize = pFont->size * Scale();


			ci::Font font;
			try{

				if( nameExt.length() == 0 ){
					font = ci::Font( name, pFont->realsize );
				}
				else {
					
			#ifdef GWEN_RESOURCES
						if( GWEN_RESOURCE_MAP.count( name ) > 0 ) {
							font = ci::Font( ci::app::loadResource( name, GWEN_RESOURCE_MAP[ name ], "TTF" ), pFont->realsize );
						}
						else {
							font = ci::Font( ci::app::loadAsset( ci::app::getAssetPath( name ) ), pFont->realsize );
						}
			#else
						font = ci::Font( ci::app::loadAsset( ci::app::getAssetPath( name ) ), pFont->realsize );
			#endif
				}
			}
			catch( ... ){
				return;
			}
			ci::gl::TextureFontRef texFont = ci::gl::TextureFont::create( font );
			mFonts[ name ] = font;
			mTextureFonts[ name ] = texFont;
		}
	};
	void Renderer::FreeFont( Gwen::Font* pFont ){
		std::string name;
		name.assign( pFont->facename.begin(), pFont->facename.end());

		if( mFonts.count( name ) == 0 ) return;
		
		mFonts.erase( mFonts.find( name ) );
		mTextureFonts.erase( mTextureFonts.find( name ) );
	};

	void Renderer::RenderText( Gwen::Font* pFont, Gwen::Point pos, const Gwen::UnicodeString& text ){
		std::string name;
		name.assign( pFont->facename.begin(), pFont->facename.end());
	
		std::string stext;
		stext.assign( text.begin(), text.end() );
	
		ci::gl::TextureFontRef texFont = mTextureFonts.find( name )->second;
		if( mTextureFonts.count( name ) == 0 ){
			ci::gl::drawString( stext, ci::Vec2f( pos.x, pos.y + texFont->getAscent() ) );
			return;
		}

		if ( !texFont ) {
			ci::gl::drawString( stext, ci::Vec2f( pos.x, pos.y + texFont->getAscent() ) );
			return;
		}
		
		//ci::Vec2f size = texFont->measureString( stext );

		Translate( pos.x, pos.y );
		ci::gl::color( mColor );
		texFont->drawString( stext, ci::Vec2f( pos.x, pos.y + texFont->getAscent() ) );
	}
	Gwen::Point Renderer::MeasureText( Gwen::Font* pFont, const Gwen::UnicodeString& text ){
		std::string name;
		name.assign( pFont->facename.begin(), pFont->facename.end());

		if( mFonts.count( name ) == 0 ) LoadFont( pFont );

		ci::gl::TextureFontRef texFont = mTextureFonts.find( name )->second;
		if ( ! texFont ) LoadFont( pFont );

		std::string stext;
		stext.assign( text.begin(), text.end() );
		
		ci::Vec2f size = texFont->measureString( stext ) * Scale();
		return Gwen::Point( size.x, 1.25f * pFont->size );
	}



	Gwen::Color Renderer::PixelColour( Gwen::Texture* pTexture, unsigned int x, unsigned int y, const Gwen::Color& col_default ){
		std::string name;
		name.assign( pTexture->name.GetUnicode().begin(), pTexture->name.GetUnicode().end() );
	
		if( mTextures.count( name ) == 0 ) return col_default;
	
		ci::gl::Texture tex = mTextures.find( name )->second;
		if ( !tex ) return col_default;

		ci::Surface surf( tex );
		ci::ColorA c = surf.getPixel( ci::Vec2i( x, y ) );

		return Gwen::Color( c.r * 255.0f, c.g * 255.0f, c.b * 255.0f, c.a * 255.0f );
	}


	
	void Renderer::FboCTT::Initialize(){
	}
	void Renderer::FboCTT::ShutDown(){
	}
	void Renderer::FboCTT::SetupCacheTexture( Gwen::Controls::Base* control ){
		//	glDisable( GL_TEXTURE_2D );
		//ci::app::console() << "SetupCacheTexture " << std::endl;
		////ci::gl::pushMatrices();

		//mFbo.bindFramebuffer();
		////ci::gl::clear( ci::ColorA::black() );
		///*ci::gl::setMatricesWindow( ci::Vec2i( control->GetBounds().w, control->GetBounds().h ) );
		////ci::gl::setViewport( Area( control->GetBounds().x, control->GetBounds().y, control->GetBounds().x + control->GetBounds().w, control->GetBounds().y + control->GetBounds().h ) );

		//ci::gl::enableAlphaBlending();
		//ci::gl::enableDepthRead();
		//ci::gl::enableDepthWrite();
		//ci::gl::enableAlphaTest();*/
	}
	void Renderer::FboCTT::FinishCacheTexture( Gwen::Controls::Base* control ){
		//ci::app::console() << "FinishCacheTexture " << std::endl;
		//
		///*ci::gl::disableAlphaBlending();
		//ci::gl::disableAlphaTest();
		//ci::gl::disableDepthRead();
		//ci::gl::disableDepthWrite();*/

		//mFbo.unbindFramebuffer();

		////ci::gl::popMatrices();
	}
	void Renderer::FboCTT::DrawCachedControlTexture( Gwen::Controls::Base* control ){
		/*if( mFbo && mFbo.getTexture() ) {

			glDisable( GL_TEXTURE_2D );
			glBindTexture( GL_TEXTURE_2D, 0 );

			ci::gl::color( ci::ColorA::white() );
			ci::gl::draw( mFbo.getTexture(), ci::Rectf( control->GetBounds().x, control->GetBounds().y, control->GetBounds().x + control->GetBounds().w, control->GetBounds().y + control->GetBounds().h ) );
		}*/
	}
	void Renderer::FboCTT::CreateControlCacheTexture( Gwen::Controls::Base* control ){
		/*ci::gl::Fbo::Format format;
		format.enableColorBuffer( true, 1 );
		format.enableDepthBuffer( true, false );
		format.setDepthInternalFormat( GL_DEPTH24_STENCIL8 );
		format.setColorInternalFormat( GL_RGBA8 );
		mFbo = ci::gl::Fbo( control->GetBounds().w, control->GetBounds().h, format );

		mFbo.bindFramebuffer();
		ci::gl::clear( ci::ColorA::black() );
		mFbo.unbindFramebuffer();

		ci::app::console() << "Create " << control->GetBounds().w <<", "<<control->GetBounds().h<< " fbo" << std::endl;*/
	}
	void Renderer::FboCTT::UpdateControlCacheTexture( Gwen::Controls::Base* control ){
		/*ci::gl::Fbo::Format format;
		format.enableColorBuffer( true, 1 );
		format.setColorInternalFormat( GL_RGBA );
		mFbo = ci::gl::Fbo( control->GetBounds().w, control->GetBounds().h, format );*/
	}
	void Renderer::FboCTT::SetRenderer( Gwen::Renderer::Base* renderer ){
		mRenderer = renderer;	
	}

};