// St‚phane Charette, charette@writeme.com
// Open Class Library POV Front-end Launch Tool
// Application & source code (c) St‚phane Charette


// required resource and class definitions
#include "p_finish.hpp"
#include "pov_launch.h"

#include <ipopmenu.hpp> // IPopUpMenu

#define P_FINISH_QUALITY_TXT  (P_FINISH + 0x0001)
#define P_FINISH_QUALITY_CB   (P_FINISH + 0x0002)
#define P_FINISH_ANTIALIAS_TXT (P_FINISH+ 0x0003)
#define P_FINISH_ANTIALIAS_CB (P_FINISH + 0x0004)
#define P_FINISH_METHOD_TXT   (P_FINISH + 0x0005)
#define P_FINISH_METHOD_CB    (P_FINISH + 0x0006)
#define P_FINISH_JITTER_TXT   (P_FINISH + 0x0007)
#define P_FINISH_JITTER_CB    (P_FINISH + 0x0008)
#define P_FINISH_DEPTH_TXT    (P_FINISH + 0x0009)
#define P_FINISH_DEPTH_CB     (P_FINISH + 0x000A)
#define P_FINISH_RADIOSITY_CBX (P_FINISH+ 0x000B)
#define P_FINISH_LIGHT_CBX    (P_FINISH + 0x000C)
#define P_FINISH_VISTA_CBX    (P_FINISH + 0x000D)
#define P_FINISH_MENU         (P_FINISH + 0x000E)
#define P_FINISH_SET_BASE     (P_FINISH + 0x000F)
#define P_FINISH_SET_1        (P_FINISH_SET_BASE + 0x0001 )
#define P_FINISH_SET_2        (P_FINISH_SET_BASE + 0x0002 )

