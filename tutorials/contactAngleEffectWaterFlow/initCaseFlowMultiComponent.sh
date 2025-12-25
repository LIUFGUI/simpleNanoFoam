#!/bin/bash

###### USERS INPUT ############################################################

## Define inlet veclocity (m/s)
#Ux=0.0001
#Uy=0
#Uz=0

## Define inlet pressure 
p0=0.01

## Define the kinematic viscocity of the fluid (m^2/s)
##(e.g for water this is 1e-6, for air this would be 1.478e-5)
Viscorig=1.2e-5
Visc_Bulk=1.2e-5
Visc_walls=0.6e-5
Visc_fixedWalls_org=0.6e-5
Visc_fixedWalls_inorg=0.6e-5

## Define the thickness of near-wall region (m)
Dist_walls=2e-9
Dist_fixedWalls_org=2e-9
Dist_fixedWalls_inorg=2e-9

## Define slip length
Ls_walls=1e-9
Ls_fixedWalls_org=1e-9
Ls_fixedWalls_inorg=1e-9

#### END OF USER INPUT #######################################################

cp constant/transportProperties2 constant/transportProperties
sed -i "s/Viscorig/$Viscorig/g" constant/transportProperties
sed -i "s/Visc_Bulk/$Visc_Bulk/g" constant/transportProperties

sed -i "s/Visc_walls/$Visc_walls/g" constant/transportProperties
sed -i "s/Visc_fixedWalls_org/$Visc_fixedWalls_org/g" constant/transportProperties
sed -i "s/Visc_fixedWalls_inorg/$Visc_fixedWalls_inorg/g" constant/transportProperties

sed -i "s/Dist_walls/$Dist_walls/g" constant/transportProperties
sed -i "s/Dist_fixedWalls_org/$Dist_fixedWalls_org/g" constant/transportProperties
sed -i "s/Dist_fixedWalls_inorg/$Dist_fixedWalls_inorg/g" constant/transportProperties



cp -r 0_origP_MultiComponent 0
sed -i "s/p0/$p0/g" 0/p
sed -i "s/Ls_walls/$Ls_walls/g" 0/U
sed -i "s/Ls_fixedWalls_org/$Ls_fixedWalls_org/g" 0/U
sed -i "s/Ls_fixedWalls_inorg/$Ls_fixedWalls_inorg/g" 0/U
#sed -i "s/Ux/$Ux/g" 0/U
#sed -i "s/Uy/$Uy/g" 0/U
#sed -i "s/Uz/$Uz/g" 0/U

if [ -d "processor0" ]
then
    if [ ! -d "constant/polyMesh" ]
    then
            echo -e "reconstruct parallel mesh"
            reconstructParMesh -constant > reconstructParMesh.out
    fi
    
    echo -e "Viscosity Processing"
    processViscosity > processViscosity.out

    echo -e "DecomposePar"
    decomposePar -fields > decomposeParFlow.out
    #rm -rf 0
else
    echo -e "Viscosity Processing"
    processViscosity > processViscosity.out
fi

echo -e "Case initialised"
