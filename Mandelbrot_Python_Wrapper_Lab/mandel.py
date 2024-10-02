import matplotlib.pyplot as plot
import numpy as np

def mandelSet(width, height):
    xmin = -2
    xmax = 1
    ymin = -1
    ymax = 1
    delta_x= (xmax - xmin)/width
    delta_y= (ymax-ymin)/height
    
    array = []
    for i in range(0,height,1):
        for j in range(0,width,1):
            value = 0
            c_real = (xmin + j*delta_x)
            c_imaginary = (ymax - i*delta_y)

            c_real_new = c_real
            c_imaginary_new = c_imaginary

            flag = 0
            counter = 0
            
            mag = np.abs(c_real + c_imaginary*1j)
            print(mag)
            while mag < 2:
                
                c_real_new_1 = c_real_new * c_real_new + c_real -c_imaginary_new * c_imaginary_new
                c_imaginary_new =  2*c_imaginary_new * c_real_new + c_imaginary
                c_real_new = c_real_new_1

                mag = np.abs(c_real_new + c_imaginary_new*1j)

                if counter == 100:
                    flag = 1
                    break
                counter = counter + 1

            array.append(flag)
    return array


if __name__ == "__main__":

    w = 800
    h = 700
    
    array = mandelSet(w, h)

    plotar = np.reshape(array, (h, w))
    plot.imshow(plotar)
    plot.show()