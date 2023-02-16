/**************************************************************************************************
 * 
 * Copyright 2022 https://github.com/fe-dagostino
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this 
 * software and associated documentation files (the "Software"), to deal in the Software 
 * without restriction, including without limitation the rights to use, copy, modify, 
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, and to 
 * permit persons to whom the Software is furnished to do so, subject to the following 
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies 
 * or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE 
 * FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
 * DEALINGS IN THE SOFTWARE.
 *
 *************************************************************************************************/

#include <ure_application.h>
#include <ure_resources_fetcher.h>
#include <ure_window.h>
#include <ure_view_port.h>
#include <ure_position.h>
#include <ure_size.h>


class Wall : public ure::ApplicationEvents, public ure::WindowEvents, public ure::ResourcesFetcherEvents
{
public:
  /***/
  Wall( int argc, char** argv );

  /***/
  void run();

  /***/
  void dispose();

protected:
  /***/
  void init( [[__maybe_unused__]] int argc, [[__maybe_unused__]] char** argv );
  /***/
  void loadResources();
  /***/
  void addCamera();
  /***/
  void addLevel0();

// ure::WindowEvents implementation
protected:
  /***/
  virtual ure::void_t  on_mouse_scroll( [[maybe_unused]] ure::Window* pWindow, [[maybe_unused]] ure::double_t dOffsetX, [[maybe_unused]] ure::double_t dOffsetY ) noexcept override;

// ure::ApplicationEvents implementation  
protected:
  /***/
  virtual ure::void_t on_initialize() override;

  /***/
  virtual ure::void_t on_initialized() override;

  /***/
  virtual ure::void_t on_finalize() override;

  /***/
  virtual ure::void_t on_finalized() override;

  virtual ure::void_t on_run() override; 

  /***/
  virtual ure::void_t on_initialize_error(/* @todo */) override;
  /***/
  virtual ure::void_t on_error( [[maybe_unused]] int32_t error, [[maybe_unused]] const std::string& description ) override;
  /***/
  virtual ure::void_t on_finalize_error(/* @todo */) override;

// ure::ResourcesFetcherEvents implementation
protected:  
  /***/
  virtual ure::void_t on_download_succeeded( [[maybe_unused]] const std::string& name, [[maybe_unused]] const ure::byte_t* data, [[maybe_unused]] ure::uint_t length ) override;
  /***/
  virtual ure::void_t on_download_failed   ( [[maybe_unused]] const std::string& name ) override;

private:
  bool                        m_bFullScreen;
  ure::position_t<ure::int_t> m_position;
  ure::Size                   m_size;
  ure::Size                   m_fb_size;    // Frame Buffer Size

  ure::Window*                m_pWindow;
  ure::ViewPort*              m_pViewPort;

};

