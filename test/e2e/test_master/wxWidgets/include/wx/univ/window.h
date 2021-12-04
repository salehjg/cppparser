///////////////////////////////////////////////////////////////////////////////
// Name:        wx/univ/window.h
// Purpose:     wxWindow class which is the base class for all
//              wxUniv port controls, it supports the customization of the
//              window drawing and input processing.
// Author:      Vadim Zeitlin
// Modified by:
// Created:     06.08.00
// Copyright:   (c) 2000 SciTech Software, Inc. (www.scitechsoft.com)
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////
#ifndef _WX_UNIV_WINDOW_H_
#  define _WX_UNIV_WINDOW_H_
#  include "wx/bitmap.h"
class WXDLLIMPEXP_FWD_CORE wxControlRenderer;
class WXDLLIMPEXP_FWD_CORE wxEventLoop;
#  if  wxUSE_MENUS
class WXDLLIMPEXP_FWD_CORE wxMenu;
class WXDLLIMPEXP_FWD_CORE wxMenuBar;
#  endif
class WXDLLIMPEXP_FWD_CORE wxRenderer;
#  if  wxUSE_SCROLLBAR
class WXDLLIMPEXP_FWD_CORE wxScrollBar;
#  endif
#  ifdef __WXX11__
#    define wxUSE_TWO_WINDOWS	1
#  else 
#    define wxUSE_TWO_WINDOWS	0
#  endif
// ----------------------------------------------------------------------------
// wxWindow
// ----------------------------------------------------------------------------
#  if  defined(__WXMSW__)
#    define wxWindowNative	wxWindowMSW
#  elif  defined(__WXGTK__)
#    define wxWindowNative	wxWindowGTK
#  elif  defined(__WXX11__)
#    define wxWindowNative	wxWindowX11
#  elif  defined(__WXMAC__)
#    define wxWindowNative	wxWindowMac
#  endif
class WXDLLIMPEXP_CORE wxWindow : public wxWindowNative
{
public:
    // ctors and create functions
    // ---------------------------
  wxWindow()
  {
    Init();
  }
  wxWindow(wxWindow* parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0, const wxString& name = wxASCII_STR(wxPanelNameStr))
    : wxWindowNative(parent, id, pos, size, style | wxCLIP_CHILDREN, name)
  {
    Init();
  }
  bool Create(wxWindow* parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0, const wxString& name = wxASCII_STR(wxPanelNameStr));
  virtual ~wxWindow();
    // background pixmap support
    // -------------------------
  virtual void SetBackground(const wxBitmap& bitmap, int alignment = wxALIGN_CENTRE, wxStretch stretch = wxSTRETCH_NOT);
  const wxBitmap& GetBackgroundBitmap(int* alignment = NULL, wxStretch* stretch = NULL) const;
    // scrollbars: we (re)implement it ourselves using our own scrollbars
    // instead of the native ones
    // ------------------------------------------------------------------
  void SetScrollbar(int orient, int pos, int page, int range, bool refresh = true) override;
  void SetScrollPos(int orient, int pos, bool refresh = true) override;
  int GetScrollPos(int orient) const override;
  int GetScrollThumb(int orient) const override;
  int GetScrollRange(int orient) const override;
  void ScrollWindow(int dx, int dy, const wxRect* rect = NULL) override;
    // take into account the borders here
  wxPoint GetClientAreaOrigin() const override;
    // popup menu support
    // ------------------

    // NB: all menu related functions are implemented in menu.cpp
#  if  wxUSE_MENUS
    // this is wxUniv-specific private method to be used only by wxMenu
  void DismissPopupMenu();
#  endif
    // miscellaneous other methods
    // ---------------------------

    // get the state information
  virtual bool IsFocused() const;
  virtual bool IsCurrent() const;
  virtual bool IsPressed() const;
  virtual bool IsDefault() const;
    // return all state flags at once (combination of wxCONTROL_XXX values)
  int GetStateFlags() const;
    // set the "highlighted" flag and return true if it changed
  virtual bool SetCurrent(bool doit = true);
#  if  wxUSE_SCROLLBAR
    // get the scrollbar (may be NULL) for the given orientation
  wxScrollBar* GetScrollbar(int orient) const
  {
    return orient & wxVERTICAL ? m_scrollbarVert : m_scrollbarHorz;
  }
#  endif
    // methods used by wxColourScheme to choose the colours for this window
    // --------------------------------------------------------------------

