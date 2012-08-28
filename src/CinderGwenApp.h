//
//  CinderGwenApp.h
//  Prototype
//
//  Created by Simon Geilfus on 28/08/12.
//
//

#pragma once


#include "cinder/app/AppBasic.h"
#include "cinder/Display.h"
#include "cinder/Function.h"

#include "CinderGwenParams.h"

class CinderGwenApp : public cigw::Inspectable, public ci::app::AppBasic {
public:
    
    void setStatusBarMessage( const std::string &message );
    
    cigw::Menu*             getMenu();
    cigw::Params*           getInspector();
    cigw::StatusBar*        getStatusBar();
    cigw::Flow*             getFlow();
    cigw::Timeline*         getTimelineUi();
    cigw::ObjectExplorer*   getObjectExplorer();
    
    ci::TimelineRef         getTimeline();
    
    virtual void handleFileMenu( cigw::MenuEvent event );
    virtual void handleViewMenu( cigw::MenuEvent event );
    virtual void handleInspector( const std::string &inspName );
    
	virtual void	privateSetup__();
	virtual void	privateShutdown__();
	virtual void	privateUpdate__();
	virtual void	privateDraw__();
    
protected:
    
    void privateSetupUi__();
    void privateUpdateUi__();
    void privateDrawUi__();
    
    ci::TimelineRef         mTimeline;
    
    cigw::Menu*             mUiMenu;
    cigw::Params*           mUiInspector;
    cigw::StatusBar*        mUiStatusBar;
    cigw::Flow*             mUiFlow;
    cigw::Timeline*         mUiTimeline;
    cigw::ObjectExplorer*   mUiObjectExplorer;
};