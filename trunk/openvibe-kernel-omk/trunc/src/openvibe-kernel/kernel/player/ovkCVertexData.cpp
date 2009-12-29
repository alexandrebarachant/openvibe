#include "ovkCVertexData.h"

using namespace OpenViBE;
//using namespace OMK ;
//using namespace OMK::Type ;

//const Transform Transform::sk_identity;

CVertexData::CVertexData() :
/*	_verticesAction(CVertexData::VertexDataAction_None),
	_colorsAction(CVertexData::VertexDataAction_None)*/
	m_ui32VertexPositionCount(0)
#ifndef USE_STL_CONTAINERS
	,m_pVertexPositionArray(NULL)
#endif
	,m_ui32VertexColorCount(0)
#ifndef USE_STL_CONTAINERS
	,m_pVertexColorArray(NULL)
#endif
{
}

CVertexData::~CVertexData()
{
	//FIXME
	//cleanup();
}

void CVertexData::cleanup()
{
#ifndef USE_STL_CONTAINERS
	if(m_pVertexPositionArray != NULL)
	{
		delete m_pVertexPositionArray;
	}
	if(m_pVertexColorArray != NULL)
	{
		delete m_pVertexColorArray;
	}
#endif
}

/*
void CVertexData::setVerticesAction(EVertexDataAction action)
{
	_verticesAction = action;
}

void CVertexData::setVertices( const std::vector< Wm4::Vector3f >& vertices )
{
  _vertices = vertices;
}*/

void CVertexData::setVertexPositionArray( uint32 ui32VertexPositionCount, const float32* pVertexPositionArray )
{
	m_ui32VertexPositionCount = ui32VertexPositionCount;

#ifdef ALLOCATE_VERTEX_DATA_MEMORY
	if(m_pVertexPositionArray != NULL)
	{
		delete m_pVertexPositionArray;
	}
	m_pVertexPositionArray = new float32[3*ui32VertexPositionCount];
	memcpy(m_pVertexPositionArray, pVertexPositionArray, 3*ui32VertexPositionCount*sizeof(float32));
#endif

#ifdef SHARE_MEMORY_WITH_CALLER
	m_pVertexPositionArray = pVertexPositionArray;
#endif

#ifdef USE_STL_CONTAINERS
	m_pVertexPositionArray.resize(3*ui32VertexPositionCount);
	const float32* l_pVertexPosition = pVertexPositionArray;
	for(unsigned int i=0; i<3*ui32VertexPositionCount; i++, l_pVertexPosition++)
	{
		m_pVertexPositionArray[i] = *l_pVertexPosition;
	}
#endif
}
/*
CVertexData::EVertexDataAction CVertexData::getVerticesAction()
{
	return _verticesAction;
}

const std::vector< Wm4::Vector3f >& CVertexData::getVertices() const
{
	return _vertices;
}
*/

void CVertexData::getVertexPositionArray( uint32& ui32VertexPositionCount, const float32*& pVertexPositionArray) const
{
	ui32VertexPositionCount = m_ui32VertexPositionCount;
	if(m_ui32VertexPositionCount > 0)
		pVertexPositionArray = &m_pVertexPositionArray[0];
	else
		pVertexPositionArray = NULL;
}
/*
void CVertexData::setColorsAction(EVertexDataAction action)
{
	_colorsAction = action;
}

void CVertexData::setColors( const std::vector< Wm4::Vector4f >& colors )
{
  _colors = colors;
}
*/
void CVertexData::setVertexColorArray( uint32 ui32VertexColorCount, const float32* pVertexColorArray )
{
	m_ui32VertexColorCount = ui32VertexColorCount;
#ifdef ALLOCATE_VERTEX_DATA_MEMORY
	if(m_pVertexColorArray != NULL)
	{
		delete m_pVertexColorArray;
	}
	m_pVertexColorArray = new float32[4*ui32VertexColorCount];
	memcpy(m_pVertexColorArray, pVertexColorArray, 4*ui32VertexColorCount*sizeof(float32));
#endif

#ifdef SHARE_MEMORY_WITH_CALLER
	m_pVertexColorArray = pVertexColorArray;
#endif

#ifdef USE_STL_CONTAINERS
	m_pVertexColorArray.resize(4*ui32VertexColorCount);
	const float32* l_pVertexColor = pVertexColorArray;
	for(unsigned int i=0; i<4*ui32VertexColorCount; i++, l_pVertexColor++)
	{
		m_pVertexColorArray[i] = *l_pVertexColor;
	}
#endif
}
/*
CVertexData::EVertexDataAction CVertexData::getColorsAction()
{
	return _colorsAction;
}

const std::vector< Wm4::Vector4f >& CVertexData::getColors() const
{
	return _colors;
}*/

