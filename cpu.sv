`define LOW8 7:0
`define LOW16 15:0

module cpu(input clock, input reset);

    localparam DATA_SIZE = 8;
    localparam INSTRUCTION_SIZE = 24;
    localparam ADDRESS_SPACE = 16;

    localparam STACK_TOP = (1 << (ADDRESS_SPACE - 1)) - 1;

    reg[7:0] register_file[4]; //x, y, z, w
    reg[14:0] instruction_pointer;
    reg[7:0] stack_pointer;

    reg[7:0] ram[1 << (ADDRESS_SPACE - 1)];
    reg[7:0] rom[1 << (ADDRESS_SPACE - 2)];
    reg[7:0] dsp[1 << (ADDRESS_SPACE - 2)]; //active when 2 MSBs are set

    wire[23:0] fetch = { rom[instruction_pointer], rom[instruction_pointer+1], rom[instruction_pointer+2] };

    wire opcode = fetch[23:20];
    wire operand1 = fetch[19:18];
    wire operand2 = fetch[17:16];
    wire immediate = fetch[15:8];
    wire memory = fetch[15:0];


    initial 
    begin
        //for (integer i = 0; i < )
    end

    always @(posedge clock)
    begin
        if (reset)
        begin

        end

        else 
        begin
            case (opcode)
                4'b0000:
                begin
                    //move reg, reg
                    register_file[operand1] = register_file[operand2];
                end

                4'b0001:
                begin
                    //copy reg, imm
                    register_file[operand1] = immediate;
                end

                4'b0010:
                begin
                    //load reg, mem
                    register_file[operand1] = ram[operand2];
                end

                4'b0011:
                begin
                    //save mem, reg
                    ram[operand1] = register_file[operand2];
                end

                4'b0100:
                begin
                    //iadd reg, reg
                    register_file[operand1] += register_file[operand2];
                end

                4'b0101:
                begin
                    //isub reg, reg
                    register_file[operand1] -= register_file[operand2];
                end

                4'b0110:
                begin
                    //band reg, reg
                    register_file[operand1] &= register_file[operand2];
                end

                4'b0111:
                begin
                    //bior reg, reg
                    register_file[operand1] |= register_file[operand2];
                end

                4'b1000:
                begin
                    //bxor reg, reg
                    register_file[operand1] ^= register_file[operand2];
                end

                4'b1001:
                begin
                    //bnot reg
                    register_file[operand1] = ~register_file[operand2];
                end

                4'b1010:
                begin
                    //call mem
                    ram[STACK_TOP - stack_pointer] = instruction_pointer[14:8];
                    stack_pointer++;
                    ram[STACK_TOP - stack_pointer] = instruction_pointer[7:0];
                    stack_pointer++;
                    instruction_pointer = memory;
                end

                4'b1011:
                begin
                    //exit
                    stack_pointer--;
                    instruction_pointer[7:0] = ram[STACK_TOP - stack_pointer];
                    stack_pointer--;
                    instruction_pointer[14:8] = ram[STACK_TOP - stack_pointer];
                end

                4'b1100:
                begin
                    //push reg
                    ram[STACK_TOP - stack_pointer] = register_file[operand1];
                    stack_pointer++;
                end

                4'b1101:
                begin
                    //pull reg
                    stack_pointer--;
                    register_file[operand1] = ram[STACK_TOP - stack_pointer];
                end

                4'b1110:
                begin
                    //brez mem, reg
                    if (register_file[operand2] == 0)
                    begin
                        instruction_pointer = memory;
                    end
                end
                
                4'b1111:
                begin
                    //XXXX
                end
            endcase

            instruction_pointer += 3;
        end
    end
endmodule