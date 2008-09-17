#include "Exporter.h"
#include "math.h"

using namespace vrender ;
using namespace std ;

int FIGExporter::FigCoordX(double x) const
{
	float MaxX = 12000 ;
	float MaxY = MaxX * _sizeY/(float)_sizeX ;

	if(MaxY > 7000)
	{
		MaxX *= 7000/(float)MaxY ;
		MaxY = 7000 ;
	}

	return int(0.5f + x/_sizeX*MaxX) ;
}

int FIGExporter::FigCoordY(double y) const
{
	float MaxX = 12000 ;
	float MaxY = MaxX * _sizeY/(float)_sizeX ;

	if(MaxY > 7000)
	{
		MaxX *= 7000/(float)MaxY ;
		MaxY = 7000 ;
	}

	return int(0.5f + (1.0 - y/_sizeY)*MaxY) ;
}

int FIGExporter::FigGrayScaleIndex(float red, float green, float blue) const
{
	float intensity = 0.3f*red+0.6f*green+0.1f*blue ;

	return int(intensity * 20.0) ;
}

FIGExporter::FIGExporter()
{
}

void FIGExporter::writeHeader(QTextStream& out) const
{
	out << "#FIG 3.2\nPortrait\nCenter\nInches\nLetter\n100.00\nSingle\n0\n1200 2\n";
	_depth = 999 ;
	_sizeX = int(0.5f + _xmax - _xmin) ;
	_sizeY = int(0.5f + _ymax - _ymin) ;
}

void FIGExporter::writeFooter(QTextStream& out) const
{
	Q_UNUSED(out);
}

void FIGExporter::spewPoint(const Point *P, QTextStream& out)
{
	out << "2 1 0 5 0 7 " << (_depth--) << " 0 -1 0.000 0 1 -1 0 0 1\n";

	out << "\t " << FigCoordX(P->vertex(0)[0]) << " " << FigCoordY(P->vertex(0)[1]) << "\n";
	if(_depth > 0) _depth = 0 ;
}

void FIGExporter::spewSegment(const Segment *S, QTextStream& out)
{
	const Feedback3DColor& P1 = Feedback3DColor(S->sommet3DColor(0)) ;
	const Feedback3DColor& P2 = Feedback3DColor(S->sommet3DColor(1)) ;

	GLdouble dx, dy;
	GLfloat dr, dg, db, absR, absG, absB, colormax;
	int steps;
	GLdouble xstep, ystep;
	GLfloat rstep, gstep, bstep;
	GLdouble xnext, ynext, distance;
	GLfloat rnext, gnext, bnext;

	dr = P2.red()   - P1.red();
	dg = P2.green() - P1.green();
	db = P2.blue()  - P1.blue();

	if (dr != 0 || dg != 0 || db != 0)
	{
		/* Smooth shaded line. */

		dx = P2.x() - P1.x();
		dy = P2.y() - P1.y();

		distance = sqrt(dx * dx + dy * dy);

		absR = fabs(dr);
		absG = fabs(dg);
		absB = fabs(db);

		colormax = max(absR, max(absG, absB));
		steps = int(0.5f + max(1.0, colormax * distance * EPS_SMOOTH_LINE_FACTOR));

		xstep = dx / steps;
		ystep = dy / steps;

		rstep = dr / steps;
		gstep = dg / steps;
		bstep = db / steps;

		xnext = P1.x();
		ynext = P1.y();
		rnext = P1.red();
		gnext = P1.green();
		bnext = P1.blue();

		/* Back up half a step; we want the end points to be
   			exactly the their endpoint colors. */

		xnext -= xstep / 2.0;
		ynext -= ystep / 2.0;
		rnext -= rstep / 2.0f;
		gnext -= gstep / 2.0f;
		bnext -= bstep / 2.0f;
	}
	else
	{
		/* Single color line. */
		steps = 0;
	}

	out << "2 1 0 1 0 7 " << (_depth--) << " 0 -1 0.000 0 0 -1 0 0 2\n";
	out << "\t " << FigCoordX(P1.x()) << " " << FigCoordY(P1.y());

	out << " " << FigCoordX(P2.x()) << " " << FigCoordY(P2.y())<< "\n";
	if(_depth > 0) _depth = 0 ;
}

void FIGExporter::spewPolygone(const Polygone *P, QTextStream& out)
{
	int nvertices;
	GLfloat red, green, blue;

	nvertices = P->nbVertices() ;

	Feedback3DColor vertex(P->sommet3DColor(0)) ;

	if (nvertices > 0)
	{
		red   = 0 ;
		green = 0 ;
		blue  = 0 ;

		for(int i = 0; i < nvertices; i++)
		{
			red   += P->sommet3DColor(i).red() ;
			green += P->sommet3DColor(i).green() ;
			blue  += P->sommet3DColor(i).blue() ;
		}

		red   /= nvertices ;
		green /= nvertices ;
		blue  /= nvertices ;

		/* Flat shaded polygon; all vertex colors the same. */

		if(_blackAndWhite)
			out << "2 3 0 0 0 7 " << (_depth--) << " 0 20 0.000 0 0 -1 0 0 " << (nvertices+1) << "\n";
		else
			out << "2 3 0 0 0 7 " << (_depth--) << " 0 " << (FigGrayScaleIndex(red,green,blue)) << " 0.000 0 0 -1 0 0 " << (nvertices+1) << "\n";

		/* Draw a filled triangle. */

		out << "\t";

		for (int j = 0; j < nvertices; j++)
			out << " " << FigCoordX(P->sommet3DColor(j).x()) << " " << FigCoordY(P->sommet3DColor(j).y());

		out << " " << FigCoordX(P->sommet3DColor(0).x()) << " " << FigCoordY(P->sommet3DColor(0).y()) << "\n";
	}

	if(_depth > 0) _depth = 0 ;
}


