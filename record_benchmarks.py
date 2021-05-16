#!/usr/bin/env python3
import subprocess
import sys
from math import log, exp
import numpy as np
import matplotlib.pyplot as plt

ENCODING = 'utf8'
INPUTS = ['apple.in_new', 'apple.in_old', 'beaver.in', 'box.in', 'explosion.in',
          'koch.in', 'mit.in', 'sin_wave.in', 'smalllines.in', 
          'fewcoronavirus.in', 'manycoronavirus.in', 'trees.in', 
          'bigcoronavirus.in', 'large.in', 'diagonal.in']

if 'limited' in sys.argv:
    INPUTS = INPUTS[:9]

def run_test(executable, input_file):
    r = subprocess.run(f'./{executable} < {input_file}', 
                       shell=True, executable = '/bin/bash.exe')
##    print(subprocess.run('dir > temp.out', shell=True))
##    r = subprocess.run(f'(./{executable} < {input_file}) > temp.out', 
##                       shell=True)
    print("command is", f'(./{executable} < {input_file}) > temp.out')
    print("out is", open('temp.out', 'r').read())
    return open('temp.out', 'r').read()
    result = str(r.stdout, encoding=ENCODING)
    print(result)
    return result

if __name__ == "__main__":
    files = [f'benchmark_1e{i+1}_1e{i}.in' for i in range(6)]
    print("files is", files)
    ns = []
    runtimes = []
    ops = []
    stdevs = []
    
    for f in files:
        
        runtime = []
        for i in range(10):
            print("benchmark is", run_test("benchmark", f))
            t, op, n = run_test("benchmark", f).split()
            t = float(t)
            op, n = int(op), int(n)
            runtime.append(float(t))
        ns.append(n)
        ops.append(op)
        runtimes.append(np.mean(runtime))
        stdevs.append(np.std(runtime))
    plt.errorbar(ns, runtimes, stdevs)
    plt.show()

def parse_awsrun_output(output):
    result = {}
    for i in output.split('\n'):
        if i[:16] == 'Number of frames':
            result['Frames:'] = int(i.split()[-1])
        if i[:18] == 'Input file path is':
            result['File:'] = i.split()[-1].split('/')[-1]
        if i[:22] == 'Elapsed execution time':
            result['Time:'] = float(i.split()[-1][:-1])
        if i[-20:] == 'Line-Wall Collisions':
            result['Line-Wall Collisions:'] = int(i.split()[0])
        if i[-20:] == 'Line-Line Collisions':
            result['Line-Line Collisions:'] = int(i.split()[0])
        if i[:7] == 'COUNTER':
            result[i.split('(')[1].split(')')[0]] = i.split('<')[1].split('>')[0]
    return result

def pretty_print(parsed_out, tabular=False):
    if isinstance(parsed_out, list):
        for i in parsed_out:
            pretty_print(i, tabular=tabular)
            if not tabular:
                print()
        return
    if tabular:
        result = [parsed_out['File:'] + ' ' * (20 - len(parsed_out['File:'])), 
                  parsed_out['Time:']] + \
                  ([parsed_out['Time Improvement:']] 
                  if 'Time Improvement:' in parsed_out 
                  else [])
        print('\t'.join([str(i) for i in result]))
    else:
        for i in sorted(parsed_out):
            print(i, parsed_out[i])

def comp_correctness(reference, current):
    for characteristic in ['Line-Line Collisions:', 'Line-Wall Collisions:']:
        if reference[characteristic] != current[characteristic]:
            print("Mismatch!\nReference=")
            print(reference)
            print("Current:")
            print(current)
            print()
            return False
    return True

def comp_correctness_list(reference, current):
    failures = 0
    for output in current:
        for correct in reference:
            if correct['Frames:'] == output['Frames:'] and correct['File:'] == output['File:']:
                if comp_correctness(correct, output):
                    output['Time Improvement:'] = output['Time:'] / correct['Time:']
                    break
                else:
                    failures += 1
                    break
        else:
            failures += 1
            print(f'Missing reference values for {output["File:"]}')
    if failures == 0:
        print("All Correct!")
    return failures

def run_all_inputs(executable, num_frames, inputs, local=False):
    results = []
    for input_filename in inputs:
        results.append(parse_awsrun_output(run_test(executable, num_frames, input_filename, local=local)))
    return results

