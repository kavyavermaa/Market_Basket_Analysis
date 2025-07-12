#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <set>
#include <map>

using namespace std;

// Function to read CSV file and store data in a vector
vector<vector<string>> readCSV(const string &filename) {
    vector<vector<string>> data;
    ifstream file(filename);
    string line, word;
    
    while (getline(file, line)) {
        vector<string> row;
        stringstream s(line);
        while (getline(s, word, ',')) {
            row.push_back(word);
        }
        data.push_back(row);
    }
    file.close();
    return data;
}

// Function to generate frequent itemsets based on minimum support
map<set<string>, int> generateFrequentItemsets(vector<vector<string>> &records, int min_support) {
    map<set<string>, int> itemsets;
    
    // Count occurrences of individual items
    for (const auto &transaction : records) {
        set<string> unique_items(transaction.begin(), transaction.end());
        for (const auto &item : unique_items) {
            itemsets[{item}]++;
        }
    }
    
    // Remove itemsets that do not meet the minimum support threshold
    for (auto it = itemsets.begin(); it != itemsets.end();) {
        if (it->second < min_support) {
            it = itemsets.erase(it);
        } else {
            ++it;
        }
    }
    return itemsets;
}

int main() {
    // Read transaction data from CSV file
    vector<vector<string>> store_data = readCSV("store_data.csv");
    cout << "Data Loaded. Rows: " << store_data.size() << endl;

    vector<vector<string>> records;
    
    // Extract up to 7500 transactions, considering at most 20 items per transaction
    for (size_t i = 1; i < min(store_data.size(), size_t(7501)); ++i) {
        vector<string> transaction;
        for (size_t j = 0; j < min(store_data[i].size(), size_t(20)); ++j) {
            transaction.push_back(store_data[i][j]);
        }
        records.push_back(transaction);
    }

    cout << "Records extracted: " << records.size() << endl;
    
    // Define the minimum support threshold
    int min_support = 30;
    
    // Generate frequent itemsets
    map<set<string>, int> frequent_itemsets = generateFrequentItemsets(records, min_support);
    
    // Display frequent itemsets that meet the minimum support
    cout << "Frequent Itemsets (support >= " << min_support << "):\n";
    for (const auto &itemset : frequent_itemsets) {
        cout << "{ ";
        for (const auto &item : itemset.first) {
            cout << item << " ";
        }
        cout << "} -> " << itemset.second << endl;
    }
    
    return 0;
}