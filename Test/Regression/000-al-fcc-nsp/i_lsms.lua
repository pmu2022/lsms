
--[[

Test for non-spin polarized Aluminium

]]--

systemid="al"
system_title = "Aluminium test for LSMS 3"
pot_in_type=1
pot_out_type=1

relativity="s"
core_relativity="f"

num_atoms=4
nspin=1
mtasa=0

xcFunctional = { 1, 1, 7 }

iprint=-1
default_iprint=-1
print_node=0
istop="main"

nscf=30
rmsTolerance=1.0e-16

xRepeat=1
yRepeat=1
zRepeat=1
makeTypesUnique=1

energyContour = {npts=31,grid=2,ebot=-0.3,etop=0.0,eitop=0.825,eibot=0.0025}

a = 4.05 / 0.529177

bravais = {}
bravais[1]={ 1.0*a, 0.0, 0.0}
bravais[2]={ 0.0, 1.0*a, 0.0}
bravais[3]={ 0.0, 0.0, 1.0*a}

site_default={lmax=3,rLIZ=12.5,rsteps={89.5,91.5,93.2,99.9},atom="Al",Z=13,Zc=2,Zs=8,Zv=3,rad=2}

mixing = { {quantity = "potential", algorithm = "broyden", mixing_parameter = 0.05} }

numberOfMixQuantities = 0
for k,v in pairs(mixing) do
numberOfMixQuantities = numberOfMixQuantities + 1
end

site = {}
for i =1,num_atoms do site[i]={} end

site[1].pos={0,0,0}
site[1].evec={0,0,1}
site[2].pos={0.5*a,0.5*a,0}
site[2].evec={0,0,1}
site[3].pos={0.5*a,0,0.5*a}
site[3].evec={0,0,1}
site[4].pos={0,0.5*a,0.5*a}
site[4].evec={0,0,1}

for i =1,num_atoms do
for k,v in pairs(site_default) do
if(site[i][k]==nil) then site[i][k]=v end
end
end