void CVertexData::getVertexColorArray( uint32& ui32VertexColorCount, const float32*& pVertexColorArray) const
{
	ui32VertexColorCount = m_ui32VertexColorCount;
	if(m_ui32VertexColorCount)
		pVertexColorArray = &m_pVertexColorArray[0];
	else
		pVertexColorArray = NULL;
}

/*
bool
OMK::Type::operator == ( const CVertexData& vd0, const CVertexData& vd1 )
{
  if(vd0._verticesAction != vd1._verticesAction || vd0._vertices.size() != vd1._vertices.size() ||
		vd0._colorsAction != vd1._colorsAction || vd0._colors.size() != vd1._colors.size())
		return false;

	size_t vsize = vd0._vertices.size();

	for(size_t i = 0; i<vsize; i++)
	{
		if( vd0._vertices[i].X() != vd1._vertices[i].X())
			return false;
		if( vd0._vertices[i].Y() != vd1._vertices[i].Y())
			return false;
		if( vd0._vertices[i].Z() != vd1._vertices[i].Z())
			return false;
	}

	vsize = vd0._colors.size();

	for(size_t i = 0; i<vsize; i++)
	{
		if( vd0._colors[i].X() != vd1._colors[i].X())
			return false;
		if( vd0._colors[i].Y() != vd1._colors[i].Y())
			return false;
		if( vd0._colors[i].Z() != vd1._colors[i].Z())
			return false;
		if( vd0._colors[i].W() != vd1._colors[i].W())
			return false;
	}

	return true;
}*/
bool operator == ( const CVertexData& vd0, const CVertexData& vd1 )
{
  if(vd0.m_ui32VertexPositionCount != vd1.m_ui32VertexPositionCount ||
		 vd0.m_ui32VertexColorCount != vd1.m_ui32VertexColorCount)
		return false;

	for(uint32 i = 0; i < vd0.m_ui32VertexPositionCount; i+=3)
	{
		if( vd0.m_pVertexPositionArray[i] != vd1.m_pVertexPositionArray[i])
			return false;
		if( vd0.m_pVertexPositionArray[i+1] != vd1.m_pVertexPositionArray[i+1])
			return false;
		if( vd0.m_pVertexPositionArray[i+2] != vd1.m_pVertexPositionArray[i+2])
			return false;
	}

	for(uint32 i = 0; i < vd0.m_ui32VertexColorCount; i+=4)
	{
		if( vd0.m_pVertexColorArray[i] != vd1.m_pVertexColorArray[i])
			return false;
		if( vd0.m_pVertexColorArray[i+1] != vd1.m_pVertexColorArray[i+1])
			return false;
		if( vd0.m_pVertexColorArray[i+2] != vd1.m_pVertexColorArray[i+2])
			return false;
		if( vd0.m_pVertexColorArray[i+3] != vd1.m_pVertexColorArray[i+3])
			return false;
	}

	return true;
}

bool operator != ( const CVertexData& vd0, const CVertexData& vd1 )
{
  return !( vd0 == vd1 ) ;
}

