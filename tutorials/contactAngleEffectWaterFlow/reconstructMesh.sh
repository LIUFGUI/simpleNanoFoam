#!/bin/bash

if [ -d "processor0" ]
then
    if [ ! -d "constant/polyMesh" ]
    then
            echo -e "reconstruct parallel mesh"
            reconstructParMesh -constant > reconstructParMesh.out
    fi
    rm -rf processor*
    
    #echo -e "Viscosity Processing"
    #processViscosity > processViscosity.out

    #echo -e "DecomposePar"
    #decomposePar -fields > decomposeParFlow.out
    #rm -rf 0
fi

echo -e "Mesh Reconstructed"
