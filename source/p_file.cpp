// St‚phane Charette, charette@writeme.com
// Open Class Library POV Front-end Launch Tool
// Application & source code (c) St‚phane Charette


// required resource and class definitions
#include "p_file.hpp"
#include "pov_launch.h"
#include "pov_launch.hpp"
#include "p_output.hpp"
#include "p_render.hpp"
#include "view.hpp"

#include <ifiledlg.hpp> // IFileDialog
#include <imsgbox.hpp>  // IMessageBox
#include <ipopmenu.hpp> // IPopUpMenu
#include <ithread.hpp>  // ICurrentThread

#define INCL_WIN
#define INCL_PM
#include <os2.h>
#define P_FILE_INPUT_TXT   (P_FILE + 0x0001)
#define P_FILE_INPUT_CB    (P_FILE + 0x0002)
#define P_FILE_EDIT_PB     (P_FILE + 0x0003)
#define P_FILE_OUTPUT_TXT  (P_FILE + 0x0004)
#define P_FILE_OUTPUT_CB   (P_FILE + 0x0005)
#define P_FILE_VIEW_PB     (P_FILE + 0x0006)
#define P_FILE_MENU        (P_FILE + 0x0007)
#define P_FILE_EDIT_MENU   (P_FILE + 0x0008)
#define P_FILE_VIEW_MENU   (P_FILE + 0x0009)
#define P_FILE_EDIT_SET    (P_FILE + 0x000A)
#define P_FILE_VIEW_SET    (P_FILE + 0x000B)
#define P_FILE_VIEW_INT    (P_FILE + 0x000C)
#define P_FILE_EDIT_FIND   (P_FILE + 0x000D)

P_File::P_File( IWindow *dlg ) :

   ISetCanvas( P_FILE, dlg, dlg, IRectangle(),
     (ISetCanvas::classDefaultStyle    |
      ISetCanvas::decksByGroup         |
      ISetCanvas::packEven)            &
     ~ISetCanvas::packTight            ),

   inputTxt( P_FILE_INPUT_TXT, this, this, IRectangle(),
     (IStaticText::classDefaultStyle   |
      IStaticText::vertCenter          |
      IControl::group)                 &
     ~IStaticText::top                 ),

   inputCB( P_FILE_INPUT_CB, this, this, IRectangle(),
     (IComboBox::classDefaultStyle     |
      IComboBox::dropDownType          |
      IControl::tabStop)               &
     ~IComboBox::simpleType            ),

   editPB( P_FILE_EDIT_PB, this, this, IRectangle(),
      IPushButton::classDefaultStyle   |
      IControl::tabStop                ),

   outputTxt( P_FILE_OUTPUT_TXT, this, this, IRectangle(),
     (IStaticText::classDefaultStyle   |
      IStaticText::vertCenter          |
      IControl::group)                 &
     ~IStaticText::top                 ),

   outputCB( P_FILE_OUTPUT_CB, this, this, IRectangle(),
     (IComboBox::classDefaultStyle     |
      IComboBox::dropDownType          |
      IControl::tabStop)               &
     ~IComboBox::simpleType            ),

   viewPB( P_FILE_VIEW_PB, this, this, IRectangle(),
      IPushButton::classDefaultStyle   |
      IControl::tabStop                )
{
   // force a groupbox around the canvas
   setText( "File" ).enableGroup();

   // set the input filename prompt and combobox
   inputTxt.setText( "input" );
   inputCB.setMinimumRows( 5 );

   // set the output filename prompt and combobox
   outputTxt.setText( "output" );
   outputCB.setMinimumRows( 5 );

   // set the 2 pushbuttons
   editPB.setText( "~Edit" );
   viewPB.setText( "~View" );

   useInternalViewer = true;
   editor = IString( "e.exe" );
   viewer = IString( "pmview.exe" );

   // start handling events
   ICommandHandler::handleEventsFor( this );
   IMenuHandler::handleEventsFor( this );
   IMenuHandler::handleEventsFor( &inputTxt );
   IMenuHandler::handleEventsFor( &inputCB );
   IMenuHandler::handleEventsFor( &editPB );
   IMenuHandler::handleEventsFor( &outputTxt );
   IMenuHandler::handleEventsFor( &outputCB );
   IMenuHandler::handleEventsFor( &viewPB );
   IFocusHandler::handleEventsFor( &inputCB );
   IFocusHandler::handleEventsFor( &outputCB );

   return;
}


P_File::~P_File()
{
   // stop handling events
   IFocusHandler::stopHandlingEventsFor( &outputCB );
   IFocusHandler::stopHandlingEventsFor( &inputCB );
   IMenuHandler::stopHandlingEventsFor( &viewPB );
   IMenuHandler::stopHandlingEventsFor( &outputCB );
   IMenuHandler::stopHandlingEventsFor( &outputTxt );
   IMenuHandler::stopHandlingEventsFor( &editPB );
   IMenuHandler::stopHandlingEventsFor( &inputCB );
   IMenuHandler::stopHandlingEventsFor( &inputTxt );
   IMenuHandler::stopHandlingEventsFor( this );
   ICommandHandler::stopHandlingEventsFor( this );

   return;
}


