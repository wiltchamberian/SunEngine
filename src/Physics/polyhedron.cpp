#include "polyhedron.h"
#include "Math/MathUtils.h"
#include "Math/Algorithm.h"
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include "assert.h"
#include "Math/Graph.h"
#include "windows.h"

//for test
#include <fstream>
static std::fstream g_fs("./polyhedronG.txt",std::ios::ate|std::ios::out);
static std::fstream g_fs2("./eraseFaces.txt", std::ios::ate | std::ios::out);
static std::fstream g_topo("./polyhedronG_Topo.txt", std::ios::ate | std::ios::out);
static std::set<Sun::Face*> g_testedFaces;

//#define USE_CONFLICT_GRAPH

namespace Sun
{
    int Polyhedron::sideCross(int index, const Plane& plane) const
    {
        int side = 0;
        for (int i = 0; i < sides[index].indexes.size(); ++i) {
            const vec3& pt = pts[sides[index].indexes[i]];
            side |= plane.pointSide(pt);
        }
        return side;
    }

    void PolyhedronG::addNewHalfEdge(HalfEdge* halfedge)
    {
        _edges.insert(halfedge);
        halfedge->_number = _numEdgeClock;
        _numEdgeClock++;
    }

    //生成单纯形，输入需要保证4个点不共面，任意3点不共线条件，内部不做判定
    void PolyhedronG::generateSimplex(const vec3& p0, const vec3& p1, const vec3& p2, const vec3& p3) {
        //以p0为基准点，构建三个矢量
        vec3 v1 = p1 - p0;
        vec3 v2 = p2 - p0;
        vec3 v3 = p3 - p0;

        std::vector<PVertex*> vertice;
        std::vector<HalfEdge*> edges;
        std::vector<Face*> faces;

        vec3 c = v1.crossProduct(v2);
        //计算v3在c上的投影有向长度
        float f = directionProject(v3, c);
        if (Math::equal(f, 0.0)) return;

        for (int i = 0; i < 4; ++i) {
            vertice.push_back(new PVertex);
        }
        vertice[0]->set(p0.x, p0.y, p0.z);
        vertice[1]->set(p1.x, p1.y, p1.z);
        vertice[2]->set(p2.x, p2.y, p2.z);
        vertice[3]->set(p3.x, p3.y, p3.z);
        //6个正向边 +6个负向边;
        for (int i = 0; i < 12; ++i) {
            edges.push_back(new HalfEdge);
        }
        //4个面
        for (int i = 0; i < 4; ++i) {
            faces.push_back(new Face);
        }
        //说明从四面体外部看，p1->p2->p3为逆时针
        //(这里处理f>0的逻辑，但是为了复用代码，f<0也复用这段逻辑)
        if (f > 0 || f<0) {
            /*
                   p3
                  /|\
                 / | \
                /  p0 \
              p1-------p2
            
            */
            //p0->p2->p1为第一个面 ,edges[0]:p0->p2
            faces[0]->outEdge = edges[0];
            //p0->p1->p3为第二个面 edges[3]:p0->p1
            faces[1]->outEdge = edges[4];
            //p1->p2->p3为第三个面 ,edges[6]:p1->p2
            faces[2]->outEdge = edges[6];
            //p0->p3->p2为第四个面, edges[9]:p0->p3
            faces[3]->outEdge = edges[9];

            //p0->p2
            edges[0]->origin = vertice[0];
            edges[0]->prev = edges[2];
            edges[0]->next = edges[1];
            edges[0]->twin = edges[11];
            edges[0]->incidentFace = faces[0];
            //p2->p1
            edges[1]->origin = vertice[2];
            edges[1]->prev = edges[0];
            edges[1]->next = edges[2];
            edges[1]->twin = edges[6];
            edges[1]->incidentFace = faces[0];
            //p1->p0
            edges[2]->origin = vertice[1];
            edges[2]->prev = edges[1];
            edges[2]->next = edges[0];
            edges[2]->twin = edges[3];
            edges[2]->incidentFace = faces[0];
            //p0->p1
            edges[3]->origin = vertice[0];
            edges[3]->prev = edges[5];
            edges[3]->next = edges[4];
            edges[3]->twin = edges[2];
            edges[3]->incidentFace = faces[1];
            //p1->p3
            edges[4]->origin = vertice[1];
            edges[4]->prev = edges[3];
            edges[4]->next = edges[5];
            edges[4]->twin = edges[8];
            edges[4]->incidentFace = faces[1];
            //p3->p0
            edges[5]->origin = vertice[3];
            edges[5]->prev = edges[4];
            edges[5]->next = edges[3];
            edges[5]->twin = edges[9];
            edges[5]->incidentFace = faces[1];
            //p1->p2
            edges[6]->origin = vertice[1];
            edges[6]->prev = edges[8];
            edges[6]->next = edges[7];
            edges[6]->twin = edges[1];
            edges[6]->incidentFace = faces[2];
            //p2->p3
            edges[7]->origin = vertice[2];
            edges[7]->prev = edges[6];
            edges[7]->next = edges[8];
            edges[7]->twin = edges[10];
            edges[7]->incidentFace = faces[2];
            //p3->p1
            edges[8]->origin = vertice[3];
            edges[8]->prev = edges[7];
            edges[8]->next = edges[6];
            edges[8]->twin = edges[4];
            edges[8]->incidentFace = faces[2];
            //p0->p3
            edges[9]->origin = vertice[0];
            edges[9]->prev = edges[11];
            edges[9]->next = edges[10];
            edges[9]->twin = edges[5];
            edges[9]->incidentFace = faces[3];
            //p3->p2
            edges[10]->origin = vertice[3];
            edges[10]->prev = edges[9];
            edges[10]->next = edges[11];
            edges[10]->twin = edges[7];
            edges[10]->incidentFace = faces[3];
            //p2->p0
            edges[11]->origin = vertice[2];
            edges[11]->prev = edges[10];
            edges[11]->next = edges[9];
            edges[11]->twin = edges[0];
            edges[11]->incidentFace = faces[3];

            vertice[0]->incidentEdge = edges[0];
            vertice[1]->incidentEdge = edges[6];
            vertice[2]->incidentEdge = edges[11];
            vertice[3]->incidentEdge = edges[8];
        }
        //说明从四面体外部看，p1->p2->p3为逆时针
        //此时逻辑和上面相比就是从p0->p1->p2->p3变为p0->p3->p2->p1
        //因此，只需交换p1和p3坐标即可
        if (f < 0) {
            std::swap(vertice[1]->x,vertice[3]->x);
            std::swap(vertice[1]->y, vertice[3]->y);
            std::swap(vertice[1]->z, vertice[3]->z);
        }
        //计算平面参数
        faces[0]->plane = Plane(*vertice[0], *vertice[2], *vertice[1]);
        faces[1]->plane = Plane(*vertice[0], *vertice[1], *vertice[3]);
        faces[2]->plane = Plane(*vertice[1], *vertice[2], *vertice[3]);
        faces[3]->plane = Plane(*vertice[0], *vertice[3], *vertice[2]);
        //_faces =std::set<Face*>(faces.begin(), faces.end());
        //_edges = std::set<HalfEdge*>(edges.begin(), edges.end());
        //output._vertice = std::set<Vertex*>(vertice.begin(), vertice.end());
        for (auto& aa : vertice) {
            addNewVertex(aa);
        }
        for (auto& h : edges) {
            addNewHalfEdge(h);
        }
        for (auto& it : faces) {
            addNewFace(it);
        }
        return;
    }