#reference = run_all_inputs('prev', 4200, INPUTS, local=True)
#prev_reference = [{'Frames:': 4200, 'File:': 'apple.in_new', 'Time:': 33.778843, 'Line-Wall Collisions:': 3095, 'Line-Line Collisions:': 106037}, {'Frames:': 4200, 'File:': 'apple.in_old', 'Time:': 34.179665, 'Line-Wall Collisions:': 3328, 'Line-Line Collisions:': 107993}, {'Frames:': 4200, 'File:': 'beaver.in', 'Time:': 10.382135, 'Line-Wall Collisions:': 234, 'Line-Line Collisions:': 1904}, {'Frames:': 4200, 'File:': 'box.in', 'Time:': 18.550533, 'Line-Wall Collisions:': 6628, 'Line-Line Collisions:': 105341}, {'Frames:': 4200, 'File:': 'explosion.in', 'Time:': 38.607492, 'Line-Wall Collisions:': 1379, 'Line-Line Collisions:': 44737}, {'Frames:': 4200, 'File:': 'koch.in', 'Time:': 1724.076757, 'Line-Wall Collisions:': 10297, 'Line-Line Collisions:': 39269}, {'Frames:': 4200, 'File:': 'mit.in', 'Time:': 64.551017, 'Line-Wall Collisions:': 1330, 'Line-Line Collisions:': 20958}, {'Frames:': 4200, 'File:': 'sin_wave.in', 'Time:': 127.907795, 'Line-Wall Collisions:': 1878, 'Line-Line Collisions:': 483707}, {'Frames:': 4200, 'File:': 'smalllines.in', 'Time:': 28.608238, 'Line-Wall Collisions:': 25163, 'Line-Line Collisions:': 357401}]
#bitree_reference_oldest = [{'Frames:': 4200, 'File:': 'apple.in_new', 'Time:': 0.235245, 'Line-Wall Collisions:': 3095, 'Line-Line Collisions:': 106037, 'Calls to Intersect': '587205'}, {'Frames:': 4200, 'File:': 'apple.in_old', 'Time:': 4.504476, 'Line-Wall Collisions:': 3328, 'Line-Line Collisions:': 107993, 'Calls to Intersect': '64006170'}, {'Frames:': 4200, 'File:': 'beaver.in', 'Time:': 0.148022, 'Line-Wall Collisions:': 234, 'Line-Line Collisions:': 1904, 'Calls to Intersect': '339447'}, {'Frames:': 4200, 'File:': 'box.in', 'Time:': 0.217908, 'Line-Wall Collisions:': 6628, 'Line-Line Collisions:': 105341, 'Calls to Intersect': '234362'}, {'Frames:': 4200, 'File:': 'explosion.in', 'Time:': 0.418182, 'Line-Wall Collisions:': 1379, 'Line-Line Collisions:': 44737, 'Calls to Intersect': '2603736'}, {'Frames:': 4200, 'File:': 'koch.in', 'Time:': 1.079042, 'Line-Wall Collisions:': 10297, 'Line-Line Collisions:': 39269, 'Calls to Intersect': '256879'}, {'Frames:': 4200, 'File:': 'mit.in', 'Time:': 0.624103, 'Line-Wall Collisions:': 1330, 'Line-Line Collisions:': 20958, 'Calls to Intersect': '3592865'}, {'Frames:': 4200, 'File:': 'sin_wave.in', 'Time:': 0.554152, 'Line-Wall Collisions:': 1878, 'Line-Line Collisions:': 483707, 'Calls to Intersect': '337247'}, {'Frames:': 4200, 'File:': 'smalllines.in', 'Time:': 1.61107, 'Line-Wall Collisions:': 25163, 'Line-Line Collisions:': 357401, 'Calls to Intersect': '19905781'}]
#bitree_reference_older = [{'Frames:': 4200, 'File:': 'apple.in_new', 'Time:': 0.233018, 'Line-Wall Collisions:': 3095, 'Line-Line Collisions:': 106037}, {'Frames:': 4200, 'File:': 'apple.in_old', 'Time:': 0.231149, 'Line-Wall Collisions:': 3328, 'Line-Line Collisions:': 107993}, {'Frames:': 4200, 'File:': 'beaver.in', 'Time:': 0.146079, 'Line-Wall Collisions:': 234, 'Line-Line Collisions:': 1904}, {'Frames:': 4200, 'File:': 'box.in', 'Time:': 0.217397, 'Line-Wall Collisions:': 6628, 'Line-Line Collisions:': 105341}, {'Frames:': 4200, 'File:': 'explosion.in', 'Time:': 0.367552, 'Line-Wall Collisions:': 1379, 'Line-Line Collisions:': 44737}, {'Frames:': 4200, 'File:': 'koch.in', 'Time:': 1.065638, 'Line-Wall Collisions:': 10297, 'Line-Line Collisions:': 39269}, {'Frames:': 4200, 'File:': 'mit.in', 'Time:': 0.459392, 'Line-Wall Collisions:': 1330, 'Line-Line Collisions:': 20958}, {'Frames:': 4200, 'File:': 'sin_wave.in', 'Time:': 0.404295, 'Line-Wall Collisions:': 1878, 'Line-Line Collisions:': 483707}, {'Frames:': 4200, 'File:': 'smalllines.in', 'Time:': 1.406935, 'Line-Wall Collisions:': 25163, 'Line-Line Collisions:': 357401}]
#bitree_reference_old = [{'Frames:': 4200, 'File:': 'apple.in_new', 'Time:': 0.212423, 'Line-Wall Collisions:': 3095, 'Line-Line Collisions:': 106037}, {'Frames:': 4200, 'File:': 'apple.in_old', 'Time:': 0.219372, 'Line-Wall Collisions:': 3328, 'Line-Line Collisions:': 107993}, {'Frames:': 4200, 'File:': 'beaver.in', 'Time:': 0.14405, 'Line-Wall Collisions:': 234, 'Line-Line Collisions:': 1904}, {'Frames:': 4200, 'File:': 'box.in', 'Time:': 0.215232, 'Line-Wall Collisions:': 6628, 'Line-Line Collisions:': 105341}, {'Frames:': 4200, 'File:': 'explosion.in', 'Time:': 0.359232, 'Line-Wall Collisions:': 1379, 'Line-Line Collisions:': 44737}, {'Frames:': 4200, 'File:': 'koch.in', 'Time:': 1.057604, 'Line-Wall Collisions:': 10297, 'Line-Line Collisions:': 39269}, {'Frames:': 4200, 'File:': 'mit.in', 'Time:': 0.409512, 'Line-Wall Collisions:': 1330, 'Line-Line Collisions:': 20958}, {'Frames:': 4200, 'File:': 'sin_wave.in', 'Time:': 0.398808, 'Line-Wall Collisions:': 1878, 'Line-Line Collisions:': 483707}, {'Frames:': 4200, 'File:': 'smalllines.in', 'Time:': 1.291577, 'Line-Wall Collisions:': 25163, 'Line-Line Collisions:': 357401}]
#bitree_reference = [{'Frames:': 4200, 'File:': 'apple.in_new', 'Time:': 0.233538, 'Line-Wall Collisions:': 3095, 'Line-Line Collisions:': 106037}, {'Frames:': 4200, 'File:': 'apple.in_old', 'Time:': 0.210735, 'Line-Wall Collisions:': 3328, 'Line-Line Collisions:': 107993}, {'Frames:': 4200, 'File:': 'beaver.in', 'Time:': 0.153505, 'Line-Wall Collisions:': 234, 'Line-Line Collisions:': 1904}, {'Frames:': 4200, 'File:': 'box.in', 'Time:': 0.210866, 'Line-Wall Collisions:': 6628, 'Line-Line Collisions:': 105341}, {'Frames:': 4200, 'File:': 'explosion.in', 'Time:': 0.343651, 'Line-Wall Collisions:': 1379, 'Line-Line Collisions:': 44737}, {'Frames:': 4200, 'File:': 'koch.in', 'Time:': 0.995638, 'Line-Wall Collisions:': 10297, 'Line-Line Collisions:': 39269}, {'Frames:': 4200, 'File:': 'mit.in', 'Time:': 0.374971, 'Line-Wall Collisions:': 1330, 'Line-Line Collisions:': 20958}, {'Frames:': 4200, 'File:': 'sin_wave.in', 'Time:': 0.4016, 'Line-Wall Collisions:': 1878, 'Line-Line Collisions:': 483707}, {'Frames:': 4200, 'File:': 'smalllines.in', 'Time:': 0.894918, 'Line-Wall Collisions:': 25163, 'Line-Line Collisions:': 357401}]
#bitree_reference_march_haswell = [{'Frames:': 4200, 'File:': 'apple.in_new', 'Time:': 0.214379, 'Line-Wall Collisions:': 3095, 'Line-Line Collisions:': 106037}, {'Frames:': 4200, 'File:': 'apple.in_old', 'Time:': 0.21183, 'Line-Wall Collisions:': 3328, 'Line-Line Collisions:': 107993}, {'Frames:': 4200, 'File:': 'beaver.in', 'Time:': 0.141414, 'Line-Wall Collisions:': 234, 'Line-Line Collisions:': 1904}, {'Frames:': 4200, 'File:': 'box.in', 'Time:': 0.204422, 'Line-Wall Collisions:': 6628, 'Line-Line Collisions:': 105341}, {'Frames:': 4200, 'File:': 'explosion.in', 'Time:': 0.302382, 'Line-Wall Collisions:': 1379, 'Line-Line Collisions:': 44737}, {'Frames:': 4200, 'File:': 'koch.in', 'Time:': 0.993172, 'Line-Wall Collisions:': 10297, 'Line-Line Collisions:': 39269}, {'Frames:': 4200, 'File:': 'mit.in', 'Time:': 0.366451, 'Line-Wall Collisions:': 1330, 'Line-Line Collisions:': 20958}, {'Frames:': 4200, 'File:': 'sin_wave.in', 'Time:': 0.381727, 'Line-Wall Collisions:': 1878, 'Line-Line Collisions:': 483707}, {'Frames:': 4200, 'File:': 'smalllines.in', 'Time:': 0.706005, 'Line-Wall Collisions:': 25163, 'Line-Line Collisions:': 357401}]

