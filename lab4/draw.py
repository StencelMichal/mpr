filename = 'default.csv'

import pandas as pd
import matplotlib.pyplot as plt

# Wczytaj plik csv z separatorem średnika
data = pd.read_csv(filename, header=None, sep=';')

# Grupuj dane po rozmiarze tablicy (kolumna 0)
groups = data.groupby(data.iloc[:, 0])

# Twórz wykresy z czasem wykonania dla każdej grupy
fig_czasu, ax1 = plt.subplots()
fig_czasu.set_size_inches(8, 6)

for group_name, group_data in groups:
    ax1.plot(group_data.iloc[:, 1], group_data.iloc[:, 2], 'o-', label=f'rozmiar_tablicy: {group_name}')

ax1.set_xlabel('liczba wątków')
ax1.set_ylabel('Czas wykonania [s]')
ax1.set_title('Czas wykonania dla różnych rozmiarów tablicy')
ax1.legend()
ax1.grid(True)

# Ustawienie osi x - zaznaczenie wszystkich ilości wątków
ax1.set_xticks(data.iloc[:, 1].unique())

# Zapisz wykres czasu do pliku PNG
fig_czasu.savefig('wykres_czasu.png', dpi=300)

# Twórz wykresy z przyśpieszeniem dla każdej grupy
fig_przyspieszenia, ax2 = plt.subplots()
fig_przyspieszenia.set_size_inches(8, 6)

for group_name, group_data in groups:
    speedup = group_data.iloc[0, 2] / group_data.iloc[:, 2]
    ax2.plot(group_data.iloc[:, 1], speedup, 'o-', label=f'rozmiar_tablicy: {group_name}')

ax2.set_xlabel('liczba wątków')
ax2.set_ylabel('Przyśpieszenie')
ax2.set_title('Przyśpieszenie dla różnych rozmiarów tablicy')
ax2.legend()
ax2.grid(True)

# Ustawienie osi x - zaznaczenie wszystkich ilości wątków
ax2.set_xticks(data.iloc[:, 1].unique())

# Zapisz wykres przyśpieszenia do pliku PNG
fig_przyspieszenia.savefig('wykres_przyspieszenia.png', dpi=300)
