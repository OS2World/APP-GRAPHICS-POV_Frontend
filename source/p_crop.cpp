// St‚phane Charette, charette@writeme.com
// Open Class Library POV Front-end Launch Tool
// Application & source code (c) St‚phane Charette


// required resource and class definitions

#include "p_crop.hpp"
#include "pov_launch.h"
#include "pov_launch.hpp"
#include "p_size.hpp"

#include <ipopmenu.hpp> // IPopUpMenu

#define P_CROP_XSTART_TXT  (P_CROP + 0x0001)
#define P_CROP_XSTART_SB   (P_CROP + 0x0002)
#define P_CROP_XEND_TXT    (P_CROP + 0x0003)
#define P_CROP_XEND_SB     (P_CROP + 0x0004)
#define P_CROP_YSTART_TXT  (P_CROP + 0x0005)
#define P_CROP_YSTART_SB   (P_CROP + 0x0006)
#define P_CROP_YEND_TXT    (P_CROP + 0x0007)
#define P_CROP_YEND_SB     (P_CROP + 0x0008)
#define P_CROP_ENABLE_CBX  (P_CROP + 0x0009)
#define P_CROP_MENU        (P_CROP + 0x000A)

P_Crop::P_Crop( IWindow *dlg ) :

   ISetCanvas( P_CROP, dlg, dlg, IRectangle(),
     (ISetCanvas::classDefaultStyle    |
      ISetCanvas::decksByGroup         |
      ISetCanvas::packEven)            &
     ~ISetCanvas::packTight            ),

   enableCBx( P_CROP_ENABLE_CBX, this, this, IRectangle(),
      ICheckBox::classDefaultStyle     |
      IControl::group                  |
      IControl::tabStop                )
{
   // initialize variables
   resetVariables();

   // force a groupbox around the canvas
   setText( "Crop" ).disableGroup();

   // set the enable/disable checkbox
   enableCBx.setText( "enable" );

   // setup handlers
   ISelectHandler::  handleEventsFor( &enableCBx );
   ICommandHandler:: handleEventsFor( this );
   IMenuHandler::    handleEventsFor( this );
   IMenuHandler::    handleEventsFor( &enableCBx );

   return;
}


P_Crop::~P_Crop()
{
   // stop handling events
   IMenuHandler::    stopHandlingEventsFor( &enableCBx );
   IMenuHandler::    stopHandlingEventsFor( this );
   ICommandHandler:: stopHandlingEventsFor( this );
   ISelectHandler::  stopHandlingEventsFor( &enableCBx );

   if( xStartTxt )
   {
      // delete the extra controls
      deleteExtraControls();
   }

   return;
}


Boolean P_Crop::lostFocus( IControlEvent &event )
{
   if( xStartSB->value() < 1 )
   {
      xStartSB->spinTo( 1 );
   }
   if( yStartSB->value() < 1 )
   {
      yStartSB->spinTo( 1 );
   }
   if( xEndSB->value() < 1 )
   {
      xEndSB->spinTo( 1 );
   }
   if( yEndSB->value() < 1 )
   {
      yEndSB->spinTo( 1 );
   }
   if( xEndSB->value() < xStartSB->value() )
   {
      // swap the xEnd and xStart values
      long value = xStartSB->value();
      xStartSB->spinTo( xEndSB->value() );
      xEndSB->spinTo( value );
   }

   if( yEndSB->value() < yStartSB->value() )
   {
      // swap the yEnd and yStart values
      long value = yStartSB->value();
      yStartSB->spinTo( yEndSB->value() );
      yEndSB->spinTo( value );
   }

   event.setResult( false );
   return false;
}


Boolean P_Crop::command( ICommandEvent &event )
{
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
   return false;
}


Boolean P_Crop::selected( IControlEvent &event )
{
   // find out if the checkbox is selected or not
   if( enableCBx.isSelected() )
   {
      showExtraControls();
   }
   else
   {
      deleteExtraControls();
   }

   event.setResult( false );
   return false;
}


Boolean P_Crop::makePopUpMenu( IMenuEvent &event )
{
   // create the popup menu
   IPopUpMenu *menu = new IPopUpMenu( this, P_CROP_MENU );

   // set the items in the popup menu and check it if necessary
   menu->   addText( P_CROP_ENABLE_CBX, "enable" ).
            checkItem( P_CROP_ENABLE_CBX, enableCBx.isSelected() ).
            setAutoDeleteObject( true );

   // show the popup menu where the mouse is located
   menu->   show( event.mousePosition() );

   // return the event as having been handled
   event.setResult( true );
   return true;
}


