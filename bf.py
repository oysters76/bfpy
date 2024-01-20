import sys 

BF_C = "+-<>[],."
SIZE = 3000 
PROGRAM_MEM = [0 for i in range(SIZE)] 
EOF = "-1"

def isbf(l):
    return l in BF_C

def read_and_parse_bf(path):
    with open(path) as f:
        program = "" 
        for line in f.readlines(): 
            if line == "\n":
                continue 
            for l in line:
                if isbf(l):
                    program += l
    return program 

def find_all_jumps(program):
    stack = []  
    jumps = {}
    jumps_i = {} 
    for i, c in enumerate(program):
        if (c == "["):
            stack.append(i)
        if (c == "]"):
            prev_i = stack.pop()
            jumps[prev_i] = i; 
            jumps_i[i] = prev_i;
    return jumps, jumps_i
    
def run(program, jumps,jumps_i, mem):
    data_pointer = 0; 
    ip = 0 
    while True:
        if (ip >= len(program)):
            break 
        c = program[ip]; 
        if (c == ">"):
            data_pointer += 1; 
        if (c == "<"):
            data_pointer -= 1; 
        if (c == "+"):
            mem[data_pointer] += 1 
        if (c == "-"):
            mem[data_pointer] -= 1 
        if (c == "."):
            print(chr(mem[data_pointer]), end="") 
        if (c == ","):
            v = input() 
            if (v == EOF): #EOF input
                ip += 1 
                continue
            mem[data_pointer] = ord(v)  
        if (c == "["):
            b = mem[data_pointer] 
            if (b == 0):
                ip = jumps[ip]
        if (c == "]"):
            b = mem[data_pointer]
            if (b != 0):
                ip = jumps_i[ip]
        ip += 1
            
def main(program_path):
    program = read_and_parse_bf(program_path) 
    jumps, jumps_i = find_all_jumps(program)
    run(program, jumps, jumps_i, PROGRAM_MEM)

if (len(sys.argv) == 1):
    print("[Error] Usage: <input.bf>") 
    sys.exit(1) 

main(sys.argv[1])