    void PolyhedronG::reInitConflictGraph(const std::set<PVertex*>& ss)
    {
        conflictGraph.clear();
        for (auto& it : ss) {
            conflictGraph.addVertex(it);
        }
        for (auto& it : _faces) {
            conflictGraph.addFace(it);
            for (auto& pt : ss) {
                //判断点是否在平面正面
                if (it->plane.pointSide(*pt) == PLANE_FRONT) {
                    conflictGraph.addRelation(it, pt);
                    //for test
                    if (it->outEdge == nullptr) {
                        assert(false);
                    }
                }
            }
        }
    }

    //找出顶点的可视面
    std::unordered_set<Face*> PolyhedronG::getFaces(const PVertex* vt)
    {
        std::unordered_set<Face*> set;

        for (auto& it : _faces) {
            if (it && it->plane.pointSide(*vt) == PLANE_FRONT) {
                set.insert(it);
            }
        }

        return std::move(set);
    }

    std::unordered_set<Face*> PolyhedronG::getFacesFromConflictGraph(PVertex* vt)
    {
        return conflictGraph.outputFaces(vt);
    }

    std::map<HalfEdge*, Face*> PolyhedronG::updateConflictGraph(PVertex* vt ,const std::set<HalfEdge*>& boundary)
    {
        //faces将被删除，并新加入一个顶点，对2分图的影响是:
            //1该顶点从图中删除;
            //2这些面将被删除
            //3新加入的三角面分2种情形:
            //3-1:和边缘另一边的面平行，则该面合并到原来的面即可（顶点集不用更改)
            //3-2:不平行，则加入新面，该面的顶点集从 edge关联的2面计算得到;
            //我们这里修改一下原著的算法，先更新2分图，而更新2分图时先将删除操作移动到最后再修改多面体（会删除一些face);
            //从而保证修改2分图时能够应用到正确的face
            //新face集合
        std::map<HalfEdge*, Face*> edge_face_map;
        //平行标记
        std::set<HalfEdge*> parall_face_edge_set;
        /************************2分图相关操作*****************************/
        //新加入面,每条边关联一个三角面(新增2分图操作）
        for (auto& edge : boundary) {
            Face* face = new Face();
            if (edge && edge->origin && edge->next) {
                //此时edge仍然是逆时针的
                face->plane = Plane(*edge->origin, *edge->twin->origin, *vt);

                //判定face和edge另一边的face是否平行
                Face* incidentFace = edge->twin->incidentFace;
                //平行
                if (face->plane.isParell(incidentFace->plane)) {
                    //不用更新图
                    parall_face_edge_set.insert(edge);
                }
                //不平行
                else {
                    //从该edge关联的原来两个面中，筛选出conflit pts;
                    Face* face1 = edge->incidentFace;
                    Face* face2 = edge->twin->incidentFace;
                    if (conflictGraph.isFaceExist(face1)) {
                        std::unordered_set<PVertex*>& ref = conflictGraph.getVerticesSetRef(face1);
                        for (auto& vt : ref) {
                            if (face->plane.pointSide(*vt) == PLANE_FRONT) {
                                conflictGraph.addRelation(face, vt);
                                //for test
                                if (face->outEdge == nullptr) {
                                    //assert(false);
                                }
                            }
                        }
                    }
                    if (conflictGraph.isFaceExist(face2)) {
                        std::unordered_set<PVertex*> ref = conflictGraph.getVerticesSetRef(face2);
                        for (auto& vt : ref) {
                            if (face->plane.pointSide(*vt) == PLANE_FRONT) {
                                conflictGraph.addRelation(face, vt);
                                //for test
                                if (face->outEdge == nullptr) {
                                    //assert(false);
                                }
                            }
                        }
                    }
                    
                }
                //for test 
                if (edge_face_map.find(edge) != edge_face_map.end()) {
                    assert(false);
                }
                edge_face_map.insert(std::make_pair(edge, face));

                //edge->incidentFace = face;
            }
            else {
                delete face;
                assert(false);
            }
        }
        //删除面和顶点
        for (auto& face : _faces) {
            conflictGraph.eraseFace(face);
        }
        conflictGraph.eraseVertex(vt);

        return std::move(edge_face_map);
    }

