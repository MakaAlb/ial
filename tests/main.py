cases = [
    "2-node1",
    "2-node2",
    "3-node1",
    "3-node2",
    "3-node3",
    "3-node4",
    "3-node5",
    "3-node6",
    "4-node1",
    "4-node2",
    "4-node3",
    "4-node4",
    "4-node5",
    "4-node6",
    "4-node7",
    "4-node8",
    "4-node9",


]

PROGRAMME_PATH = "../hamiltonian_algorithm"


def test_case(case, outfile):

    output = outfile.readline()
    output = output.strip().split(" ")
    output[0] = PROGRAMME_PATH
    stdin = ""
    import subprocess
    # runs process and saves stdin and stdout to variables
    process = subprocess.Popen(output, stdout=subprocess.PIPE, stdin=subprocess.PIPE,
                               stderr=subprocess.PIPE)
    stdout, stderr = process.communicate(stdin.encode())

    if stderr:
        print(stderr.decode())
        print("Error in programme")
        return 0

    expected_output = ""
    tmp = outfile.readline()
    while tmp != "\n" and tmp != "":
        expected_output += tmp
        tmp = outfile.readline()
    expected_output += tmp

    if stdout.decode().strip() != expected_output.strip():
        print(f"Test case {case} failed")
        print(f"Expected: \n{expected_output.strip()}")
        print(":".join("{:02x}".format(ord(c)) for c in expected_output))
        print(f"Got: \n{stdout.decode().strip()}")
        print(":".join("{:02x}".format(ord(c)) for c in stdout.decode()))

        return 0
    print(f"Test case {case} passed")
    return 1


def main():
    success = 0
    for case in cases:
        print(f"Running test case: {case}")
        outfile = open(f"{case}.out", "r")
        success += test_case(case, outfile)
        success += test_case(case, outfile)
        outfile.close()
    print(f"Passed {success}/{len(cases)*2} test cases")




if __name__ == "__main__":
    main()
