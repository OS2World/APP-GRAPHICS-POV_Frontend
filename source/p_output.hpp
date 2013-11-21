// St‚phane Charette, charette@writeme.com
// Open Class Library POV Front-end Launch Tool
// Application & source code (c) St‚phane Charette


#ifndef P_OUTPUT_HPP
#define P_OUTPUT_HPP

// ocl includes
#include <icheckbx.hpp> // ICheckBox
#include <icmdhdr.hpp>  // ICommandHandler
#include <icombobx.hpp> // IComboBox
#include <imenuhdr.hpp> // IMenuHandler
#include <iprofile.hpp> // IProfile
#include <iselhdr.hpp>  // ISelectHandler
#include <isetcv.hpp>   // ISetCanvas
#include <ispinnum.hpp> // INumericSpinButton
#include <istattxt.hpp> // IStaticText
#include <istring.hpp>  // IString

// class prototype
class P_Output;

class P_Output :
   public ISetCanvas,
   protected ICommandHandler,
   protected IMenuHandler,
   protected ISelectHandler
{
   public:
      // instance variables
      Boolean     compress;
      Boolean     showCompress;
      IStaticText bufferTxt;
      INumericSpinButton bufferSB;
      IStaticText formatTxt;
      IComboBox   formatCB;
      ICheckBox  *compressCBx;
      // constructor/destructor
      P_Output( IWindow *dlg );
      virtual ~P_Output( void );
      // inherited methods
      virtual Boolean command(         ICommandEvent &event );
      virtual Boolean makePopUpMenu(   IMenuEvent    &event );
      virtual Boolean selected(        IControlEvent &event );
      // other methods
      void showExtraControls( void );
      void deleteExtraControls( void );
      IString getOutputExtension( void );
      void resetVariables( void );
      void readSettings( IProfile &profile );
      void saveSettings( IProfile &profile );
      virtual IString getOptions( void );
};

#endif