    bool PolyhedronG::generateConvexHull(const std::vector<vec3>& pts)
    {
        //找到4个点，构成单纯形
        if (pts.size() < 4) return false;
        std::vector<int> vec;
        bool found = false;
        for (int i = 1; i < pts.size(); ++i) {
            if (!(pts[0] == pts[i])) {
                for (int j = 1; j < pts.size(); ++j) {
                    if (j == i)continue;
                    vec3 v1 = pts[i] - pts[0];
                    vec3 v2 = pts[j] - pts[0];
                    if (!parallel(v1,v2)) {
                        for (int k = 1; k < pts.size(); ++k) {
                            if (k == i || k == j) continue;
                            vec3 v3 = pts[k] - pts[0];
                            vec3 vc = v1.crossProduct(v2);
                            if (!Math::equal(vc.dotProduct(v3), 0.0)) {
                                vec = { 0,i,j,k };
                                found = true;
                                goto label;
                            }
                        }
                    }
                }
            }
        }
    label:
        if (!found) return false;
        generateSimplex(pts[vec[0]], pts[vec[1]], pts[vec[2]], pts[vec[3]]);

        //for test
        assert(testPolytope(*this)==true);
        //检测相邻平面的法向关系是否正确
        for (auto& face : _faces) {
            faceCheck(face);
        }

        //将剩余点放入集合ss
        std::set<PVertex*> ss;
        for (int i = 0; i < pts.size(); ++i) {
            if (i == vec[0] || i == vec[1] || i == vec[2] || i == vec[3])
                continue;
            PVertex* v = new PVertex;
            v->set(pts[i].x, pts[i].y, pts[i].z);
            ss.insert(v);
        }
        
#ifdef USE_CONFLICT_GRAPH
        //构造2分图
        reInitConflictGraph(ss);
#endif

        //循环迭代更新
        int roundTime = 0;
        for (auto& it : ss) {
            roundTime += 1;
            addPointAndformNewConvexHull(it);
        }

        return true;
    }

