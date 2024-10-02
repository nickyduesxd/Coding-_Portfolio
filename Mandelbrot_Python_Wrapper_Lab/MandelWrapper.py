import numpy as np
from ctypes import *


#Load shared library
lib = CDLL("./mandel.so")

# Declare function prototype
mandelSet = lib.mandelSet
mandelSet.argtypes = [POINTER(c_ubyte), c_int, c_int]
mandelSet.restype = POINTER(c_ubyte)

# Define width and height
w = 800
h = 700

# Create arrays
length = w * h
data = np.zeros(length, dtype=np.uint8)
data_p = data.ctypes.data_as(POINTER(c_ubyte))


#Call function
new_p = mandelSet(data_p, w, h)

#Retrieve data from pointer - only way i could make it work, i tried using data_p
new_array = np.ctypeslib.as_array(new_p, shape=(h,w ))

plotar = np.reshape(new_array, (h, w))


# Graphing element
import matplotlib.pyplot as plt

plt.imshow(plotar)
plt.show()
