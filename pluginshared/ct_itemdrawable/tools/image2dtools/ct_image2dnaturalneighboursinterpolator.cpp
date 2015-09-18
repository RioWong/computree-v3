#include "ct_image2dnaturalneighboursinterpolator.h"

#ifdef USE_OPENCV
#include "ct_math/delaunay2d/ct_delaunaytriangulation.h"


CT_Image2DNaturalNeighboursInterpolator::CT_Image2DNaturalNeighboursInterpolator(CT_Image2D<float>* raster, CT_Image2D<float>* rasterI, int ncells)
{
    _raster = raster;
    _rasterInterpol = rasterI;
    _nCells = ncells;
}


void CT_Image2DNaturalNeighboursInterpolator::interpolateForOneCell(const size_t &index)
{
    size_t xi, yi;
    _raster->indexToGrid(index, xi, yi);

    // Triangulation des points non interpoles du MNT, autour de l'indice concerné
    CT_DelaunayTriangulation delaunay;
    delaunay.init((int)xi - _nCells - 1, (int)yi - _nCells - 1, (int)xi + _nCells + 1, (int)yi + _nCells + 1);

    int cpt = 0;
    for (int xx = (int)xi - _nCells ; xx <= (int)xi + _nCells ; xx++) {
        for (int yy = (int)yi - _nCells ; yy  <= (int)yi + _nCells ; yy++) {
            if (xx >= 0 && yy >= 0 && (xx != xi || yy != yi))
            {
                float value = _raster->value(xx,yy);
                if (value != _raster->NA())
                {
                    float distCell = sqrt(pow(xi - xx, 2) + pow(yi - yy, 2));
                    if (distCell <= _nCells)
                    {
                        Eigen::Vector3d* pt = new Eigen::Vector3d();
                        (*pt)(0) = (double)xx;
                        (*pt)(1) = (double)yy;
                        (*pt)(2) = value;
                        delaunay.addVertex(pt, true);
                        cpt++;
                    }
                }
            }
        }
    }

    if (cpt > 0)
    {
        float value = _raster->value(xi,yi);
        Eigen::Vector3d* pt = new Eigen::Vector3d();
        (*pt)(0) = (double)xi;
        (*pt)(1) = (double)yi;
        (*pt)(2) = value;

        CT_DelaunayVertex* vt = delaunay.addVertex(pt, true);

        delaunay.doInsertion();
        delaunay.getVerticesNeighbors();

        QList<CT_DelaunayVertex*> &voisins = vt->getNeighbors();

        double sum = 0;
        double sum_poids = 0;

        // Calcul de la somme ponderee
        for (int nei = 0 ; nei < voisins.size() ; nei++)
        {
            CT_DelaunayVertex* neigh = voisins.at(nei);
            Eigen::Vector3d* nodePt = neigh->getData();

            double dx = (double)xi - (*nodePt)(0);
            double dy = (double)yi - (*nodePt)(1);
            double distance = sqrt(dx*dx + dy*dy);

            sum += (*nodePt)(2) / distance;
            sum_poids += 1.0 / distance;
        }
        if (sum_poids != 0)
        {
            _rasterInterpol->setValue(xi, yi, sum / sum_poids);
        }
    }
}

#endif