    void PolyhedronG::addPointAndformNewConvexHull(PVertex* it)
    {  
#ifdef USE_CONFLICT_GRAPH
        std::unordered_set<Face*> faces = conflictGraph.outputFaces(it);
        //如果视觉点能看到的faces为空，则直接将该点从图中移除即可
        if (faces.empty())
            conflictGraph.eraseVertex(it);
#else
        std::unordered_set<Face*> faces = getFaces(it);
#endif
        //for test
        std::unordered_set<Face*> visibleFaces = getFaces(it);
        if (visibleFaces != faces) {
            assert(false);
        }

        if (faces.empty()) {
            return;
        }

#ifdef USE_CONFLICT_GRAPH
        std::set<HalfEdge*> boundary = calBoundary(faces);
        std::map<HalfEdge*, Face*> edge_face_map = updateConflictGraph(it, boundary);
#else
        HalfEdge* edge = findBoundaryEdge(faces);
        assert(edge != nullptr);
#endif

        //删除面，并得到新边界
        //for test
        assert(testPolytope(*this) == true);
        /*HalfEdge* edge = */eraseFacesAndcalBoundary(faces);
        //for test
        std::set<HalfEdge*> roundEdges = getRoundEdges(edge);
        //assert(roundEdges == boundary);
        assert(checkSameDirectionRoundEdges(edge));
        edge->next; edge->origin; if (edge == nullptr) assert(false);

        g_topo << "afterErase:\n" << outputTopology(); g_topo.flush();
        //

        if (edge == nullptr)return;
        //新增面
        HalfEdge* move = edge;
        HalfEdge* tmp_half_next = nullptr;
        HalfEdge* tmp_half_prev = nullptr;
        HalfEdge* first_next = nullptr;
        HalfEdge* last_move = nullptr;
        //对boundary遍历，同时新增面
        //for test
        std::vector<Face*> tmpFaces;
        int roundTime = 0;
        do {
            roundTime += 1;
            HalfEdge* wn = move->next;
#ifdef USE_CONFLICT_GRAPH
            //for test
            if (edge_face_map.find(move) == edge_face_map.end()) {
                assert(false);
            } //

            Face* face = edge_face_map[move];
#else
            Face* face = new Face();
            //此时edge已经翻转了
            //第三个输入参数无论用move->next->origin或move->twin->origin都有问题;
            assert(move->origin == move->twin->origin);
            face->plane = Plane(*move->origin, *it, *move->twin->next->origin);
#endif
            //for test
            tmpFaces.push_back(face);

            //创建三角形的两个新边
            HalfEdge* half_next = new HalfEdge();
            HalfEdge* half_prev = new HalfEdge();
            //_edges.insert(half_prev);
            //_edges.insert(half_next);
            //_faces.insert(face);
            addNewHalfEdge(half_prev);
            addNewHalfEdge(half_next);
            addNewFace(face);

            if (move == edge)
                first_next = half_next;

            //翻转前的起始点和终止点
            PVertex* start = move->origin;
            //Vertex* end = move->next->origin; //此时move->next可能已经被翻转过了(1)
            PVertex* end = move->twin->next->origin;//最好断言一下twin和move同向
            //翻转该edge;
            move->origin = move->twin->next->origin; //同(1),最好断言一下twin和move同向
            //翻转后的起始点和终止点
            std::swap(start, end);

            move->incidentFace = face;
            move->next = half_next;
            move->prev = half_prev;

            half_next->next = half_prev;
            half_next->prev = move;
            half_next->incidentFace = face;
            half_next->origin = end;
            //half_next->twin = tmp_half_prev;

            half_prev->prev = half_next;
            half_prev->next = move;
            half_prev->incidentFace = face;
            half_prev->origin = it;

            half_next->twin = tmp_half_prev;
            if (tmp_half_prev != nullptr)
                tmp_half_prev->twin = half_next;
            /*half_prev->twin = tmp_half_next;
            if (tmp_half_prev != nullptr)
                tmp_half_next->twin = half_prev;*/

            face->outEdge = half_next;
            it->incidentEdge = half_prev;

            tmp_half_next = half_next;
            tmp_half_prev = half_prev;
            last_move = move;
            move = wn;
        } while (move != edge);
        if (last_move) {
            last_move->prev->twin = first_next;
            if (first_next)
                first_next->twin = last_move->prev;
        }
        //有些面可能平行，需要把中间的edge删除 FIX ME
        //for (auto& it : parall_face_edge_set) {
        //    eraseEdge(it);
        //}

        //output._vertice.insert(it);
        addNewVertex(it);

        //for test
        g_topo << "final:" << outputTopology(); g_topo.flush();
        g_testedFaces.clear();
        //检测相邻平面的法向关系是否正确
        for (auto& face : _faces) {
            faceCheck(face);
        }

        assert(testPolytope(*this) == true);
    }

    bool testPolytope(const PolyhedronG& polytope)
    {
        bool res = true;
        //检测欧拉公式是否成立 V+F-E=2
        bool ok1 = (polytope._edges.size()%2==0)&&(polytope._vertice.size() + polytope._faces.size() - polytope._edges.size()/2 == 2);
        assert(ok1 == true);
        //bool ok1 = true;
        //检测每个面关联的半边数之和是否等于总边数；且所有这些面关联的所有半边都不同
        //同时检测每个一个面上的半边是否构成双向环形链表
        bool ok2 = true;
        int edgeNum = 0;
        std::set<HalfEdge*> edgeSet;
        for (auto& it : polytope._faces) {
            HalfEdge* w = it->outEdge;
            do {
                ok2 = ok2 && (w->next->prev == w);
                assert(ok2 == true);
                ok2 = ok2 && (w->prev->next == w);
                assert(ok2 == true);
                ok2 = ok2 && (w->twin->origin != w->origin);
                assert(ok2 == true);
                ok2 = ok2 && (w->next->origin == w->twin->origin);
                assert(ok2 == true);
                ok2 = ok2 && (w->prev->twin->origin == w->origin);
                assert(ok2 == true);
                ok2 = ok2 && (w->twin->twin == w);
                assert(ok2 == true);
                ok2 = ok2 && (w->twin->origin != w->origin);
                assert(ok2 == true);
                w = w->next;
                edgeSet.insert(w);
            } while (w != it->outEdge);
        }
        ok2 = ok2 && (edgeSet.size() == polytope._edges.size());
        assert(ok2 == true);
        //检测每个顶点的关联边是否以该顶点为起始点
        bool ok3 = true;
        //for (auto& it : polytope._vertice) {
        //    ok3 = ok3 && (it->incidentEdge->origin == it);
        //    assert(ok3 == true);
        //}

        res = ok1 && ok2 && ok3;
        assert(res == true);
        return res;
    }

