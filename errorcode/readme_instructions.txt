	--error analysis program instructions--

NB:  "./"  refers to  "P3Group/errorocode/" 
(which is kept up to date on Github branch: ErrorAnalysis)

_________________________________________________________
INSTRUCTIONS to gain percentage difference between two .dat 
files as a .dat file and a .pdf heatmap


run ./percentdif.bsh arg1 arg2
(to compare arg1 to arg2)
eg
	./percentdif.bsh pot.dat analytical.dat

this will give you:
1) pd_arg1.dat 	(the percentage difference between the two)
2) arg1.pdf	(a heatmap of the percentage between the two)

well done!
_________________________________________________________
INSTRUCTIONS to make animated gif of how percentage 
difference between two files changes as number of 
iterations increases.


move to P3Group/errorcode/giffing

**this animated gif generation program runs main multiple times to 
generate (then delete) each .png file for the gif**

BEFORE YOU RUN GIFCREATE.BSH 
You need to update:
1) multiiter.sh 
2) gifcreate.bsh 
3) giffer.bsh 
so they all agree on the number and 'spacing' of iterations.

simply look for line:

for i in {1000..20000..500} //{x1..x2..x3} iterates from x1 to x2 in steps of x3

this line is included in these three programs and they must all agree!
Then change a line in giffer.bsh:

num="$((${i} / bla))"

to
num="$((${i} / x3))"
(where x3 is 'step size as defined above)

finaly,
run ./giffing/gifcreate.bsh to generate a gif (may take a while). 

and you will see tlaps.gif in the current folder. YAY!
