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

#include "wall.h"

#include <ure_utils.h>
#include <ure_image.h>
#include <ure_texture.h>
#include <ure_scene_graph.h>
#include <ure_scene_layer_node.h>
#include <widgets/ure_layer.h>

#include <core/utils.h>
  
Wall::Wall( int argc, char** argv )
  : m_bFullScreen(false), m_position{0,0}, m_size{1024,768}, m_fb_size{0,0}
{
  init(argc, argv);
}

void Wall::run()
{
  ure::utils::log( "OglGui::Run(): ENTER" );
  ure::Application::get_instance()->run();
  ure::utils::log( "OglGui::Run(): EXIT"  );
}

void Wall::dispose()
{
  if ( m_pWindow != nullptr )
  {
    m_pWindow->destroy();

    delete m_pWindow;
    m_pWindow = nullptr;
  }

  if ( m_pViewPort != nullptr )
  {
    delete m_pViewPort;
    m_pViewPort = nullptr;
  }

  ure::Application::get_instance()->finalize();
}

void Wall::init( [[__maybe_unused__]] int argc, [[__maybe_unused__]] char** argv )
{
  const std::string sShadersPath( "./resources/shaders/" );
  const std::string sMediaPath  ( "./resources/media/" );

  ure::Application::initialize( core::unique_ptr<ure::ApplicationEvents>(this,false), sShadersPath );

  //
  m_pWindow = new(std::nothrow) ure::Window();
  if ( m_pWindow == nullptr )
  {
    ure::utils::log( "Unable to start application" );
    return ;
  }

  // Add this as listener for WindowEvents
  m_pWindow->connect( this );
  
  std::unique_ptr<ure::window_options> options = 
        std::make_unique<ure::window_options>( 
              (m_bFullScreen==true)?"main":"", 
                "Map Application",
                m_position, 
                m_size 
        );
  
  m_pWindow->create( std::move(options), static_cast<ure::enum_t>(ure::Window::processing_flag_t::epfCalling) );
  m_pWindow->set_swap_interval(1);

  //////////////////////////////////////////////////////////////////////////

  const ure::Renderer* renderer = m_pWindow->get_renderer();

  std::string  sVendor;
  std::string  sRenderer;
  std::string  sVersion;
  std::string  sShaderVersion;
  
  renderer->get_vendor  ( sVendor );
  ure::utils::log( core::utils::format( "Vendor:         [%s]", sVendor.c_str() )        );
  renderer->get_renderer( sRenderer );
  ure::utils::log( core::utils::format( "Renderer:       [%s]", sRenderer.c_str() )      );
  renderer->get_version( sVersion );
  ure::utils::log( core::utils::format( "Version:        [%s]", sVersion.c_str() )       );
  renderer->get_shader_version( sShaderVersion );
  ure::utils::log( core::utils::format( "Shader Version: [%s]", sShaderVersion.c_str() ) );

  //////////////////////////////////////////////////////////////////////////

  std::unique_ptr<ure::SceneGraph> ptrSceneGraph = std::make_unique<ure::SceneGraph>();
  if ( ptrSceneGraph == nullptr )
  {
    ure::utils::log( "Unable to allocate SceneGraph" );
    return ;
  }

  glm::mat4 mProjection = glm::perspectiveFov(45.0f, (float)m_size.width, (float)m_size.height, 0.1f, 500.0f);

  m_pViewPort   = new(std::nothrow) ure::ViewPort( std::move(ptrSceneGraph), mProjection );
  if ( m_pViewPort == nullptr )
  {
    ure::utils::log( "Unable to allocate ViewPort" );
    return ;
  }

  loadResources();

  addCamera();

  addLevel0();
}

void Wall::loadResources()
{
  ure::Image    bkImage; 

  bkImage.load( ure::Image::loader_t::eStb, "./resources/media/images/wall.jpg" );

  std::unique_ptr<ure::Texture>  ptrTexture = std::make_unique<ure::Texture>( std::move(bkImage) );

  m_rc.attach<ure::Texture>("wall", std::move(ptrTexture) );  
}

