//
//  DEMGridUtils.hpp
//  G3MiOSSDK
//
//  Created by Diego Gomez Deck on 11/7/16.
//
//

#ifndef DEMGridUtils_hpp
#define DEMGridUtils_hpp

class Mesh;
class DEMGrid;
class Planet;
class Geodetic3D;
class Sector;
class Vector2S;


class DEMGridUtils {
private:
  DEMGridUtils() {}

public:

  static Mesh* createDebugMesh(const DEMGrid* grid,
                               const Planet* planet,
                               float verticalExaggeration,
                               const Geodetic3D& offset,
                               float pointSize);

  static const DEMGrid* bestGridFor(const DEMGrid*  grid,
                                    const Sector&   sector,
                                    const Vector2S& extent);

};

#endif