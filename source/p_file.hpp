// St‚phane Charette, charette@writeme.com
// Open Class Library POV Front-end Launch Tool
// Application & source code (c) St‚phane Charette


#ifndef P_FILE_HPP
#define P_FILE_HPP

// ocl includes
#include <icmdhdr.hpp>  // ICommandHandler
#include <icombobx.hpp> // IComboBox
#include <ifocshdr.hpp> // IFocusHandler
#include <imenuhdr.hpp> // IMenuHandler
#include <iprofile.hpp> // IProfile
#include <ipushbut.hpp> // IPushButton
#include <isetcv.hpp>   // ISetCanvas
#include <istattxt.hpp> // IStaticText
#include <istring.hpp>  // IString

// class prototype
class P_File;

class P_File :
   public ISetCanvas,
   protected ICommandHandler,
   protected IFocusHandler,
   protected IMenuHandler
{
   public:
      // instance variables
      Boolean     useInternalViewer;
      IString     editor;     // name of the editor to use
      IString     viewer;     // name of the viewer to use
      IStaticText inputTxt;
      IComboBox   inputCB;
      IPushButton editPB;
      IStaticText outputTxt;
      IComboBox   outputCB;
      IPushButton viewPB;
      // constructor/destructor
      P_File( IWindow *dlg );
      virtual ~P_File( void );
      // inherited methods
      virtual Boolean command(         ICommandEvent &event );
      virtual Boolean lostFocus(       IControlEvent &event );
      virtual Boolean makePopUpMenu(   IMenuEvent    &event );
      // other methods
      void updateOutputFilename( void );
      void readSettings( IProfile &profile );
      void saveSettings( IProfile &profile );
      virtual IString getOptions( void );
};

#endif