P_Finish::P_Finish( IWindow *dlg ) :

   ISetCanvas( P_FINISH, dlg, dlg, IRectangle(),
     (ISetCanvas::classDefaultStyle    |
      ISetCanvas::decksByGroup         |
      ISetCanvas::packEven)            &
     ~ISetCanvas::packTight            ),

   qualityTxt( P_FINISH_QUALITY_TXT, this, this, IRectangle(),
     (IStaticText::classDefaultStyle   |
      IStaticText::vertCenter          |
      IControl::group)                 &
     ~IStaticText::top                 ),

   qualityCB( P_FINISH_QUALITY_CB, this, this, IRectangle(),
     (IComboBox::classDefaultStyle     |
      IComboBox::readOnlyDropDownType  |
      IControl::tabStop)               &
     ~IComboBox::simpleType            ),

   antiAliasTxt( P_FINISH_ANTIALIAS_TXT, this, this, IRectangle(),
     (IStaticText::classDefaultStyle   |
      IStaticText::vertCenter          |
      IControl::group)                 &
     ~IStaticText::top                 ),

   antiAliasCB( P_FINISH_ANTIALIAS_CB, this, this, IRectangle(),
     (IComboBox::classDefaultStyle     |
      IComboBox::readOnlyDropDownType  |
      IControl::tabStop)               &
     ~IComboBox::simpleType            ),

   methodTxt( P_FINISH_METHOD_TXT, this, this, IRectangle(),
     (IStaticText::classDefaultStyle   |
      IStaticText::vertCenter          |
      IControl::group)                 &
     ~IStaticText::top                 ),

   methodCB( P_FINISH_METHOD_CB, this, this, IRectangle(),
     (IComboBox::classDefaultStyle     |
      IComboBox::readOnlyDropDownType  |
      IControl::tabStop)               &
     ~IComboBox::simpleType            ),

   jitterTxt( P_FINISH_JITTER_TXT, this, this, IRectangle(),
     (IStaticText::classDefaultStyle   |
      IStaticText::vertCenter          |
      IControl::group)                 &
     ~IStaticText::top                 ),

   jitterCB( P_FINISH_JITTER_CB, this, this, IRectangle(),
     (IComboBox::classDefaultStyle     |
      IComboBox::readOnlyDropDownType  |
      IControl::tabStop)               &
     ~IComboBox::simpleType            ),

   depthTxt( P_FINISH_DEPTH_TXT, this, this, IRectangle(),
     (IStaticText::classDefaultStyle   |
      IStaticText::vertCenter          |
      IControl::group)                 &
     ~IStaticText::top                 ),

   depthCB( P_FINISH_DEPTH_CB, this, this, IRectangle(),
     (IComboBox::classDefaultStyle     |
      IComboBox::readOnlyDropDownType  |
      IControl::tabStop)               &
     ~IComboBox::simpleType            ),

   radiosityCBx( P_FINISH_RADIOSITY_CBX, this, this, IRectangle(),
      ICheckBox::classDefaultStyle     |
      IControl::group                  |
      IControl::tabStop                ),

   lightBufferCBx( P_FINISH_LIGHT_CBX, this, this, IRectangle(),
      ICheckBox::classDefaultStyle     |
      IControl::group                  |
      IControl::tabStop                ),

   vistaBufferCBx( P_FINISH_VISTA_CBX, this, this, IRectangle(),
      ICheckBox::classDefaultStyle     |
      IControl::group                  |
      IControl::tabStop                ),

   values()
{
   // force a groupbox around the canvas
   setText( "Finish" );
   set = 1;

   // set the control labels
   qualityTxt.    setText( "quality"      );
   antiAliasTxt.  setText( "anti-alias"   );
   methodTxt.     setText( "  method"     );
   jitterTxt.     setText( "  jitter"     );
   depthTxt.      setText( "  depth"      );
   radiosityCBx.  setText( "radiosity"    );
   lightBufferCBx.setText( "light buffer" );
   vistaBufferCBx.setText( "vista buffer" );

   // setup the handler
   ISelectHandler::  handleEventsFor( &qualityCB      );
   ISelectHandler::  handleEventsFor( &antiAliasCB    );
   ISelectHandler::  handleEventsFor( &methodCB       );
   ISelectHandler::  handleEventsFor( &jitterCB       );
   ISelectHandler::  handleEventsFor( &depthCB        );
   ISelectHandler::  handleEventsFor( &radiosityCBx   );
   ISelectHandler::  handleEventsFor( &lightBufferCBx );
   ISelectHandler::  handleEventsFor( &vistaBufferCBx );
   ICommandHandler:: handleEventsFor( this            );
   IMenuHandler::    handleEventsFor( this            );
   IMenuHandler::    handleEventsFor( &qualityCB      );
   IMenuHandler::    handleEventsFor( &qualityTxt     );
   IMenuHandler::    handleEventsFor( &antiAliasCB    );
   IMenuHandler::    handleEventsFor( &antiAliasTxt   );
   IMenuHandler::    handleEventsFor( &methodCB       );
   IMenuHandler::    handleEventsFor( &methodTxt      );
   IMenuHandler::    handleEventsFor( &jitterCB       );
   IMenuHandler::    handleEventsFor( &jitterTxt      );
   IMenuHandler::    handleEventsFor( &depthCB        );
   IMenuHandler::    handleEventsFor( &depthTxt       );
   IMenuHandler::    handleEventsFor( &radiosityCBx   );
   IMenuHandler::    handleEventsFor( &lightBufferCBx );
   IMenuHandler::    handleEventsFor( &vistaBufferCBx );

   // set the combobox text
   qualityCB.  addAsLast( "0 - quick colours" );
   qualityCB.  addAsLast( "1" );
   qualityCB.  addAsLast( "2 - difuse light" );
   qualityCB.  addAsLast( "3 - ambient light" );
   qualityCB.  addAsLast( "4 - shadows" );
   qualityCB.  addAsLast( "5 - extended light" );
   qualityCB.  addAsLast( "6 - texture" );
   qualityCB.  addAsLast( "7" );
   qualityCB.  addAsLast( "8 - reflections" );
   qualityCB.  addAsLast( "9 - halos" );
   qualityCB.  setMinimumRows( qualityCB.count() + 1 ).
               select( values.qualityCB[set-1] ).
               setLimit( 23 );

   antiAliasCB.addAsLast( "0.0 - off" );
   antiAliasCB.addAsLast( "0.1" );
   antiAliasCB.addAsLast( "0.2" );
   antiAliasCB.addAsLast( "0.3 - default" );
   antiAliasCB.addAsLast( "0.4" );
   antiAliasCB.addAsLast( "0.5" );
   antiAliasCB.addAsLast( "0.6" );
   antiAliasCB.addAsLast( "0.7" );
   antiAliasCB.addAsLast( "0.8" );
   antiAliasCB.addAsLast( "0.9" );
   antiAliasCB.setMinimumRows( antiAliasCB.count() + 1 ).
               select( values.antiAliasCB[set-1] ).
               setLimit( 23 );

   methodCB.   addAsLast( "non-adaptive - default" );
   methodCB.   addAsLast( "supersampling" );
   methodCB.   setMinimumRows( methodCB.count() + 1 ).
               select( values.methodCB[set-1] ).
               setLimit( 23 );

   jitterCB.   addAsLast( "0.0 - off" );
   jitterCB.   addAsLast( "0.1" );
   jitterCB.   addAsLast( "0.2" );
   jitterCB.   addAsLast( "0.3" );
   jitterCB.   addAsLast( "0.4" );
   jitterCB.   addAsLast( "0.5" );
   jitterCB.   addAsLast( "0.6" );
   jitterCB.   addAsLast( "0.7" );
   jitterCB.   addAsLast( "0.8" );
   jitterCB.   addAsLast( "0.9" );
   jitterCB.   addAsLast( "1.0 - default" );
   jitterCB.   setMinimumRows( jitterCB.count() + 1 ).
               select( values.jitterCB[set-1] ).
               setLimit( 23 );

   depthCB.    addAsLast( "1 x 1" );
   depthCB.    addAsLast( "2 x 2" );
   depthCB.    addAsLast( "3 x 3 - default" );
   depthCB.    addAsLast( "4 x 4" );
   depthCB.    addAsLast( "5 x 5" );
   depthCB.    addAsLast( "6 x 6" );
   depthCB.    addAsLast( "7 x 7" );
   depthCB.    addAsLast( "8 x 8" );
   depthCB.    addAsLast( "9 x 9" );
   depthCB.    setMinimumRows( depthCB.count() + 1 ).
               select( values.depthCB[set-1] ).
               setLimit( 23 );

   return;
}