    bool testFace(Face* face)
    {
        bool ok2 = true;
        HalfEdge* w = face->outEdge;
        do {
            ok2 = ok2 && (w->next->prev == w);
            assert(ok2 == true);
            ok2 = ok2 && (w->prev->next == w);
            assert(ok2 == true);
            ok2 = ok2 && (w->twin->origin != w->origin);
            assert(ok2 == true);
            ok2 = ok2 && (w->next->origin == w->twin->origin);
            assert(ok2 == true);
            ok2 = ok2 && (w->prev->twin->origin == w->origin);
            assert(ok2 == true);
            ok2 = ok2 && (w->twin->twin == w);
            assert(ok2 == true);
            ok2 = ok2 && (w->twin->origin != w->origin);
            assert(ok2 == true);
            
            HalfEdge* w2 = w->twin;
            do {
                ok2 = ok2 && (w2->next->prev == w2);
                assert(ok2 == true);
                ok2 = ok2 && (w2->prev->next == w2);
                assert(ok2 == true);
                ok2 = ok2 && (w2->twin->origin != w2->origin);
                assert(ok2 == true);
                ok2 = ok2 && (w2->next->origin == w2->twin->origin);
                assert(ok2 == true);
                ok2 = ok2 && (w2->prev->twin->origin == w2->origin);
                assert(ok2 == true);
                ok2 = ok2 && (w2->twin->twin == w2);
                assert(ok2 == true);
                ok2 = ok2 && (w2->twin->origin != w2->origin);
                assert(ok2 == true);
                w2 = w2->next;    
            } while (w2 != w);
            g_testedFaces.insert(w2->incidentFace);
            w = w->next;
        } while (w != face->outEdge);
        g_testedFaces.insert(w->incidentFace);
        return ok2;
    }

    bool testEdge(const PolyhedronG& polytope, HalfEdge* edge)
    {
        int siz = polytope._edges.size();
        HalfEdge* w = edge->next;
        for (int i = 0; i < siz+1; ++i) {
            if (w == edge) {
                return true;
            }
            w = w->next;
        }
        return false;
    }

    std::string outputFaces(const std::unordered_set<Face*>& faces)
    {
        std::stringstream ss;
        int i = 0;
        for (auto& face : faces) {
            ss << "face:" << "(" << face->outEdge->origin->_number << "," <<
                face->outEdge->next->origin->_number << ")\n";

            HalfEdge* halfEdge = face->outEdge;
            HalfEdge* move = halfEdge;
            do {
                ss << "(" << move->origin->_number << ",";
                move = move->next;
                ss << move->origin->_number << ")" << "\n";
            } while (move != halfEdge);
        }
        ss << "\\\\\\\\\\\\\\\\\\\\\\\\\\\\";
        ss << "\n";
        return ss.str();
    }

    PolyhedronG::~PolyhedronG()
    {
        for (auto& it : _vertice) {
            delete it;
        }
        for (auto& it : _faces) {
            delete it;
        }
        for (auto& it : _edges) {
            delete it;
        }
    }

    std::set<HalfEdge*> PolyhedronG::calBoundaryAndEraseFaces_old(const std::set<Face*>& faces)
    {
        std::set<HalfEdge*> boundary;

        //第一步:把faces的所有halfedge和vertices提取出来
        std::vector<HalfEdge*> edges;
        std::set<PVertex*> vertice;
        //boundary上的edges,判定准则为其twin的face不在faces中
        for (auto& it : faces) {
            HalfEdge* halfEdge = it->outEdge;
            HalfEdge* w = halfEdge;
            do {
                if (faces.find(w->twin->incidentFace) == faces.end()) {
                    boundary.insert(w);
                }
                edges.push_back(w);
                vertice.insert(w->origin);
                w = w->next;
            } while (w != halfEdge);
        }

        //第二步:删除所有faces,更新edges
        for (auto& it : faces) {
            //不判定faces是否有效，默认有效
            HalfEdge* halfEdge = it->outEdge;
            HalfEdge* w = halfEdge;
            do {
                w->incidentFace = nullptr;
                w = w->next;
            } while (w != halfEdge);
        }
        for (auto& it : faces) {
            delete it;
            _faces.erase(it);
        }
        
        //第三步:遍历所有halfEdge,更新相关顶点信息，再删除
        for (auto& it : edges) {
            //对于boundary上的edge，需要保留,如果其twin的face不在该face中
            if (boundary.find(it) != boundary.end()) {
                continue;
            }
            //说明该边的邻居已经被删除，则该边在结构中也已经被删除，直接释放
            if (it->twin == nullptr) {
                delete it;
                _edges.erase(it);
                continue;
            }
            //更新顶点信息
            PVertex* vo = it->origin;
            PVertex* vt = it->twin->origin;
            if (vo->incidentEdge == it) {
                vo->incidentEdge = it->twin->next;
                if (it->twin->next == it) {
                    vo->incidentEdge = nullptr;
                }
            }
            if (vt->incidentEdge == it->twin) {
                vt->incidentEdge = it->next;
                if (it->next == it->twin) {
                    vt->incidentEdge = nullptr;
                }
            }
            //更新边连接信息
            it->prev->next = it->twin->next;
            it->twin->next->prev = it->prev;
            it->next->prev = it->twin->prev;
            it->twin->prev->next = it->next;

            //删除第一个halfEdge
            delete it;
            it->twin->twin = nullptr;
            _edges.erase(it);
        }
        //最后删除顶点
        for (auto& it : vertice) {
            if (it->incidentEdge == nullptr) {
                delete it;
                _vertice.erase(it);
            }
        }
        return boundary;
    }

