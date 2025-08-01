import pandas as pd
import numpy as np
from xgboost import XGBClassifier
from sklearn.model_selection import train_test_split
from sklearn.metrics import classification_report, accuracy_score
import matplotlib.pyplot as plt

# Load data
df = pd.read_csv("SPY_Features.csv")
X = df[["macd_scaled", "rsi_scaled"]]
y = df["target"]

# split
X_train, X_test, y_train, y_test = train_test_split(X, y, shuffle=False, test_size=0.2)

# train
model = XGBClassifier(use_label_encoder=False, eval_metric="logloss")
model.fit(X_train, y_train)

# eval
y_pred = model.predict(X_test)
print(f"\n Accuracy: {accuracy_score(y_test, y_pred):.4f}")
print("\n Classification Report:")
print(classification_report(y_test, y_pred))

