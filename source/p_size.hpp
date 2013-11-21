// St‚phane Charette, charette@writeme.com
// Open Class Library POV Front-end Launch Tool
// Application & source code (c) St‚phane Charette


#ifndef P_SIZE_HPP
#define P_SIZE_HPP

// ocl includes
#include <icmdhdr.hpp>  // ICommandHandler
#include <icombobx.hpp> // IComboBox
#include <ifocshdr.hpp> // IFocusHandler
#include <imenuhdr.hpp> // IMenuHandler
#include <iprofile.hpp> // IProfile
#include <iselhdr.hpp>  // ISelectHandler
#include <isetcv.hpp>   // ISetCanvas
#include <ispinnum.hpp> // INumericSpinButton
#include <istattxt.hpp> // IStaticText
#include <istring.hpp>  // IString

// class prototype
class P_Size;

class P_Size :
   public ISetCanvas,
   protected ISelectHandler,
   protected ICommandHandler,
   protected IMenuHandler,
   protected IFocusHandler
{
   public:
      // instance variables
      int                  width;
      int                  height;
      IStaticText          resTxt;
      IComboBox            resCB;
      IStaticText         *xTxt;
      INumericSpinButton  *xSB;
      IStaticText         *yTxt;
      INumericSpinButton  *ySB;
      // constructor/destructor
      P_Size( IWindow *dlg );
      virtual ~P_Size( void );
      // inherited methods
      virtual Boolean command(         ICommandEvent &event );
      virtual Boolean selected(        IControlEvent &event );
      virtual Boolean makePopUpMenu(   IMenuEvent    &event );
      virtual Boolean lostFocus(       IControlEvent &event );
      // other methods
      void showExtraControls( void );
      void deleteExtraControls( void );
      void resetVariables( void );
      void readSettings( IProfile &profile );
      void saveSettings( IProfile &profile );
      virtual IString getOptions( void );
};

#endif

