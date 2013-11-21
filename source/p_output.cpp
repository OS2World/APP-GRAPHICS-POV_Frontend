// St‚phane Charette, charette@writeme.com
// Open Class Library POV Front-end Launch Tool
// Application & source code (c) St‚phane Charette


// required resource and class definitions
#include "p_output.hpp"
#include "pov_launch.h"
#include "pov_launch.hpp"
#include "p_file.hpp"

#include <ipopmenu.hpp> // IPopUpMenu

#define P_OUTPUT_FORMAT_TXT   (P_OUTPUT + 0x0001)
#define P_OUTPUT_FORMAT_CB    (P_OUTPUT + 0x0002)
#define P_OUTPUT_BUFFER_TXT   (P_OUTPUT + 0x0003)
#define P_OUTPUT_BUFFER_SB    (P_OUTPUT + 0x0004)
#define P_OUTPUT_COMPRESS_CBX (P_OUTPUT + 0x0005)
#define P_OUTPUT_MENU_SHOW    (P_OUTPUT + 0x0006)
#define P_OUTPUT_MENU         (P_OUTPUT + 0x0007)

P_Output::P_Output( IWindow *dlg ) :

   ISetCanvas( P_OUTPUT, dlg, dlg, IRectangle(),
     (ISetCanvas::classDefaultStyle    |
      ISetCanvas::decksByGroup         |
      ISetCanvas::packEven)            &
     ~ISetCanvas::packTight            ),

   bufferTxt( P_OUTPUT_BUFFER_TXT, this, this, IRectangle(),
     (IStaticText::classDefaultStyle   |
      IStaticText::bottom              |
      IControl::group)                 &
     ~IStaticText::top                 ),

   bufferSB( P_OUTPUT_BUFFER_SB, this, this, IRectangle(),
      INumericSpinButton::classDefaultStyle |
      INumericSpinButton::fastSpin     |
      IControl::tabStop                ),

   formatTxt( P_OUTPUT_FORMAT_TXT, this, this, IRectangle(),
     (IStaticText::classDefaultStyle   |
      IStaticText::vertCenter          |
      IControl::group)                 &
     ~IStaticText::top                 ),

   formatCB( P_OUTPUT_FORMAT_CB,   this, this, IRectangle(),
     (IComboBox::classDefaultStyle     |
      IComboBox::readOnlyDropDownType  |
      IControl::tabStop)               &
     ~IComboBox::simpleType            )
{
   // initialize variables
   resetVariables();

   // force a groupbox around the canvas
   setText( "Output" ).disableGroup();

   // set the buffer prompt and spinbutton
   bufferTxt.setText( "buffer" );
   bufferSB.setRange( IRange(0, 512), false ).setLimit( 3 );

   // set the output type text prompt
   formatTxt.setText(   "format" );

   // set the combobox options
   formatCB.addAsLast(  "Targa" );
   formatCB.addAsLast(  "PNG" );
   formatCB.addAsLast(  "PPM" );
   formatCB.addAsLast(  "BMP" );
   formatCB.setMinimumRows( formatCB.count() + 1 ).
            select( 0 ).
            setLimit( 6 );

   compress     = true;
   showCompress = false;

   // setup the handler
   ISelectHandler::  handleEventsFor( &formatCB );
   ICommandHandler:: handleEventsFor( this );
   IMenuHandler::    handleEventsFor( this );
   IMenuHandler::    handleEventsFor( &formatTxt );
   IMenuHandler::    handleEventsFor( &formatCB );

   return;
}


P_Output::~P_Output()
{
   // stop handling events
   IMenuHandler::    stopHandlingEventsFor( &formatCB );
   IMenuHandler::    stopHandlingEventsFor( &formatTxt );
   IMenuHandler::    stopHandlingEventsFor( this );
   ICommandHandler:: stopHandlingEventsFor( this );
   ISelectHandler::  stopHandlingEventsFor( &formatCB );

   if( compressCBx )
   {
      deleteExtraControls();
   }

   return;
}


Boolean P_Output::command( ICommandEvent &event )
{
   // handle command events
   if( ( event.commandId() > P_OUTPUT_MENU ) &&
       ( event.commandId() < P_OUTPUT_MENU+formatCB.count()+1 ) )
   {
      // select a format from the combobox
      formatCB.select( event.commandId() - P_OUTPUT_MENU - 1 );
      return true;
   }
   else switch( event.commandId() )
   {
      case P_OUTPUT_COMPRESS_CBX:
      {
         // toggle the state of the compress flag
         compress = compress ? false : true;
         // reset the checkbox if it is being shown
         if( compressCBx )
         {
            showExtraControls();
         }
         event.setResult( true );
         return true;
         break;
      }
      case P_OUTPUT_MENU_SHOW:
      {
         // toggle the state of the show flag
         showCompress = showCompress ? false : true;
         if( showCompress )
         {
            // show the compress control
            showExtraControls();
         }
         else
         {
            // delete the compress control
            deleteExtraControls();
         }
         event.setResult( true );
         return true;
         break;
      }
   }

   return false;
}


