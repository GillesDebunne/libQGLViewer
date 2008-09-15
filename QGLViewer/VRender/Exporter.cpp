#include <stdexcept>
#include "VRender.h"
#include "Exporter.h"

using namespace vrender ;
using namespace std ;

Exporter::Exporter()
{
	_xmin=_xmax=_ymin=_ymax=_zmin=_zmax = 0.0 ;
	_pointSize=1 ;
}

void Exporter::exportToFile(const char *filename,
										const vector<PtrPrimitive>& primitive_tab,
										VRenderParams& vparams)
{
	FILE *f;
	fopen_s(&f, filename, "w") ;

	if (f == NULL)
		throw runtime_error(string("could not open file ") + filename) ;

	writeHeader(f) ;

	int N = primitive_tab.size()/200 + 1 ;

	for(unsigned int i=0;i<primitive_tab.size();++i)
	{
		Point *p = dynamic_cast<Point *>(primitive_tab[i]) ;
		Segment *s = dynamic_cast<Segment *>(primitive_tab[i]) ;
		Polygone *P = dynamic_cast<Polygone *>(primitive_tab[i]) ;

		if(p != NULL) spewPoint(p,f) ;
		if(s != NULL) spewSegment(s,f) ;
		if(P != NULL) spewPolygone(P,f) ;

		if(i%N == 0)
			vparams.progress(i/(float)primitive_tab.size(),string("Exporting to file ")+filename) ;
	}

	writeFooter(f) ;

	fclose(f) ;
}

void Exporter::setBoundingBox(float xmin,float ymin,float xmax,float ymax)
{
	_xmin = xmin ;
	_ymin = ymin ;
	_xmax = xmax ;
	_ymax = ymax ;
}

void Exporter::setClearColor(float r, float g, float b) { _clearR=r; _clearG=g; _clearB=b; }
void Exporter::setClearBackground(bool b) { _clearBG=b; }
void Exporter::setBlackAndWhite(bool b) { _blackAndWhite = b; }

