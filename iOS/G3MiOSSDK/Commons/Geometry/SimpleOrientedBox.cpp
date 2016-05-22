//
//  SimpleOrientedBox.cpp
//  G3MiOSSDK
//
//  Created by Agustin Trujillo Pino on 17/07/12.
//

#include "SimpleOrientedBox.hpp"
#include "Vector2D.hpp"
#include "Camera.hpp"
#include "FloatBufferBuilderFromCartesian3D.hpp"
#include "ShortBufferBuilder.hpp"
#include "IndexedMesh.hpp"
#include "GLConstants.hpp"
#include "Color.hpp"
#include "Sphere.hpp"
#include "Vector2F.hpp"
#include "OrientedBox.hpp"

SimpleOrientedBox::~SimpleOrientedBox() {
  delete _mesh;
  
#ifdef JAVA_CODE
  super.dispose();
#endif

};

const std::vector<Vector3D> SimpleOrientedBox::getCorners() const {
#ifdef C_CODE
  const Vector3D c[8] = {
    _lower,
    Vector3D(_lower._x, _lower._y, _upper._z),
    Vector3D(_lower._x, _upper._y, _lower._z),
    Vector3D(_lower._x, _upper._y, _upper._z),
    Vector3D(_upper._x, _lower._y, _lower._z),
    Vector3D(_upper._x, _lower._y, _upper._z),
    Vector3D(_upper._x, _upper._y, _lower._z),
    _upper
  };
  
  return std::vector<Vector3D>(c, c+8);
#endif
#ifdef JAVA_CODE
  if (_cornersD == null) {
    _cornersD = new java.util.ArrayList<Vector3D>(8);
    
    _cornersD.add(_lower);
    _cornersD.add(new Vector3D(_lower._x, _lower._y, _upper._z));
    _cornersD.add(new Vector3D(_lower._x, _upper._y, _lower._z));
    _cornersD.add(new Vector3D(_lower._x, _upper._y, _upper._z));
    _cornersD.add(new Vector3D(_upper._x, _lower._y, _lower._z));
    _cornersD.add(new Vector3D(_upper._x, _lower._y, _upper._z));
    _cornersD.add(new Vector3D(_upper._x, _upper._y, _lower._z));
    _cornersD.add(_upper);
  }
  return _cornersD;
#endif
}

const std::vector<Vector3F> SimpleOrientedBox::getCornersF() const {
#ifdef C_CODE
  const Vector3F c[8] = {
    Vector3F((float) _lower._x, (float) _lower._y, (float) _lower._z),
    Vector3F((float) _lower._x, (float) _lower._y, (float) _upper._z),
    Vector3F((float) _lower._x, (float) _upper._y, (float) _lower._z),
    Vector3F((float) _lower._x, (float) _upper._y, (float) _upper._z),
    Vector3F((float) _upper._x, (float) _lower._y, (float) _lower._z),
    Vector3F((float) _upper._x, (float) _lower._y, (float) _upper._z),
    Vector3F((float) _upper._x, (float) _upper._y, (float) _lower._z),
    Vector3F((float) _upper._x, (float) _upper._y, (float) _upper._z)
  };

  return std::vector<Vector3F>(c, c+8);
#endif
#ifdef JAVA_CODE
  if (_cornersF == null) {
    _cornersF = new java.util.ArrayList<Vector3F>(8);

    _cornersF.add(new Vector3F((float) _lower._x, (float) _lower._y, (float) _lower._z));
    _cornersF.add(new Vector3F((float) _lower._x, (float) _lower._y, (float) _upper._z));
    _cornersF.add(new Vector3F((float) _lower._x, (float) _upper._y, (float) _lower._z));
    _cornersF.add(new Vector3F((float) _lower._x, (float) _upper._y, (float) _upper._z));
    _cornersF.add(new Vector3F((float) _upper._x, (float) _lower._y, (float) _lower._z));
    _cornersF.add(new Vector3F((float) _upper._x, (float) _lower._y, (float) _upper._z));
    _cornersF.add(new Vector3F((float) _upper._x, (float) _upper._y, (float) _lower._z));
    _cornersF.add(new Vector3F((float) _upper._x, (float) _upper._y, (float) _upper._z));
  }
  return _cornersF;
#endif
}