    // return true if this is a panel/canvas window which contains other
    // controls only
  virtual bool IsCanvasWindow() const
  {
    return false;
  }
    // return true if this control can be highlighted when the mouse is over
    // it (the theme decides itself whether it is really highlighted or not)
  virtual bool CanBeHighlighted() const
  {
    return false;
  }
    // return true if we should use the colours/fonts returned by the
    // corresponding GetXXX() methods instead of the default ones
  bool UseFgCol() const
  {
    return m_hasFgCol;
  }
  bool UseFont() const
  {
    return m_hasFont;
  }
    // return true if this window serves as a container for the other windows
    // only and doesn't get any input itself
  virtual bool IsStaticBox() const
  {
    return false;
  }
    // returns the (low level) renderer to use for drawing the control by
    // querying the current theme
  wxRenderer* GetRenderer() const
  {
    return m_renderer;
  }
    // scrolling helper: like ScrollWindow() except that it doesn't refresh the
    // uncovered window areas but returns the rectangle to update (don't call
    // this with both dx and dy non zero)
  wxRect ScrollNoRefresh(int dx, int dy, const wxRect* rect = NULL);
    // after scrollbars are added or removed they must be refreshed by calling
    // this function
  void RefreshScrollbars();
    // erase part of the control
  virtual void EraseBackground(wxDC& dc, const wxRect& rect);
    // overridden base class methods
    // -----------------------------

    // the rect coordinates are, for us, in client coords, but if no rect is
    // specified, the entire window is refreshed
  void Refresh(bool eraseBackground = true, const wxRect* rect = (const wxRect*) NULL) override;
    // we refresh the window when it is dis/enabled
  bool Enable(bool enable = true) override;
    // should we use the standard control colours or not?
  bool ShouldInheritColours() const override
  {
    return false;
  }
  bool IsClientAreaChild(const wxWindow* child) const override
  {
#  if  wxUSE_SCROLLBAR
    if (child == (wxWindow*) m_scrollbarHorz || child == (wxWindow*) m_scrollbarVert)
    {
      return false;
    }
#  endif
    return wxWindowNative::IsClientAreaChild(child);
  }
protected:
    // common part of all ctors
  void Init();
#  if  wxUSE_MENUS
  bool DoPopupMenu(wxMenu* menu, int x, int y) override;
#  endif
    // we deal with the scrollbars in these functions
  void DoSetClientSize(int width, int height) override;
  void DoGetClientSize(int* width, int* height) const override;
  wxHitTest DoHitTest(wxCoord x, wxCoord y) const override;
    // event handlers
  void OnSize(wxSizeEvent& event);
  void OnNcPaint(wxNcPaintEvent& event);
  void OnPaint(wxPaintEvent& event);
  void OnErase(wxEraseEvent& event);
#  if  wxUSE_ACCEL || wxUSE_MENUS
  void OnKeyDown(wxKeyEvent& event);
#  endif
#  if  wxUSE_MENUS
  void OnChar(wxKeyEvent& event);
  void OnKeyUp(wxKeyEvent& event);
#  endif
    // draw the control background, return true if done
  virtual bool DoDrawBackground(wxDC& dc);
    // draw the controls border
  virtual void DoDrawBorder(wxDC& dc, const wxRect& rect);
    // draw the controls contents
  virtual void DoDraw(wxControlRenderer* renderer);
    // override the base class method to return the size of the window borders
  wxSize DoGetBorderSize() const override;
    // adjust the size of the window to take into account its borders
  wxSize AdjustSize(const wxSize& size) const;
    // put the scrollbars along the edges of the window
  void PositionScrollbars();
#  if  wxUSE_MENUS
    // return the menubar of the parent frame or NULL
  wxMenuBar* GetParentFrameMenuBar() const;
#  endif
    // the renderer we use
  wxRenderer* m_renderer;
    // background bitmap info
  wxBitmap m_bitmapBg;
  int m_alignBgBitmap;
  wxStretch m_stretchBgBitmap;
    // old size
  wxSize m_oldSize;
    // is the mouse currently inside the window?
  bool m_isCurrent;
#  ifdef __WXMSW__
public:
    // override MSWWindowProc() to process WM_NCHITTEST
  WXLRESULT MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam);
#  endif
#  if  wxUSE_SCROLLBAR
private:
    // the window scrollbars
  wxScrollBar* m_scrollbarHorz, *m_scrollbarVert;
#  endif
#  if  wxUSE_MENUS
    // the current modal event loop for the popup menu we show or NULL
  static wxEventLoop* ms_evtLoopPopup;
    // the last window over which Alt was pressed (used by OnKeyUp)
  static wxWindow* ms_winLastAltPress;
#  endif
  wxDECLARE_DYNAMIC_CLASS(wxWindow);
  wxDECLARE_EVENT_TABLE();
};
#endif
