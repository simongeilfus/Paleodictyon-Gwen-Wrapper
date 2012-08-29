//
//  CinderGwenApp.cpp
//  Prototype
//
//  Created by Simon Geilfus on 28/08/12.
//
//

#include "CinderGwenApp.h"


cigw::Menu* CinderGwenApp::getMenu()
{
    return mUiMenu;
}
cigw::Params* CinderGwenApp::getInspector()
{
    return mUiInspector;
}
cigw::StatusBar* CinderGwenApp::getStatusBar()
{
    return mUiStatusBar;
}
cigw::Flow* CinderGwenApp::getFlow()
{
    return mUiFlow;
}
cigw::Timeline* CinderGwenApp::getTimelineUi()
{
    return mUiTimeline;
}
cigw::ObjectExplorer* CinderGwenApp::getObjectExplorer()
{
    return mUiObjectExplorer;
}


ci::TimelineRef CinderGwenApp::getTimeline()
{
    return mTimeline;
}


void CinderGwenApp::setStatusBarMessage( const std::string &message )
{
    mUiStatusBar->setStatus( message );
}

void CinderGwenApp::handleFileMenu( cigw::MenuEvent event )
{
    
    if( event.getSubName() == "Save" ) {
        if( !ci::fs::exists( getAppPath() / "LastState" ) ) ci::fs::create_directory( getAppPath() / "LastState" );
        
        cigw::Inspectable::writeAll( ci::writeFile( getAppPath() / "LastState/params.ui" ) );
    }
    else if( event.getSubName() == "Save As" ) {
        if( !ci::fs::exists( getAppPath() / "Presets" ) ) ci::fs::create_directory( getAppPath() / "Presets" );
        
        ci::fs::path path = getSaveFilePath( getAppPath() / "Presets", boost::assign::list_of( "params" ) );
        if( !path.empty() )
            cigw::Inspectable::writeAll( ci::writeFile( path ) );
        
    }
    else if( event.getSubName() == "Load" ) {
        if( !ci::fs::exists( getAppPath() / "Presets" ) ) ci::fs::create_directory( getAppPath() / "Presets" );
        
        ci::fs::path path = getOpenFilePath( getAppPath() / "Presets", boost::assign::list_of( "params" ) );
        if( !path.empty() )
            cigw::Inspectable::readAll( ci::loadFile( path ) );
    }
    else if( event.getSubName() == "Quit" ) {
        quit();
    }
}
void CinderGwenApp::handleViewMenu( cigw::MenuEvent event )
{
    if( event.getSubName() == "Inspector" ) mUiInspector->setVisible( event.isChecked() );
    else if( event.getSubName() == "FlowGraph" ) mUiFlow->setVisible( event.isChecked() );
    else if( event.getSubName() == "Timeline" ) mUiTimeline->setVisible( event.isChecked() );
    else if( event.getSubName() == "ObjectExplorer" ) mUiObjectExplorer->setVisible( event.isChecked() );
    else if( event.getSubName() == "FullScreen" ) setFullScreen( !isFullScreen() );
}
void CinderGwenApp::handleInspector( const std::string &inspName )
{
    
}


void CinderGwenApp::privateSetup__()
{
    privateSetupUi__();
    ci::app::AppBasic::privateSetup__();
    cigw::Inspectable::readAll( ci::loadFile( getAppPath() / "LastState/params.ui" ) );
}
void CinderGwenApp::privateShutdown__()
{
    if( !ci::fs::exists( getAppPath() / "LastState" ) ) ci::fs::create_directory( getAppPath() / "LastState" );
    cigw::Inspectable::writeAll( ci::writeFile( getAppPath() / "LastState/params.ui" ) );
    mUiFlow->getGraph().write( ci::writeFile( getAppPath() / "LastState/graph.flow" ) );
    
    ci::app::AppBasic::privateShutdown__();
}
void CinderGwenApp::privateUpdate__()
{
    ci::app::AppBasic::privateUpdate__();
    privateUpdateUi__();
}
void CinderGwenApp::privateDraw__()
{
    ci::app::AppBasic::privateDraw__();
    privateDrawUi__();
}