P_Finish::~P_Finish()
{
   // stop handling events
   IMenuHandler::    stopHandlingEventsFor( &qualityCB      );
   IMenuHandler::    stopHandlingEventsFor( &qualityTxt     );
   IMenuHandler::    stopHandlingEventsFor( &antiAliasCB    );
   IMenuHandler::    stopHandlingEventsFor( &antiAliasTxt   );
   IMenuHandler::    stopHandlingEventsFor( &methodCB       );
   IMenuHandler::    stopHandlingEventsFor( &methodTxt      );
   IMenuHandler::    stopHandlingEventsFor( &jitterCB       );
   IMenuHandler::    stopHandlingEventsFor( &jitterTxt      );
   IMenuHandler::    stopHandlingEventsFor( &depthCB        );
   IMenuHandler::    stopHandlingEventsFor( &depthTxt       );
   IMenuHandler::    stopHandlingEventsFor( &radiosityCBx   );
   IMenuHandler::    stopHandlingEventsFor( &lightBufferCBx );
   IMenuHandler::    stopHandlingEventsFor( &vistaBufferCBx );
   IMenuHandler::    stopHandlingEventsFor( this            );
   ICommandHandler:: stopHandlingEventsFor( this            );
   ISelectHandler::  stopHandlingEventsFor( &qualityCB      );
   ISelectHandler::  stopHandlingEventsFor( &antiAliasCB    );
   ISelectHandler::  stopHandlingEventsFor( &methodCB       );
   ISelectHandler::  stopHandlingEventsFor( &jitterCB       );
   ISelectHandler::  stopHandlingEventsFor( &depthCB        );
   ISelectHandler::  stopHandlingEventsFor( &radiosityCBx   );
   ISelectHandler::  stopHandlingEventsFor( &lightBufferCBx );
   ISelectHandler::  stopHandlingEventsFor( &vistaBufferCBx );

   return;
}


