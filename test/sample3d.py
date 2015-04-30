# -*- coding: utf-8 -*-
#!/usr/bin/python2

# Copyright (C) 2015 Leslie Zhai <xiang.zhai@i-soft.com.cn>

import sys
import numpy as np
from mpl_toolkits.mplot3d import axes3d
import matplotlib.pyplot as plt

def parse_sample(filename='sample.txt'):
    x = []
    z = []
    y = []

    f = open(filename)
    samples = f.readlines()
    f.close()
    for line in samples:
        arr = line.split(' ')
        x.append(float(arr[0]))
        z.append(float(arr[2]))
        y.append(float(arr[1]))

    fig = plt.figure('original')
    ax = fig.add_subplot(111, projection='3d')
    ax.plot_wireframe(x, z, y, rstride=10, cstride=10)
    plt.show()

if __name__=='__main__':
    if len(sys.argv) > 1:
        parse_sample(sys.argv[1])
    else:
        parse_sample()
