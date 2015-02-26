	--error analysis program instructions--

NB:  "./"  refers to  "P3Group/errorocode/" 
(which is kept up to date on Github branch: ErrorAnalysis)


1)
run ./main [arguments as usual]
	to output analytical.dat and pot.dat

2)
run ./merge.bsh [argument]
	where argument is "pot.dat" (or whatever you want to compare 
	to analytic.dat)
	this finds difference between analytical.dat and pot.dat
	(to compare to another thing, go into file and edit "analytic.dat"
	to whatever you want to compare to)

	output: pd_[argument].pdf



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
