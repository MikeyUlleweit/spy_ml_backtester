import pandas as pd
import numpy as np

# Read the features file
df = pd.read_csv("SPY_Features.csv")

# Convert macd_scaled and rsi_scaled to prediction and probability
# This is a simple example - you might want to adjust the logic
df['prediction'] = (df['macd_scaled'] > df['rsi_scaled']).astype(int)
df['probability'] = df['macd_scaled'].rolling(window=5).mean()

# Ensure probabilities are between 0 and 1
df['probability'] = df['probability'].clip(0, 1)

# Save as spy_predictions.csv with required columns
df.to_csv("spy_predictions.csv", index=False)
print("Created spy_predictions.csv from SPY_Features.csv")