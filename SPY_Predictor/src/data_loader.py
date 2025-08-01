import yfinance as yf

def download_data():
    # Download SPY data from Yahoo adjust columns/rows
    df = yf.download("SPY", start="2015-01-01")
    df.reset_index(inplace=True)
    return df

def create_csv():
    df = download_data()
    # get rid of ticker row
    df.columns = df.columns.get_level_values(0)
    df.to_csv("SPY_Data.csv")

if __name__ == "__main__":
    create_csv()
