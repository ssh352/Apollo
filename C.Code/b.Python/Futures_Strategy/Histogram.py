# encoding: utf-8

from numpy import array
from matplotlib import pyplot as plt


# 参数依次为list,抬头,X轴标签,Y轴标签,XY轴的范围
def draw_hist(myList, Title, Xlabel, Ylabel, Xmin, Xmax, Ymin, Ymax):
    plt.hist(myList, 100, normed=1, edgecolor='None', facecolor='blue')
    plt.xlabel(Xlabel)
    plt.xlim(Xmin, Xmax)
    plt.ylabel(Ylabel)
    plt.ylim(Ymin, Ymax)
    plt.title(Title)
    plt.show()


def get_data_from_csv():

    data_list = []

    with open('data_cu.csv', 'r') as file:
        for line in file:
            line = float(line)
            data_list.append(line)

    return data_list

areaList = get_data_from_csv()

min_value = min(areaList)
max_value = max(areaList)

draw_hist(areaList,'Rollover-Histogram','Rollover Yield','Frequency',min_value, max_value,0.0,50)   # 直方图展示
