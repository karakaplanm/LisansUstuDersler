rasmol 1PGB.pdb
echo "OPLS-AA/L all-atom force field, ve SPC/E for water "
gmx pdb2gmx -f 1PGB.pdb -o conf.pdb
rasmol conf.pdb
more topol.top
echo "topoloji dosyası proteinin atomları arasındaki etkileşimleri ve parametreleri içerir (bonds, angles, torsion angles, Lennard-Jones interactions and electrostatic interactions)"

gmx editconf -f conf.pdb -o box.pdb -d 0.7
echo "unitcell true"
rasmol box.pdb
echo "Su doldur"
#genbox -cp box.pdb -cs spc216 -o water.pdb -p topol.top
gmx solvate -cp box.pdb -cs spc216 -o water.pdb -p topol.top
rasmol water.pdb 
gmx grompp -f em.mdp -c water.pdb -p topol.top -o em.tpr -maxwarn 2

gmx genion -s em.tpr -o ions.pdb -np 4 -p topol.top
echo "select na"
echo "cpk"
rasmol ions.pdb
gmx grompp -f em.mdp -c ions.pdb -p topol.top -o em.tpr -maxwarn 2
gmx mdrun -v -s em.tpr -c em.pdb 
gmx grompp -f md.mdp -c em.pdb -p topol.top -o md.tpr -maxwarn 2
gmx mdrun -v -s md.tpr -c md.pdb -nice 0
gmx trjconv -s md.tpr -f traj_comp.xtc -o traj.pdb -dt 1.
pymol traj.pdb

echo "1 1" 
gmx rms -s md.tpr -f traj_comp.xtc
xmgrace rmsd.xvg
