import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import mplcyberpunk

plt.style.use("cyberpunk")
for i in range(1000):
    print("Plotting file #%d"%i)
    fig = plt.figure(figsize=(4, 4))
    sticks = pd.read_csv("dat_files/stick_0%d.dat"%i, names=["ID", "x1", "y1", "x2", "y2"])


    for _,stick in sticks.iterrows():
        
        plt.plot([stick["x1"], stick["x2"]], [stick["y1"], stick["y2"]], 'y',label=str(stick["ID"]), linewidth=0.25, solid_capstyle="round")
        #plt.plot(stick['x1'], stick['y1'], 'k*')
        #plt.text(stick['x1'], stick['y1'], stick['ID'], fontsize=2)

    #mplcyberpunk.make_lines_glow(n_glow_lines=3, diff_linewidth=1.01)


    plt.savefig("img/stick_0%d.png"%i, dpi=400)
    plt.close(fig)
