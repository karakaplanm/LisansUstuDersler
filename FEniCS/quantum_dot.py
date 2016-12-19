"""

   quantum_dot.py numerically computes the ground state energy and 
   eigenfunction of a hemispherical quantum dot using FEniCS with inverse
   iteration

"""

import dolfin as df
import numpy as np

if __name__ == '__main__': 
    
    # Computational domain radius
    r_dom = 2

    # Minimum computational domain height
    z_dom_min = -1

    # Maximum computational domain height 
    z_dom_max = 2

    # Number of cells in the radial direction
    nr = 200
 
    # Number of cells in the radial direction
    nz = 200   

    mesh = df.RectangleMesh(Point(0,z_dom_min),Point(r_dom,z_dom_max),nr,nz)

    # Define interior of quantum dot r^2+z^2<1 and z>0    
    class QuantumDot(df.SubDomain):
        def inside(self,x,on_boundary):
            return df.between(x[0]**2+x[1]**2,(0,1)) and df.between(x[1],(0,1))   

    quantumDot = QuantumDot()

    domains = df.CellFunction("size_t",mesh)
    domains.set_all(0)
    quantumDot.mark(domains,1)

    V = df.FunctionSpace(mesh,"CG",1)

    u = df.TrialFunction(V)
    v = df.TestFunction(V)     

    drdz = df.Measure("dx")[domains]
    r = df.Expression("x[0]")

    # Confining potential
    potential = df.Constant(100)

    # Partial derivatives of trial and test functions
    u_r = u.dx(0)
    v_r = v.dx(0)
    u_z = u.dx(1)
    v_z = v.dx(1)

    # Initial guess of ground state is 1 inside dot, 0 outside dot
    psi0 = v*r*drdz(1)
    Psi0 = df.PETScVector()
    df.assemble(psi0,tensor=Psi0)

    # Hamiltonian and mass matrix forms
    h = (u_r*v_r+u_z*v_z)*r*(drdz(0)+drdz(1))+potential*r*u*v*r*drdz(0)
    m = (u*v*r)*(drdz(0)+drdz(1))
   
    # Mass matrix 
    M = df.PETScMatrix()
    df.assemble(m,tensor=M)

    # Hamiltonian matrix
    H = df.PETScMatrix()
    df.assemble(h,tensor=H)
 
    # Solution
    psi = df.Function(V)
    solver = df.PETScLUSolver(H)
    solver.parameters['symmetric'] = True

    solver.solve(psi.vector(),Psi0) 
 
    q = psi.vector()
    
    # Do inverse iteration
    for k in range(5):
        Mq = M*q
        qHq = q.inner(H*q)
        qMq = q.inner(Mq)

        # Rayleigh quotient
        E = qHq/qMq
        print(E)
 
        q /= np.sqrt(qMq)

        solver.solve(q,Mq) 

    Mq = M*q
    q /= np.sqrt(q.inner(Mq))

    psi.vector()[:] = q
  
    df.plot(psi,title="Ground State")
    df.interactive() 
