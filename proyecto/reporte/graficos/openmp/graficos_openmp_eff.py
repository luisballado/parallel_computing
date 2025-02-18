import time
import matplotlib.pyplot as plt
import numpy as np

def graficar(ys, xc1, yc1, xc2, yc2, xc4, yc4, xc8, yc8, xc16, yc16):
    
    xc1 = np.array(xc1)
    yc1 = np.array(yc1)

    xc2 = np.array(xc2)
    yc2 = np.array(yc2)

    xc4 = np.array(xc4)
    yc4 = np.array(yc4)

    xc8 = np.array(xc8)
    yc8 = np.array(yc8)

    xc16 = np.array(xc16)
    yc16 = np.array(yc16)
        
    plt.plot(xc1, yc1 ,lw=2, marker="o",linestyle='--',color="#ff3333",label='32 ROBOTS')
    plt.plot(xc2, yc2, lw=2, marker="o",linestyle='--',color="#ffe933",label='64 ROBOTS')
    plt.plot(xc4, yc4, lw=2, marker="o",linestyle='--',color="#36ff33",label='128 ROBOTS')
    plt.plot(xc8, yc8, lw=2, marker="o",linestyle='--',color="#33fff3",label='256 ROBOTS')
    plt.plot(xc16, yc16, lw=2, marker="o",linestyle='--',color="#8333ff",label='512 ROBOTS')
    
    plt.title("OPENMP EFICIENCIA")
    plt.xlabel("Número de procesadores")
    plt.ylabel("Eficiencia")
    plt.grid()
    plt.legend()
    plt.savefig('openmp_eff.png')
    plt.show()
    
xs = [32,64,128,256,512]
ys = [0.554,1.024,1.969,3.683,7.498]

xc1 = [1,2,4,8,16,32]
yc1 = [1,0.8,0.64,0.48,0.32,0.17]

xc2 = [1,2,4,8,16,32]
yc2 = [0.98,0.92,0.80,0.67,0.50,0.29]

xc4 = [1,2,4,8,16,32]
yc4 = [0.70,0.48,0.63,0.56,0.44,0.28]

xc8 = [1,2,4,8,16,32]
yc8 = [1,0.60,0.43,0.68,0.57,0.37]

xc16 = [1,2,4,8,16,32]
yc16 = [0.97,0.86,0.83,0.46,0.68,0.45]

# create data
graficar(ys,xc1,yc1,xc2,yc2,xc4,yc4,xc8,yc8,xc16,yc16)