num_frames = 4200

#reference = [{'Frames:': 4200, 'File:': 'apple.in_new', 'Time:': 0.19752, 'Line-Wall Collisions:': 3095, 'Line-Line Collisions:': 106037}, {'Frames:': 4200, 'File:': 'apple.in_old', 'Time:': 0.191497, 'Line-Wall Collisions:': 3328, 'Line-Line Collisions:': 107993}, {'Frames:': 4200, 'File:': 'beaver.in', 'Time:': 0.134518, 'Line-Wall Collisions:': 234, 'Line-Line Collisions:': 1904}, {'Frames:': 4200, 'File:': 'box.in', 'Time:': 0.196432, 'Line-Wall Collisions:': 6628, 'Line-Line Collisions:': 105341}, {'Frames:': 4200, 'File:': 'explosion.in', 'Time:': 0.288299, 'Line-Wall Collisions:': 1379, 'Line-Line Collisions:': 44737}, {'Frames:': 4200, 'File:': 'koch.in', 'Time:': 0.948028, 'Line-Wall Collisions:': 10297, 'Line-Line Collisions:': 39269}, {'Frames:': 4200, 'File:': 'mit.in', 'Time:': 0.346177, 'Line-Wall Collisions:': 1330, 'Line-Line Collisions:': 20958}, {'Frames:': 4200, 'File:': 'sin_wave.in', 'Time:': 0.363307, 'Line-Wall Collisions:': 1878, 'Line-Line Collisions:': 483707}, {'Frames:': 4200, 'File:': 'smalllines.in', 'Time:': 0.691577, 'Line-Wall Collisions:': 25163, 'Line-Line Collisions:': 357401}, {'Frames:': 4200, 'File:': 'fewcoronavirus.in', 'Time:': 1.317982, 'Line-Wall Collisions:': 3970, 'Line-Line Collisions:': 41902}, {'Frames:': 4200, 'File:': 'manycoronavirus.in', 'Time:': 7.742849, 'Line-Wall Collisions:': 16265, 'Line-Line Collisions:': 765201}, {'Frames:': 4200, 'File:': 'trees.in', 'Time:': 0.167548, 'Line-Wall Collisions:': 587, 'Line-Line Collisions:': 11886}, {'Frames:': 4200, 'File:': 'bigcoronavirus.in', 'Time:': 6.243817, 'Line-Wall Collisions:': 501, 'Line-Line Collisions:': 184636}, {'Frames:': 4200, 'File:': 'large.in', 'Time:': 0.494158, 'Line-Wall Collisions:': 107629, 'Line-Line Collisions:': 1196853}]
#reference = [{'Frames:': 4200, 'File:': 'apple.in_new', 'Time:': 0.196589, 'Line-Wall Collisions:': 3095, 'Line-Line Collisions:': 106037}, {'Frames:': 4200, 'File:': 'apple.in_old', 'Time:': 0.192437, 'Line-Wall Collisions:': 3328, 'Line-Line Collisions:': 107993}, {'Frames:': 4200, 'File:': 'beaver.in', 'Time:': 0.139989, 'Line-Wall Collisions:': 234, 'Line-Line Collisions:': 1904}, {'Frames:': 4200, 'File:': 'box.in', 'Time:': 0.194925, 'Line-Wall Collisions:': 6628, 'Line-Line Collisions:': 105341}, {'Frames:': 4200, 'File:': 'explosion.in', 'Time:': 0.284492, 'Line-Wall Collisions:': 1379, 'Line-Line Collisions:': 44737}, {'Frames:': 4200, 'File:': 'koch.in', 'Time:': 0.934395, 'Line-Wall Collisions:': 10297, 'Line-Line Collisions:': 39269}, {'Frames:': 4200, 'File:': 'mit.in', 'Time:': 0.345473, 'Line-Wall Collisions:': 1330, 'Line-Line Collisions:': 20958}, {'Frames:': 4200, 'File:': 'sin_wave.in', 'Time:': 0.356244, 'Line-Wall Collisions:': 1878, 'Line-Line Collisions:': 483707}, {'Frames:': 4200, 'File:': 'smalllines.in', 'Time:': 0.643035, 'Line-Wall Collisions:': 25163, 'Line-Line Collisions:': 357401}]
#reference = [{'Frames:': 4200, 'File:': 'apple.in_new', 'Time:': 0.196583, 'Line-Wall Collisions:': 3095, 'Line-Line Collisions:': 106037}, {'Frames:': 4200, 'File:': 'apple.in_old', 'Time:': 0.192087, 'Line-Wall Collisions:': 3328, 'Line-Line Collisions:': 107993}, {'Frames:': 4200, 'File:': 'beaver.in', 'Time:': 0.12908, 'Line-Wall Collisions:': 234, 'Line-Line Collisions:': 1904}, {'Frames:': 4200, 'File:': 'box.in', 'Time:': 0.191983, 'Line-Wall Collisions:': 6628, 'Line-Line Collisions:': 105341}, {'Frames:': 4200, 'File:': 'explosion.in', 'Time:': 0.275383, 'Line-Wall Collisions:': 1379, 'Line-Line Collisions:': 44737}, {'Frames:': 4200, 'File:': 'koch.in', 'Time:': 0.937729, 'Line-Wall Collisions:': 10297, 'Line-Line Collisions:': 39269}, {'Frames:': 4200, 'File:': 'mit.in', 'Time:': 0.334876, 'Line-Wall Collisions:': 1330, 'Line-Line Collisions:': 20958}, {'Frames:': 4200, 'File:': 'sin_wave.in', 'Time:': 0.356591, 'Line-Wall Collisions:': 1878, 'Line-Line Collisions:': 483707}, {'Frames:': 4200, 'File:': 'smalllines.in', 'Time:': 0.593633, 'Line-Wall Collisions:': 25163, 'Line-Line Collisions:': 357401}, {'Frames:': 4200, 'File:': 'fewcoronavirus.in', 'Time:': 1.303609, 'Line-Wall Collisions:': 3970, 'Line-Line Collisions:': 41902}, {'Frames:': 4200, 'File:': 'manycoronavirus.in', 'Time:': 7.819592, 'Line-Wall Collisions:': 16265, 'Line-Line Collisions:': 765201}, {'Frames:': 4200, 'File:': 'trees.in', 'Time:': 0.169435, 'Line-Wall Collisions:': 587, 'Line-Line Collisions:': 11886}, {'Frames:': 4200, 'File:': 'bigcoronavirus.in', 'Time:': 6.315121, 'Line-Wall Collisions:': 501, 'Line-Line Collisions:': 184636}, {'Frames:': 4200, 'File:': 'large.in', 'Time:': 0.446079, 'Line-Wall Collisions:': 107629, 'Line-Line Collisions:': 1196853}, {'Frames:': 4200, 'File:': 'diagonal.in', 'Time:': 4.933607, 'Line-Wall Collisions:': 13152, 'Line-Line Collisions:': 12557117}]
#reference = [{'Frames:': 4200, 'File:': 'apple.in_new', 'Time:': 0.190539, 'Line-Wall Collisions:': 3095, 'Line-Line Collisions:': 106037}, {'Frames:': 4200, 'File:': 'apple.in_old', 'Time:': 0.184464, 'Line-Wall Collisions:': 3328, 'Line-Line Collisions:': 107993}, {'Frames:': 4200, 'File:': 'beaver.in', 'Time:': 0.110699, 'Line-Wall Collisions:': 234, 'Line-Line Collisions:': 1904}, {'Frames:': 4200, 'File:': 'box.in', 'Time:': 0.188103, 'Line-Wall Collisions:': 6628, 'Line-Line Collisions:': 105341}, {'Frames:': 4200, 'File:': 'explosion.in', 'Time:': 0.261352, 'Line-Wall Collisions:': 1379, 'Line-Line Collisions:': 44737}, {'Frames:': 4200, 'File:': 'koch.in', 'Time:': 0.918036, 'Line-Wall Collisions:': 10297, 'Line-Line Collisions:': 39269}, {'Frames:': 4200, 'File:': 'mit.in', 'Time:': 0.319014, 'Line-Wall Collisions:': 1330, 'Line-Line Collisions:': 20958}, {'Frames:': 4200, 'File:': 'sin_wave.in', 'Time:': 0.3406, 'Line-Wall Collisions:': 1878, 'Line-Line Collisions:': 483707}, {'Frames:': 4200, 'File:': 'smalllines.in', 'Time:': 0.557003, 'Line-Wall Collisions:': 25163, 'Line-Line Collisions:': 357401}, {'Frames:': 4200, 'File:': 'fewcoronavirus.in', 'Time:': 1.280509, 'Line-Wall Collisions:': 3970, 'Line-Line Collisions:': 41902}, {'Frames:': 4200, 'File:': 'manycoronavirus.in', 'Time:': 7.649679, 'Line-Wall Collisions:': 16265, 'Line-Line Collisions:': 765201}, {'Frames:': 4200, 'File:': 'trees.in', 'Time:': 0.160682, 'Line-Wall Collisions:': 587, 'Line-Line Collisions:': 11886}, {'Frames:': 4200, 'File:': 'bigcoronavirus.in', 'Time:': 6.172591, 'Line-Wall Collisions:': 501, 'Line-Line Collisions:': 184636}, {'Frames:': 4200, 'File:': 'large.in', 'Time:': 0.447973, 'Line-Wall Collisions:': 107629, 'Line-Line Collisions:': 1196853}, {'Frames:': 4200, 'File:': 'diagonal.in', 'Time:': 4.832937, 'Line-Wall Collisions:': 13152, 'Line-Line Collisions:': 12557117}]
#reference = [{'Frames:': 4200, 'File:': 'apple.in_new', 'Time:': 0.185476, 'Line-Wall Collisions:': 3095, 'Line-Line Collisions:': 106037}, {'Frames:': 4200, 'File:': 'apple.in_old', 'Time:': 0.17686, 'Line-Wall Collisions:': 3328, 'Line-Line Collisions:': 107993}, {'Frames:': 4200, 'File:': 'beaver.in', 'Time:': 0.106526, 'Line-Wall Collisions:': 234, 'Line-Line Collisions:': 1904}, {'Frames:': 4200, 'File:': 'box.in', 'Time:': 0.182027, 'Line-Wall Collisions:': 6628, 'Line-Line Collisions:': 105341}, {'Frames:': 4200, 'File:': 'explosion.in', 'Time:': 0.253244, 'Line-Wall Collisions:': 1379, 'Line-Line Collisions:': 44737}, {'Frames:': 4200, 'File:': 'koch.in', 'Time:': 0.912919, 'Line-Wall Collisions:': 10297, 'Line-Line Collisions:': 39269}, {'Frames:': 4200, 'File:': 'mit.in', 'Time:': 0.302653, 'Line-Wall Collisions:': 1330, 'Line-Line Collisions:': 20958}, {'Frames:': 4200, 'File:': 'sin_wave.in', 'Time:': 0.350016, 'Line-Wall Collisions:': 1878, 'Line-Line Collisions:': 483707}, {'Frames:': 4200, 'File:': 'smalllines.in', 'Time:': 0.547338, 'Line-Wall Collisions:': 25163, 'Line-Line Collisions:': 357401}, {'Frames:': 4200, 'File:': 'fewcoronavirus.in', 'Time:': 1.201864, 'Line-Wall Collisions:': 3970, 'Line-Line Collisions:': 41902}, {'Frames:': 4200, 'File:': 'manycoronavirus.in', 'Time:': 6.937902, 'Line-Wall Collisions:': 16265, 'Line-Line Collisions:': 765201}, {'Frames:': 4200, 'File:': 'trees.in', 'Time:': 0.156068, 'Line-Wall Collisions:': 587, 'Line-Line Collisions:': 11886}, {'Frames:': 4200, 'File:': 'bigcoronavirus.in', 'Time:': 5.832011, 'Line-Wall Collisions:': 501, 'Line-Line Collisions:': 184636}, {'Frames:': 4200, 'File:': 'large.in', 'Time:': 0.451987, 'Line-Wall Collisions:': 107629, 'Line-Line Collisions:': 1196853}, {'Frames:': 4200, 'File:': 'diagonal.in', 'Time:': 4.772403, 'Line-Wall Collisions:': 13152, 'Line-Line Collisions:': 12557117}]
reference = [{'Frames:': 4200, 'File:': 'apple.in_new', 'Time:': 0.184273, 'Line-Wall Collisions:': 3095, 'Line-Line Collisions:': 106037}, {'Frames:': 4200, 'File:': 'apple.in_old', 'Time:': 0.168463, 'Line-Wall Collisions:': 3328, 'Line-Line Collisions:': 107993}, {'Frames:': 4200, 'File:': 'beaver.in', 'Time:': 0.118305, 'Line-Wall Collisions:': 234, 'Line-Line Collisions:': 1904}, {'Frames:': 4200, 'File:': 'box.in', 'Time:': 0.164685, 'Line-Wall Collisions:': 6628, 'Line-Line Collisions:': 105341}, {'Frames:': 4200, 'File:': 'explosion.in', 'Time:': 0.244051, 'Line-Wall Collisions:': 1379, 'Line-Line Collisions:': 44737}, {'Frames:': 4200, 'File:': 'koch.in', 'Time:': 0.819569, 'Line-Wall Collisions:': 10297, 'Line-Line Collisions:': 39269}, {'Frames:': 4200, 'File:': 'mit.in', 'Time:': 0.294875, 'Line-Wall Collisions:': 1330, 'Line-Line Collisions:': 20958}, {'Frames:': 4200, 'File:': 'sin_wave.in', 'Time:': 0.345822, 'Line-Wall Collisions:': 1878, 'Line-Line Collisions:': 483707}, {'Frames:': 4200, 'File:': 'smalllines.in', 'Time:': 0.521632, 'Line-Wall Collisions:': 25163, 'Line-Line Collisions:': 357401}, {'Frames:': 4200, 'File:': 'fewcoronavirus.in', 'Time:': 1.077873, 'Line-Wall Collisions:': 3970, 'Line-Line Collisions:': 41902}, {'Frames:': 4200, 'File:': 'manycoronavirus.in', 'Time:': 5.683057, 'Line-Wall Collisions:': 16265, 'Line-Line Collisions:': 765201}, {'Frames:': 4200, 'File:': 'trees.in', 'Time:': 0.162149, 'Line-Wall Collisions:': 587, 'Line-Line Collisions:': 11886}, {'Frames:': 4200, 'File:': 'bigcoronavirus.in', 'Time:': 4.564342, 'Line-Wall Collisions:': 501, 'Line-Line Collisions:': 184636}, {'Frames:': 4200, 'File:': 'large.in', 'Time:': 0.453027, 'Line-Wall Collisions:': 107629, 'Line-Line Collisions:': 1196853}, {'Frames:': 4200, 'File:': 'diagonal.in', 'Time:': 4.705417, 'Line-Wall Collisions:': 13152, 'Line-Line Collisions:': 12557117}]
if 'prev' in sys.argv:
    reference = run_all_inputs('prev', num_frames, INPUTS, local=('local' in sys.argv))
