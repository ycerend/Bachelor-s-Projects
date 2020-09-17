`timescale 1ns/1ns
module source (
	output reg [1:0]y,
	input x,
	input rst,
	input clk
);

parameter S0 = 3'b000,S1 = 3'b001, S2 = 3'b010,S3 = 3'b011,S4 = 3'b100,S5 = 3'b101,S6 = 3'b110,S7 = 3'b111;

reg [2:0] stateReg;
reg [2:0] nextState;

initial begin
	stateReg[0] <= 1'b0;
	stateReg[1] <= 1'b0;
	stateReg[2] <= 1'b0;
end



always @(stateReg, x) begin
  case(stateReg)
    S0: begin
      if(x == 0) begin
        nextState <= S1;
        y <= 2'b00;
      end
      else begin
        nextState <= S0;
        y <= 2'b00;
      end
    end
    S1: begin
      if(x == 0) begin
        nextState <= S1;
        y <= 2'b00;
      end
      else begin
        nextState <= S2;
        y <= 2'b00;
      end
    end
    S2: begin
      if(x == 0) begin
        nextState <= S1;
        y <= 2'b01;
      end
      else begin
        nextState <= S3;
        y <= 2'b00;
      end
    end
    S3: begin
      if(x == 0) begin
        nextState <= S1;
        y <= 2'b10;
      end
      else begin
        nextState <= S4;
        y <= 2'b00;
      end
    end
    S4: begin
      if(x == 0) begin
        nextState <= S1;
        y <= 2'b11;
      end
      else begin
        nextState <= S4;
        y <= 2'b00;
      end
    end
    default: begin
      if(x == 0) begin
        nextState <= S0;
        y <= 2'b00;
      end
      else begin
        nextState <= S0;
        y <= 2'b00;
      end
    end
  endcase
end


always @(posedge clk) begin
    stateReg <= nextState;
end

always @(negedge clk) begin
  if(rst == 1'b1) begin
    stateReg <= S0;
    nextState <= S0;
    y <= 2'b00;
  end
end

endmodule

