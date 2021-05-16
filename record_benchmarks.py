#!/usr/bin/env python3
import subprocess
import sys
from math import log, exp
import numpy as np
import matplotlib.pyplot as plt
import pickle

ENCODING = 'utf8'
INPUTS = ['apple.in_new', 'apple.in_old', 'beaver.in', 'box.in', 'explosion.in',
          'koch.in', 'mit.in', 'sin_wave.in', 'smalllines.in', 
          'fewcoronavirus.in', 'manycoronavirus.in', 'trees.in', 
          'bigcoronavirus.in', 'large.in', 'diagonal.in']

if 'limited' in sys.argv:
    INPUTS = INPUTS[:9]

def run_test(executable, input_file):
    r = subprocess.run(f'(./{executable} < {input_file}) > temp.out', 
                       shell=True)
##    print(subprocess.run('dir > temp.out', shell=True))
##    r = subprocess.run(f'(./{executable} < {input_file}) > temp.out', 
##                       shell=True)
    # print("command is", f'(./{executable} < {input_file}) > temp.out')
    # print("out is", open('temp.out', 'r').read())
    return open('temp.out', 'r').read()
    result = str(r.stdout, encoding=ENCODING)
    print(result)
    return result

def test_generation(executable):
    ns = []
    for i in range(2, 5):
        for j in range(1, 10):
            n = 10**i*j
            ops = 10*n
            output_file = f"benchmark_{i}_{j}.in"
            with open('temp.in', 'w') as f:
                f.write(f"{ops} {n}")
            r = subprocess.run(f'(./{executable} < temp.in) > {output_file}', 
                        shell=True)
            print(f"done with {i}, {j}")


if __name__ == "__main__":
    # test_generation('benchmark')
    files = []
    for i in range(2, 5):
        for j in range(1, 10):
            files.append(f"benchmark_{i}_{j}.in")
            if i == 4:
                break
    ns = []
    runtimes = []
    ops = []
    stdevs = []


    out = []
    for f in files:
        
        runtime = []
        for i in range(10):
            # print("benchmark is", run_test("benchmark", f))
            t, op, n = run_test("benchmark", f).split()
            t = float(t)
            op, n = int(op), int(n)
            runtime.append(float(t))
        ns.append(n)
        ops.append(op)
        runtimes.append(np.mean(runtime))
        stdevs.append(np.std(runtime))
        print(op, n, np.mean(runtime), np.std(runtime))
        out.append((op, n, np.mean(runtime), np.std(runtime)))

    with open('out.out', 'wb') as f:
        pickle.dump((ns, runtimes, stdevs), f)