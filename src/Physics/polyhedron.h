#ifndef __POLYHEDRON_H
#define __POLYHEDRON_H

#include <vector>
#include <set>
#include <map>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include "Physics/winding.h"
#include "Physics/side.h"
#include "Math/Vector3D.h"
#include "Math/BipartiteGraph.h"

namespace Sun
{
    class Polyhedron
    {
    public:
        std::vector<vec3> pts;
        std::vector<Side> sides;
        //判断某个winding是否穿过平面
        int sideCross(int index, const Plane& plane) const;
        inline Polyhedron& getPolyhedronRef() {
            return *this;
        }


    };

    struct HalfEdge;
    struct Face;
    struct PVertex;

    struct HalfEdge
    {
        PVertex* origin = nullptr;
        HalfEdge* twin = nullptr;
        Face* incidentFace = nullptr;
        HalfEdge* prev = nullptr;
        HalfEdge* next =nullptr;

        //for test
        int _number = 0;
    };

    struct Face
    {
        HalfEdge* outEdge = nullptr;
        //实际可能有多个inner指针，如果不考虑拓扑上的孔，可以认为只有一个
        HalfEdge* innerEdge = nullptr;
        Plane plane;

        //for test
        int _number = 0;
    };

    struct PVertex:public vec3
    {
        HalfEdge* incidentEdge = nullptr;

        //for test
        int _number = 0;
    };

    //通用表示的多面体，参考了计算几何的doubly connected edge list表示法
    //负责管理内存
    class PolyhedronG
    {
    public:
        ~PolyhedronG();
        //计算若干连通的faces的boundaries，并按照逆时针的顺序输出,同时把face相关的节点和edge
        //除了boundary上的全部删除
        std::set<HalfEdge*> calBoundaryAndEraseFaces_old(const std::set<Face*>& faces);

        //删除一个face,同时修改周其关联的halfEdge的方向
        //输出删除的face的boundary上的一个halfedge
        HalfEdge* eraseFace(Face* face);

        HalfEdge* eraseFacesAndcalBoundary(const std::set<Face*>& faces);
        HalfEdge* eraseFacesAndcalBoundary(const std::unordered_set<Face*>& faces);

        std::set<HalfEdge*> calBoundary(const std::set<Face*>& faces);
        std::set<HalfEdge*> calBoundary(const std::unordered_set<Face*>& faces);
        std::vector<HalfEdge*> calBoundaryInOrder(const std::unordered_set<Face*>& faces);
        HalfEdge* findBoundaryEdge(const std::unordered_set<Face*>& faces);

        //删除一条edge,会把输入半边和twin半边全删除，并合并edge两边的面，目前默认把右边的面合并到左边，也就是删除右边的面
        void eraseEdge(HalfEdge* edge);
      
        //通过一个face，输出其周围的halfEdge*
        std::set<HalfEdge*> getOuterEdges(Face* face);
        std::set<HalfEdge*> getRoundEdges(HalfEdge* edge);
        bool checkSameDirectionRoundEdges(HalfEdge* edge);
        bool faceCheck(Face* face);
        //按照面输出结构
        std::string outputStruct();       
        std::string outputStruct2();
        //输出完整拓扑结构
        std::string outputTopology();

        //初始化2分图 bipartite 
        void reInitConflictGraph(const std::set<PVertex*>& ss);
        //从2分图获取顶点相关的面
        std::unordered_set<Face*> getFacesFromConflictGraph(PVertex* vt);
        //更新2分图,输出boundary和新增face的对应map
        std::map<HalfEdge*, Face*> updateConflictGraph(PVertex* vt, const std::set<HalfEdge*>& boundary);

        //获取顶点相关的面
        std::unordered_set<Face*> getFaces(const PVertex* vt);
    public:
        void addNewVertex(PVertex* vertex);
        void addNewFace(Face* face);
        void addNewHalfEdge(HalfEdge* halfedge);

        void generateSimplex(const vec3& p1, const vec3& v2, const vec3& v3, const vec3& v4);
        //algorithm of generate convex hulls 
        //凸包生成算法
        bool generateConvexHull(const std::vector<vec3>& pts);

        //迭代凸包生成
        void addPointAndformNewConvexHull(PVertex* pt);
    public:
        std::set<PVertex*> _vertice;
        std::set<Face*> _faces;
        std::set<HalfEdge*> _edges;
        //for test
        int _numberclock = 0;
        int _numEdgeClock = 0;
        int _numFaceClock = 0;

        //tmp variables
        BipartiteGraph<Face, PVertex> conflictGraph;
    };

    //test，检测一个多胞体是否满足基本条件
    bool testPolytope(const PolyhedronG& polytope);
    bool testFace(Face* face);

    //test edge
    bool testEdge(const PolyhedronG& polytope, HalfEdge* edge);

    std::string outputFaces(const std::unordered_set<Face*>& faces);
}

#endif