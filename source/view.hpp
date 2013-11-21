// St‚phane Charette, charette@writeme.com
// Open Class Library POV Front-end Launch Tool
// Application & source code (c) St‚phane Charette


#ifndef VIEW_HPP
#define VIEW_HPP

// ocl includes
#include <icmdhdr.hpp>  // ICommandHandler
#include <idrawcv.hpp>  // IDrawingCanvas
#include <iframe.hpp>   // IFrameWindow
#include <igbitmap.hpp> // IGBitmap
#include <iglist.hpp>   // IGList
#include <imenuhdr.hpp> // IMenuHandler
#include <iprofile.hpp> // IProfile
#include <iselhdr.hpp>  // ISelectHandler
#include <istring.hpp>  // IString
#include <ivport.hpp>   // IViewPort

// class prototype
class ViewFrame;

class ViewFrame :
   public IFrameWindow,
   protected ICommandHandler,
   protected IMenuHandler,
   protected ISelectHandler
{
   public:
      // class variables
      static int     viewNumber;
      // instance variables
      IViewPort      viewPort;
      IDrawingCanvas canvas;
      IString        fileName;
      IGBitmap       image;
      IGList         glist;
      // constructor/destructor
      ViewFrame( IWindow *dlg, char *filename = "" );
      virtual ~ViewFrame( void );
      // inherited methods
      virtual Boolean command(         ICommandEvent &event );
      virtual Boolean selected(        IControlEvent &event );
      virtual Boolean makePopUpMenu(   IMenuEvent    &event );
      // other methods
      void resetVariables();
      void readSettings( IProfile &profile );
      void saveSettings( IProfile &profile );
};

#endif

