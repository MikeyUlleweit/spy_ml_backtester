# SPY Directional ML Backtester (Python + C++)

This project builds a machine learning pipeline to predict the directional movement of the SPY ETF using technical indicators. A custom C++ backtester simulates execution logic and strategy performance based on the model’s predictions.

---

## Overview

- **Model**: XGBoost classifier using MACD and RSI features  
- **Objective**: Predict whether SPY will go up or down the next day  
- **Threshold Logic**: Long if confidence > 60%, short if < 40%, or if MACD & RSI > 0.5  
- **Backtester**: Custom C++ engine that handles position sizing and risk-limited profit  
- **Visualization**: Python script to generate and annotate equity curve

---

## Tech Stack

- **Python**: `pandas`, `numpy`, `matplotlib`, `scikit-learn`, `xgboost`, `yfinance`  
- **C++**: Custom logic for backtesting, risk management, trade tracking  
- **CSV**: Model predictions exported from Python → consumed by C++

---

## Results (from backtest)

| Metric         | Value        |
|----------------|--------------|
| Initial Equity | $10,000.00   |
| Final Equity   | $14,055.90   |
| Total Return   | 40.56%       |
| Total Trades   | 2,639        |
| Win Rate       | 54.15%       |
| Total Days     | 2,639        |

> Note: This strategy places a trade nearly every day due to signal thresholds.

---

## 📊 Equity Curve(go to outpu folder if not loading)

![Equity Curve](equity_curve_plot.png)

> Annotated using `matplotlib` based on the C++-generated equity curve.

---

## 📂 Folder Structure

SPY_PYTHON/
│
├── README.md                ← Project overview, setup instructions, usage, etc.
│
├── data/                    ← Raw and processed CSVs
│   ├── SPY_Data.csv
│   ├── SPY_Features.csv
│   ├── spy_predictions.csv
│   └── equity_curve.csv
│
├── notebooks/               ← Jupyter Notebooks for analysis and plotting
│   ├── plot_equity.ipynb
│   └── plot_features.ipynb
│
├── src/                     ← Python source code
│   ├── __init__.py
│   ├── data_loader.py
│   ├── prepare_backtest_data.py
│   ├── features.py
│   └── tuned_xboost_model.py
│
├── backtest/                ← C++ or other backtesting logic
│   ├── backtest.cpp
│ 
│
├── outputs/                 ← Results graphed/plotted
│   └── equity_curve_plot.png