    //删除一个face,同时修改周其关联的halfEdge的方向
    HalfEdge* PolyhedronG::eraseFace(Face* face)
    {
        if (face == nullptr) {
            assert(false);
            return nullptr;
        }
        HalfEdge* output = nullptr;
        //第一步，遍历edge，删除和更新顶点
        HalfEdge* w = face->outEdge;
        //for test
        if (face->outEdge == nullptr) {
            assert(false);
        }
        do {
            if (w->twin->origin == w->origin
                && w->prev->twin == w->twin->prev/* &&w->prev->twin->origin == w->prev->origin*/) {
                if (w->prev->twin->origin != w->twin->prev->origin) {
                    assert(false);
                }
                PVertex* node = w->origin;
                w->origin = nullptr;
                w->twin->origin = nullptr;
                _vertice.erase(node);
                delete node;
            }
            else if (w->twin->origin == w->origin
                && w->prev->twin != w->twin->prev/*w->prev->twin->origin != w->prev->origin*//*w->origin == w->prev->twin->origin*/) {
                if (w->origin->incidentEdge == w
                    || w->origin->incidentEdge == w->twin) {
                    if (w->prev->twin->origin == w->origin)w->origin->incidentEdge = w->prev->twin;
                    else w->origin->incidentEdge = w->prev->twin->next;
                }
            }
            else if (w->prev->origin == w->prev->twin->origin
                && w->origin != w->twin->origin) {
                //do nothing
            }
            w = w->next;
        } while (w != face->outEdge);
        //删除和更新边的信息(w->next有可能回不去了，如果初始的边被删除，按照逻辑，最后一个边
        //的next将不是初始的边，所以提前记录最后一个边的指针
        HalfEdge* last = w->prev;
        //for test
        int enter = 0;
        while (true) {
            HalfEdge* w_n = w->next;
            //由于w可能被以下操作改动（比如释放)，需要记录一下w的正确状态，以便和last比较
            HalfEdge* cur_real_w = w;
            //反向边
            if (w->twin->origin != w->origin) {
                //去除点引用
 

                w->origin = w->twin->origin;
                std::swap(w->next, w->prev);
                output = w;
            }
            //同向边
            else {
                enter += 1;
                if ((w->prev!=nullptr && w->twin->prev!=nullptr)&&(w->prev->twin != w->twin->prev)) {
                    //w->twin->prev->next = w->prev;
                    //w->prev->next = w->twin->prev;        
                    if (w->prev->origin == w->origin)//说明前条edge已经翻转过
                    {
                        w->twin->prev->next = w->prev;
                        w->prev->prev = w->twin->prev;
                    }
                    else {
                        w->twin->prev->next = w->prev;
                        w->prev->next = w->twin->prev;
                    }
                }
                if ((w->next!=nullptr && w->twin->next!=nullptr)&&(w->next->twin != w->twin->next)) {
                    //w->twin->next->prev = w->next;
                    //w->next->prev = w->twin->next;
                    if (w->next->origin == w->twin->next->origin) {//说明后条边没翻转过
                        w->twin->next->prev = w->next;
                        w->next->prev = w->twin->next;
                    }
                    //翻转过
                    else {
                        w->twin->next->prev = w->next;
                        w->next->next = w->twin->next;
                    }
                }
                //删除edge前，消去face的引用
                //if (w->twin->incidentFace && w->twin->incidentFace->outEdge == w->twin) {
                //    w->twin->incidentFace->outEdge = w->twin->prev;
                //}
                //if (w->incidentFace && w->incidentFace->outEdge == w) {
                //    w->incidentFace->outEdge = w->prev;
                //}

                //消去edge引用
                if ((w->twin->prev!=nullptr)&&(w->twin->prev->next == w->twin)) {
                    w->twin->prev->next = nullptr;
                }
                if ((w->prev!=nullptr)&&(w->prev->next == w)) {
                    w->prev->next = nullptr;
                }
                //是否要消去后面引用?
                //todo
 
                _edges.erase(w->twin);
                delete w->twin;
                w->twin = nullptr;
                _edges.erase(w);
                delete w;
                w = nullptr;
            }
            if (w) {
                w->incidentFace = nullptr;
            }
            if (cur_real_w == last)break;
            w = w_n;
        };
        //删除面
        //for test
        //g_fs << outputStruct2(); g_fs.flush();
        _faces.erase(face); 
        delete face;

        if (output == nullptr) {
            //assert(false);
        }

        return output;
    }

