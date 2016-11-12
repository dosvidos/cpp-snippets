movapd xmm4,xmm0
movapd xmm5,xmm0   // Save xmm0 for later
shufpd xmm4,xmm4,1
cmpltpd xmm0,xmm4  // Mask in xmm0: all 1s in low qword if xmm4's low qword was less
shufpd xmm4,xmm4,0 // Set both qwords in xmm4 to the low qword of xmm4
shufpd xmm5,xmm5,0 // Set both qwords in xmm5 to the (original) high qword of xmm4
andpd xmm4,xmm0    // Places the low qword from xmm4 in its rightful place in xmm4
andnpd xmm0,xmm5   // Places the high qword from xmm4 in its rightful place
orpd xmm0,xmm4      // Combine the two qwords