//*****************************************************************************
// File:   LatLong-UTMconversion.cpp
// Author: 
// Date:   September 2016
// Coms:   funciones para la conversión entre coordenadas UTM y latitud/longitud
//         Versión ligeramente adaptada de "http://www.gpsy.com/gpsinfo/geotoutm/gantz/"
//*****************************************************************************

#include <string.h>

#ifndef LATLONGCONV
#define LATLONGCONV


//converts lat/long to UTM coords.  Equations from USGS Bulletin 1532 
//East Longitudes are positive, West longitudes are negative. 
//North latitudes are positive, South latitudes are negative
//Lat and Long are in decimal degrees
	//Written by Chuck Gantz- chuck.gantz@globalstar.com
void LLtoUTM(int ReferenceEllipsoid, const double Lat, const double Long, 
			 double &UTMNorthing, double &UTMEasting, char* UTMZone);


void UTMtoLL(int ReferenceEllipsoid, const double UTMNorthing, const double UTMEasting, const char* UTMZone,
			  double& Lat,  double& Long );

//This routine determines the correct UTM letter designator for the given latitude
//returns 'Z' if latitude is outside the UTM limits of 84N to 80S
	//Written by Chuck Gantz- chuck.gantz@globalstar.com
char UTMLetterDesignator(double Lat);

class Ellipsoid
{
public:
	Ellipsoid(){};
	Ellipsoid(int Id, std::string name, double radius, double ecc)
	{
		id = Id; ellipsoidName = name; 
		EquatorialRadius = radius; eccentricitySquared = ecc;
	}

	int id;
	std::string ellipsoidName;
	double EquatorialRadius; 
	double eccentricitySquared;  

};

#endif
