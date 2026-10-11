import random


class DRAM:
    def __init__(self, latency):
        self.latency = latency
        self.memory = {
            i: random.randint(0, 10)
            for i in range(100)
        }

    def load(self, address, cycle_count):
        print(f"DRAM Access for address {address}.")
        cycle_count += self.latency
        return self.memory[address], cycle_count


class Cache:
    def __init__(self, name, latency, next_level):
        self.name = name
        self.latency = latency
        self.store = {}
        self.next_level = next_level

    def store_data(self, address, data):
        self.store[address] = data

    def load(self, address, cycle_count):
        if address in self.store:
            print(f"{self.name} Cache Hit for address {address}.")
            cycle_count += self.latency
            return self.store[address], cycle_count
        else:
            print(
                f"{self.name} Cache Miss for address {address}. "
                f"Accessing next level..."
            )

            if self.next_level:
                data, cycle_count = self.next_level.load(
                    address,
                    cycle_count
                )

                self.store[address] = data
                cycle_count += self.latency

                return data, cycle_count
            else:
                raise ValueError(
                    "Address not found in memory hierarchy."
                )


class ExecutionContext:
    def __init__(self):
        self.registers = {
            f"R{i}": 0
            for i in range(8)
        }
        self.PC = 0


class ALU:
    def __init__(self):
        pass

    def add(self, a, b):
        result, latency = self.execute("ADD", a, b)
        return result

    def mul(self, a, b):
        result, latency = self.execute("MUL", a, b)
        return result

    def execute(self, opcode, operand1, operand2):
        if opcode == "ADD":
            latency = 1
            return operand1 + operand2, latency

        elif opcode == "MUL":
            latency = 3
            return operand1 * operand2, latency

        else:
            raise ValueError(
                f"Unknown opcode: {opcode}"
            )


class ControlUnit:
    def __init__(self, context, alu, cache):
        self.context = context
        self.alu = alu
        self.cache = cache
        self.cycle_count = 0

    def fetch(self, program):
        instr = program[self.context.PC]
        self.cycle_count += 1

        print(
            f"Fetching instruction at PC={self.context.PC}: "
            f"{instr}"
        )

        self.context.PC += 1
        return instr

    def decode(self, instr):
        self.cycle_count += 1

        print(
            f"Decoding instruction: {instr}, "
            f"Current Cycle Count: {self.cycle_count}"
        )

        opcode, dest, src1, src2 = instr
        return opcode, dest, src1, src2

    def execute(self, opcode, dest, src1, src2):
        self.cycle_count += 1

        print(
            f"Executing instruction: {opcode} "
            f"with operands {src1} and {src2}, "
            f"Current Cycle Count: {self.cycle_count}"
        )

        if isinstance(src1, str):
            if src1.startswith("R"):
                val1 = self.context.registers[src1]
            else:
                val1, self.cycle_count = self.cache.load(
                    src1,
                    self.cycle_count
                )
        else:
            val1 = src1

        if isinstance(src2, str):
            if src2.startswith("R"):
                val2 = self.context.registers[src2]
            else:
                val2, self.cycle_count = self.cache.load(
                    src2,
                    self.cycle_count
                )
        else:
            val2 = src2

        print(f"Operand values: {val1}, {val2}")

        result, alu_latency = self.alu.execute(
            opcode,
            val1,
            val2
        )

        for _ in range(alu_latency - 1):
            self.cycle_count += 1

            print(
                f"ALU operation in progress... "
                f"Current Cycle Count: {self.cycle_count}"
            )

        print(f"ALU Result: {result}")

        return result, dest

    def write_back(self, dest, result):
        self.context.registers[dest] = result
        self.cycle_count += 1

        print(
            f"Writing back result to {dest}, "
            f"Current Cycle Count: {self.cycle_count}"
        )

        return self.cycle_count


if __name__ == "__main__":

    dram = DRAM(latency=50)

    L3 = Cache(
        "L3",
        latency=20,
        next_level=dram
    )

    L2 = Cache(
        "L2",
        latency=10,
        next_level=L3
    )

    L1 = Cache(
        "L1",
        latency=2,
        next_level=L2
    )

    context = ExecutionContext()

    alu = ALU()

    cu = ControlUnit(
        context=context,
        alu=alu,
        cache=L1
    )

    program = [
        ("ADD", "R1", 10, 12),
        ("MUL", "R2", "R1", 15)
    ]

    print(
        "Starting Scalar Processor Simulation "
        "with Pipeline and Memory Latencies...\n"
    )

    for idx in range(len(program)):
        instr = cu.fetch(program)

        opcode, dest, src1, src2 = cu.decode(instr)

        result, dest = cu.execute(
            opcode,
            dest,
            src1,
            src2
        )

        cycles_used = cu.write_back(
            dest,
            result
        )

        print(
            f"Instruction {idx + 1}: {instr} "
            f"executed. Result = {result}, "
            f"stored in {dest}."
        )

        print(
            f"Total cycles so far: "
            f"{cycles_used}\n"
        )

    print("\nFinal Register States:")

    for register, value in context.registers.items():
        print(f"{register} = {value}")

    print(
        f"\nTotal Cycles Used: "
        f"{cu.cycle_count}"
    )