// St‚phane Charette, charette@writeme.com
// Open Class Library POV Front-end Launch Tool
// Application & source code (c) St‚phane Charette


#ifndef P_FINISH_HPP
#define P_FINISH_HPP

// ocl includes
#include <icheckbx.hpp> // ICheckBox
#include <icmdhdr.hpp>  // ICommandHandler
#include <icombobx.hpp> // IComboBox
#include <imenuhdr.hpp> // IMenuHandler
#include <iprofile.hpp> // IProfile
#include <iselhdr.hpp>  // ISelectHandler
#include <isetcv.hpp>   // ISetCanvas
#include <istattxt.hpp> // IStaticText
#include <istring.hpp>  // IString

// class prototype
class P_Finish;

class P_Finish :
   public ISetCanvas,
   protected ICommandHandler,
   protected IMenuHandler,
   protected ISelectHandler
{
   public:
      // imbedded class to keep track of the different set values
      class setValues
      {
         public:
            // instance variables
            int      qualityCB[2];
            int      antiAliasCB[2];
            int      methodCB[2];
            int      jitterCB[2];
            int      depthCB[2];
            Boolean  radiosityCBx[2];
            Boolean  lightBufferCBx[2];
            Boolean  vistaBufferCBx[2];
            // constructor
            setValues( void );
      };

      // instance variables
      IStaticText    qualityTxt;
      IComboBox      qualityCB;
      IStaticText    antiAliasTxt;
      IComboBox      antiAliasCB;
      IStaticText    methodTxt;
      IComboBox      methodCB;
      IStaticText    jitterTxt;
      IComboBox      jitterCB;
      IStaticText    depthTxt;
      IComboBox      depthCB;
      ICheckBox      radiosityCBx;
      ICheckBox      lightBufferCBx;
      ICheckBox      vistaBufferCBx;
      setValues      values;
      int            set;  // used when accessing arrays in P_Finish::setValues
      // constructor/destructor
      P_Finish( IWindow *dlg );
      virtual ~P_Finish( void );
      // inherited methods
      virtual Boolean command(         ICommandEvent &event );
      virtual Boolean makePopUpMenu(   IMenuEvent    &event );
      virtual Boolean selected(        IControlEvent &event );
      // other methods
      void readSettings( IProfile &profile );
      void saveSettings( IProfile &profile );
      virtual IString getOptions( void );
};

#endif

