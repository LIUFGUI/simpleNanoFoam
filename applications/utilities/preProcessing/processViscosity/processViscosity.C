/*---------------------------------------------------------------------------*\

License
    This file is part of simpleNanoFoam, an Open source software using OpenFOAM
    for confined liquid flow simulation in pore-scale
    geological domain.

    simpleNanoFoam is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation, either version 3 of the License, or (at your
    option) any later version. See <http://www.gnu.org/licenses/>.

    The code was developed by Fugui Liu as part of his research work
    at Heriot-Watt University and China University of Petroleum.

Application
    processVsicosity

Description
    update viscosity field for each cells

\*---------------------------------------------------------------------------*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include "fvCFD.H"
#include "argList.H"
#include "primitivePatchInterpolation.H"
#include "timeSelector.H"
#include "wallDist.H"
#include "wallFvPatch.H"


using namespace Foam;

int main(int argc, char *argv[])
{
	#include "setRootCase.H"
	#include "createTime.H"
	#include "createMesh.H"
	
	//const volScalarField dist(wallDist::New(mesh).y());
	
	IOdictionary transportProperties
	(
	    IOobject
	    (
	        "transportProperties",
	        runTime.constant(),
	        mesh,
	        IOobject::MUST_READ,
	        IOobject::NO_WRITE
	    )
	);
	dimensionedScalar nuBulk("nuBulk", transportProperties);
	
	volScalarField nuCorrected
	(
	    IOobject
	    (
	        "nuCorrected",
	        runTime.timeName(),
	        mesh,
	        IOobject::NO_READ,
	        IOobject::AUTO_WRITE
	    ),
	    mesh,
	    nuBulk,
	    zeroGradientFvPatchScalarField::typeName
	);
	
	labelList nearestPatchID(mesh.nCells(), -1);
	scalarField minDist(mesh.nCells(), GREAT);
	const vectorField& cellCenters = mesh.C();
	
	forAll(mesh.boundary(), patchi)
	{
	    const fvPatch& patch = mesh.boundary()[patchi];
	    if (isA<wallFvPatch>(patch))
	    {
	    	const vectorField& patchFaceCenters = patch.Cf();
	    	forAll(cellCenters, celli)
	    	{
	    	    scalar minDforCell = GREAT;
	    	    forAll(patchFaceCenters, facei)
	    	    {
	    		scalar d = mag(cellCenters[celli] - patchFaceCenters[facei]);
	    		if (d < minDforCell)
	    		{
	    		    minDforCell = d;
	    		}
	    	    }
	    	    
	    	    if (minDforCell < minDist[celli])
	    	    {
	    	        minDist[celli] = minDforCell;
	    	        nearestPatchID[celli] = patchi;
	    	    }
	    	}
	    }
	}
	
	
	forAll(mesh.boundary(), patchi)
	{
	    const fvPatch& patch = mesh.boundary()[patchi];
	
	    if (isA<wallFvPatch>(patch))
	    {
	        const word patchName = patch.name();
	        Info << "Processing wall patch: " << patchName << endl;
	
	        word nuName   = "nu_" + patchName;
	        word distName = "dist_" + patchName;
	
	        if (
	            !transportProperties.found(nuName) ||
	            !transportProperties.found(distName)
	        )
	        {
	            Info << "  Skipping patch: no nu or dist defined for " << patchName << endl;
	            continue;
	        }
	
	        dimensionedScalar nuPatch(nuName, transportProperties);
	        dimensionedScalar distPatch(distName, transportProperties);
	
	        //volScalarField dist(wallDist::New(mesh).y());
	        
	        forAll(nuCorrected, celli)
	        {
	            //if (dist[celli] < distPatch.value())
	            if (nearestPatchID[celli] == patchi && minDist[celli] < distPatch.value())
	            {
	                nuCorrected[celli] = nuPatch.value();
	            }
	        }
	    }
	}
	nuCorrected.write();
	Info << "nuCorrected written to 0/" << endl;
	


	return 0;
}


// ************************************************************************* //
