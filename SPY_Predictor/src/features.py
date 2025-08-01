import pandas as pd
import numpy as np

def scale_feature(series):
    # Min-max scale 0 and 1
    return (series - series.min()) / (series.max() - series.min())

def macd_rsi_features(df):
    df = df.copy()

    # MACD
    ema_12 = df["Close"].ewm(span=12, adjust=False).mean()
    ema_26 = df["Close"].ewm(span=26, adjust=False).mean()
    df["macd"] = ema_12 - ema_26
    df["macd_scaled"] = scale_feature(df["macd"])

    # RSI
    delta = df["Close"].diff()
    gain = delta.where(delta > 0, 0)
    loss = -delta.where(delta < 0, 0)
    avg_gain = gain.rolling(14).mean()
    avg_loss = loss.rolling(14).mean()
    rs = avg_gain / avg_loss
    rsi = 100 - (100 / (1 + rs))
    df["rsi"] = rsi
    df["rsi_scaled"] = scale_feature(rsi)

    return df

def target(df):
    df = df.copy()
    df["target"] = (df["Close"].shift(-1) > df["Close"]).astype(int)
    return df

def generate_macd_rsi_only(df):
    df = macd_rsi_features(df)
    df = target(df)
    df = df[["Date", "macd_scaled", "rsi_scaled", "target"]].dropna().reset_index(drop=True)
    return df

if __name__ == "__main__":
    df = pd.read_csv("SPY_Data.csv")
    df["Date"] = pd.to_datetime(df["Date"])
    df = generate_macd_rsi_only(df)
    df.to_csv("SPY_Features.csv", index=False)
    print("Bang")
