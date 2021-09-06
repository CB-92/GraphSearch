import sys
import os

if len(sys.argv)<2:
    print("Usage: python execute.py version")
    exit

version = str(sys.argv[1])
test_files = {
    "5000": "5K_nodes.txt",
    "10000": "10K_nodes.txt",
    "50000": "50K_nodes.txt",
    "100000": "100K_nodes.txt"
}


if version == 'sequential':
    for n in test_files:
        cmd = './sBFS data/' + test_files[n] + ' ' + n + ' 2 1'
        print(cmd)
        for _ in range(10):
            os.system(cmd)

elif version == 'thread':
    for n in test_files:
        for th in range(1, 5, 1):
        #for th in (2**p for p in range(1,9)):
            cmd = './tBFS data/' + test_files[n] + ' ' + n +' 2 1 ' + str(th)
            print(cmd)
            for _ in range(10):
                os.system(cmd)

elif version == 'fastflow':
    for n in test_files:
        #for th in (2**p for p in range(1,9)):
        for th in range(1, 5, 1):
            cmd = './fBFS data/' + test_files[n] + ' ' + n +' 2 1 ' + str(th)
            print(cmd)
            for _ in range(10):
                os.system(cmd)

else:
    print("Version not found")