#!/usr/bin/env python
import spead2
import spead2.recv
import sys
import logging

logging.basicConfig(level=logging.INFO)

items = []

stream = spead2.recv.Stream(spead2.BUG_COMPAT_PYSPEAD_0_5_2, 8)
receiver = spead2.recv.Receiver()
if 0:
    with open('junkspeadfile', 'rb') as f:
        text = f.read()
    receiver.add_buffer_reader(stream, text)
else:
    receiver.add_udp_reader(stream, 8888)
receiver.start()

ig = spead2.recv.ItemGroup()
for heap in stream:
    print "Got heap", heap.cnt
    ig.update(heap)
    for item in ig.items.itervalues():
        print heap.cnt, item.name, item.value.shape
receiver.stop()
