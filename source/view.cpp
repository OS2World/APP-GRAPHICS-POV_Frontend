// St‚phane Charette, charette@writeme.com
// Open Class Library POV Front-end Launch Tool
// Application & source code (c) St‚phane Charette


// required resource and class definitions

#include "view.hpp"
#include "pov_launch.h"

#include <ipopmenu.hpp> // IPopUpMenu
#include <ititle.hpp>   // ITitle

#define VIEW_CANVAS        (P_VIEWER + 0x0001)
#define VIEW_PORT          (P_VIEWER + 0x0002)
#define VIEW_MENU          (P_VIEWER + 0x0003)


// initialize static class variables
int ViewFrame::viewNumber = 0;


ViewFrame::ViewFrame( IWindow *dlg, char *filename ) :

   IFrameWindow(  IResourceId( P_VIEWER ),
                  IWindow::desktopWindow(),
                  dlg,
                  IRectangle(),
                  IFrameWindow::classDefaultStyle  |
                  IFrameWindow::clipToParent       |
                  IFrameWindow::noMoveWithOwner    ),

   viewPort(      VIEW_PORT, this, this, IRectangle(),
                  IViewPort::classDefaultStyle     |
                  IViewPort::noViewWindowFill      ),
   canvas(        VIEW_CANVAS, &viewPort, &viewPort ),
   fileName(      filename ),
   image(         fileName ),
   glist(         image    )
{
   // initialize variables
   resetVariables();

   // create the view canvas
   canvas.setGraphicList( &glist );

   // set the application's title
   ITitle title( this, APP_NAME, POV_VIEWER_TITLE, ++viewNumber );

   setClient( &viewPort );

   // setup handlers
   ISelectHandler::  handleEventsFor( &viewPort );
   ICommandHandler:: handleEventsFor( this );
   IMenuHandler::    handleEventsFor( &canvas );

   moveSizeTo( IRectangle( 100, 100, 500, 500 ) );

   show().setFocus();

   return;
}


ViewFrame::~ViewFrame()
{
   // stop handling events
   IMenuHandler::    stopHandlingEventsFor( &canvas );
   ICommandHandler:: stopHandlingEventsFor( this );
   ISelectHandler::  stopHandlingEventsFor( &viewPort );
   return;
}


Boolean ViewFrame::command( ICommandEvent &event )
{
/*
   // handle command events
   switch( event.commandId() )
   {
      // the only command we should receive:
      // "enable" checkbox was toggled via the popup menu
      case P_CROP_ENABLE_CBX:
      {
         // simulate a mouse click on the "enable" checkbox
         enableCBx.click();
         return true;
         break;
      }
   }
*/
   return false;
}


Boolean ViewFrame::selected( IControlEvent &event )
{
   return false;
}


Boolean ViewFrame::makePopUpMenu( IMenuEvent &event )
{
   // create the popup menu
   IPopUpMenu *menu = new IPopUpMenu( this, VIEW_MENU );

   // set the items in the popup menu and check it if necessary
   menu->   addText( VIEW_MENU+1, "testing" ).
            setAutoDeleteObject( true );

   // show the popup menu where the mouse is located
   menu->   show( event.mousePosition() -
                  viewPort.viewWindowDrawRectangle().bottomLeft() );
                  // note that we need to subtract the offset of the
                  // viewport rectangle, or else the menu will appear
                  // too far top-right when the viewport is not at (0,0)

   // return the event as having been handled
   event.setResult( true );
   return true;
}


void ViewFrame::resetVariables()
{
   return;
}


void ViewFrame::readSettings( IProfile &profile )
{
   // assume the profile has already been opened
   // process the settings
/*
   if( enableCBx.isSelected() != profile.integerWithKey( "crop_cb" ) )
   {
      enableCBx.click();
   }
*/
   return;
}


void ViewFrame::saveSettings( IProfile &profile )
{
   // assume the profile has already been opened
   // save the settings
/*
   profile.addOrReplaceElementWithKey( "crop_cb", enableCBx.isSelected() );
*/
   return;
}

