/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2019-2021 OpenCFD Ltd.
    Copyright (C) YEAR AUTHOR, AFFILIATION
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "mixedFvPatchFieldTemplate.H"
#include "addToRunTimeSelectionTable.H"
#include "fvPatchFieldMapper.H"
#include "volFields.H"
#include "surfaceFields.H"
#include "unitConversion.H"
#include "PatchFunction1.H"

//{{{ begin codeInclude

//}}} end codeInclude


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * Local Functions * * * * * * * * * * * * * * //

//{{{ begin localCode

//}}} end localCode

// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * //

// dynamicCode:
// SHA1 = 1eaf784f08235f873bef1f75ebfac0ec69a42369
//
// unique function name that can be checked if the correct library version
// has been loaded
extern "C" void fixedWalls_clay_1eaf784f08235f873bef1f75ebfac0ec69a42369(bool load)
{
    if (load)
    {
        // Code that can be explicitly executed after loading
    }
    else
    {
        // Code that can be explicitly executed before unloading
    }
}

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

makeRemovablePatchTypeField
(
    fvPatchVectorField,
    fixedWalls_clayMixedValueFvPatchVectorField
);

} // End namespace Foam


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::
fixedWalls_clayMixedValueFvPatchVectorField::
fixedWalls_clayMixedValueFvPatchVectorField
(
    const fvPatch& p,
    const DimensionedField<vector, volMesh>& iF
)
:
    parent_bctype(p, iF)
{
    if (false)
    {
        printMessage("Construct fixedWalls_clay : patch/DimensionedField");
    }
}


Foam::
fixedWalls_clayMixedValueFvPatchVectorField::
fixedWalls_clayMixedValueFvPatchVectorField
(
    const fixedWalls_clayMixedValueFvPatchVectorField& rhs,
    const fvPatch& p,
    const DimensionedField<vector, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    parent_bctype(rhs, p, iF, mapper)
{
    if (false)
    {
        printMessage("Construct fixedWalls_clay : patch/DimensionedField/mapper");
    }
}


Foam::
fixedWalls_clayMixedValueFvPatchVectorField::
fixedWalls_clayMixedValueFvPatchVectorField
(
    const fvPatch& p,
    const DimensionedField<vector, volMesh>& iF,
    const dictionary& dict
)
:
    parent_bctype(p, iF, dict)
{
    if (false)
    {
        printMessage("Construct fixedWalls_clay : patch/dictionary");
    }
}


Foam::
fixedWalls_clayMixedValueFvPatchVectorField::
fixedWalls_clayMixedValueFvPatchVectorField
(
    const fixedWalls_clayMixedValueFvPatchVectorField& rhs
)
:
    parent_bctype(rhs),
    dictionaryContent(rhs)
{
    if (false)
    {
        printMessage("Copy construct fixedWalls_clay");
    }
}


Foam::
fixedWalls_clayMixedValueFvPatchVectorField::
fixedWalls_clayMixedValueFvPatchVectorField
(
    const fixedWalls_clayMixedValueFvPatchVectorField& rhs,
    const DimensionedField<vector, volMesh>& iF
)
:
    parent_bctype(rhs, iF)
{
    if (false)
    {
        printMessage("Construct fixedWalls_clay : copy/DimensionedField");
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::
fixedWalls_clayMixedValueFvPatchVectorField::
~fixedWalls_clayMixedValueFvPatchVectorField()
{
    if (false)
    {
        printMessage("Destroy fixedWalls_clay");
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void
Foam::
fixedWalls_clayMixedValueFvPatchVectorField::updateCoeffs()
{
    if (this->updated())
    {
        return;
    }

    if (false)
    {
        printMessage("updateCoeffs fixedWalls_clay");
    }

//{{{ begin code
    #line 144 "/home/parallels/works/GeoChemFoam-5.1/runs/nanoPorousMedia1f/2DmicroPorousModel/multicomponentModel/0/U/boundaryField/fixedWalls_clay"
const scalar slipLength = 0.11177e-9;
                const scalarField delta = patch().deltaCoeffs();
                const vectorField nHat = patch().nf();
                const vectorField pif = patchInternalField();

                const scalarField valueF = 1.0 / (1.0 + slipLength * delta);

                Field<vector> refVal(patch().size(), vector::zero);
                const tensorField IField(nHat.size(), tensor::I);
                Field<vector> Utan = transform(IField - sqr(nHat), pif);
                Field<vector> val = (1.0 - valueF) * Utan + valueF * refVal;

                operator==(val);
                refValue() = refVal;
                valueFraction() = valueF;
//}}} end code

    this->parent_bctype::updateCoeffs();
}


// ************************************************************************* //

