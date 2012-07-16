//
//  G3MWidget.h
//  G3MiOSSDK
//
//  Created by José Miguel S N on 31/05/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef G3MiOSSDK_G3MWidget_h
#define G3MiOSSDK_G3MWidget_h

#include "Renderer.hpp"
#include "Planet.hpp"
#include "IFactory.hpp"
#include "Camera.hpp"
#include "Color.hpp"
#include "Downloader.hpp"

class G3MWidget {
public:
  
  static G3MWidget* create(IFactory* factory,
                           ILogger *logger,
                           IGL* gl,
                           TexturesHandler* texturesHandler,
                           Downloader * downloader,
                           const Planet* planet,
                           Renderer* renderer,
                           int width, int height,
                           Color backgroundColor,
                           const bool logFPS);
  
  ~G3MWidget();
  
  int render();
  
  void onTouchEvent(const TouchEvent* myEvent);
  
  void onResizeViewportEvent(int width, int height);
  
  IGL* getGL() const {
    return _gl;
  }
  
  
private:
  IFactory*        _factory;
  ILogger*         _logger;
  IGL*             _gl;
  const Planet*    _planet;
  Renderer*        _renderer;
  Camera*          _camera;
  Downloader*      _downloader;
  TexturesHandler* _texturesHandler;
  const Color      _backgroundColor;
  
  ITimer*          _timer;
  long             _renderCounter;
  long             _totalRenderTime;
  const bool       _logFPS;
  
  void initializeGL();
  
  G3MWidget(IFactory* factory,
            ILogger *logger,
            IGL* gl,
            TexturesHandler* texturesHandler,
            Downloader* downloader,
            const Planet* planet,
            Renderer* renderer,
            int width, int height,
            Color backgroundColor,
            const bool logFPS):
  _factory(factory),
  _logger(logger),
  _gl(gl),
  _texturesHandler(texturesHandler),
  _planet(planet),
  _renderer(renderer),
  _camera(new Camera(width, height)),
  _backgroundColor(backgroundColor),
  _timer(factory->createTimer()),
  _renderCounter(0),
  _totalRenderTime(0),
  _logFPS(logFPS),
  _downloader(downloader)
  {
    initializeGL();
    
    InitializationContext ic(_factory, _logger, _planet, _downloader);
    _renderer->initialize(&ic);
  }
  
};

#endif
