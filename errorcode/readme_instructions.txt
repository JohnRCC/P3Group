	--error analysis program instructions--

NB:  "./"  refers to  "P3Group/errorocode/" 
(which is kept up to date on Github branch: ErrorAnalysis)


1)
run ./percentdif.bsh arg1 arg2
(to compare arg1 to arg2)
eg
	./percentdif.bsh pot.dat analytical.dat


_________________________________________________________

	--animated gif generation instructions--
	--(animated gif of how percentage difference-- 
	--between two files changes with iteration)--

move to P3Group/errorcode/giffing

this animated gif generation program runs main multiple times to generate each .png file for the gif):

BEFORE YOU RUN gifcreate.bsh You need to update
1) multiiter.sh 
2) gifcreate.bsh 
3) giffer.bsh 
so they all agree on the number and 'spacing' of iterations.

ie look for line:

for i in {1000..20000..500} //{x1..x2..x3} iterates from x1 to x2 in steps of x3

the line above is included in these three programs and they must all agree!
Finally,
run ./giffing/gifcreate.bsh to generate a gif (may take a while). 
