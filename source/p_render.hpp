// St‚phane Charette, charette@writeme.com
// Open Class Library POV Front-end Launch Tool
// Application & source code (c) St‚phane Charette


#ifndef P_RENDER_HPP
#define P_RENDER_HPP

// ocl includes
#include <icmdhdr.hpp>  // ICommandHandler
#include <imenuhdr.hpp> // IMenuHandler
#include <iprofile.hpp> // IProfile
#include <ipushbut.hpp> // IPushButton
#include <isetcv.hpp>   // ISetCanvas
#include <istring.hpp>  // IString

// class prototype
class P_Render;

class P_Render :
   public ISetCanvas,
   protected ICommandHandler,
   protected IMenuHandler
{
   public:
      // instance variables
      IPushButton startPB;
      IString     renderer;
      // constructor/destructor
      P_Render( IWindow *dlg );
      virtual ~P_Render( void );
      // inherited methods
      virtual Boolean command(         ICommandEvent &event );
      virtual Boolean makePopUpMenu(   IMenuEvent    &event );
      // other methods
      void readSettings( IProfile &profile );
      void saveSettings( IProfile &profile );
};

#endif

