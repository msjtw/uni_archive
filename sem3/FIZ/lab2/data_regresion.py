import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import linregress

# Data from the table in the image
temp = np.array([25.1, 30.9, 34.9, 39.5, 40.4, 45.4, 45.4, 50.5, 50.6, 54.8, 55.2, 59.9, 60.1, 65.2, 66.1, 69.3])
miedz = np.array([0.05, 0.11, 0.17, 0.23, 0.19, 0.31, 0.25, 0.32, 0.38, 0.44, 0.38, 0.52, 0.45, 0.53, 0.59, 0.62])
mosiadz = np.array([0.06, 0.12, 0.2, 0.25, 0.2, 0.34, 0.27, 0.35, 0.43, 0.49, 0.42, 0.57, 0.5, 0.58, 0.65, 0.69])
stal = np.array([0.03, 0.08, 0.11, 0.16, 0.11, 0.22, 0.17, 0.22, 0.27, 0.32, 0.26, 0.37, 0.32, 0.37, 0.42, 0.44])

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
slope_mosiadz, intercept_mosiadz, r_value_mosiadz, _, _ = linregress(temp, mosiadz)
slope_stal, intercept_stal, r_value_stal, _, _ = linregress(temp, stal)

delta_slope_miedz = slope_uncertainty(temp, miedz, slope_miedz, intercept_miedz)
delta_slope_mosiadz = slope_uncertainty(temp, mosiadz, slope_mosiadz, intercept_mosiadz)
delta_slope_stal = slope_uncertainty(temp, stal, slope_stal, intercept_stal)

# Generate regression lines
x_fit = np.linspace(min(temp), max(temp), 100)
y_fit_miedz = slope_miedz * x_fit + intercept_miedz
y_fit_mosiadz = slope_mosiadz * x_fit + intercept_mosiadz
y_fit_stal = slope_stal * x_fit + intercept_stal

# Plotting
plt.figure(figsize=(10, 6))

# Copper
plt.errorbar(temp, miedz, xerr=temp_error, yerr=data_error, fmt='o', label='Miedź', color='red', ecolor='lightcoral')
plt.plot(x_fit, y_fit_miedz, '-', color='red')

# Brass
plt.errorbar(temp, mosiadz, xerr=temp_error, yerr=data_error, fmt='o', label='Mosiądz', color='blue', ecolor='lightblue')
plt.plot(x_fit, y_fit_mosiadz, '-', color='blue')

# Steel
plt.errorbar(temp, stal, xerr=temp_error, yerr=data_error, fmt='o', label='Stal', color='green', ecolor='lightgreen')
plt.plot(x_fit, y_fit_stal, '-', color='green')

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
print(f"Mosiądz : a = {slope_mosiadz:.6f} +- {delta_slope_mosiadz:.6f}")
print(f"Stal : a = {slope_stal:.6f} +- {delta_slope_stal:.6f}")