    HalfEdge* PolyhedronG::eraseFacesAndcalBoundary(const std::set<Face*>& faces)
    {
        HalfEdge* output = nullptr;
        for (auto& it : faces) {
            output = eraseFace(it);
            //check edge

        }
        return output;
    }

    HalfEdge* PolyhedronG::eraseFacesAndcalBoundary(const std::unordered_set<Face*>& faces)
    {
        HalfEdge* output = nullptr;
        //打印输出PolyhedronG的结构
        ::OutputDebugStringA(outputStruct2().c_str());
        g_fs << outputStruct2();
        g_fs.flush();
        g_fs2 << "this is a new start!\n"; g_fs2 << outputFaces(faces); g_fs2.flush();
        HalfEdge* tmp = nullptr;
        for (auto& it : faces) {
            //for test
            if (_faces.find(it) == _faces.end()) {
                assert(false);
            }

            tmp = eraseFace(it);
            if (tmp != nullptr) //可能为空，比如网格周边被清空的情形，再删除该网格即会返回空
                output = tmp;
            ::OutputDebugStringA(outputStruct2().c_str());
            g_fs << outputStruct2();
            g_fs.flush();
        }
        return output;
    }

    std::set<HalfEdge*> PolyhedronG::calBoundary(const std::set<Face*>& faces)
    {
        std::set<HalfEdge*> res;
        for (auto& it : faces) {
            HalfEdge* edge = it->outEdge;
            HalfEdge* w = edge;
            do {
                if (faces.find(w->twin->incidentFace) == faces.end()) {
                    res.insert(w);
                }
                w = w->next;
            } while (w != edge);
        }
        return res;
    }

    std::set<HalfEdge*> PolyhedronG::calBoundary(const std::unordered_set<Face*>& faces)
    {
        std::set<HalfEdge*> res;
        for (auto& it : faces) {
            HalfEdge* edge = it->outEdge;
            HalfEdge* w = edge;
            do {
                if (faces.find(w->twin->incidentFace) == faces.end()) {
                    res.insert(w);
                }
                w = w->next;
            } while (w != edge);
        }
        return res;
    }

    std::vector<HalfEdge*> PolyhedronG::calBoundaryInOrder(const std::unordered_set<Face*>& faces)
    {
        //找到第一个在边界的face,之后通过其边界edge,顺序输faces的boundary
        std::vector<HalfEdge*> vec;
        HalfEdge* boundaryEdge = nullptr;
        for (auto& face : faces) {
            HalfEdge* edge = face->outEdge;
            HalfEdge* w = edge;
            do {
                if (faces.find(w->twin->incidentFace) == faces.end()) {
                    boundaryEdge = w;
                    break;
                }
                w = edge->next;
            } while (w != edge);
        }
        if (boundaryEdge == nullptr) {
            assert(false); return {};
        }
        HalfEdge* move = boundaryEdge;
        do {
            vec.push_back(move);
            if (faces.find(move->next->twin->incidentFace) != faces.end()) {
                do {
                    move = move->next->twin;
                } while (faces.find(move->next->twin->incidentFace) != faces.end());
            }
            move = move->next;
        } while (move != boundaryEdge);
        return std::move(vec);
    }

    HalfEdge* PolyhedronG::findBoundaryEdge(const std::unordered_set<Face*>& faces)
    {
        HalfEdge* res = nullptr;
        for (auto& it : faces) {
            HalfEdge* edge = it->outEdge;
            HalfEdge* w = edge;
            do {
                if (faces.find(w->twin->incidentFace) == faces.end()) {
                    return w;
                }
                w = w->next;
            } while (w != edge);
        }
        return res;
    }

    void PolyhedronG::eraseEdge(HalfEdge* edge)
    {
        if (edge == nullptr) return;
        if (_edges.find(edge) == _edges.end())
            return;

        HalfEdge* twin = edge->twin;
        Face* face = edge->incidentFace;
        //将指向该face的halfedge全部指向twin的face;
        HalfEdge* move = edge;
        do {
            move->incidentFace = twin->incidentFace;
            move = move->next;
        } while (move != edge);
        delete face;
        _faces.erase(face);

        //更新点
        if (edge->origin->incidentEdge == edge) {
            edge->origin->incidentEdge = twin->next;
        }
        if (edge->twin->origin->incidentEdge == edge->twin) {
            edge->twin->origin->incidentEdge = edge->next;
        }

        //更新edge
        edge->prev->next = edge->twin->next;
        edge->twin->next->prev = edge->prev;
        edge->next->prev = edge->twin->prev;
        edge->twin->prev->next = edge->next;

        //删除edge
        _edges.erase(edge);
        delete edge;
        _edges.erase(twin);
        delete twin;
        
        return;
    }

    std::set<HalfEdge*> PolyhedronG::getOuterEdges(Face* face)
    {
        if (face == nullptr) return {};
        return getRoundEdges(face->outEdge);
    }

    std::set<HalfEdge*> PolyhedronG::getRoundEdges(HalfEdge* edge)
    {
        if (edge == nullptr) return {};
        std::set<HalfEdge*> s;
        HalfEdge* move = edge;
        do {
            s.insert(move);
            move = move->next;
        } while (move != edge);
        return s;
    }