void P_Crop::showExtraControls()
{
   if( xStartTxt == 0 )
   {
      // create all of the necessary controls
      xStartTxt = new
         IStaticText( P_CROP_XSTART_TXT, this, this, IRectangle(),
        (IStaticText::classDefaultStyle   |
         IStaticText::bottom              |
         IControl::group)                 &
        ~IStaticText::top                 );

      xStartSB = new
         INumericSpinButton( P_CROP_XSTART_SB, this, this, IRectangle(),
         INumericSpinButton::classDefaultStyle |
         INumericSpinButton::fastSpin     |
         IControl::tabStop                );

      xEndTxt = new
         IStaticText( P_CROP_XEND_TXT, this, this, IRectangle(),
        (IStaticText::classDefaultStyle   |
         IStaticText::bottom)             &
        ~IStaticText::top                 );

      xEndSB = new
         INumericSpinButton( P_CROP_XEND_SB, this, this, IRectangle(),
         INumericSpinButton::classDefaultStyle |
         INumericSpinButton::fastSpin     |
         IControl::tabStop                );

      yStartTxt = new
         IStaticText( P_CROP_YSTART_TXT, this, this, IRectangle(),
        (IStaticText::classDefaultStyle   |
         IStaticText::bottom              |
         IControl::group)                 &
        ~IStaticText::top                 );

      yStartSB = new
         INumericSpinButton( P_CROP_YSTART_SB, this, this, IRectangle(),
         INumericSpinButton::classDefaultStyle |
         INumericSpinButton::fastSpin     |
         IControl::tabStop                );

      yEndTxt = new
         IStaticText( P_CROP_YEND_TXT, this, this, IRectangle(),
        (IStaticText::classDefaultStyle   |
         IStaticText::bottom)             &
        ~IStaticText::top                 );

      yEndSB = new
         INumericSpinButton( P_CROP_YEND_SB, this, this, IRectangle(),
         INumericSpinButton::classDefaultStyle |
         INumericSpinButton::fastSpin     |
         IControl::tabStop                );

      IMenuHandler::handleEventsFor( xStartTxt  );
      IMenuHandler::handleEventsFor( yStartTxt  );
      IMenuHandler::handleEventsFor( xEndTxt    );
      IMenuHandler::handleEventsFor( yEndTxt    );
      IFocusHandler::handleEventsFor( xStartSB  );
      IFocusHandler::handleEventsFor( yStartSB  );
      IFocusHandler::handleEventsFor( xEndSB    );
      IFocusHandler::handleEventsFor( yEndSB    );

      // set the initial crop size to cover the entire image
      P_Size *p_size = ((POV_LaunchDialog*)(owner()->owner()))->PSize;
      checkSize();
      xStartSB->  spinTo( 1               ).setLimit( 5 );
      yStartSB->  spinTo( 1               ).setLimit( 5 );
      xEndSB->    spinTo( p_size->width   ).setLimit( 5 );
      yEndSB->    spinTo( p_size->height  ).setLimit( 5 );

      // set the text prompt
      xStartTxt-> setText( "start X"   );
      yStartTxt-> setText( "start Y"   );
      xEndTxt->   setText( "end X"     );
      yEndTxt->   setText( "end Y"     );
   }

   return;
}


void P_Crop::deleteExtraControls()
{
   // delete all of the unused controls
   IMenuHandler::stopHandlingEventsFor( xStartTxt );
   IMenuHandler::stopHandlingEventsFor( yStartTxt );
   IMenuHandler::stopHandlingEventsFor( xEndTxt   );
   IMenuHandler::stopHandlingEventsFor( yEndTxt   );
   IFocusHandler::stopHandlingEventsFor( xStartSB  );
   IFocusHandler::stopHandlingEventsFor( yStartSB  );
   IFocusHandler::stopHandlingEventsFor( xEndSB    );
   IFocusHandler::stopHandlingEventsFor( yEndSB    );
   delete( xStartTxt );
   delete( yStartTxt );
   delete( xStartSB  );
   delete( yStartSB  );
   delete( xEndTxt   );
   delete( yEndTxt   );
   delete( xEndSB    );
   delete( yEndSB    );
   resetVariables();

   return;
}


void P_Crop::resetVariables()
{
   xStartTxt   = 0;
   xStartSB    = 0;
   xEndTxt     = 0;
   xEndSB      = 0;
   yStartTxt   = 0;
   yStartSB    = 0;
   yEndTxt     = 0;
   yEndSB      = 0;

   return;
}


void P_Crop::readSettings( IProfile &profile )
{
   // assume the profile has already been opened
   // process the settings
   if( enableCBx.isSelected() != profile.integerWithKey( "crop_cb" ) )
   {
      enableCBx.click();
   }
   if( enableCBx.isSelected() )
   {
      xStartSB->  spinTo( profile.integerWithKey( "crop_x1" ) );
      yStartSB->  spinTo( profile.integerWithKey( "crop_y1" ) );
      xEndSB->    spinTo( profile.integerWithKey( "crop_x2" ) );
      yEndSB->    spinTo( profile.integerWithKey( "crop_y2" ) );
   }

   return;
}


void P_Crop::saveSettings( IProfile &profile )
{
   // assume the profile has already been opened
   // save the settings
   profile.addOrReplaceElementWithKey( "crop_cb", enableCBx.isSelected() );
   if( enableCBx.isSelected() )
   {
      profile.addOrReplaceElementWithKey( "crop_x1", xStartSB->value() );
      profile.addOrReplaceElementWithKey( "crop_y1", yStartSB->value() );
      profile.addOrReplaceElementWithKey( "crop_x2", xEndSB->  value() );
      profile.addOrReplaceElementWithKey( "crop_y2", yEndSB->  value() );
   }
   return;
}

IString P_Crop::getOptions()
{
   // return the crop values (if enabled) as parameters to POVRAY
   IString options( "" );

   if( enableCBx.isSelected() )
   {
      options =   IString( " +SC"            ) +
                  IString( xStartSB->value() ) +
                  IString( " +EC"            ) +
                  IString( xEndSB->value()   ) +
                  IString( " +SR"            ) +
                  IString( yStartSB->value() ) +
                  IString( " +ER"            ) +
                  IString( yEndSB->value()   );
   }

   return options;
}


void P_Crop::checkSize()
{
   // is cropping even enabled?
   if( enableCBx.isSelected() )
   {
      // make certain that cropping is not beyond the scope of the image size
      P_Size *p_size = ((POV_LaunchDialog*)(owner()->owner()))->PSize;
      xStartSB->  setRange( IRange(1, p_size->width ) );
      yStartSB->  setRange( IRange(1, p_size->height) );
      xEndSB->    setRange( IRange(1, p_size->width ) );
      yEndSB->    setRange( IRange(1, p_size->height) );
   }
   return;
}

