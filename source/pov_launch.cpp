// St‚phane Charette, charette@writeme.com
// Open Class Library POV Front-end Launch Tool
// Application & source code (c) St‚phane Charette


// required resource and class definitions
#include "pov_launch.hpp"
#include "pov_launch.h"
#include "p_output.hpp"
#include "p_size.hpp"
#include "p_crop.hpp"
#include "p_file.hpp"
#include "p_finish.hpp"
#include "p_render.hpp"

#define P_LAUNCH_MENU         (POV_DIALOGCANVAS + 0x0001)
#define P_LAUNCH_MENU_SAVE    (POV_DIALOGCANVAS + 0x0002)
#define P_LAUNCH_SAVE_ON_EXIT (POV_DIALOGCANVAS + 0x0003)
#define P_LAUNCH_RELOAD       (POV_DIALOGCANVAS + 0x0004)
#define P_LAUNCH_MENU_ABOUT   (POV_DIALOGCANVAS + 0x0005)

// ocl includes
#include <iexcbase.hpp> // IException
#include <imsgbox.hpp>  // IMessageBox
#include <ipopmenu.hpp> // IPopUpMenu
#include <iprofile.hpp> // IProfile
#include <isysmenu.hpp> // ISystemMenu
#include <ititle.hpp>   // ITitle

// function prototypes
int main( void );


POV_LaunchDialog::POV_LaunchDialog() :

   IFrameWindow( (IFrameWindow::classDefaultStyle  |
                  IFrameWindow::alignNoAdjust      |
                  IFrameWindow::animated           |
                  IFrameWindow::minimizedIcon      |
                  IFrameWindow::dialogBackground)  &
                 ~IFrameWindow::maximizeButton     ,
                  IResourceId(POV_LAUNCHDIALOG)    ),

   dialogCanvas(  POV_DIALOGCANVAS, this, this, IRectangle(),
                  ISetCanvas::classDefaultStyle    |
                  ISetCanvas::decksByGroup         )
{
   // initialize variables
   saveSettingsOnExit = false;

   // set some default static styles
   IStaticText::setDefaultStyle(   (IStaticText::classDefaultStyle   |
                                    IStaticText::vertCenter)         &
                                   ~IStaticText::top                 );

   // create the different canvases
   PFile    = new P_File(     &dialogCanvas );
   POutput  = new P_Output(   &dialogCanvas );
   PSize    = new P_Size(     &dialogCanvas );
   PCrop    = new P_Crop(     &dialogCanvas );
   PFinish  = new P_Finish(   &dialogCanvas );
   PRender  = new P_Render(   &dialogCanvas );

   // set the application's title
   ITitle title( this, APP_NAME, POV_LAUNCHDIALOG_TITLE );

   // acknowledge the multi-cell canvas as the main client
   setClient( &dialogCanvas );

   // setup handlers
   ICommandHandler:: handleEventsFor( &dialogCanvas );
   IMenuHandler::    handleEventsFor( &dialogCanvas );

   readSettings();

   // show the window and set the input focus
   show().setFocus();

   return;
}


POV_LaunchDialog::~POV_LaunchDialog()
{
   // stop handlers
   IMenuHandler::    stopHandlingEventsFor( &dialogCanvas );
   ICommandHandler:: stopHandlingEventsFor( &dialogCanvas );
   delete( PCrop );
   delete( PSize );
   delete( POutput );
   delete( PFile );
   delete( PFinish );
   delete( PRender );
   return;
}


Boolean POV_LaunchDialog::command( ICommandEvent &event )
{
   switch( event.commandId() )
   {
      case P_LAUNCH_MENU_SAVE:
      {
         // user has selected to save settings
         saveSettings();
         return true;
         break;
      }
      case P_LAUNCH_MENU_ABOUT:
      {
         IMessageBox aboutMessage( this );
         aboutMessage.setTitle( APP_NAME );
         // show the message box
         aboutMessage.
            show( IString( "This copy of "      ) +
                  IString( APP_NAME             ) +
                  IString( " is at version "    ) +
                  IString( APP_VERSION          ) +
                  IString( ", dated "           ) +
                  IString( APP_DATE             ) +
                  IString( ".\n\n(C) 1997, all rights reserved by St‚phane "
                           "Charette.\nWritten using IBM's Open Class "
                           "Library for OS/2.\n\nFor additional information, "
                           "please contact the author at "
                           "\"charette@writeme.com\"." ),
                  IMessageBox::applicationModal |
                  IMessageBox::defButton1       |
                  IMessageBox::informationIcon  |
                  IMessageBox::moveable         |
                  IMessageBox::okButton         );
         return true;
         break;
      }
      case P_LAUNCH_RELOAD:
      {
         // reload saved settings
         readSettings();
         return true;
         break;
      }
      case P_LAUNCH_SAVE_ON_EXIT:
      {
         // toggle the "save on settings" flag
         saveSettingsOnExit = saveSettingsOnExit ? false : true;
         return true;
         break;
      }
   }
   return false;
}


Boolean POV_LaunchDialog::systemCommand( ICommandEvent &event )
{
   // is this a WM_CLOSE event?
   if( event.commandId() == ISystemMenu::idClose )
   {
      // has the user selected to save the settings on close?
      if( saveSettingsOnExit )
      {
         // ...then save the settings!
         saveSettings();
      }
   }
   // return the event as if it has not been handled
   return false;
}


