package org.glob3.mobile.generated; 
//
//  MeshShape.cpp
//  G3MiOSSDK
//
//  Created by Diego Gomez Deck on 11/5/12.
//
//

//
//  MeshShape.hpp
//  G3MiOSSDK
//
//  Created by Diego Gomez Deck on 11/5/12.
//
//


//C++ TO JAVA CONVERTER NOTE: Java has no need of forward class declarations:
//class Mesh;

public abstract class MeshShape extends Shape
{
  private Mesh _mesh;

  protected abstract Mesh createMesh(G3MRenderContext rc);

  protected final Mesh getMesh(G3MRenderContext rc)
  {
	if (_mesh == null)
	{
	  _mesh = createMesh(rc);
	}
	return _mesh;
  }

  protected final void cleanMesh()
  {
	if (_mesh != null)
		_mesh.dispose();
	_mesh = null;
  }

  public MeshShape(Geodetic3D position)
  {
	  super(position);
	  _mesh = null;

  }

  public final boolean isReadyToRender(G3MRenderContext rc)
  {
	final Mesh mesh = getMesh(rc);
	return (mesh != null);
  }

  public final void rawRender(G3MRenderContext rc)
  {
	final Mesh mesh = getMesh(rc);
	if (mesh != null)
	{
	  mesh.render(rc);
	}
  }


  ///#include "GL.hpp"
  
  public void dispose()
  {
	if (_mesh != null)
		_mesh.dispose();
  }

  public final boolean isTransparent(G3MRenderContext rc)
  {
	final Mesh mesh = getMesh(rc);
	if (mesh == null)
	{
	  return false;
	}
	return mesh.isTransparent(rc);
  }

}