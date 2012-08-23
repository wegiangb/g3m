//
//  IDownloadListener.h
//  G3MiOSSDK
//
//  Created by José Miguel S N on 26/06/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef G3MiOSSDK_IDownloadListener_h
#define G3MiOSSDK_IDownloadListener_h

#include "Response.hpp"

class IDownloadListener {
public:
#ifdef C_CODE
  virtual ~IDownloadListener() {  }
#endif

  virtual void onDownload(const Response* response) = 0;
  virtual void onError(const Response* response) = 0;
  virtual void onCancel(const URL* url) = 0;
  
  /* this method will be call, before onCancel, when the data arrived before the cancelation */
  virtual void onCanceledDownload(const Response* response) = 0;
};

#endif
