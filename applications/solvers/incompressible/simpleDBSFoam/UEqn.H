    // Momentum predictor

    surfaceScalarField phiEps = phi/(fvc::interpolate(eps)+1e-30);
    
    MRF.correctBoundaryVelocity(U);

    tmp<fvVectorMatrix> tUEqn
    (
        (1/eps)*fvm::div(phiEps, U, "div(phiEps,U)")
      + (1/eps/eps)*MRF.DDt(U)
      - fvm::laplacian(fvc::interpolate(nu)/(fvc::interpolate(eps)+1e-30), U)
      //+ turbulence->divDevReff(U)
      + fvm::Sp(nu*Kinv,U)
     ==
        fvOptions(U)
    );
    fvVectorMatrix& UEqn = tUEqn.ref();

    UEqn.relax();

    fvOptions.constrain(UEqn);

    if (simple.momentumPredictor())
    {
        solve(UEqn == -fvc::grad(p));

        fvOptions.correct(U);
    }
