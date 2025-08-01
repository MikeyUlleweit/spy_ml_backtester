#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <stdexcept>
#include <algorithm>

class Backtest {
private:
    struct Trade {
        std::string date;
        double entryPrice;
        double exitPrice;
        double profit;
        bool isLong;
    };

    double initialEquity;
    double positionSize;
    double currentEquity;
    double minProbThreshold;
    double maxProbThreshold;
    std::vector<Trade> trades;
    std::vector<double> equityCurve;

public:
    Backtest(double initialEquity_, double positionSize_, double minProbThreshold_, double maxProbThreshold_)
        : initialEquity(initialEquity_), positionSize(positionSize_), currentEquity(initialEquity_),
          minProbThreshold(minProbThreshold_), maxProbThreshold(maxProbThreshold_) {}

    void runBacktest(const std::string& filename) {
    auto data = loadData(filename);
    std::cout << "Loaded " << data.size() << " rows from " << filename << std::endl;
    
    for (size_t i = 0; i < data.size() - 1; ++i) {
        try {
            // Parse
            std::string date = data[i][0];
            double macd = std::stod(data[i][1]);
            double rsi = std::stod(data[i][2]);
            double pred = std::stod(data[i][3]);
            double price = std::stod(data[i][4]) * 100;  // Convert to actual price
            double nextPrice = std::stod(data[i + 1][4]) * 100;
            
            // first few rows weird
            if (i < 5) {
                std::cout << "Row " << i << ": " << date 
                         << "\n  MACD=" << macd
                         << ", RSI=" << rsi 
                         << ", Pred=" << pred
                         << "\n  Price=$" << price 
                         << ", NextPrice=$" << nextPrice << std::endl;
            }
            
            // Trading logic
            double maxRisk = currentEquity * 0.02;
            if ((macd > 0.5 && rsi > 0.5) || pred > 0.5) {  // Combined signals
                double profit = std::min(positionSize * (nextPrice - price) / price, maxRisk);
                currentEquity += profit;
                trades.push_back({date, price, nextPrice, profit, true});
            }
            
            equityCurve.push_back(currentEquity);
        }
        catch (const std::exception& e) {
            std::cerr << "Error at " << data[i][0] << ": " << e.what() << std::endl;
            continue;
        }
    }
    
    printResults();
}

private:
    void printResults() {
        int totalTrades = trades.size();
        int winningTrades = std::count_if(trades.begin(), trades.end(),
            [](const Trade& t) { return t.profit > 0; });
        
        double winRate = totalTrades > 0 ? 100.0 * winningTrades / totalTrades : 0;
        double totalReturn = (currentEquity - initialEquity) / initialEquity * 100;
        
        std::cout << "\nBacktest Results:" << std::endl;
        std::cout << "----------------" << std::endl;
        std::cout << "Initial Equity: $" << initialEquity << std::endl;
        std::cout << "Final Equity: $" << currentEquity << std::endl;
        std::cout << "Total Return: " << totalReturn << "%" << std::endl;
        std::cout << "Total Trades: " << totalTrades << std::endl;
        std::cout << "Win Rate: " << winRate << "%" << std::endl;
        
        
        std::ofstream out("equity_curve.csv");
        out << "day,equity\n";
        for (size_t i = 0; i < equityCurve.size(); ++i) {
            out << i << "," << equityCurve[i] << "\n";
        }
    }
    
    
    std::vector<std::vector<std::string>> loadData(const std::string& filename) {
        std::vector<std::vector<std::string>> data;
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file: " + filename);
        }
        std::string line;
        
        if (std::getline(file, line)) {
            std::istringstream ss(line);
            std::string cell;
            std::getline(ss, cell, ',');
            if (!isdigit(cell[0])) {
                
            } else {
                
                std::vector<std::string> row;
                std::istringstream ss2(line);
                while (std::getline(ss2, cell, ',')) {
                    row.push_back(cell);
                }
                data.push_back(row);
            }
        }
        while (std::getline(file, line)) {
            std::vector<std::string> row;
            std::istringstream ss(line);
            std::string cell;
            while (std::getline(ss, cell, ',')) {
                row.push_back(cell);
            }
            data.push_back(row);
        }
        return data;
    }
};

int main(int argc, char* argv[]) {
    try {
        std::string filename = "spy_predictions.csv";
        if (argc > 1) {
            filename = argv[1];
        }

        Backtest bt(10000.0, 1000.0, 0.4, 0.6);
        bt.runBacktest(filename);
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}