void Wall::addCamera()
{
  std::unique_ptr<ure::Camera>  camera =  std::make_unique<ure::Camera>( true );

  glm::vec3 cameraPosition = glm::vec3(0,0,2);  // Camera is at (0,0,1), in World Space
  glm::vec3 cameraTarget   = glm::vec3(0,0,0);  // and looks at the origin
  glm::vec3 upVector       = glm::vec3(0,1,0);  // Head is up (set to 0,-1,0 to look upside-down)

  glm::mat4 CameraMatrix = glm::lookAt(
                                          cameraPosition, // the position of your camera, in world space
                                          cameraTarget,   // where you want to look at, in world space
                                          upVector        // probably glm::vec3(0,1,0), but (0,-1,0) would make you looking upside-down, which can be great too
                                      );
  camera->set_view_matrix( CameraMatrix );

  m_pViewPort->get_scene().add_scene_node( new ure::SceneCameraNode( "MainCamera", std::move(camera) ) );
}

void Wall::addLevel0()
{
  std::shared_ptr<ure::widgets::Layer> layer = std::make_shared<ure::widgets::Layer>( *m_pViewPort );

  m_pWindow->connect(layer->get_windows_events());

  layer->set_visible( true );
  layer->set_enabled( true );

  layer->set_position( -1.0f*m_size.width/2, -1.0f*m_size.height/2, true );

  auto texture = m_rc.find<ure::Texture>("wall");
  if ( texture.has_value() )
    layer->set_background( texture.value(), ure::widgets::Widget::BackgroundOptions::eboAsIs );

  ure::SceneLayerNode* pNode = new(std::nothrow) ure::SceneLayerNode( "Layer1", layer );
  
  glm::mat4 mModel =  glm::ortho( -1.0f*(float)m_size.width/2, (float)m_size.width/2, (float)m_size.height/2, -1.0f*(float)m_size.height/2, 0.1f, 1000.0f );
  pNode->set_model_matrix( mModel );
  pNode->get_model_matrix().translate( 0, 0, 0 );

  m_pViewPort->get_scene().add_scene_node( pNode );  
}

/////////////////////////////////////////////////////
// ure::WindowEvents implementation
/////////////////////////////////////////////////////

ure::void_t  Wall::on_mouse_scroll( [[maybe_unused]] ure::Window* pWindow, [[maybe_unused]] ure::double_t dOffsetX, [[maybe_unused]] ure::double_t dOffsetY ) noexcept 
{
  printf("scroll %f  %f \n", dOffsetX, dOffsetY );
}

/////////////////////////////////////////////////////
// ure::ApplicationEvents implementation
/////////////////////////////////////////////////////

ure::void_t Wall::on_initialize() 
{
  ure::ResourcesFetcher::initialize();
}

ure::void_t Wall::on_initialized() 
{
}

ure::void_t Wall::on_finalize() 
{
  ure::ResourcesFetcher::get_instance()->finalize();
}

ure::void_t Wall::on_finalized() 
{

}

ure::void_t Wall::on_run()
{
  if ( m_pWindow->check( ure::Window::window_flag_t::eWindowShouldClose ) )
  {
    ure::Application::get_instance()->exit(true);
  }

  m_pWindow->get_framebuffer_size( m_fb_size );
    
  ///////////////
  m_pViewPort->set_area( 0, 0, m_fb_size.width, m_fb_size.height );
  m_pViewPort->use();

  // Update background color
  m_pViewPort->get_scene().set_background( 0.2f, 0.2f, 0.2f, 0.0f );

  ///////////////
  m_pViewPort->clear_buffer( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );

  ///////////////
  m_pViewPort->render();

  ///////////////
  m_pWindow->swap_buffers();
  
  ///////////////
  // Will process messages that requires to be executed on main thread.
  m_pWindow->process_message();

  ure::Application::get_instance()->poll_events();  
}

ure::void_t Wall::on_initialize_error(/* @todo */) 
{

}

ure::void_t Wall::on_error( [[maybe_unused]] int32_t error, [[maybe_unused]] const std::string& description )
{

}

ure::void_t Wall::on_finalize_error(/* @todo */) 
{

}


/////////////////////////////////////////////////////
// ure::ResourcesFetcherEvents implementation
/////////////////////////////////////////////////////

ure::void_t Wall::on_download_succeeded( [[maybe_unused]] const std::string& name, [[maybe_unused]] const std::type_info& type, [[maybe_unused]] const ure::byte_t* data, [[maybe_unused]] ure::uint_t length ) noexcept(true)
{

}

ure::void_t Wall::on_download_failed   ( [[maybe_unused]] const std::string& name ) noexcept(true)
{

}