Boolean P_File::lostFocus( IControlEvent &event )
{
   // update the name of the output file with the new extension
   updateOutputFilename();

   event.setResult( false );
   return false;
}


Boolean P_File::command( ICommandEvent &event )
{
   switch( event.commandId() )
   {
      case P_FILE_EDIT_PB:
      {
         // start the text editor
         PROGDETAILS details;
         details.Length           = sizeof( PROGDETAILS );
         details.progt.progc      = PROG_DEFAULT;
         details.progt.fbVisible  = SHE_VISIBLE | SHE_PROTECTED;
         details.pszTitle         = (char*)(IString(APP_NAME) + IString(" "));
         details.pszExecutable    = (char*)editor;
         details.pszParameters    = (char*)inputCB.text();
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
            // error while starting text editor!
            ERRORID error = WinGetLastError(IThread::current().anchorBlock());
            // display the error message
            IMessageBox errorMessage = IMessageBox( this );
            errorMessage.  setTitle( APP_NAME ).
                           show( IString( "WinStartApp returned error #"   ) +
                                 IString( error                            ) +
                                 IString( " while opening the editor \""   ) +
                                 IString( editor                           ) +
                                 IString( "\"."                            ),
                                 IMessageBox::applicationModal |
                                 IMessageBox::defButton1       |
                                 IMessageBox::errorIcon        |
                                 IMessageBox::moveable         |
                                 IMessageBox::okButton         );
         }
         return true;
         break;
      }
      case P_FILE_EDIT_FIND:
      {
         // set the basic settings for the file dialog
         IFileDialog::Settings fileSettings = IFileDialog::Settings();
         fileSettings.
            setTitle( IString(APP_NAME) + IString(" - Select Input File") ).
            setFileName( inputCB.text()==""? "*.POV" : (char*)inputCB.text() ).
            setOpenDialog();
         // create and show the file dialog
         IFileDialog fileDialog = IFileDialog(
            IWindow::desktopWindow(),
            this,
            fileSettings );
         // see if we've decided to set a new file
         if( fileDialog.pressedOK() )
         {
            inputCB.setText( fileDialog.fileName() );
            updateOutputFilename();
         }
         return true;
         break;
      }
      case P_FILE_EDIT_SET:
      {
         // set the basic settings for the file dialog
         IFileDialog::Settings fileSettings = IFileDialog::Settings();
         fileSettings.  setTitle( IString( APP_NAME ) + IString( " - Select Editor To Use" ) ).
                        setOKButtonText( "Set" ).
                        setFileName( editor ).
                        setOpenDialog();
         // create and show the file dialog
         IFileDialog fileDialog = IFileDialog(
            IWindow::desktopWindow(),
            this,
            fileSettings );
         // see if we've decided to set a new editor
         if( fileDialog.pressedOK() )
         {
            editor = fileDialog.fileName();
         }
         return true;
         break;
      }
      case P_FILE_VIEW_PB:
      {
         // start the image viewer
         if( useInternalViewer )
         {
            try
            {
               // user has chosen to use the internal viewer
               ViewFrame *view = new ViewFrame( owner()->owner(),
                                                outputCB.text() );
            }
            catch( IAccessError &exception )
            {
               // error starting image viewer -- assume the file doesn't exist
               // display the error message
               IMessageBox errorMessage = IMessageBox( this );
               errorMessage.  setTitle( APP_NAME ).
                              show( "The image file specified either could "
                                    "not be found or is not supported in the "
                                    "internal viewer.  Only OS/2 bitmaps and "
                                    "uncompressed Targa files are supported.",
                                    IMessageBox::applicationModal |
                                    IMessageBox::defButton1       |
                                    IMessageBox::informationIcon  |
                                    IMessageBox::moveable         |
                                    IMessageBox::okButton         );
            }
            return true;
            break;
         }

         // if the code gets here, then we're starting an external viewer

         PROGDETAILS details;
         details.Length           = sizeof( PROGDETAILS );
         details.progt.progc      = PROG_DEFAULT;
         details.progt.fbVisible  = SHE_VISIBLE | SHE_PROTECTED;
         details.pszTitle         = (char*)(IString(APP_NAME) + IString(" "));
         details.pszExecutable    = (char*)viewer;
         details.pszParameters    = (char*)(IString(" ") + outputCB.text());
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
            // error while starting image viewer!
            ERRORID error = WinGetLastError(IThread::current().anchorBlock());
            // display the error message
            IMessageBox errorMessage = IMessageBox( this );
            errorMessage.  setTitle( APP_NAME ).
                           show( IString( "WinStartApp returned error #"   ) +
                                 IString( error                            ) +
                                 IString( " while opening the viewer \""   ) +
                                 IString( viewer                           ) +
                                 IString( "\"."                            ),
                                 IMessageBox::applicationModal |
                                 IMessageBox::defButton1       |
                                 IMessageBox::errorIcon        |
                                 IMessageBox::moveable         |
                                 IMessageBox::okButton         );
         }
         return true;
         break;
      }
      case P_FILE_VIEW_INT:
      {
         // toggle the flag for "use internal viewer"
         useInternalViewer = useInternalViewer ? false : true;
         return true;
         break;
      }
      case P_FILE_VIEW_SET:
      {
         // set the basic settings for the file dialog
         IFileDialog::Settings fileSettings = IFileDialog::Settings();
         fileSettings.  setTitle( IString( APP_NAME ) + IString( " - Select Image Viewer To Use" ) ).
                        setOKButtonText( "Set" ).
                        setFileName( viewer ).
                        setOpenDialog();
         // create and show the file dialog
         IFileDialog fileDialog = IFileDialog(
            IWindow::desktopWindow(),
            this,
            fileSettings );
         // see if we've decided to set a new viewer
         if( fileDialog.pressedOK() )
         {
            viewer = fileDialog.fileName();
         }
         return true;
         break;
      }
   }

   return false;
}