/*
//old version
std::ostream& OMK::Type::operator << ( std::ostream& out, const CVertexData& vd )
{
	const std::vector< Wm4::Vector3f > &v = vd.getVertices();

	int toto = (int)vd._verticesAction;
  out << toto << " " << v.size() << " " ;

	for(size_t i = 0; i<v.size(); i++)
	{
		out << v[i].X()  << " "
        << v[i].Y()  << " "
        << v[i].Z()  << " "  ;
  }

	const std::vector< Wm4::Vector4f > &c = vd.getColors();

	toto = (int)vd._colorsAction;
	out << toto << " " << c.size() << " " ;

	for(size_t i = 0; i<c.size(); i++)
	{
		out << c[i].X()  << " "
        << c[i].Y()  << " "
        << c[i].Z()  << " "
				<< c[i].W()  << " "  ;
  }

  return out;
}*/
/*
std::ostream& OMK::Type::operator << ( std::ostream& out, const CVertexData& vd )
{
	out << vd.m_ui32VertexPositionCount << " ";

	for(uint32 i=0; i<vd.m_ui32VertexPositionCount; i+=3)
	{
		out << vd.m_pVertexPositionArray[i]  << " "
        << vd.m_pVertexPositionArray[i+1]  << " "
        << vd.m_pVertexPositionArray[i+2]  << " ";
  }

	out << vd.m_ui32VertexColorCount << " ";

	for(uint32 i=0; i<vd.m_ui32VertexColorCount; i+=4)
	{
		out << vd.m_pVertexColorArray[i]  << " "
        << vd.m_pVertexColorArray[i+1]  << " "
        << vd.m_pVertexColorArray[i+2]  << " "
				<< vd.m_pVertexColorArray[i+3]  << " ";
  }

  return out;
}*/

/*
//old version
std::istream& OMK::Type::operator >> ( std::istream& in, CVertexData& vd )
{
	int toto;
	in >> toto;
	vd._verticesAction = (CVertexData::EVertexDataAction)toto;

	size_t vsize;
	in >> vsize;

	vd._vertices.resize(vsize);

	for(size_t i = 0; i<vsize; i++)
	{
		in >> vd._vertices[i].X()
       >> vd._vertices[i].Y()
       >> vd._vertices[i].Z();
  }

	in >> toto;
	vd._colorsAction = (CVertexData::EVertexDataAction)toto;

	in >> vsize;
  vd._colors.resize(vsize);

	for(size_t i = 0; i<vsize; i++)
	{
		in >> vd._colors[i].X()
			 >> vd._colors[i].Y()
       >> vd._colors[i].Z()
			 >>	vd._colors[i].W();
  }

  return in;
}
*/

/*
std::istream& OMK::Type::operator >> ( std::istream& in, CVertexData& vd )
{
	in >> vd.m_ui32VertexPositionCount;

	if(vd.m_ui32VertexPositionCount != 0)
	{
		float32* l_pVertexPositionArray = new float32[vd.m_ui32VertexPositionCount * 3];

		for(uint32 i=0; i<vd.m_ui32VertexPositionCount; i+=3)
		{
			in >> l_pVertexPositionArray[i]
				 >> l_pVertexPositionArray[i+1]
				 >> l_pVertexPositionArray[i+2];
		}

#ifndef USE_STL_CONTAINERS
		vd.m_pVertexPositionArray = l_pVertexPositionArray;
#else
		vd.setVertexPositionArray(vd.m_ui32VertexPositionCount, l_pVertexPositionArray);
		delete l_pVertexPositionArray;
#endif
	}

	in >> vd.m_ui32VertexColorCount;

	if(vd.m_ui32VertexColorCount != 0)
	{
		float32* l_pVertexColorArray = new float32[vd.m_ui32VertexColorCount * 4];

		for(uint32 i=0; i<vd.m_ui32VertexColorCount; i+=4)
		{
			in >> l_pVertexColorArray[i]
				 >> l_pVertexColorArray[i+1]
				 >> l_pVertexColorArray[i+2]
				 >> l_pVertexColorArray[i+3];
		}

#ifndef USE_STL_CONTAINERS
		vd.m_pVertexColorArray = l_pVertexColorArray;
#else
		vd.setVertexColorArray(vd.m_ui32VertexColorCount, l_pVertexColorArray);
		delete l_pVertexColorArray;
#endif
	}

  return in;
}
*/

