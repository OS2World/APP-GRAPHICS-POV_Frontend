// St‚phane Charette, charette@writeme.com
// Open Class Library POV Front-end Launch Tool
// Application & source code (c) St‚phane Charette


// required resource and class definitions
#include "P_Render.hpp"
#include "pov_launch.h"

#include <ifiledlg.hpp> // IFileDialog
#include <imsgbox.hpp>  // IMessageBox
#include <ipopmenu.hpp> // IPopUpMenu
#include <ithread.hpp>  // IThread

#include "pov_launch.hpp"
#include "p_file.hpp"
#include "p_output.hpp"
#include "p_size.hpp"
#include "p_crop.hpp"
#include "p_finish.hpp"

#define INCL_WIN
#define INCL_PM
#include <os2.h>

#define P_RENDER_MENU         (P_RENDER + 0x0001)
#define P_RENDER_START_PB     (P_RENDER + 0x0002)
#define P_RENDER_SET_POV      (P_RENDER + 0x0003)
#define P_RENDER_SHOW_OPTIONS (P_RENDER + 0x0004)

P_Render::P_Render( IWindow *dlg ) :

   ISetCanvas( P_RENDER, dlg, dlg, IRectangle(),
     (ISetCanvas::classDefaultStyle    |
      ISetCanvas::decksByGroup         |
      ISetCanvas::packEven)            &
     ~ISetCanvas::packTight            ),

   startPB( P_RENDER_START_PB, this, this, IRectangle(),
      IPushButton::classDefaultStyle   |
      IPushButton::disabled            |
      IControl::tabStop                ),

   renderer( "POVRAY.EXE" )
{

   // force a groupbox around the canvas
   setText( "Render" ).disableGroup();

   // set the pushbutton text
   startPB.disableDefault().setText( "Start ~Rendering" );

   // setup the handler
   ICommandHandler:: handleEventsFor( this );
   IMenuHandler::    handleEventsFor( this );
   IMenuHandler::    handleEventsFor( &startPB );

   return;
}


P_Render::~P_Render()
{
   // stop handling events
   IMenuHandler::    stopHandlingEventsFor( &startPB );
   IMenuHandler::    stopHandlingEventsFor( this );
   ICommandHandler:: stopHandlingEventsFor( this );

   return;
}


Boolean P_Render::command( ICommandEvent &event )
{
   // handle command events
   switch( event.commandId() )
   {
      case P_RENDER_SET_POV:
      {
         // set the basic settings for the file dialog
         IFileDialog::Settings fileSettings = IFileDialog::Settings();
         fileSettings.
            setTitle( IString(APP_NAME) + IString(" - Set POVRAY Executable") ).
            setOKButtonText( "Set" ).
            setFileName( renderer ).
            setOpenDialog();
         // create and show the file dialog
         IFileDialog fileDialog = IFileDialog(
            IWindow::desktopWindow(),
            this,
            fileSettings );
         // see if we've decided to set a new file
         if( fileDialog.pressedOK() )
         {
            renderer = fileDialog.fileName();
         }
         return true;
         break;
      }
      case P_RENDER_START_PB:
      {
         // get the options selected
         POV_LaunchDialog *dialog = (POV_LaunchDialog*)(owner()->owner());
         IString optionsString = dialog->PFile  ->getOptions() +
                                 dialog->POutput->getOptions() +
                                 dialog->PSize  ->getOptions() +
                                 dialog->PCrop  ->getOptions() +
                                 dialog->PFinish->getOptions();


         // start the rendering tool
         PROGDETAILS details;
         details.Length           = sizeof( PROGDETAILS );
         details.progt.progc      = PROG_DEFAULT;
         details.progt.fbVisible  = SHE_VISIBLE | SHE_PROTECTED;
         details.pszTitle         = (char*)(IString(APP_NAME) + IString(" "));
         details.pszExecutable    = (char*)renderer;
         details.pszParameters    = (char*)optionsString;
         details.pszStartupDir    = "";
         details.pszIcon          = NULL;
         details.pszEnvironment   = "\0\0";
         details.swpInitial.fl    = SWP_ACTIVATE | SWP_SHOW;
         details.swpInitial.cy    = 0;
         details.swpInitial.cx    = 0;
         details.swpInitial.y     = 0;
         details.swpInitial.x     = 0;
         details.swpInitial.hwndInsertBehind = HWND_TOP;
         details.swpInitial.hwnd  = NULLHANDLE;
         details.swpInitial.ulReserved1 = 0;
         details.swpInitial.ulReserved2 = 0;
         if( WinStartApp(  NULLHANDLE,    // notify window on terminate
                           &details,      // details structure from above
                           NULL,          // parameters to use
                           NULL,          // reserved
                           SAF_INSTALLEDCMDLINE ) == NULL )
         {
            // error while starting renderer!
            ERRORID error = WinGetLastError(IThread::current().anchorBlock());
            // display the error message
            IMessageBox errorMessage = IMessageBox( this );
            errorMessage.  setTitle( APP_NAME ).
                           show( IString( "WinStartApp returned error #"   ) +
                                 IString( error                            ) +
                                 IString( " while starting \""             ) +
                                 IString( renderer                         ) +
                                 IString( "\" with these options: \""      ) +
                                 IString( optionsString                    ) +
                                 IString( "\"."                            ),
                                 IMessageBox::applicationModal |
                                 IMessageBox::defButton1       |
                                 IMessageBox::errorIcon        |
                                 IMessageBox::moveable         |
                                 IMessageBox::okButton         );
         }
         // no matter what happened, return indicating this event was handled
         return true;
         break;
      }
   }

   return false;
}


Boolean P_Render::makePopUpMenu( IMenuEvent &event )
{
   // create the popup menu
   IPopUpMenu *menu = new IPopUpMenu( this, P_RENDER_MENU );

   menu->addText( P_RENDER_SET_POV, "set POVRAY executable" ).
         addSeparator().
         addText( P_RENDER_SHOW_OPTIONS, "show additional options" ).
         disableItem( P_RENDER_SHOW_OPTIONS );

   // show the popup menu where the mouse is located
   menu->show( event.mousePosition() ).
         setAutoDeleteObject( true );

   // return the event as having been handled
   event.setResult( true );
   return true;
}


void P_Render::readSettings( IProfile &profile )
{
   // assume the profile has already been opened
   // process the settings
   renderer = profile.elementWithKey( "render_executable" );

   return;
}


void P_Render::saveSettings( IProfile &profile )
{
   // assume the profile has already been opened
   // save the settings
   profile.addOrReplaceElementWithKey( "render_executable", renderer );

   return;
}

