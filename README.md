# Radar Signal Processing in C (Step-by-Step Implementation)

## 📌 Overview

This project implements radar signal processing algorithms step-by-step in C.

Main features:

* 1D CA-CFAR
* Peak Detection
* Range Conversion
* Range-Doppler Map (RDM)
* 2D CA-CFAR
* Target Tracking (Nearest Neighbor)

---

## 🧠 Processing Flow

```
Range FFT → Doppler FFT → CFAR → Target Detection → Tracking
```

---

## 📂 Project Structure

```
src/
 ├── step04_cfar_1d.c
 ├── step05_peak_detection_1d.c
 ├── step06_range_conversion.c
 ├── step07_range_doppler_map.c
 ├── step08_cfar_2d.c
 └── step09_nearest_neighbor_tracking.c
```

---

## 🚀 How to Run

Each step is implemented as an independent program.

Compile and run a specific step:

```bash
gcc src/step04_cfar_1d.c -o step04
./step04
```

Example:

```bash
gcc src/step09_nearest_neighbor_tracking.c -o step09
./step09
```

## 📈 Future Work

* Prediction-based tracking
* Kalman Filter
* Multi-target tracking
* Real radar data integration

updated for GitHub practice 260506
