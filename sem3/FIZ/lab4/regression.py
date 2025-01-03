import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import linregress

# Data from the table in the image
temp = np.array([
    0.00334952269301625, 0.00329760923330585, 0.0032441200324412,
    0.00319131961065901, 0.00314317146000314, 0.00309453814018258,
    0.00304553068372164, 0.0030016509079994, 0.00295814228664399,
    0.00291502696399942, 0.00287150035893755, 0.00283005518607613,
    0.00279212620410443, 0.00275368305108082
])
miedz = np.array([
    -12.1281111040605, -11.9316357998284, -11.7117763237154,
    -11.5028751291167, -11.3022044336546, -11.1124478983731,
    -10.9331069697173, -10.7579028806922, -10.5966347330961,
    -10.4341158035983, -10.2399597891573, -10.0858091093301,
    -9.95227771670556, -9.7981270368783
])

# Errors
temp_error = 0.5
data_error = 0.02

def slope_uncertainty(x, y, slope, intercept):
    n = len(x)
    residuals = y - (slope * x + intercept)
    RSS = np.sum(residuals**2)
    SE_m = np.sqrt(RSS / (n - 2)) / np.sqrt(np.sum((x - np.mean(x))**2))
    return SE_m

# Perform linear regressions
slope_miedz, intercept_miedz, r_value_miedz, _, _ = linregress(temp, miedz)

delta_slope_miedz = slope_uncertainty(temp, miedz, slope_miedz, intercept_miedz)

# Generate regression lines
x_fit = np.linspace(min(temp), max(temp), 100)
y_fit_miedz = slope_miedz * x_fit + intercept_miedz

# Plotting
plt.figure(figsize=(10, 6))

# Copper
plt.errorbar(temp, miedz, xerr=temp_error, yerr=data_error, fmt='o', label='Miedź', color='red', ecolor='lightcoral')
plt.plot(x_fit, y_fit_miedz, '-', color='red')

# Labels and legend
plt.xlabel('Temperatura (°C)')
plt.ylabel('Wydłużenie (mm)')
plt.title('Wydłużenie ciał względem temperatury')
plt.legend()
plt.grid()
plt.show()

# Print regression details
print("Wyniki:")
print(f"Miedź : a = {slope_miedz:.6f} +- {delta_slope_miedz:.6f}")
