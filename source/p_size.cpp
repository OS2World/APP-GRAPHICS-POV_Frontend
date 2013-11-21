// St‚phane Charette, charette@writeme.com
// Open Class Library POV Front-end Launch Tool
// Application & source code (c) St‚phane Charette


// ocl includes
#include <ipopmenu.hpp> // IPopUpMenu
#include <istparse.hpp> // IStringParser
#include <istring.hpp>  // IString

// required resource and class definitions
#include "p_size.hpp"
#include "pov_launch.h"
#include "pov_launch.hpp"
#include "p_crop.hpp"
#define P_SIZE_RES_TXT     (P_SIZE + 0x0001)
#define P_SIZE_RES_CB      (P_SIZE + 0x0002)
#define P_SIZE_X_TXT       (P_SIZE + 0x0003)
#define P_SIZE_X_SB        (P_SIZE + 0x0004)
#define P_SIZE_Y_TXT       (P_SIZE + 0x0005)
#define P_SIZE_Y_SB        (P_SIZE + 0x0006)
#define P_SIZE_MENU        (P_SIZE + 0x0007)

P_Size::P_Size( IWindow *dlg ) :

   ISetCanvas( P_SIZE, dlg, dlg, IRectangle(),
     (ISetCanvas::classDefaultStyle    |
      ISetCanvas::decksByGroup         |
      ISetCanvas::packEven)            &
     ~ISetCanvas::packTight            ),

   resTxt( P_SIZE_RES_TXT, this, this, IRectangle(),
     (IStaticText::classDefaultStyle   |
      IStaticText::vertCenter          |
      IControl::group)                 &
     ~IStaticText::top                 ),

   resCB( P_SIZE_RES_CB,   this, this, IRectangle(),
     (IComboBox::classDefaultStyle     |
      IComboBox::readOnlyDropDownType  |
      IControl::tabStop)               &
     ~IComboBox::simpleType            )
{
   // initialize variables
   resetVariables();

   // force a groupbox around the canvas
   setText( "Size" );

   // set the main text prompt
   resTxt.setText(   "resolution" );

   // set the combobox options
   resCB.addAsLast(  "320x200" );
   resCB.addAsLast(  "640x400" );
   resCB.addAsLast(  "640x480" );
   resCB.addAsLast(  "800x600" );
   resCB.addAsLast(  "1024x768" );
   resCB.addAsLast(  "1280x1024" );
   resCB.addAsLast(  "1600x1200" );
   resCB.addAsLast(  "custom" );
   resCB.setMinimumRows( resCB.count() + 1 ).
         select( 0 ).
         setLimit( 10 );
   width  = 320;
   height = 200;

   // setup the handler
   ISelectHandler::  handleEventsFor( &resCB );
   ICommandHandler:: handleEventsFor( this );
   IMenuHandler::    handleEventsFor( this );
   IMenuHandler::    handleEventsFor( &resTxt );
   IMenuHandler::    handleEventsFor( &resCB );

   return;
}


P_Size::~P_Size()
{
   // stop handling events
   IMenuHandler::    stopHandlingEventsFor( &resCB );
   IMenuHandler::    stopHandlingEventsFor( &resTxt );
   IMenuHandler::    stopHandlingEventsFor( this );
   ICommandHandler:: stopHandlingEventsFor( this );
   ISelectHandler::  stopHandlingEventsFor( &resCB );

   if( xTxt )
   {
      deleteExtraControls();
   }
   return;
}


Boolean P_Size::command( ICommandEvent &event )
{
   // handle command events
   if( ( event.commandId() > P_SIZE_MENU ) &&
       ( event.commandId() < P_SIZE_MENU+resCB.count()+1 ) )
   {
      // select a resolution from the combobox
      resCB.select( event.commandId() - P_SIZE_MENU - 1 );
      return true;
   }
   return false;
}


Boolean P_Size::lostFocus( IControlEvent &event )
{
   // either the two custom size spinbuttons or the combo box has lost focus
   switch( event.controlId() )
   {
      case P_SIZE_X_SB:
      {
         width = xSB->value();
         break;
      }
      case P_SIZE_Y_SB:
      {
         height = ySB->value();
         break;
      }
   }

   // since the size of the image could have been changed, check on P_Crop
   ((POV_LaunchDialog*)(owner()->owner()))->PCrop->checkSize();

   event.setResult( false );
   return false;
}


