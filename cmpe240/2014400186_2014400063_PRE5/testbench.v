`timescale 1ns/1ns
module testbench();

wire [4:0]F;
wire Cout;
wire Overflow;
reg [1:0]S;
reg [4:0]X;
reg [4:0]Y;

source s(.F(F),.Cout(Cout),.Overflow(Overflow),.S(S),.X(X),.Y(Y));

initial begin
    $dumpfile("TimingDiagram.vcd");
    $dumpvars(0, F,Cout,Overflow,S,X,Y);
  X = 5'b01111;
  Y = 5'b00001;
  S = 2'b00;
  #60;
  X = 5'b10100;
  Y = 5'b10110;
  S = 2'b01;
  #60;
  X = 5'b01001;
  Y = 5'b11110;
  S = 2'b10;
  #60;
  X = 5'b11000;
  Y = 5'b11110;
  S = 2'b11;
  #60;
  X = 5'b01111;
  Y = 5'b10101;
  S = 2'b01;
  #60;
  X = 5'b01110;
  Y = 5'b11101;
  S = 2'b10;
  #60;
  X = 5'b11110;
  Y = 5'b01001;
  S = 2'b11;
  #60;
  X = 5'b10101;
  Y = 5'b10101;
  S = 2'b00;
  #60;
  X = 5'b11010;
  Y = 5'b11111;
  S = 2'b10;
  #60;
  X = 5'b01111;
  Y = 5'b11001;
  S = 2'b01;
  #60;
  X = 5'b01110;
  Y = 5'b11111;
  S = 2'b11;
  #60;
  
    $finish;
end

endmodule