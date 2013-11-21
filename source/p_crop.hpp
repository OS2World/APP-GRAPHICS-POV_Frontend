// St‚phane Charette, charette@writeme.com
// Open Class Library POV Front-end Launch Tool
// Application & source code (c) St‚phane Charette


#ifndef P_CROP_HPP
#define P_CROP_HPP

// ocl includes
#include <icheckbx.hpp> // ICheckBox
#include <icmdhdr.hpp>  // ICommandHandler
#include <ifocshdr.hpp> // IFocusHandler
#include <imenuhdr.hpp> // IMenuHandler
#include <iprofile.hpp> // IProfile
#include <iselhdr.hpp>  // ISelectHandler
#include <isetcv.hpp>   // ISetCanvas
#include <ispinnum.hpp> // INumericSpinButton
#include <istattxt.hpp> // IStaticText
#include <istring.hpp>  // IString

// class prototype
class P_Crop;

class P_Crop :
   public ISetCanvas,
   protected ICommandHandler,
   protected IFocusHandler,
   protected IMenuHandler,
   protected ISelectHandler
{
   public:
      // instance variables
      ICheckBox            enableCBx;
      IStaticText         *xStartTxt;
      INumericSpinButton  *xStartSB;
      IStaticText         *xEndTxt;
      INumericSpinButton  *xEndSB;
      IStaticText         *yStartTxt;
      INumericSpinButton  *yStartSB;
      IStaticText         *yEndTxt;
      INumericSpinButton  *yEndSB;
      // constructor/destructor
      P_Crop( IWindow *dlg );
      virtual ~P_Crop( void );
      // inherited methods
      virtual Boolean command(         ICommandEvent &event );
      virtual Boolean selected(        IControlEvent &event );
      virtual Boolean makePopUpMenu(   IMenuEvent    &event );
      virtual Boolean lostFocus(       IControlEvent &event );
      // other methods
      void showExtraControls();
      void deleteExtraControls();
      void resetVariables();
      void readSettings( IProfile &profile );
      void saveSettings( IProfile &profile );
      virtual IString getOptions( void );
      void checkSize( void );
};

#endif

