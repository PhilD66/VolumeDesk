////////////////////////////////////////////////////////////////////////////////
//  Title       : CubeLUT.h
//  Description : Class definition for CCubeFaceLUT.
//					Provides a lookup table for faces and normals used in the
//					marching cubes algorithm.
//
//  Author      : Phil.D
//  Date        : 03 February 2002
//  Changes     : 
//               $CHANGE$
////////////////////////////////////////////////////////////////////////////////
//                                                                              
//  Copyright (c) Aicom Limited 2002                                            
//                                                                              
//  This software is the property of Aicom Limited and may be used and          
//  copied only as specifically permitted under written licence agreement.      
//                                                                              
//  Whether this software is copied in whole or in part and whether this        
//  software is copied in original or modified form, ALL COPIES OF THIS         
//  SOFTWARE MUST DISPLAY THIS NOTICE OF COPYRIGHT AND OWNERSHIP IN FULL.       
//                                                                              
////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
//
//       Vertex ordering              vector scaling
//
//          7-------6                1
//         /|      /|                |
//        / |     / |                |     1.0
//       3--+----2  |               0.5   /
//       |  4----+--5                |  0.5
//       | /     | /                 | /
//       |/      |/                 0.0---0.5---1.0
//       0-------1
//
//////////////////////////////////////////////////////////////////

#ifndef	_FACELIST_77011223403204583487
#define	_FACELIST_77011223403204583487

typedef struct
{
	int		number_of_faces;
	float	vertices[64];

}	 TFaceList;

typedef struct
{
	unsigned long	number_of_normals;
	float			vertices[64];

}	 TNormalsList;

class CCubeFaceLUT
{
public:
	TFaceList		*GetFaces( int CubeIndex );
	void			DumpFaces();
	TNormalsList	*GetNormals( int CubeIndex );
	int				getIndexFromCubeValues( float *pValues, float threshold );
	int				getFaceCount( float *pDataValues, float threshold, int xDim, int yDim, int zDim );
	int 			renderVolume( float *pValues, float threshold, int xDim, int yDim, int zDim, float *pVertexValues, float * pNormalValues );
	int				CheckFaceNormal(float *pTriangles, float *pNormals);
	void			CheckAllNormals();
protected:

};


#endif