Boolean POV_LaunchDialog::makePopUpMenu( IMenuEvent &event )
{
   // create the popup menu
   IPopUpMenu *menu = new IPopUpMenu( this, P_LAUNCH_MENU );

   // add the menu item
   menu->addText( P_LAUNCH_MENU_ABOUT, "about" ).
         addSeparator().
         addText( P_LAUNCH_MENU_SAVE, "save settings" ).
         addText( P_LAUNCH_RELOAD, "reload settings" ).
         addSeparator().
         addText( P_LAUNCH_SAVE_ON_EXIT, "save on exit" ).
         checkItem( P_LAUNCH_SAVE_ON_EXIT, saveSettingsOnExit );


   // show the menu and set it up to be automatically deleted
   menu->show( event.mousePosition() ).
         setAutoDeleteObject( true );

   // return the event as having been handled
   event.setResult( true );
   return true;
}


void POV_LaunchDialog::readSettings()
{
   // open the profile in which the settings are saved
   IProfile profile( IProfile::userProfile() );
   if( profile.containsApplication( APP_NAME ) )
   {
      profile.setDefaultApplicationName( APP_NAME );
      if( profile.elementWithKey( "version" ) != APP_VERSION )
      {
         // an incorrect version was detected -- process anyway?
         // create a warning message box
         IMessageBox warningMessage( this );
         warningMessage.setTitle( APP_NAME );
         // show the message box
         if( warningMessage.
            show( IString( "The settings found are for version "           ) +
                  IString( profile.elementWithKey( "version" )             ) +
                  IString( " while this application is at version "        ) +
                  IString( APP_VERSION                                     ) +
                  IString( ".  Do you wish to ignore these settings and "  ) +
                  IString( "use default values instead?"                   ),
                  IMessageBox::applicationModal |
                  IMessageBox::defButton1       |
                  IMessageBox::warningIcon      |
                  IMessageBox::moveable         |
                  IMessageBox::yesNoButton      ) == IMessageBox::yes )
         {
            // the user selected to ignore the saved settings and use defaults
            return;
         }
      }
      // process the settings file
      moveSizeTo( IRectangle( profile.integerWithKey( "frame_x1" ),
                              profile.integerWithKey( "frame_y1" ),
                              profile.integerWithKey( "frame_x2" ),
                              profile.integerWithKey( "frame_y2" ) ) );
      saveSettingsOnExit =    profile.integerWithKey( "saveOnExit" );

      PSize->  readSettings( profile );
      POutput->readSettings( profile );
      PCrop->  readSettings( profile );
      PFile->  readSettings( profile );
      PFinish->readSettings( profile );
      PRender->readSettings( profile );
   }
   else
   {
      // profile doesn't exist yet, so create it
      saveSettings();
   }

   return;
}


void POV_LaunchDialog::saveSettings()
{
   // open the profile in which the settings are saved
   IProfile profile( IProfile::userProfile() );
   profile.setDefaultApplicationName( APP_NAME );
   profile.addOrReplaceElementWithKey( "version", IString( APP_VERSION ) );
   // save the main window size
   profile.addOrReplaceElementWithKey( "frame_x1", rect().minX() );
   profile.addOrReplaceElementWithKey( "frame_y1", rect().minY() );
   profile.addOrReplaceElementWithKey( "frame_x2", rect().maxX() );
   profile.addOrReplaceElementWithKey( "frame_y2", rect().maxY() );
   profile.addOrReplaceElementWithKey( "saveOnExit", saveSettingsOnExit );

   PSize->  saveSettings( profile );
   POutput->saveSettings( profile );
   PCrop->  saveSettings( profile );
   PFile->  saveSettings( profile );
   PFinish->saveSettings( profile );
   PRender->saveSettings( profile );

   return;
}


int main()
{
   try
   {
      // start the frame class & application
      POV_LaunchDialog *dialog;
      dialog = new POV_LaunchDialog();
      // run until the frame is closed
      IApplication::current().run();
      // free resources
      delete( dialog );
   }
   catch( IException &exception )
   {
      // get the location of the exception
      IString location( "" );
      for( int i=exception.locationCount(); i>0; i-- )
      {
         location += IString( "\n(" ) +
                     IString( exception.locationAtIndex(i-1)->fileName() ) +
                     IString( ", " ) +
                     IString( exception.locationAtIndex(i-1)->functionName() )+
                     IString( ", " ) +
                     IString( exception.locationAtIndex(i-1)->lineNumber() )+
                     IString( ")" );
      }

      IMessageBox exceptionError( IWindow::desktopWindow() );
      exceptionError.setTitle( APP_NAME );
      // show the message box
      exceptionError.show( IString( exception.isRecoverable() ?
                                "A recoverable \"" : "An unrecoverable \"" ) +
                           IString( exception.name()                       ) +
                           IString( "\" exception with an error id of "    ) +
                           IString( exception.errorId()                    ) +
                           IString( " was caught in \""                    ) +
                           IString( exception.errorCodeGroup()             ) +
                           IString( ".\"\n\nThe error text states \""      ) +
                           IString( exception.text()                       ) +
                           IString( "\"\n\nLocations: ("                   ) +
                           IString( exception.locationCount()              ) +
                           IString( ")"                                    ) +
                           location,
                           IMessageBox::systemModal                        |
                           IMessageBox::defButton1                         |
                           IMessageBox::errorIcon                          |
                           IMessageBox::moveable                           |
                           IMessageBox::okButton                           );
   }
   return 0;
}

