These terms refer to **spatial resolution and statistical summaries** of elevation data in **GeoTIFF or DEM (Digital Elevation Model) datasets**. Let’s break them down:

---

## **1. Arc-second Resolutions (7.5 Arc, 15 Arc, 30 Arc)**
These numbers refer to the **spatial resolution of the dataset** in **arc-seconds**, a unit of angular measurement.

- **7.5 Arc-Second (~250m resolution)**  
  - Each pixel represents about **250 meters** in the real world.
- **15 Arc-Second (~500m resolution)**  
  - Each pixel represents about **500 meters**.
- **30 Arc-Second (~1km resolution)**  
  - Each pixel represents about **1 kilometer**.

📌 **Smaller values mean higher resolution** (more detail), while larger values mean lower resolution (less detail but covering a bigger area faster).

---

## **2. Statistical Measures in GeoTIFF**
These terms describe how **elevation values** are processed within each grid cell (or pixel) of the heightmap.

### **Minimum Stat**
- The **minimum elevation** value found within a grid cell.
- Helps identify the lowest terrain points.

### **Standard Dev Stat (Standard Deviation)**
- Measures how much the elevation values **vary** within the cell.
- High standard deviation = more rugged terrain.
- Low standard deviation = flat areas.

### **Median Stat**
- The **middle elevation** value in a sorted list of heights for a grid cell.
- Less sensitive to extreme values than the mean.

### **Systematic Subsample**
- A method for reducing data by selecting specific points at regular intervals.
- Used when generating lower-resolution versions of a dataset.

### **Mean Stat**
- The **average elevation** in a grid cell.
- Used for **generalized terrain representations**.

### **Maximum Stat**
- The **highest elevation** value within a grid cell.
- Useful for identifying peaks.

### **Breakline Emphasis**
- **Enhances important terrain features**, like ridgelines, rivers, and cliffs.
- Used in DEM processing to preserve significant terrain changes.

---

## **How Does This Apply to GeoTIFF?**
- Different GeoTIFF datasets may **use different resolutions (arc-seconds)** and **statistical methods** to process elevation.
- You may need **higher resolution (7.5 arc-sec)** if you need **detailed terrain**, or **lower resolution (30 arc-sec)** for a broad overview.
- If you are **downsampling** a dataset for a simulation, you may choose to retain the **mean, median, or minimum elevation values** depending on your needs.