Boolean P_File::makePopUpMenu( IMenuEvent &event )
{
   // create the popup menu
   IPopUpMenu *menu = new IPopUpMenu( this, P_FILE_MENU );

   menu->addText(    P_FILE_EDIT_MENU, "edit file"                     ).
         addSubmenu( P_FILE_EDIT_MENU                                  ).
         addText(    P_FILE_EDIT_PB,   "open",        P_FILE_EDIT_MENU ).
         addText(    P_FILE_EDIT_FIND, "find file",   P_FILE_EDIT_MENU ).
         addText(    P_FILE_EDIT_SET,  "set editor",  P_FILE_EDIT_MENU ).
         setConditionalCascade( P_FILE_EDIT_MENU,
            inputCB.text()=="" ? P_FILE_EDIT_FIND : P_FILE_EDIT_PB     ).
         addSeparator().
         addText(    P_FILE_VIEW_MENU, "view image "                   ).
         addSubmenu( P_FILE_VIEW_MENU                                  ).
         addText(    P_FILE_VIEW_PB,   "open image",  P_FILE_VIEW_MENU ).
         addText(    P_FILE_VIEW_SET,  "set viewer",  P_FILE_VIEW_MENU ).
         addSeparator( P_FILE_VIEW_MENU                                ).
         addText(    P_FILE_VIEW_INT, "use internal viewer", P_FILE_VIEW_MENU).
         checkItem(  P_FILE_VIEW_INT, useInternalViewer                ).
         setConditionalCascade( P_FILE_VIEW_MENU,     P_FILE_VIEW_PB   );

   // show the popup menu where the mouse is located
   menu->   show( event.mousePosition() ).
            setAutoDeleteObject( true );

   // return the event as having been handled
   event.setResult( true );
   return true;
}


void P_File::updateOutputFilename()
{
   if( outputCB.text() == "" )
   {
      // if the output field is empty, default to the same as the input field
      outputCB.setText( inputCB.text() );
   }

   IString outputName = outputCB.text();
   if( outputName != "" )
   {
      int offsetOfPeriod = outputName.lastIndexOf( '.' );
      int length         = outputName.length();
      IString name;
      // if the name does not have a period, then copy all of it and append "."
      if( offsetOfPeriod == 0 )
      {
         name = outputName + IString( "." );
      }
      else
      {
         // find out what is the name
         name = outputName.subString( 1, offsetOfPeriod );
      }
      // what file format are we using?
      IString extension = ((POV_LaunchDialog*)(owner()->owner()))->
         POutput->getOutputExtension();
      // set the new filename
      outputCB.setText( name + extension );
   }

   // validate the render button
   Boolean state = (outputCB.text() != "") && (inputCB.text() != "" );
   ((POV_LaunchDialog*)(owner()->owner()))->PRender->
      startPB.enableDefault( state ).enable( state );

   return;
}


void P_File::readSettings( IProfile &profile )
{
   // assume the profile has already been opened
   // process the settings
   editor = IString( profile.elementWithKey( "file_editor" ) );
   viewer = IString( profile.elementWithKey( "file_viewer" ) );
   useInternalViewer = profile.integerWithKey( "file_intViewer" );
   return;
}


void P_File::saveSettings( IProfile &profile )
{
   // assume the profile has already been opened
   // save the settings for this canvas
   profile.addOrReplaceElementWithKey( "file_editor", (char*)editor );
   profile.addOrReplaceElementWithKey( "file_viewer", (char*)viewer );
   profile.addOrReplaceElementWithKey( "file_intViewer", useInternalViewer );
   return;
}

IString P_File::getOptions()
{
   // return the filenames as parameters to POVRAY
   IString options = IString( " +i"             ) +
                     IString( inputCB.text()    ) +
                     IString( " +o"             ) +
                     IString( outputCB.text()   );

   return options;
}