Boolean P_Size::selected( IControlEvent &event )
{
   if( resCB.itemText( resCB.selection() ) == "custom" )
   {
      showExtraControls();
   }
   else
   {
      // delete the extra controls if they exist
      if( xTxt )
      {
         deleteExtraControls();
      }

      // remember the size that we just selected
      IString strX;
      IString strY;
      // parse the string
      resCB.itemText( resCB.selection() ) >> strX >> 'x' >> strY;
      // convert to integer
      width  = strX.asInt();
      height = strY.asInt();
   }

   // since the size of the image could have been changed, check on P_Crop
   ((POV_LaunchDialog*)(owner()->owner()))->PCrop->checkSize();

   event.setResult( false );
   return false;
}


Boolean P_Size::makePopUpMenu( IMenuEvent &event )
{
   // create the popup menu
   IPopUpMenu *menu = new IPopUpMenu( this, P_SIZE_MENU );

   // set the items in the popup menu from the text in the combobox
   for( int i=1; i <= resCB.count(); i++ )
   {
      menu->addText( P_SIZE_MENU + i, resCB.itemText( i - 1 ) );
      menu->checkItem( P_SIZE_MENU + i, resCB.selection()+1==i );
   }

   // show the popup menu where the mouse is located
   menu->   show( event.mousePosition() ).
            setAutoDeleteObject( true );

   // return the event as having been handled
   event.setResult( true );
   return true;
}


void P_Size::showExtraControls()
{
   if( xTxt == 0 )
   {
      // create new fields
      xTxt = new IStaticText( P_SIZE_X_TXT, this, this, IRectangle(),
        (IStaticText::classDefaultStyle         |
         IStaticText::bottom                    |
         IControl::group)                       &
        ~IStaticText::top                       );

      xSB  = new INumericSpinButton( P_SIZE_X_SB, this, this, IRectangle(),
         INumericSpinButton::classDefaultStyle  |
         INumericSpinButton::fastSpin           |
         IControl::tabStop                      );

      yTxt = new IStaticText( P_SIZE_Y_TXT,  this, this, IRectangle(),
        (IStaticText::classDefaultStyle         |
         IStaticText::bottom                    |
         IControl::group)                       &
        ~IStaticText::top                       );

      ySB  = new INumericSpinButton( P_SIZE_Y_SB, this, this, IRectangle(),
         INumericSpinButton::classDefaultStyle  |
         INumericSpinButton::fastSpin           |
         IControl::tabStop                      );

      IMenuHandler::handleEventsFor( xTxt );
      IMenuHandler::handleEventsFor( yTxt );
      IFocusHandler::handleEventsFor( xSB );
      IFocusHandler::handleEventsFor( ySB );

   }

   // set custom size fields
   xTxt->setText( "X" );
   yTxt->setText( "Y" );
   xSB-> setRange( IRange(1, 99999), false ).setLimit( 5 );
   ySB-> setRange( IRange(1, 99999), false ).setLimit( 5 );

   xSB-> spinTo( width );
   ySB-> spinTo( height );

   return;
}


void P_Size::deleteExtraControls()
{
   // delete all of the custom size controls
   IMenuHandler::stopHandlingEventsFor( xTxt );
   IMenuHandler::stopHandlingEventsFor( yTxt );
   IFocusHandler::stopHandlingEventsFor( xSB );
   IFocusHandler::stopHandlingEventsFor( ySB );
   delete( ySB );
   delete( xSB );
   delete( yTxt );
   delete( xTxt );
   resetVariables();

   return;
}


void P_Size::resetVariables()
{
   xTxt = 0;
   yTxt = 0;
   xSB = 0;
   ySB = 0;

   return;
}


void P_Size::readSettings( IProfile &profile )
{
   // assume the profile has already been opened
   // process the settings
   resCB.select(  profile.integerWithKey( "size_cb" ) );
   width =        profile.integerWithKey( "size_width" );
   height =       profile.integerWithKey( "size_height" );
   if( xTxt )
   {
      showExtraControls();
   }
   return;
}


void P_Size::saveSettings( IProfile &profile )
{
   // assume the profile has already been opened
   // save the value from the combo box
   profile.addOrReplaceElementWithKey( "size_cb", resCB.selection() );
   // save the custom X and Y settings
   profile.addOrReplaceElementWithKey( "size_width",  width );
   profile.addOrReplaceElementWithKey( "size_height", height );
   return;
}


IString P_Size::getOptions()
{
   // return the width and height as parameters to POVRAY
   IString options = IString( " +w"    ) +
                     IString( width    ) +
                     IString( " +h"    ) +
                     IString( height   );
   return options;
}