Vector2F SimpleOrientedBox::projectedExtent(const G3MRenderContext* rc) const {
  const std::vector<Vector3F> corners = getCornersF();

  const Camera* currentCamera = rc->getCurrentCamera();

  const Vector2F pixel0 = currentCamera->point2Pixel(corners[0]);

  float lowerX = pixel0._x;
  float upperX = pixel0._x;
  float lowerY = pixel0._y;
  float upperY = pixel0._y;

  const size_t cornersSize = corners.size();
  for (size_t i = 1; i < cornersSize; i++) {
    const Vector2F pixel = currentCamera->point2Pixel(corners[i]);

    const float x = pixel._x;
    const float y = pixel._y;

    if (x < lowerX) { lowerX = x; }
    if (y < lowerY) { lowerY = y; }

    if (x > upperX) { upperX = x; }
    if (y > upperY) { upperY = y; }
  }

  const float width  = upperX - lowerX;
  const float height = upperY - lowerY;

  return Vector2F(width, height);
}

double SimpleOrientedBox::projectedArea(const G3MRenderContext* rc) const {
  // this is not exact.
  // now is returning the area of the 2D bounding box of the box projection
  const Vector2F extent = projectedExtent(rc);
  return (double) (extent._x * extent._y);
}

bool SimpleOrientedBox::contains(const Vector3D& p) const {
  const static double margin = 1e-3;
  if (p._x < _lower._x - margin) return false;
  if (p._x > _upper._x + margin) return false;
  
  if (p._y < _lower._y - margin) return false;
  if (p._y > _upper._y + margin) return false;
  
  if (p._z < _lower._z - margin) return false;
  if (p._z > _upper._z + margin) return false;
  
  return true;
}

Vector3D SimpleOrientedBox::intersectionWithRay(const Vector3D& origin,
                                  const Vector3D& direction) const {
  //MIN X
  {
    Plane p( Vector3D(1.0, 0.0, 0.0), _lower._x);
    Vector3D inter = p.intersectionWithRay(origin, direction);
    if (!inter.isNan() && contains(inter)) return inter;
  }
  
  //MAX X
  {
    Plane p( Vector3D(1.0, 0.0, 0.0), _upper._x);
    Vector3D inter = p.intersectionWithRay(origin, direction);
    if (!inter.isNan() && contains(inter)) return inter;
  }
  
  //MIN Y
  {
    Plane p( Vector3D(0.0, 1.0, 0.0), _lower._y);
    Vector3D inter = p.intersectionWithRay(origin, direction);
    if (!inter.isNan() && contains(inter)) return inter;
  }
  
  //MAX Y
  {
    Plane p( Vector3D(0.0, 1.0, 0.0), _upper._y);
    Vector3D inter = p.intersectionWithRay(origin, direction);
    if (!inter.isNan() && contains(inter)) return inter;
  }
  
  //MIN Z
  {
    Plane p( Vector3D(0.0, 0.0, 1.0), _lower._z);
    Vector3D inter = p.intersectionWithRay(origin, direction);
    if (!inter.isNan() && contains(inter)) return inter;
  }
  
  //MAX Z
  {
    Plane p( Vector3D(0.0, 0.0, 1.0), _upper._z);
    Vector3D inter = p.intersectionWithRay(origin, direction);
    if (!inter.isNan() && contains(inter)) return inter;
  }
  
  return Vector3D::nan();  
}


