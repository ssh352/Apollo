# encoding: utf-8

from numpy import array
from matplotlib import pyplot as plt


# 参数依次为list,抬头,X轴标签,Y轴标签,XY轴的范围
def draw_hist(myList, Title, Xlabel, Ylabel, Xmin, Xmax, Ymin, Ymax):
    plt.hist(myList, 200)
    plt.xlabel(Xlabel)
    plt.xlim(Xmin, Xmax)
    plt.ylabel(Ylabel)
    plt.ylim(Ymin, Ymax)
    plt.title(Title)
    plt.show()


def get_data_from_csv():

    data_list = []

    with open('data.csv', 'r') as file:
        for line in file:
            line = float(line)
            data_list.append(line)

    return data_list

areaList = get_data_from_csv()

min_value = min(areaList)
max_value = max(areaList)

draw_hist(areaList,'AreasList','Area','number',min_value, max_value,0.0,30)   # 直方图展示
