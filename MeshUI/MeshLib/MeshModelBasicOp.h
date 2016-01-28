/* ================== Library Information ================== */
// [Name] 
// MeshLib Library
//
// [Developer]
// Xu Dong
// State Key Lab of CAD&CG, Zhejiang University
// 
// [Date]
// 2005-08-05
//
// [Goal]
// A general, flexible, versatile and easy-to-use mesh library for research purpose.
// Supporting arbitrary polygonal meshes as input, but with a 
// primary focus on triangle meshes.

/* ================== File Information ================== */
// [Name]
// MeshModelBasicOp.h
//
// [Developer]
// Xu Dong
// State Key Lab of CAD&CG, Zhejiang University
// 
// [Date]
// 2005-08-05
//
// [Goal]
// Defining the basic operations of the kernel components of the mesh model
// Including predictions, queries, Euler operations, etc
//
// This class is responsible for the manipulations of the kernel components and states



#include "MeshModelKernel.h"
#include "MeshModelAuxData.h"
#include "Utility.h"
#pragma once

using namespace std;


#define INFINITE_DISTANCE   1.0e30



class MeshModelBasicOp
{
private:
    MeshModelKernel* kernel;
    MeshModelAuxData* auxdata;
    Utility util;
        vector<bool> m_VertexFlag;
        vector<bool> m_FaceFlag;

public:
    // Constructor
    MeshModelBasicOp();

    // Destructor
    ~MeshModelBasicOp();

    // Initializer
    void ClearData();
    void AttachKernel(MeshModelKernel* pKernel);
    void AttachAuxData(MeshModelAuxData* pAuxData);

    // Vertex information calculation
    void CalAdjacentInfo(); // Calculate the adjacent information for each vertex
    void CalVertexNormal(); // Calculate normal vector of all vertices
    void CalVertexNormal(IntArray& arrIndex);   // Calculate normal vector of selected vertices
        void CalVertexCurvature();   // Calculate vertices Curvature

    // Face information calculation
    void CalFaceNormal();   // Calculate normal vector of all faces
    void CalFaceNormal(IntArray& arrIndex);     // Calculate normal vector of selected faces
        void CalFaceBaryCenter();   // Calculate barycenter of all faces
        void CalFaceArea();
        
    // Edge information calculation, optional functions
    void CreateHalfEdge();  // Create halfedge, optional function
    void CalHalfEdgeInfo(); // Calculate the halfedge information, optional function
        void CalDihedralAngle(); // Calculate the dihedral angle information, optional function
    void CalEdgeInfo(); // Calculate the edge information, optional function
    // Model information calculation
    void CalBoundingBox();      // Bounding box calculation
    void CalBoundingBox(IntArray& arrIndex);   // Update bounding box according to updated vertex coords
    void CalComponentInfo();    // Component calculation

    // Manifold functions
    void SortAdjacentInfo();    // Make sure the 1-ring neighbors are CCW order
    void CalBoundaryInfo();     // Boundary calculation

    // Model analysis functions
    int AdjFaceNum(VertexID vID, VertexID vID2);
    void TopologyAnalysis();
    
public:
    // Topology analisis and initialization
    void InitModel();   // Analyzing and initializing the topological structure of the mesh model

    // Euler operations

    // Shortest path between two given vertices
    void GetShortestPath(VertexID vStart, VertexID vEnd, IndexArray& Path);

    // Geodesic distance functions
    double DistanceFromSeeds(IndexArray& Seeds, DoubleArray& VtxDist, double distance = INFINITE_DISTANCE);
    double DistanceFromSeeds2(IndexArray& Seeds, DoubleArray& VtxDist, DoubleArray& FaceDist, double distance = INFINITE_DISTANCE);
    double DistanceFromSeeds3(IndexArray& Seeds, DoubleArray& FaceDist, double distance = INFINITE_DISTANCE);
    double DistanceFromSeeds4(IndexArray& Seeds, IndexArray& NeiVtx, DoubleArray& NeiVtxDist, double distance = INFINITE_DISTANCE);

