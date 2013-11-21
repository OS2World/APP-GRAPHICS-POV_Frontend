// St‚phane Charette, charette@writeme.com
// Open Class Library POV Front-end Launch Tool
// Application & source code (c) St‚phane Charette


#ifndef POV_LAUNCH_HPP
#define POV_LAUNCH_HPP

// ocl includes
#include <icmdhdr.hpp>  // ICommandHandler
#include <iframe.hpp>   // IFrameWindow
#include <imenuhdr.hpp> // IMenuHandler
#include <ipushbut.hpp> // IPushButton
#include <isetcv.hpp>   // ISetCanvas
#include "pov_launch.h"

// class prototypes
class POV_LaunchDialog;
class P_File;
class P_Output;
class P_Size;
class P_Crop;
class P_Finish;
class P_Render;


class POV_LaunchDialog :
   public IFrameWindow,
   protected IMenuHandler,
   protected ICommandHandler
{
   public:
      // constructor/destructor
      POV_LaunchDialog( void );
      virtual ~POV_LaunchDialog( void );
      // instance variables
      Boolean        saveSettingsOnExit;
      ISetCanvas     dialogCanvas;
      P_File        *PFile;
      P_Output      *POutput;
      P_Size        *PSize;
      P_Crop        *PCrop;
      P_Finish      *PFinish;
      P_Render      *PRender;
   protected:
      // inherited methods
      virtual Boolean command(       ICommandEvent &event );
      virtual Boolean systemCommand( ICommandEvent &event );
      virtual Boolean makePopUpMenu( IMenuEvent    &event );
      // other methods
      void readSettings( void );
      void saveSettings( void );
};

#endif