Boolean P_Finish::command( ICommandEvent &event )
{
   Boolean changed = false;

   // handle command events
   switch( event.commandId() )
   {
      case P_FINISH_SET_1:
      {
         set = 1;
         setText( "Finish (1)" );
         changed = true;
         break;
      }
      case P_FINISH_SET_2:
      {
         set = 2;
         setText( "Finish (2)" );
         changed = true;
         break;
      }
   }

   if( changed )
   {
      // initialize the controls to the currently selected set
      qualityCB.     select( values.qualityCB      [set-1] );
      antiAliasCB.   select( values.antiAliasCB    [set-1] );
      methodCB.      select( values.methodCB       [set-1] );
      jitterCB.      select( values.jitterCB       [set-1] );
      depthCB.       select( values.depthCB        [set-1] );
      radiosityCBx.  select( values.radiosityCBx   [set-1] );
      lightBufferCBx.select( values.lightBufferCBx [set-1] );
      vistaBufferCBx.select( values.vistaBufferCBx [set-1] );
   }

   return changed;
}


Boolean P_Finish::selected( IControlEvent &event )
{
   switch( event.controlId() )
   {
      case P_FINISH_QUALITY_CB:
      {
         values.qualityCB[set-1] = qualityCB.selection();
         break;
      }
      case P_FINISH_ANTIALIAS_CB:
      {
         // enable/disable the other antialias controls if this one is disabled
         values.antiAliasCB[set-1] = antiAliasCB.selection();
         Boolean state = ( values.antiAliasCB[set-1] != 0 );
         methodTxt.  enable( state );
         methodCB.   enable( state );
         jitterTxt.  enable( state );
         jitterCB.   enable( state );
         depthTxt.   enable( state );
         depthCB.    enable( state );
         break;
      }
      case P_FINISH_METHOD_CB:
      {
         values.methodCB[set-1] = methodCB.selection();
         break;
      }
      case P_FINISH_JITTER_CB:
      {
         values.jitterCB[set-1] = jitterCB.selection();
         break;
      }
      case P_FINISH_DEPTH_CB:
      {
         values.depthCB[set-1] = depthCB.selection();
         break;
      }
      case P_FINISH_RADIOSITY_CBX:
      {
         values.radiosityCBx[set-1] = radiosityCBx.isSelected();
         break;
      }
      case P_FINISH_LIGHT_CBX:
      {
         values.lightBufferCBx[set-1] = lightBufferCBx.isSelected();
         break;
      }
      case P_FINISH_VISTA_CBX:
      {
         values.vistaBufferCBx[set-1] = vistaBufferCBx.isSelected();
         break;
      }
   }

   // return the event as not having been handled
   event.setResult( false );
   return false;
}


Boolean P_Finish::makePopUpMenu( IMenuEvent &event )
{
   // create the popup menu
   IPopUpMenu *menu = new IPopUpMenu( this, P_FINISH_MENU );

   menu->addText( P_FINISH_SET_1, "Set 1" ).
         addText( P_FINISH_SET_2, "Set 2" ).
         checkItem( P_FINISH_SET_1, set==1 ).
         checkItem( P_FINISH_SET_2, set==2 );

   // show the popup menu where the mouse is located
   menu->   show( event.mousePosition() ).
            setAutoDeleteObject( true );

   // return the event as having been handled
   event.setResult( true );
   return true;
}


void P_Finish::readSettings( IProfile &profile )
{
   // assume the profile has already been opened
   // process the settings

   // read both sets of values
   for( int i=0; i<2; i++ )
   {
      IString prefix;
      prefix = IString( "finish" ) + IString( i+1 ) + IString( "_" );

      values.qualityCB[i] =   profile.integerWithKey( prefix + "quality"   );
      values.antiAliasCB[i] = profile.integerWithKey( prefix + "antiAlias" );
      values.methodCB[i] =    profile.integerWithKey( prefix + "method"    );
      values.jitterCB[i] =    profile.integerWithKey( prefix + "jitter"    );
      values.depthCB[i] =     profile.integerWithKey( prefix + "depth"     );
      values.radiosityCBx[i] =profile.integerWithKey( prefix + "radiosity" );
      values.lightBufferCBx[i]=profile.integerWithKey(prefix + "light"     );
      values.vistaBufferCBx[i]=profile.integerWithKey(prefix + "vista"     );
   }
   set = profile.integerWithKey( "finish_set" );
   postEvent( IWindow::command, IEventParameter1( P_FINISH_SET_BASE + set ) );

   return;
}