    // Flood fill from a seed to marked boundary
    void SurfaceFloodFillVertex(VertexID vID, IndexArray& FillVtx, IndexArray& FillFace, IndexArray& SelectedVtx);
    void SurfaceFloodFillFace(FaceID fID, IndexArray& FillVtx, IndexArray& FillFace, IndexArray& SelectedFace);

    // Get the neighbors of a given face
    void GetFaceNeighborhood(FaceID fID, IndexArray& NeiFace);

    // Get the neighbors of a given vertex inside a circle with a given radius
    void GetNeighborhood(VertexID vID, IndexArray& NeiVtx, IndexArray& NeiFace, double radius, bool useFactor = true);
    void GetNeighborhoodVertex(int vID, size_t neighRingSize, bool onlyRing, vector<int>& neighVIDs);
    void ReRangeNeighRing(vector<int>& neighRingVtx);
        void GetNeighborhoodFace(int vID, size_t neighRingSize, vector<int>& neighFIDs);

    // Get the nearest vertex in a given face for a give position
    void GetNearestVertex(FaceID fID, Coord pos, VertexID& vID);
    void GetNearestVertex(Coord& pos, size_t& vID);

    //
    inline bool IsValidVertexIndex(VertexID vID);
    inline bool IsValidFaceIndex(FaceID fID);
    inline bool IsBoundaryVertex(VertexID vID);
    inline bool IsBoundaryFace(FaceID fID);

    // Get the multiply factor for any distance
    double GetDistanceFactor();

    // Get the average edge length
    double GetAvgEdgeLength();

    // Discrete differential operator
    void CalMeanCurvature(CoordArray& VtxMeanCurv);

    // Get the boundary index and the loop index
    bool GetBdyIndex(VertexID vID, BdyID& bdyID, Index& idx);

    // Get the adjacent face(s) of the edge (vID1, vID2)
    void GetAdjacentFace(VertexID vID1, VertexID vID2, FaceID& fID1, FaceID& fID2);

    // Get the adjacent vertices of the edge (vID1, vID2)
    void GetAdjacentVertex(VertexID vID1, VertexID vID2, VertexID& vID3, VertexID& vID4);

        // Get the opposite vertices of the edge (eID1, eID2)
        void GetEdgeOppositeVertexID(VertexID eID1, VertexID eID2, VertexID& vID1, VertexID& vID2);

        void GetVertexOppositeEdge(VertexID cVID, FaceID fID, VertexID& edgeV1, VertexID& edgeV2);
        void GetEdgeOppositeFace(VertexID vID1, VertexID vID2, FaceID& fID, FaceID& oppfID);
        void GetEdgeOppositeVertexID(Edge edge, FaceID vID, VertexID& oppVID);

        void CalBaryCentricCoord(FaceID fID, Coord& pos, Coord& bCenter);

        double GetBaryAdjFaceArea(VertexID vID);
};

//////////////////////////////////////////////////////////////////////////
// Inline functions
//////////////////////////////////////////////////////////////////////////

bool MeshModelBasicOp::IsValidVertexIndex(VertexID vID)
{
    if(vID < 0  || vID >= (int) kernel->GetVertexInfo().GetCoord().size())
        return false;
    
    return (!util.IsSetFlag(kernel->GetVertexInfo().GetFlag()[vID], FLAG_INVALID));
}

inline bool MeshModelBasicOp::IsValidFaceIndex(FaceID fID)
{
    if(fID < 0  || fID >= (int) kernel->GetFaceInfo().GetIndex().size())
        return false;
    
    return (!util.IsSetFlag(kernel->GetFaceInfo().GetFlag()[fID], FLAG_INVALID));
}

inline bool MeshModelBasicOp::IsBoundaryVertex(VertexID vID)
{
    return (util.IsSetFlag(kernel->GetVertexInfo().GetFlag()[vID], VERTEX_FLAG_BOUNDARY));
}

inline bool MeshModelBasicOp::IsBoundaryFace(FaceID fID)
{
    return (util.IsSetFlag(kernel->GetFaceInfo().GetFlag()[fID], FACE_FLAG_BOUNDARY));
}