void CinderGwenApp::privateSetupUi__()
{
    
    mTimeline           = ci::Timeline::create();
    
    // Main Menu ----------------------------------------------------------------------------------
    
    mUiMenu = new cigw::Menu();
    mUiMenu->addItem( "File", "Save", std::bind( &CinderGwenApp::handleFileMenu, this, std::tr1::placeholders::_1 ) );
    mUiMenu->addItem( "File", "Save As", std::bind( &CinderGwenApp::handleFileMenu, this, std::tr1::placeholders::_1 ) );
    mUiMenu->addItem( "File", "Load", std::bind( &CinderGwenApp::handleFileMenu, this, std::tr1::placeholders::_1 ) );
    mUiMenu->addItem( "File", "Quit", std::bind( &CinderGwenApp::handleFileMenu, this, std::tr1::placeholders::_1 ) );
    
    mUiMenu->addItem( "View", "FullScreen", std::bind( &CinderGwenApp::handleViewMenu, this, std::tr1::placeholders::_1 ) );
    mUiMenu->addSeparator( "View" );
    mUiMenu->addItem( "View", "Inspector", std::bind( &CinderGwenApp::handleViewMenu, this, std::tr1::placeholders::_1 ), true );
    mUiMenu->addItem( "View", "ObjectExplorer", std::bind( &CinderGwenApp::handleViewMenu, this, std::tr1::placeholders::_1 ), true );
    mUiMenu->addItem( "View", "FlowGraph", std::bind( &CinderGwenApp::handleViewMenu, this, std::tr1::placeholders::_1 ), true, false );
    mUiMenu->addItem( "View", "Timeline", std::bind( &CinderGwenApp::handleViewMenu, this, std::tr1::placeholders::_1 ), true, true );
    
    
    // Status Bar ---------------------------------------------------------------------------------
    
    mUiStatusBar = new cigw::StatusBar();
    
    float sidebarWidth = 220;
    float timelineHeight = 200;
    float objectExplorerHeight = 200;
    
    // Params -------------------------------------------------------------------------------
    
    mUiInspector = new cigw::Params( "Inspector", ci::Vec2i( sidebarWidth, getWindowHeight() - objectExplorerHeight - 50 ) );
    mUiInspector->setPosition( ci::Vec2f( 5, objectExplorerHeight + 25 ) );
    
    
    // Timeline -----------------------------------------------------------------------------------
    
    mUiTimeline = new cigw::Timeline( "Timeline", ci::Vec2i( MIN( getWindowWidth() - 10 - sidebarWidth, 800 ), timelineHeight ), 90, mTimeline );
    mUiTimeline->setTimeFormat( cigw::Timeline::TimeFormat_Frames );
    mUiTimeline->setPosition( ci::Vec2f( sidebarWidth + 5, getWindowHeight() - timelineHeight - 25 ) );
    
    // Flow Graph ---------------------------------------------------------------------------------
    
    mUiFlow = new cigw::Flow( "Flow", ci::Vec2i( getWindowWidth() - 630 - 10, timelineHeight ) );
    mUiFlow->setPosition( ci::Vec2f( sidebarWidth + 640, getWindowHeight() - timelineHeight - 25 ) );
    mUiFlow->getGraph().read( ci::loadFile( getAppPath() / "LastState/graph.flow" ) );
    mUiFlow->setVisible(false);
    
    // Object Explorer  ---------------------------------------------------------------------------
    
    mUiObjectExplorer = new cigw::ObjectExplorer( "Objects", ci::Vec2i( sidebarWidth, objectExplorerHeight ), mUiInspector, mUiTimeline );
    mUiObjectExplorer->setPosition( ci::Vec2i( 5, 25 ) );
}
void CinderGwenApp::privateUpdateUi__()
{
    mUiTimeline->update();
    mUiFlow->update();
}
void CinderGwenApp::privateDrawUi__()
{
    ci::gl::setMatricesWindow( getWindowSize() );
    cigw::draw();
}