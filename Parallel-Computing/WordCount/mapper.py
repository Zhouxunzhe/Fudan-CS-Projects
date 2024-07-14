#!/usr/bin/env python
# -*- coding:utf-8 -*-
import sys

for line in sys.stdin:
    line = line.strip()
    words = line.split()
    for word in words:
        print("%s\t%s" % (word, 1))
