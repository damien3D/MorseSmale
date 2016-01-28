#ifndef MESH_EIGENFIELD_LINE_CONNER
#define MESH_EIGENFIELD_LINE_CONNER

#include "..\Global\Global.h"
#include "..\Global\CommonData.h"
#include "CriticalPoint.h"
#include "MeshDrawer.h"
#include "NoiseRemoval.h"

class CriticalPoint;
class MeshDrawer;
class NoiseRemoval;
struct CommonData;


class CMeshConner
{
public:
        CMeshConner();
        ~CMeshConner();
        CMeshConner(const CMeshConner& meshConner);

        void Attach(MeshModel* pModel);

        bool LoadEigenField(string filename);

        void Draw();
        void setDrawMode(Global::DrawMode mode);

        void SaveMEConnLine(string filename);

        bool IsRemoveNoise();

        void RemoveNoise();

        pair<double,double> getThreshold() const
        {
                return p_removal->GetThreshold();
        }
        void setThreshold(double lowBound, double upBound)
        {
                p_removal->SetThreshold(lowBound,upBound);
        }

        void clear();
        

private:

        CommonData* p_comData;
        CriticalPoint* p_criPoint;
        MeshDrawer* p_drawer;
        NoiseRemoval* p_removal;
};

#endif