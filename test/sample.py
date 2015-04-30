# -*- coding: utf-8 -*-
#!/usr/bin/python2

# Copyright (C) 2015 Leslie Zhai <xiang.zhai@i-soft.com.cn>

import sys
import numpy as np
import matplotlib.pyplot as plt

def parse_sample(filename='sample.txt'):
    x = []
    z = []

    f = open(filename)
    samples = f.readlines()
    f.close()
    for line in samples:
        arr = line.split(' ')
        #print float(arr[0]), float(arr[2])
        x.append(float(arr[0]))
        z.append(float(arr[2]))

    plt.figure('original')
    plt.plot(x, z)
    plt.show()

if __name__=='__main__':
    if len(sys.argv) > 1:
        parse_sample(sys.argv[1])
    else:
        parse_sample()