elif 'prev2' in sys.argv:
    reference = run_all_inputs('prev2', num_frames, INPUTS, local=('local' in sys.argv))

result = run_all_inputs('screensaver', num_frames, INPUTS, local=('local' in sys.argv))
print('Result:\n', result)
comp_correctness_list(reference, result)
pretty_print(result, tabular=True)
GM = 0
for r in result:
    GM += log(r['Time:'])
GM /= len(result)
print('Geometric Mean:', exp(GM))

"""
Result:
 [{'Frames:': 4200, 'File:': 'apple.in_new', 'Time:': 0.193664, 'Line-Wall Collisions:': 3095, 'Line-Line Collisions:': 106037}, {'Frames:': 4200, 'File:': 'apple.in_old', 'Time:': 0.186976, 'Line-Wall Collisions:': 3328, 'Line-Line Collisions:': 107993}, {'Frames:': 4200, 'File:': 'beaver.in', 'Time:': 0.111576, 'Line-Wall Collisions:': 234, 'Line-Line Collisions:': 1904}, {'Frames:': 4200, 'File:': 'box.in', 'Time:': 0.189869, 'Line-Wall Collisions:': 6628, 'Line-Line Collisions:': 105341}, {'Frames:': 4200, 'File:': 'explosion.in', 'Time:': 0.264033, 'Line-Wall Collisions:': 1379, 'Line-Line Collisions:': 44737}, {'Frames:': 4200, 'File:': 'koch.in', 'Time:': 0.926876, 'Line-Wall Collisions:': 10297, 'Line-Line Collisions:': 39269}, {'Frames:': 4200, 'File:': 'mit.in', 'Time:': 0.325134, 'Line-Wall Collisions:': 1330, 'Line-Line Collisions:': 20958}, {'Frames:': 4200, 'File:': 'sin_wave.in', 'Time:': 0.348382, 'Line-Wall Collisions:': 1878, 'Line-Line Collisions:': 483707}, {'Frames:': 4200, 'File:': 'smalllines.in', 'Time:': 0.560069, 'Line-Wall Collisions:': 25163, 'Line-Line Collisions:': 357401}, {'Frames:': 4200, 'File:': 'fewcoronavirus.in', 'Time:': 1.293153, 'Line-Wall Collisions:': 3970, 'Line-Line Collisions:': 41902}, {'Frames:': 4200, 'File:': 'manycoronavirus.in', 'Time:': 7.775046, 'Line-Wall Collisions:': 16265, 'Line-Line Collisions:': 765201}, {'Frames:': 4200, 'File:': 'trees.in', 'Time:': 0.162648, 'Line-Wall Collisions:': 587, 'Line-Line Collisions:': 11886}, {'Frames:': 4200, 'File:': 'bigcoronavirus.in', 'Time:': 6.305879, 'Line-Wall Collisions:': 501, 'Line-Line Collisions:': 184636}, {'Frames:': 4200, 'File:': 'large.in', 'Time:': 0.449254, 'Line-Wall Collisions:': 107629, 'Line-Line Collisions:': 1196853}, {'Frames:': 4200, 'File:': 'diagonal.in', 'Time:': 4.891061, 'Line-Wall Collisions:': 13152, 'Line-Line Collisions:': 12557117}]
All Correct!
apple.in_new            0.193664        0.9851513101336331
apple.in_old            0.186976        0.9733922649632718
beaver.in               0.111576        0.8643941741555624
box.in                  0.189869        0.9889886083663659
explosion.in            0.264033        0.9587846744352412
koch.in                 0.926876        0.9884262937373164
mit.in                  0.325134        0.9709086348379697
sin_wave.in             0.348382        0.9769792283035748
smalllines.in           0.560069        0.9434600165422071
fewcoronavirus.in       1.293153        0.9919791900792339
manycoronavirus.in      7.775046        0.9943032833426603
trees.in                0.162648        0.9599433411042582
bigcoronavirus.in       6.305879        0.998536528437064
large.in                0.449254        1.0071175733446318
diagonal.in             4.891061        0.9913762891936871
Geometric Mean: 0.5781973418780175
"""