Mesh* SimpleOrientedBox::createMesh(const Color& color,
                      float lineWidth) const {
  double v[] = {
    _lower._x, _lower._y, _lower._z,
    _lower._x, _upper._y, _lower._z,
    _lower._x, _upper._y, _upper._z,
    _lower._x, _lower._y, _upper._z,
    _upper._x, _lower._y, _lower._z,
    _upper._x, _upper._y, _lower._z,
    _upper._x, _upper._y, _upper._z,
    _upper._x, _lower._y, _upper._z
  };

  short i[] = {
    0, 1, 1, 2, 2, 3, 3, 0,
    1, 5, 5, 6, 6, 2, 2, 1,
    5, 4, 4, 7, 7, 6, 6, 5,
    4, 0, 0, 3, 3, 7, 7, 4,
    3, 2, 2, 6, 6, 7, 7, 3,
    0, 1, 1, 5, 5, 4, 4, 0
  };

  FloatBufferBuilderFromCartesian3D* vertices = FloatBufferBuilderFromCartesian3D::builderWithFirstVertexAsCenter();
  const int numVertices = 8;
  for (int n = 0; n < numVertices; n++) {
    //vertices->add(v[n*3], v[n*3+1], v[n*3+2]);
    vertices->add(Vector3D(v[n*3], v[n*3+1], v[n*3+2]).transformedBy(_matrix, 1));
  }

  ShortBufferBuilder indices;
  const int numIndices = 48;
  for (int n = 0; n < numIndices; n++) {
    indices.add(i[n]);
  }

  Mesh* mesh = new IndexedMesh(GLPrimitive::lines(),
                               vertices->getCenter(),
                               vertices->create(),
                               true,
                               indices.create(),
                               true,
                               lineWidth,
                               1,
                               new Color(color));
  
  delete vertices;
  
  return mesh;
}

void SimpleOrientedBox::render(const G3MRenderContext* rc,
                 const GLState* parentState,
                 const Color& color) const {
  if (_mesh == NULL) {
    _mesh = createMesh(color);
  }
  _mesh->render(rc, parentState);
}

bool SimpleOrientedBox::touchesBox(const Box* that) const {
  if (_lower._x > that->_upper._x) { return false; }
  if (_upper._x < that->_lower._x) { return false; }
  if (_lower._y > that->_upper._y) { return false; }
  if (_upper._y < that->_lower._y) { return false; }
  if (_lower._z > that->_upper._z) { return false; }
  if (_upper._z < that->_lower._z) { return false; }
  return true;
}

bool SimpleOrientedBox::touchesSphere(const Sphere* that) const {
  //return that->touchesSimpleOrientedBox(this);
}


Box* SimpleOrientedBox::mergedWithBox(const Box* that) const {
/*  const IMathUtils* mu = IMathUtils::instance();

  const double lowerX = mu->min(_lower._x, that->_lower._x);
  const double lowerY = mu->min(_lower._y, that->_lower._y);
  const double lowerZ = mu->min(_lower._z, that->_lower._z);

  const double upperX = mu->max(_upper._x, that->_upper._x);
  const double upperY = mu->max(_upper._y, that->_upper._y);
  const double upperZ = mu->max(_upper._z, that->_upper._z);

  return new Box(Vector3D(lowerX, lowerY, lowerZ),
                 Vector3D(upperX, upperY, upperZ));*/
}

BoundingVolume* SimpleOrientedBox::mergedWithSphere(const Sphere* that) const {
  //return that->mergedWithSimpleOrientedBox(this);
}

SimpleOrientedBox* SimpleOrientedBox::mergedWithOrientedBox(const OrientedBox* that) const {
  //return that->mergedWithSimpleOrientedBox(this);
}

bool SimpleOrientedBox::fullContainedInBox(const Box* box) const {
  return box->contains(_upper) && box->contains(_lower);
}

bool SimpleOrientedBox::fullContainedInSphere(const Sphere* that) const {
  return that->contains(_lower) && that->contains(_upper);
}

bool SimpleOrientedBox::fullContainedInOrientedBox(const OrientedBox *that) const {
  //return that->fullContainsSimpleOrientedBox(this);
}

Vector3D SimpleOrientedBox::closestPoint(const Vector3D& point) const {
  return point.clamp(_lower, _upper);
}

Sphere* SimpleOrientedBox::createSphere() const {
  const Vector3D center = _lower.add(_upper).div(2);
  const double radius = center.distanceTo(_upper);
  return new Sphere(center, radius);
}


const std::string SimpleOrientedBox::description() const {
  IStringBuilder* isb = IStringBuilder::newStringBuilder();
  isb->addString("[SimpleOrientedBox ");
  isb->addString(_lower.description());
  isb->addString(" / ");
  isb->addString(_upper.description());
  isb->addString("]");
  const std::string s = isb->getString();
  delete isb;
  return s;
}