void P_Finish::saveSettings( IProfile &profile )
{
   // assume the profile has already been opened
   // save the settings for this canvas
   profile.addOrReplaceElementWithKey( "finish_set", set );

   profile.addOrReplaceElementWithKey( "finish1_quality",
                                       values.qualityCB[0]        );
   profile.addOrReplaceElementWithKey( "finish1_antiAlias",
                                       values.antiAliasCB[0]      );
   profile.addOrReplaceElementWithKey( "finish1_method",
                                       values.methodCB[0]         );
   profile.addOrReplaceElementWithKey( "finish1_jitter",
                                       values.jitterCB[0]         );
   profile.addOrReplaceElementWithKey( "finish1_depth",
                                       values.depthCB[0]          );
   profile.addOrReplaceElementWithKey( "finish1_radiosity",
                                       values.radiosityCBx[0]     );
   profile.addOrReplaceElementWithKey( "finish1_light",
                                       values.lightBufferCBx[0]   );
   profile.addOrReplaceElementWithKey( "finish1_vista",
                                       values.vistaBufferCBx[0]   );

   profile.addOrReplaceElementWithKey( "finish2_quality",
                                       values.qualityCB[1]        );
   profile.addOrReplaceElementWithKey( "finish2_antiAlias",
                                       values.antiAliasCB[1]      );
   profile.addOrReplaceElementWithKey( "finish2_method",
                                       values.methodCB[1]         );
   profile.addOrReplaceElementWithKey( "finish2_jitter",
                                       values.jitterCB[1]         );
   profile.addOrReplaceElementWithKey( "finish2_depth",
                                       values.depthCB[1]          );
   profile.addOrReplaceElementWithKey( "finish2_radiosity",
                                       values.radiosityCBx[1]     );
   profile.addOrReplaceElementWithKey( "finish2_light",
                                       values.lightBufferCBx[1]   );
   profile.addOrReplaceElementWithKey( "finish2_vista",
                                       values.vistaBufferCBx[1]   );
   return;
}


P_Finish::setValues::setValues()
{
   // initialize the array that keeps track of the individual controls
   for( int i=0; i<2; i++ )
   {
      qualityCB[i]      = 0;
      antiAliasCB[i]    = 0;
      methodCB[i]       = 0;
      jitterCB[i]       = 0;
      depthCB[i]        = 0;
      radiosityCBx[i]   = 0;
      lightBufferCBx[i] = 0;
      vistaBufferCBx[i] = 0;
   }

   return;
}


IString P_Finish::getOptions()
{
   // return the finishing options as parameters to POVRAY
   IString options = IString( " +q"                            ) +
                     IString( qualityCB.text().word(1)    ) +
                     IString( " +a"                            ) +
                     IString( antiAliasCB.text().word(1)  );

   if( antiAliasCB.text().word(1) != "0.0" )
   {
      options +=     IString( " +am"                           ) +
                     IString( methodCB.text().word(1)=="non-adaptive"?1:2 ) +
                     IString( " +j"                            ) +
                     IString( jitterCB.text().word(1)     ) +
                     IString( " +r"                            ) +
                     IString( depthCB.text().word(1)      );
   }

   options +=        IString( " "                              ) +
                     IString( radiosityCBx.isSelected()?"+":"-" ) +
                     IString( "qr "                            ) +
                     IString( lightBufferCBx.isSelected()?"+":"-" ) +
                     IString( "ul "                            ) +
                     IString( vistaBufferCBx.isSelected()?"+":"-" ) +
                     IString( "uv"                             );

   return options;
}