    bool PolyhedronG::checkSameDirectionRoundEdges(HalfEdge* edge)
    {
        if (edge == nullptr) return true;
        HalfEdge* move = edge;
        do {
            if (move->twin->twin != move) {
                assert(false);
                return false;
            }
            if (move->twin->origin != move->origin) {
                assert(false);
                return false;
            }
            if (move->next->origin != move->twin->next->origin) {
                assert(false);
                return false;
            }
            //对move->twin的圈进行测试
            HalfEdge* tmp = move->twin;
            do {
                if (tmp->next->prev != tmp
                    ||tmp->prev->next!= tmp)
                    assert(false);
                if (tmp->origin == tmp->prev->origin
                    || tmp->origin == tmp->next->origin)
                    assert(false);
                tmp = tmp->next;
            } while (tmp != move->twin);

            move = move->next;
        } while (move != edge);
        return true;
    }

    //检查该面和周边面的法向关系，从而判定是否法线都向外侧
    bool PolyhedronG::faceCheck(Face* face)
    {
        HalfEdge* e = face->outEdge;
        HalfEdge* move = e;
        do {
            Face* face2 = move->twin->incidentFace;
            assert(face2 != face);
            vec3 v1 = face->plane.normal;
            vec3 v2 = face2->plane.normal;
            vec3 c = v1.crossProduct(v2);
            vec3 line = *move->next->origin - *move->origin;
            if (c.dotProduct(line) <= 0) {
                assert(false);
                return false;
            }

            move = move->next;
        } while (move != e);
        return true;
    }

    std::string PolyhedronG::outputStruct()
    {
        std::stringstream ss;
        int i = 0;
        for (auto& it : _vertice) {
            ss << "pt:" << it->x << "," << it->y << "," << it->z << "\n";
        }
        for (auto& face : _faces) {
            ss << "face:" << "("<< face->outEdge->origin->x<<","<<face->outEdge->origin->y
                <<","<<face->outEdge->origin->z<<")->("
                <<face->outEdge->next->origin->x<<","
                <<face->outEdge->next->origin->y<<","
                <<face->outEdge->next->origin->z<<")\n";

            HalfEdge* halfEdge = face->outEdge;
            HalfEdge* move = halfEdge;
            do {
                ss << "(" << move->origin->x << "," << move->origin->y << ","
                    << move->origin->z << ")->(";
                move = move->next;
                ss << move->origin->x << "," << move->origin->y << ","
                    << move->origin->z <<")"<< "\n";
            } while (move != halfEdge);
        }
        return ss.str();
    }

    std::string PolyhedronG::outputStruct2()
    {
        std::stringstream ss;
        int i = 0;
        //for (auto& it : _vertice) {
        //    ss << "pt_" << it->_number << "\n";
        //}
        for (auto& face : _faces) {
            ss << "face:" << "(" << face->outEdge->origin->_number << "," <<
                face->outEdge->next->origin->_number << ")\n";

            HalfEdge* halfEdge = face->outEdge;
            HalfEdge* move = halfEdge;
            do {
                ss << "(" << move->origin->_number<<",";
                move = move->next;
                ss << move->origin->_number << ")" << "\n";
            } while (move != halfEdge);
        }
        ss << "\\\\\\\\\\\\\\\\\\\\\\\\\\\\";
        ss << "\n";
        return ss.str();
    }

    std::string PolyhedronG::outputTopology()
    {
        std::stringstream ss;
        ss << "*************************************";
        ss << "\n";
        ss << "PolyhedronG:\n";
        ss << "faces:"<<_faces.size() <<"\n";
        for (auto& it : _faces) {
            ss << "face:" << it->_number << ",edge:" << it->outEdge->_number << "\n";
        }
        ss << "edges:"<<_edges.size()<<"\n";
        std::map<Face*, std::vector<HalfEdge*>> map_;
        for (auto& it : _edges) {
            map_[it->incidentFace].push_back(it);
        }
        for (auto& face : _faces) {
            ss << "face:" << face->_number << "__edges:\n";
            std::vector<HalfEdge*>& vec = map_[face];
            for (auto& it : vec) {
                ss << "edge:" << it->_number << ",prev:" << it->prev->_number
                    << ",next:" << it->next->_number << ",origin:" << it->origin->_number <<
                    ",twin:" << it->twin->_number << "\n";
            }
        }
        ss << "vertices:"<<_vertice.size()<<"\n";
        for (auto& it : _vertice) {
            ss << "vertex:" << it->_number << ",edge:" << it->incidentEdge->_number
                << ",(" <<std::setprecision(3)<<it->x 
                << std::setprecision(3) <<it->y 
                << std::setprecision(3) <<it->z 
                <<")"<<"\n";
        }
        ss << "***************************************";
        ss << "\n\n";

        return ss.str();
    }

    void PolyhedronG::addNewVertex(PVertex* vertex)
    {
        _vertice.insert(vertex);
        vertex->_number = _numberclock;
        _numberclock++;
    }

    void PolyhedronG::addNewFace(Face* face)
    {
        _faces.insert(face);
        face->_number = _numFaceClock;
        _numFaceClock++;
    }

}

