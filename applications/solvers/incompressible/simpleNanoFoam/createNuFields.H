const volScalarField dist(wallDist::New(mesh).y());
dimensionedScalar nuBulk("nuBulk", transportProperties);

volScalarField nuCorrected
(
    IOobject
    (
        "nuCorrected",
        runTime.timeName(),
        mesh,
        IOobject::No_READ,
        IOobject::AUTO_WRITE
    ),
    mesh,
    nuBulk,
    zeroGradientFvPatchScalarField::typeName
);

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
            if (dist[celli] < distPatch.value())
            {
                nuCorrected[celli] = nuPatch.value();
            }
        }
    }
}


