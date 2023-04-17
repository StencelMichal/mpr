import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from tabulate import tabulate
# from IPython.display import display
import csv


LEGEND = ['Losowanie', 'Rozdzielanie do kubełków',
          'Sortowanie', 'Scalanie tablicy', 'Sumarycznie']

SYNC_FILE_PATH = 'results.csv'
ASYNC_FILE_PATH = 'results2.csv'


def draw_chart_1():
    df = pd.read_csv(SYNC_FILE_PATH, sep=';')

    # print(tabulate(df, headers = 'keys', tablefmt = 'psql'))
    df.to_csv("cleaned_iris_data.csv")

    grouped_df = df.groupby(['arrsize', 'bsize', 'nthreads'])[
        'total'].mean().reset_index()

    x = grouped_df['bsize']
    y = grouped_df['total']
    plt.scatter(x, y)
    plt.xlabel('Ilość kubełków')
    plt.ylabel('Całkowity czas wykonania [s]')
    plt.title('Zależność czasu wykonania algorytmu sekwencyjnego od ilości kubełków')
    plt.legend(['Algorytm sekwencyjny'])
    plt.grid(True)


    # print(x)
    # # print(y)
    # min_value = min(y)
    # print(min_value)
    # a = y.to_numpy()
    # print(a)
    # idx = np.where(a == a.min())
    # print(idx)
    # print(min_value)
    # print(x[idx], y[idx])
    # b = x.to_numpy()
    # print(x[10])


def draw_chart_2():
    df = pd.read_csv(ASYNC_FILE_PATH, sep=';')

    # display(df)

    pivoted_df = pd.pivot_table(df, values=[
        'total', 'draw', 'scatter', 'sort', 'gather'], index=['nthreads'], aggfunc='mean')

    pivoted_df = pivoted_df.iloc[0] / pivoted_df

    _, ax = plt.subplots()
    for name, values in pivoted_df.iteritems():
        ax.plot(values.index, values, label=name,
                marker='o', linestyle='--')

    plt.xlabel('Ilość wątków')
    plt.ylabel('Przyspieszenie')
    plt.title('Przyspieszenie w zależności od ilości wątków')
    plt.legend(LEGEND)
    plt.grid(True)

    nthreads_values = df['nthreads']
    ax.set_xticks(nthreads_values)


def draw_chart_3():
    bar_width = 0.3
    data = pd.read_csv(ASYNC_FILE_PATH, sep=';')
    data = data.drop([ 'arrsize', 'bsize'], axis=1)

    data = data.groupby('nthreads').mean()

    r1 = np.arange(len(data['total']))
    r1 = [x + bar_width for x in r1]

    ax = data[['draw', 'scatter', 'sort', 'gather']].plot(
        kind='bar', stacked=True, width=bar_width)

    ax.bar(r1, data['total'], color='gray',
           width=bar_width, label='total')

    ax.set_xticklabels(data.index, rotation=0)

    ax.legend(LEGEND)
    ax.set_xlabel("Ilość wątków")
    ax.set_ylabel("Czas wykonania [s]")
    ax.set_title("Czas wykonania w zależności od ilości wątków")
    ax.grid(True)


# save_csv_as_df()
# draw_chart_1()
draw_chart_2()
# draw_chart_3()
plt.show()