/*
//old version
OutgoingSynchronisationMessage& OMK::Type::operator << ( OutgoingSynchronisationMessage& out, const CVertexData& vd )
{
	int toto = (int)vd._verticesAction;
	out << toto << vd._vertices.size() ;

	for(size_t i = 0; i<vd._vertices.size(); i++)
	{
		out << vd._vertices[i].X()
        << vd._vertices[i].Y()
        << vd._vertices[i].Z() ;
  }

	toto = (int)vd._colorsAction;
	out << toto << vd._colors.size() ;

	for(size_t i = 0; i<vd._colors.size(); i++)
	{
		out << vd._colors[i].X()
        << vd._colors[i].Y()
        << vd._colors[i].Z()
				<< vd._colors[i].W() ;
  }

  return out;
}
*/

/*
OutgoingSynchronisationMessage& OMK::Type::operator << ( OutgoingSynchronisationMessage& out, const CVertexData& vd )
{
	out << vd.m_ui32VertexPositionCount;

	for(uint32 i=0; i<vd.m_ui32VertexPositionCount; i+=3)
	{
		out << vd.m_pVertexPositionArray[i]
        << vd.m_pVertexPositionArray[i+1]
        << vd.m_pVertexPositionArray[i+2];
  }

	out << vd.m_ui32VertexColorCount;

	for(uint32 i=0; i<vd.m_ui32VertexColorCount; i+=4)
	{
		out << vd.m_pVertexColorArray[i]
        << vd.m_pVertexColorArray[i+1]
        << vd.m_pVertexColorArray[i+2]
				<< vd.m_pVertexColorArray[i+3];
  }

  return out;
}*/

/*
//old version
IncomingSynchronisationMessage& OMK::Type::operator >> ( IncomingSynchronisationMessage& in, CVertexData& vd )
{
	int toto;
	in >> toto;
	vd._verticesAction = (CVertexData::EVertexDataAction)toto;

	size_t vsize;
	in >> vsize;
  vd._vertices.resize(vsize);

	for(size_t i = 0; i<vsize; i++)
	{
		in >> vd._vertices[i].X()
       >> vd._vertices[i].Y()
       >> vd._vertices[i].Z();
  }

	in >> toto;
	vd._colorsAction = (CVertexData::EVertexDataAction)toto;

	in >> vsize;
  vd._colors.resize(vsize);

	for(size_t i = 0; i<vsize; i++)
	{
		in >> vd._colors[i].X()
			 >> vd._colors[i].Y()
       >> vd._colors[i].Z()
			 >>	vd._colors[i].W();
  }

  return in;
}
*/
/*
IncomingSynchronisationMessage& OMK::Type::operator >> ( IncomingSynchronisationMessage& in, CVertexData& vd )
{
	in >> vd.m_ui32VertexPositionCount;

	if(vd.m_ui32VertexPositionCount != 0)
	{
		float32* l_pVertexPositionArray = new float32[vd.m_ui32VertexPositionCount * 3];

		for(uint32 i=0; i<vd.m_ui32VertexPositionCount; i+=3)
		{
			in >> l_pVertexPositionArray[i]
				 >> l_pVertexPositionArray[i+1]
				 >> l_pVertexPositionArray[i+2];
		}

#ifndef USE_STL_CONTAINERS
		vd.m_pVertexPositionArray = l_pVertexPositionArray;
#else
		vd.setVertexPositionArray(vd.m_ui32VertexPositionCount, l_pVertexPositionArray);
		delete l_pVertexPositionArray;
#endif
	}

	in >> vd.m_ui32VertexColorCount;

	if(vd.m_ui32VertexColorCount != 0)
	{
		float32* l_pVertexColorArray = new float32[vd.m_ui32VertexColorCount * 4];

		for(uint32 i=0; i<vd.m_ui32VertexColorCount; i+=4)
		{
			in >> l_pVertexColorArray[i]
				 >> l_pVertexColorArray[i+1]
				 >> l_pVertexColorArray[i+2]
				 >> l_pVertexColorArray[i+3];
		}

#ifndef USE_STL_CONTAINERS
		vd.m_pVertexColorArray = l_pVertexColorArray;
#else
		vd.setVertexColorArray(vd.m_ui32VertexColorCount, l_pVertexColorArray);
		delete l_pVertexColorArray;
#endif
	}

  return in;
}*/
