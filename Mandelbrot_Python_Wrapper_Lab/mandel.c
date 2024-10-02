#include <complex.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>


uint8_t* mandelSet(uint8_t* array, int width, int height){
    int xmin = -2;
    int xmax = 1;
    int ymin = -1;
    int ymax = 1;
    double delta_x= (double)(xmax - xmin)/width;
    double delta_y= (double)(ymax-ymin)/height;
    
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            int value = 0;
            double c_real = (xmin + j*delta_x);
            double c_imaginary = (ymax - i*delta_y);

            double c_real_new = c_real;
            double c_imaginary_new = c_imaginary;

            int flag = 0;
            int counter = 0;
            
            double complex mag = c_real_new + c_imaginary_new*I;
            double cmp = cabs(mag);
           
            printf("%f \n", cmp);
            while (cmp < 2.0){
                
                double c_real_new_1 = c_real_new * c_real_new + c_real -c_imaginary_new * c_imaginary_new;
                c_imaginary_new =  2*c_imaginary_new * c_real_new + c_imaginary;
                c_real_new = c_real_new_1;

                mag = c_real_new + c_imaginary_new*I;
                
                cmp = cabs(mag);
                
                if(counter == 100){
                    flag = 1;
                    break;
                }
                counter++;
            }
            array[i*width + j] = flag;
        }
        
    }
    return array;
}