Boolean P_Output::selected( IControlEvent &event )
{
   switch( event.controlId() )
   {
      case P_OUTPUT_COMPRESS_CBX:
      {
         // toggle the state of the compress flag
         compress = compressCBx->isSelected();
         event.setResult( true );
         return true;
         break;
      }
      case P_OUTPUT_FORMAT_CB:
      {
         // enable checkbutton only if it is being shown and Targa is chosen
         if( showCompress )
         {
            compressCBx->enable(
               formatCB.itemText( formatCB.selection() ) == "Targa" );
         }
         // update the name of the output file with the new extension
         ((POV_LaunchDialog*)(owner()->owner()))->
            PFile->updateOutputFilename();
         // set the event as having been handled
         event.setResult( true );
         return true;
         break;
      }

   }

   event.setResult( false );
   return false;
}


Boolean P_Output::makePopUpMenu( IMenuEvent &event )
{
   // create the popup menu
   IPopUpMenu *menu = new IPopUpMenu( this, P_OUTPUT_MENU );

   // set the items in the popup menu from the text in the combobox
   for( int i=1; i <= formatCB.count(); i++ )
   {
      int menuId = P_OUTPUT_MENU + i;

      menu->addText(    menuId, formatCB.itemText( i - 1 )  ).
            checkItem(  menuId, formatCB.selection()+1==i   );

      // if this item supports a submenu, then create it
      if( ( formatCB.itemText( i - 1 ) == "Targa" ) &&
          ( menu->isItemChecked( menuId ) ) )
      {
         menu->   addSubmenu(                                     menuId   ).
                  addText(    P_OUTPUT_COMPRESS_CBX, "compress",  menuId   ).
                  checkItem(  P_OUTPUT_COMPRESS_CBX,  compress             ).
                  addSeparator(                                   menuId   ).
                  addText(       P_OUTPUT_MENU_SHOW,  "show",     menuId   ).
                  checkItem(     P_OUTPUT_MENU_SHOW,  showCompress         );
      }
   }


   // show the popup menu where the mouse is located
   menu->   show( event.mousePosition() ).
            setAutoDeleteObject( true );

   // return the event as having been handled
   event.setResult( true );
   return true;
}


void P_Output::showExtraControls()
{
   // create extra checkbox control
   if( compressCBx == 0 )
   {
      compressCBx = new
         ICheckBox( P_OUTPUT_COMPRESS_CBX, this, this, IRectangle(),
         ICheckBox::classDefaultStyle     |
         IControl::group                  |
         IControl::tabStop                );

      compressCBx->setText( "compress" );
      // handle events for the checkbox to detect when compress is toggled
      ISelectHandler::handleEventsFor( compressCBx );
      IMenuHandler::handleEventsFor( compressCBx );
   }

   // set the state of the checkbox
   compressCBx->select( compress );
   compressCBx->enable( formatCB.itemText( formatCB.selection() )=="Targa" );

   return;
}


void P_Output::deleteExtraControls()
{
   // delete extra control
   ISelectHandler::stopHandlingEventsFor( compressCBx );
   IMenuHandler::stopHandlingEventsFor( compressCBx );
   delete( compressCBx );
   resetVariables();

   return;
}


IString P_Output::getOutputExtension()
{
   IString outputExtension;
   // get the format currently selected
   IString format = formatCB.itemText( formatCB.selection() );
   // match the format with one of the known types
   if( format == "Targa" )
   {
      outputExtension = IString( "TGA" );
   }
   else
   {
      outputExtension = format;
   }

   return outputExtension;
}


void P_Output::resetVariables()
{
   compressCBx = 0;

   return;
}


void P_Output::readSettings( IProfile &profile )
{
   // assume the profile has already been opened
   // process the settings
   bufferSB.spinTo(  profile.integerWithKey( "output_bufferSB"       ) );
   formatCB.select(  profile.integerWithKey( "output_format"         ) );
   compress =        profile.integerWithKey( "output_compress"       );
   showCompress =    profile.integerWithKey( "output_showCompress"   );

   if( showCompress )
   {
      showExtraControls();
   }
   else if( compressCBx )
   {
      deleteExtraControls();
   }


   return;
}


void P_Output::saveSettings( IProfile &profile )
{
   // assume the profile has already been opened
   // save the settings for this canvas
   profile.addOrReplaceElementWithKey( "output_bufferSB",   bufferSB.value() );
   profile.addOrReplaceElementWithKey( "output_format", formatCB.selection() );
   profile.addOrReplaceElementWithKey( "output_compress",      compress      );
   profile.addOrReplaceElementWithKey( "output_showCompress",  showCompress  );
   return;
}


IString P_Output::getOptions()
{
   // return the output options as parameters to POVRAY
   IString options = IString( " +b"             ) +
                     IString( bufferSB.value()  ) +
                     IString( " +f"             );

   // which format is being used?
   if( formatCB.text() == "Targa" )
   {
      options += IString( compress ? "c" : "t" );
   }
   else if( formatCB.text() == "PNG" )
   {
      options += IString( "n" );
   }
   else if( formatCB.text() == "PPM" )
   {
      options += IString( "p" );
   }
   else if( formatCB.text() == "BMP" )
   {
      options += IString( "s" );
   }

   return options;
}

