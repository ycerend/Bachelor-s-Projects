`timescale 1ns/1ns
module source(y, x);

input wire [4:0] x;
output wire [0:0] y;
wire X3,X4,D0,D1,D2,D3,S0,S1,S2,I0,I2,I3,I4,I5,I6,I7,W1, W2, W3, W4 , W5, W6;
// Fill here with gate level verilog code

not(nx3,x[3]);
not(nx4, x[4]);
or(W5,W2,W3);
or(W6,nx3, W4);

Dcd2x4 dcd(x[3], x[4], W1, W2, W3, W4);
mux8x1 mux8(x[0], x[1],x[2], x[3], 1'b0, nx4, nx3,W2, nx3 ,W5 , W6 , y); 

endmodule


 module mux8x1(S0, S1,S2,I0, I1, I2, I3,I4, I5 ,I6 , I7 , Y); 
 	input I0, I1, I2, I3 , I4, I5 ,I6 , I7 ;
	input S0, S1 , S2;
	output Y;
	reg Y;
	always @(S0, S1,S2, I0, I1, I2, I3,I4, I5 ,I6 , I7 )
 			begin
				if( S2 == 0 && S1==0 && S0==0 )
					Y <= I0;
				else if( S2 == 0 && S1==0 && S0==1 )
					Y <= I1;
				else if( S2 == 0 && S1==1 && S0==0 )
					Y <= I2;
				else if( S2 == 0 && S1==1 && S0==1 )
					Y <= I3;
				else if( S2 == 1 && S1==0 && S0==0 )
					Y <= I4;
				else if( S2 == 1 && S1==0 && S0==1 )
					Y <= I5;
				else if( S2 == 1 && S1==1 && S0==0 )
					Y <= I6;
				 else
					Y <= I7;

 	end endmodule

 module Dcd2x4(X3, X4, D0, D1, D2, D3);
    input X3, X4;
    output D3, D2, D1, D0;
    reg D3, D2, D1, D0;
    always @(X3, X4)
    begin
       if (X3==0 && X4==0)
       begin
          D3 <= 0; D2 <= 0; D1 <= 0; D0 <= 1;
       end
       else if (X3==1 && X4==0)
       begin
          D3 <= 0; D2 <= 0; D1 <= 1; D0 <= 0;
       end
       else if (X3==0 && X4==1)
       begin
          D3 <= 0; D2 <= 1; D1 <= 0; D0 <= 0;
       end
	else begin
          D3 <= 1; D2 <= 0; D1 <= 0; D0 <= 0;
       end
    end
 endmodule



