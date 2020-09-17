`timescale 1ns/1ns
module source(F,Cout,Overflow,S,X,Y);

output reg [4:0]F;
output reg Cout;
output reg Overflow;
input [1:0]S;
input [4:0]X;
input [4:0]Y;
reg [5:0]temp;

// Fill here

parameter S0 = 2'b00, S1 = 2'b01, S2 = 2'b10, S3 = 2'b11;

initial begin
	Overflow <= 0;
	Cout <= 0;
	F <= 5'b00000;
end

always @(S) begin
	case(S)
		S0: begin
			Overflow <= 0;
			temp = X[3:1] * Y[2:0];
			F <= temp[4:0];
			Cout <= temp[5];
		end
		S1: begin

		Overflow <= 1'b0;
		F <= 5'b00000;
		if(X>Y)
			Cout <=1'b1;
		else
			Cout <= 1'b0;
		end
		S2: begin
			temp = X + Y; 
			F <= temp[4:0];
			if((temp[4] == 0 && X[4] == 1 && Y[4] == 1) ||
				(temp[4] == 1 && X[4] == 0 && Y[4] == 0)) begin
				Overflow <= 1;
			end
			else begin
				Overflow <= 0;
			end
			Cout <= temp[5];
		end
		S3: begin
			temp = X + ~(4*Y[2:0]) + 1; 
			F <= temp[4:0];
			if((temp[4] == 0 && X[4] == 1 && (~(4*Y[2:0]) + 1) == 1) ||
				(temp[4] == 1 && X[4] == 0 && (~(4*Y[2:0]) + 1) == 0)) begin
				Overflow <= 1;
			end
			else begin
				Overflow <= 0;
			end
			Cout <= temp[5];
		end
		default: begin
			Overflow <= 0;
			Cout <= 0;
			F <= 5'b00000;
		end
	endcase
end

endmodule