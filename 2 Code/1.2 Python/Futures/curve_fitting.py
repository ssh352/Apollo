# encoding: utf-8

from numpy import array
from matplotlib import pyplot as plt

import numpy as np

def get_data_from_csv():

    data_list = []

    with open('data_cu_10.csv', 'r') as file:
        for line in file:
            line = float(line)
            data_list.append(line)

    return data_list

areaList = get_data_from_csv()

min_value = min(areaList)
max_value = max(areaList)
mean_value = sum(areaList)/len(areaList)

dev = 0.005
mu = mean_value
sigma = 0.045

count, bins, _ = plt.hist(areaList, 300, normed=1, edgecolor='None', facecolor='blue')

plt.plot(bins, 1./(np.sqrt(2*np.pi)*sigma)*np.exp(-(bins-mu)**2/(2*sigma**2)), linewidth=2, color='r')

plt.title("mu=" + str(mu) + "," + "sigma=" + str(sigma))

plt.show()
