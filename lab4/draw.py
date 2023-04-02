import pandas as pd
import matplotlib.pyplot as plt

filenames = ['static.csv', 'dynamic.csv', 'guided.csv', 'runtime.csv', 'auto.csv']

for filename in filenames:
    data = pd.read_csv(filename, header=None, sep=';')

    groups = data.groupby(data.iloc[:, 0])

    fig_czasu, ax1 = plt.subplots()
    fig_czasu.set_size_inches(8, 6)

    for group_name, group_data in groups:
        ax1.plot(group_data.iloc[:, 1], group_data.iloc[:, 2], 'o-', label=f'rozmiar_tablicy: {group_name}')

    ax1.set_xlabel('liczba wątków')
    ax1.set_ylabel('Czas wykonania [s]')
    ax1.set_title(f'Czas wykonania dla różnych rozmiarów tablicy - {filename[:-4]}')
    ax1.legend()
    ax1.grid(True)

    ax1.set_xticks(data.iloc[:, 1].unique())

    fig_czasu.savefig(f'wykres_czasu_{filename[:-4]}.png', dpi=300)

    fig_przyspieszenia, ax2 = plt.subplots()
    fig_przyspieszenia.set_size_inches(8, 6)

    for group_name, group_data in groups:
        speedup = group_data.iloc[0, 2] / group_data.iloc[:, 2]
        ax2.plot(group_data.iloc[:, 1], speedup, 'o-', label=f'rozmiar_tablicy: {group_name}')

    ax2.set_xlabel('liczba wątków')
    ax2.set_ylabel('Przyśpieszenie')
    ax2.set_title(f'Przyśpieszenie dla różnych rozmiarów tablicy - {filename[:-4]}')
    ax2.legend()
    ax2.grid(True)

    ax2.set_xticks(data.iloc[:, 1].unique())

    fig_przyspieszenia.savefig(f'wykres_przyspieszenia_{filename[:-4]}.png', dpi=300)
